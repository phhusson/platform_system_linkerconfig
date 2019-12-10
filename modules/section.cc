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

#include <unordered_map>

#include <android-base/result.h>

using android::base::Errorf;
using android::base::Result;

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

Result<void> Section::Resolve() {
  std::unordered_map<std::string, std::string> providers;
  for (auto& ns : namespaces_) {
    for (const auto& lib : ns.GetProvides()) {
      if (auto iter = providers.find(lib); iter != providers.end()) {
        return Errorf("duplicate: {} is provided by {} and {} in [{}]",
                      lib,
                      iter->second,
                      ns.GetName(),
                      name_);
      }
      providers[lib] = ns.GetName();
    }
  }
  for (auto& ns : namespaces_) {
    for (const auto& lib : ns.GetRequires()) {
      if (auto it = providers.find(lib); it != providers.end()) {
        ns.GetLink(it->second).AddSharedLib(lib);
      } else {
        return Errorf(
            "not found: {} is required by {} in [{}]", lib, ns.GetName(), name_);
      }
    }
  }
  return {};
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