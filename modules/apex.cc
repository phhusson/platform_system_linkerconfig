/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "linkerconfig/apex.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/result.h>
#include <android-base/strings.h>
#include <apexutil.h>
#include <unistd.h>
#include <vector>

#include "linkerconfig/apexlinkerconfig.h"
#include "linkerconfig/environment.h"
#include "linkerconfig/log.h"
#include "linkerconfig/stringutil.h"

// include after log.h to avoid macro redefinition error
#include "com_android_apex.h"

using android::base::ErrnoError;
using android::base::ReadFileToString;
using android::base::Result;
using android::base::StartsWith;

namespace {
bool DirExists(const std::string& path) {
  return access(path.c_str(), F_OK) == 0;
}

Result<std::set<std::string>> ReadPublicLibraries(const std::string& filepath) {
  std::string file_content;
  if (!android::base::ReadFileToString(filepath, &file_content)) {
    return ErrnoError();
  }
  std::vector<std::string> lines = android::base::Split(file_content, "\n");
  std::set<std::string> sonames;
  for (auto& line : lines) {
    auto trimmed_line = android::base::Trim(line);
    if (trimmed_line[0] == '#' || trimmed_line.empty()) {
      continue;
    }
    std::vector<std::string> tokens = android::base::Split(trimmed_line, " ");
    if (tokens.size() < 1 || tokens.size() > 3) {
      return Errorf("Malformed line \"{}\"", line);
    }
    sonames.insert(tokens[0]);
  }
  return sonames;
}

std::vector<std::string> Intersect(const std::vector<std::string>& as,
                                   const std::set<std::string>& bs) {
  std::vector<std::string> intersect;
  std::copy_if(as.begin(),
               as.end(),
               std::back_inserter(intersect),
               [&bs](const auto& a) { return bs.find(a) != bs.end(); });
  return intersect;
}

}  // namespace

namespace android {
namespace linkerconfig {
namespace modules {

std::map<std::string, ApexInfo> ScanActiveApexes(const std::string& root) {
  std::map<std::string, ApexInfo> apexes;
  const auto apex_root = root + apex::kApexRoot;
  for (const auto& [path, manifest] : apex::GetActivePackages(apex_root)) {
    bool has_bin = DirExists(path + "/bin");
    bool has_lib = DirExists(path + "/lib") || DirExists(path + "/lib64");

    auto apex_config = ParseApexLinkerConfig(path + "/etc/linker.config.txt");
    std::vector<std::string> permitted_paths;
    bool visible = false;
    if (apex_config.ok()) {
      permitted_paths = std::move(apex_config->permitted_paths);
      visible = apex_config->visible;
    }

    ApexInfo info(manifest.name(),
                  TrimPrefix(path, root),
                  {manifest.providenativelibs().begin(),
                   manifest.providenativelibs().end()},
                  {manifest.requirenativelibs().begin(),
                   manifest.requirenativelibs().end()},
                  {manifest.jnilibs().begin(), manifest.jnilibs().end()},
                  std::move(permitted_paths),
                  has_bin,
                  has_lib,
                  visible);
    apexes.emplace(manifest.name(), std::move(info));
  }

  if (!apexes.empty()) {
    const std::string info_list_file = apex_root + "/apex-info-list.xml";
    auto info_list =
        com::android::apex::readApexInfoList(info_list_file.c_str());
    if (info_list.has_value()) {
      for (const auto& info : info_list->getApexInfo()) {
        apexes[info.getModuleName()].original_path =
            info.getPreinstalledModulePath();
      }
    } else {
      PLOG(ERROR) << "Can't read " << info_list_file;
    }

    const std::string public_libraries_file =
        root + "/system/etc/public.libraries.txt";
    auto public_libraries = ReadPublicLibraries(public_libraries_file);
    if (public_libraries.ok()) {
      for (auto& [name, apex] : apexes) {
        // Only system apexes can provide public libraries.
        if (!apex.InSystem()) {
          continue;
        }
        apex.public_libs = Intersect(apex.provide_libs, *public_libraries);
      }
    } else {
      LOG(ERROR) << "Can't read " << public_libraries_file << ": "
                 << public_libraries.error();
    }
  }

  return apexes;
}

bool ApexInfo::InSystem() const {
  return StartsWith(original_path, "/system/apex/") ||
         StartsWith(original_path, "/system_ext/apex/") ||
         (!IsProductVndkVersionDefined() &&
          StartsWith(original_path, "/product/apex/"));
}

bool ApexInfo::InProduct() const {
  return IsProductVndkVersionDefined() &&
         StartsWith(original_path, "/product/apex/");
}

bool ApexInfo::InVendor() const {
  return StartsWith(original_path, "/vendor/apex/") ||
         StartsWith(original_path, "/odm/apex/");
}

}  // namespace modules
}  // namespace linkerconfig
}  // namespace android