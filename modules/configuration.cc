/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "linkerconfig/configuration.h"

#include "linkerconfig/log.h"
#include "linkerconfig/variables.h"

namespace android {
namespace linkerconfig {
namespace modules {
void Configuration::WriteConfig(ConfigWriter& writer) {
  std::map<std::string, std::string> resolved_binary_paths;

  for (auto& section : sections_) {
    auto binary_paths = section.GetBinaryPaths();
    auto section_name = section.GetName();
    for (auto& path : binary_paths) {
      auto resolved_path = Variables::ResolveVariables(path);
      auto it = resolved_binary_paths.find(resolved_path);
      if (it != resolved_binary_paths.end()) {
        LOG(WARNING) << "Binary path " << path << " already found from "
                     << it->second << ". Path from " << section_name
                     << " will be ignored.";
      } else {
        resolved_binary_paths[resolved_path] = section_name;
      }
    }
  }

  // Navigate in reverse order to keep sub directories on top of parent directory
  for (auto it = resolved_binary_paths.rbegin();
       it != resolved_binary_paths.rend();
       it++) {
    writer.WriteLine("dir.%s = %s", it->second.c_str(), it->first.c_str());
  }

  for (auto& section : sections_) {
    section.WriteConfig(writer);
  }
}

Section* Configuration::GetSection(const std::string& name) {
  for (auto& section : sections_) {
    if (section.GetName() == name) {
      return &section;
    }
  }

  return nullptr;
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android