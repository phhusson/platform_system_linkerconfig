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

#include "linkerconfig/baseconfig.h"
#include "linkerconfig/environment.h"
#include "linkerconfig/sectionbuilder.h"

using android::linkerconfig::modules::DirToSection;
using android::linkerconfig::modules::Section;

namespace {
void redirect_section(std::vector<DirToSection>& dirToSection,
                      const std::string& from, const std::string& to) {
  for (auto& [key, val] : dirToSection) {
    if (val == from) {
      val = to;
    }
  }
}
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
android::linkerconfig::modules::Configuration CreateBaseConfiguration(
    Context& ctx) {
  std::vector<Section> sections;

  if (android::linkerconfig::modules::IsVndkLiteDevice()) {
    ctx.SetCurrentLinkerConfigType(LinkerConfigType::Vndklite);
  } else {
    ctx.SetCurrentLinkerConfigType(LinkerConfigType::Default);
  }

  // Don't change the order here. The first pattern that matches with the
  // absolute path of an executable is selected.
  std::vector<DirToSection> dirToSection = {
      {"/system/bin/", "system"},
      {"/system/xbin/", "system"},
      {"/@{SYSTEM_EXT:system_ext}/bin/", "system"},

      // Processes from the product partition will have a separate section if
      // PRODUCT_PRODUCT_VNDK_VERSION is defined. Otherwise, they are run from
      // the "system" section.
      {"/@{PRODUCT:product}/bin/", "product"},

      {"/odm/bin/", "vendor"},
      {"/vendor/bin/", "vendor"},
      {"/data/nativetest/odm", "vendor"},
      {"/data/nativetest64/odm", "vendor"},
      {"/data/benchmarktest/odm", "vendor"},
      {"/data/benchmarktest64/odm", "vendor"},
      {"/data/nativetest/vendor", "vendor"},
      {"/data/nativetest64/vendor", "vendor"},
      {"/data/benchmarktest/vendor", "vendor"},
      {"/data/benchmarktest64/vendor", "vendor"},

      {"/data/nativetest/unrestricted", "unrestricted"},
      {"/data/nativetest64/unrestricted", "unrestricted"},

      // TODO(b/123864775): Ensure tests are run from /data/nativetest{,64} or
      // (if necessary) the unrestricted subdirs above. Then clean this up.
      {"/data/local/tmp", "unrestricted"},

      {"/postinstall", "postinstall"},
      // Fallback entry to provide APEX namespace lookups for binaries anywhere
      // else. This must be last.
      {"/data", "system"},
  };

  sections.emplace_back(BuildSystemSection(ctx));
  sections.emplace_back(BuildVendorSection(ctx));
  if (android::linkerconfig::modules::IsProductVndkVersionDefined() &&
      !android::linkerconfig::modules::IsVndkLiteDevice()) {
    sections.emplace_back(BuildProductSection(ctx));
  } else {
    redirect_section(dirToSection, "product", "system");
  }

  sections.emplace_back(BuildUnrestrictedSection(ctx));
  sections.emplace_back(BuildPostInstallSection(ctx));

  return android::linkerconfig::modules::Configuration(std::move(sections),
                                                       dirToSection);
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
