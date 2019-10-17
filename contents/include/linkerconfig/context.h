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
#pragma once

#include <string>

namespace android {
namespace linkerconfig {
namespace contents {

enum class SectionType {
  System,
  Vendor,
  Other,
};

enum class LinkerConfigType {
  Default,
  Legacy,
  Vndklite,
  Recovery,
};

class Context {
 public:
  Context()
      : current_section(SectionType::System),
        current_linkerconfig_type(LinkerConfigType::Default) {
  }
  bool IsSystemSection() const;
  bool IsVendorSection() const;

  bool IsDefaultConfig() const;
  bool IsLegacyConfig() const;
  bool IsVndkliteConfig() const;
  bool IsRecoveryConfig() const;

  void SetCurrentSection(SectionType value);
  void SetCurrentLinkerConfigType(LinkerConfigType value);

  // Returns the namespace that covers /system/${LIB}.
  std::string GetSystemNamespaceName() const;

 private:
  SectionType current_section;
  LinkerConfigType current_linkerconfig_type;
};
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
