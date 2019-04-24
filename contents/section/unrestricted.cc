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

#include "linkerconfig/sectionbuilder.h"

#include "linkerconfig/environment.h"
#include "linkerconfig/namespacebuilder.h"
#include "linkerconfig/section.h"

using android::linkerconfig::contents::SectionType;
using android::linkerconfig::modules::kDefaultPriority;
using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

const android::linkerconfig::modules::BinaryPathList kBinaryPath = {
    {"/data/nativetest/unrestricted", kDefaultPriority},
    {"/data/nativetest64/unrestricted", kDefaultPriority},
    {"/data/local/tmp", kDefaultPriority}};

namespace android {
namespace linkerconfig {
namespace contents {
std::shared_ptr<Section> BuildUnrestrictedSection(Context& ctx) {
  ctx.SetCurrentSection(SectionType::Other);
  std::vector<std::shared_ptr<Namespace>> namespaces;

  namespaces.push_back(BuildUnrestrictedDefaultNamespace(ctx));
  namespaces.push_back(BuildRuntimeNamespace(ctx));
  namespaces.push_back(BuildMediaNamespace(ctx));
  namespaces.push_back(BuildConscryptNamespace(ctx));
  namespaces.push_back(BuildResolvNamespace(ctx));

  return std::make_shared<Section>("unrestricted", kBinaryPath, namespaces);
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android