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

#include <unordered_map>

#include "linkerconfig/log.h"
#include "linkerconfig/variables.h"

namespace android {
namespace linkerconfig {
namespace modules {
void Configuration::WriteConfig(ConfigWriter& writer) {
  std::unordered_map<std::string, std::string> resolved_dirs;

  for (auto& dir_to_section : dir_to_section_list_) {
    auto resolved_dir = Variables::ResolveVariables(dir_to_section.first);

    auto it = resolved_dirs.find(resolved_dir);

    if (it != resolved_dirs.end()) {
      LOG(WARNING) << "Binary path " << resolved_dir << " already found from "
                   << it->second << ". Path from " << dir_to_section.second
                   << " will be ignored.";
    } else {
      resolved_dirs[resolved_dir] = dir_to_section.second;
      writer.WriteLine(
          "dir.%s = %s", dir_to_section.second.c_str(), resolved_dir.c_str());
    }
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