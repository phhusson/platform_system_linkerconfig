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

// Linker config for native tests that need access to both system and vendor
// libraries. This replicates the default linker config (done by
// init_default_namespace_no_config in bionic/linker/linker.cpp), except that it
// includes the requisite namespace setup for APEXes.

#include "linkerconfig/sectionbuilder.h"

#include "linkerconfig/common.h"
#include "linkerconfig/environment.h"
#include "linkerconfig/namespacebuilder.h"
#include "linkerconfig/section.h"

using android::linkerconfig::contents::SectionType;
using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

namespace android {
namespace linkerconfig {
namespace contents {
Section BuildUnrestrictedSection(Context& ctx) {
  ctx.SetCurrentSection(SectionType::Other);
  std::vector<Namespace> namespaces;

  namespaces.emplace_back(BuildUnrestrictedDefaultNamespace(ctx));
  namespaces.emplace_back(BuildArtNamespace(ctx));
  namespaces.emplace_back(BuildMediaNamespace(ctx));
  namespaces.emplace_back(BuildConscryptNamespace(ctx));
  namespaces.emplace_back(BuildResolvNamespace(ctx));
  namespaces.emplace_back(BuildNeuralNetworksNamespace(ctx));
  namespaces.emplace_back(BuildRuntimeNamespace(ctx));

  Section section("unrestricted", std::move(namespaces));
  AddStandardSystemLinks(ctx, &section);
  return section;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
