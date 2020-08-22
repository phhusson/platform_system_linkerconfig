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

#include <android-base/strings.h>
#include <apexutil.h>
#include <unistd.h>

#include "linkerconfig/environment.h"
#include "linkerconfig/log.h"
#include "linkerconfig/stringutil.h"

// include after log.h to avoid macro redefnition error
#include "com_android_apex.h"

using android::base::StartsWith;

namespace {

bool DirExists(const std::string& path) {
  return access(path.c_str(), F_OK) == 0;
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
    ApexInfo info(manifest.name(),
                  TrimPrefix(path, root),
                  {manifest.providenativelibs().begin(),
                   manifest.providenativelibs().end()},
                  {manifest.requirenativelibs().begin(),
                   manifest.requirenativelibs().end()},
                  {manifest.jnilibs().begin(), manifest.jnilibs().end()},
                  has_bin,
                  has_lib);
    apexes.emplace(manifest.name(), std::move(info));
  }

  if (!apexes.empty()) {
    std::string info_list_file = apex_root + "/apex-info-list.xml";
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