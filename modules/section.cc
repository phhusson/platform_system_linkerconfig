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

#include "linkerconfig/section.h"

namespace android {
namespace linkerconfig {
namespace modules {
void Section::WriteConfig(ConfigWriter& writer) {
  writer.WriteLine("[%s]", name_.c_str());

  std::string additional_namespaces = "";

  bool is_first = true;
  for (auto& ns : namespaces_) {
    if (ns.GetName() != "default") {
      if (!is_first) {
        additional_namespaces += ",";
      }

      additional_namespaces += ns.GetName();
      is_first = false;
    }
  }

  if (!is_first) {
    writer.WriteLine("additional.namespaces = " + additional_namespaces);
  }

  for (auto& ns : namespaces_) {
    ns.WriteConfig(writer);
  }
}

std::vector<std::string> Section::GetBinaryPaths() {
  return binary_paths_;
}

Namespace* Section::GetNamespace(const std::string& namespace_name) {
  for (auto& ns : namespaces_) {
    if (ns.GetName() == namespace_name) {
      return &ns;
    }
  }

  return nullptr;
}

std::string Section::GetName() {
  return name_;
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android