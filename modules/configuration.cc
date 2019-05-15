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

namespace android {
namespace linkerconfig {
namespace modules {
void Configuration::WriteConfig(ConfigWriter& writer) {
  BinaryPathMap binary_paths;

  for (auto& section : sections_) {
    section.CollectBinaryPaths(binary_paths);
  }

  // Navigate in reverse order to keep sub directories on top of parent directory
  for (auto it = binary_paths.rbegin(); it != binary_paths.rend(); it++) {
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