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

#include "linkerconfig/context.h"

using android::linkerconfig::modules::ApexInfo;
namespace android {
namespace linkerconfig {
namespace contents {
bool Context::IsSystemSection() const {
  return current_section_ == SectionType::System;
}

bool Context::IsVendorSection() const {
  return current_section_ == SectionType::Vendor;
}

bool Context::IsProductSection() const {
  return current_section_ == SectionType::Product;
}

bool Context::IsDefaultConfig() const {
  return current_linkerconfig_type_ == LinkerConfigType::Default;
}

bool Context::IsLegacyConfig() const {
  return current_linkerconfig_type_ == LinkerConfigType::Legacy;
}

bool Context::IsVndkliteConfig() const {
  return current_linkerconfig_type_ == LinkerConfigType::Vndklite;
}

bool Context::IsRecoveryConfig() const {
  return current_linkerconfig_type_ == LinkerConfigType::Recovery;
}

void Context::SetCurrentSection(SectionType section_type) {
  current_section_ = section_type;
}

std::string Context::GetSystemNamespaceName() const {
  return (IsVendorSection() || IsProductSection()) && !IsVndkliteConfig()
             ? "system"
             : "default";
}

void Context::SetCurrentLinkerConfigType(LinkerConfigType config_type) {
  current_linkerconfig_type_ = config_type;
}

void Context::AddApexModule(ApexInfo apex_module) {
  apex_modules_.push_back(std::move(apex_module));
}

const std::vector<ApexInfo>& Context::GetApexModules() const {
  return apex_modules_;
}

}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
