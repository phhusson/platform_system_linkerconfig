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

#include "linkerconfig/legacy.h"
#include "linkerconfig/sectionbuilder.h"

using android::linkerconfig::modules::DirToSection;
using android::linkerconfig::modules::Section;

namespace {
const std::vector<DirToSection> kDirToSection = {
    // All binaries gets the same configuration 'legacy'
    {"/system", "legacy"},
    {"/product", "legacy"},
    {"/vendor", "legacy"},
    {"/odm", "legacy"},
    {"/sbin", "legacy"},
    // Except for /postinstall, where only /system and /product are searched
    {"/postinstall", "postinstall"},
    // Fallback entry to provide APEX namespace lookups for binaries anywhere
    // else. This must be last.
    {"/data", "legacy"},
};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
android::linkerconfig::modules::Configuration CreateLegacyConfiguration() {
  std::vector<Section> sections;
  Context current_context;

  sections.emplace_back(BuildLegacySection(current_context));
  sections.emplace_back(BuildPostInstallSection(current_context));

  return android::linkerconfig::modules::Configuration(std::move(sections),
                                                       kDirToSection);
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android