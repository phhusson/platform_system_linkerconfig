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

#include "linkerconfig/basecontext.h"

namespace android {
namespace linkerconfig {
namespace modules {
namespace {
// TODO(b/148826508): use apex_name directly as a name of an apex namespace
// after removing all references to the short names, remove this function.
std::string LinkerNamespaceNameFor(const ApexInfo& apex) {
  static const std::map<std::string, std::string> namespaceNameForApex = {
      {"com.android.art", "art"},
      {"com.android.conscrypt", "conscrypt"},
      {"com.android.cronet", "cronet"},
      {"com.android.media", "media"},
      {"com.android.neuralnetworks", "neuralnetworks"},
      {"com.android.runtime", "runtime"},
  };
  if (auto it = namespaceNameForApex.find(apex.name);
      it != namespaceNameForApex.end()) {
    return it->second;
  }
  return apex.name;
}
}  // namespace
BaseContext::BaseContext() : strict_(false) {
}

void BaseContext::AddApexModule(ApexInfo apex_module) {
  apex_modules_.push_back(std::move(apex_module));
}

const std::vector<ApexInfo>& BaseContext::GetApexModules() const {
  return apex_modules_;
}

void BaseContext::SetStrictMode(bool strict) {
  strict_ = strict;
}

bool BaseContext::IsStrictMode() const {
  return strict_;
}

Namespace BaseContext::BuildApexNamespace(const ApexInfo& apex_info,
                                          bool visible) const {
  Namespace ns(LinkerNamespaceNameFor(apex_info),
               /*is_isolated=*/true,
               visible);
  InitializeWithApex(ns, apex_info);
  return ns;
}

}  // namespace modules
}  // namespace linkerconfig
}  // namespace android
