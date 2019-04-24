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

#include "linkerconfig/namespacebuilder.h"

using android::linkerconfig::contents::SectionType;
using android::linkerconfig::modules::kDefaultPriority;
using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

const android::linkerconfig::modules::BinaryPathList kBinaryPath = {
    {"/postinstall", kDefaultPriority}};

namespace android {
namespace linkerconfig {
namespace contents {
std::shared_ptr<Section> BuildPostInstallSection(Context& ctx) {
  ctx.SetCurrentSection(SectionType::Other);
  std::vector<std::shared_ptr<Namespace>> namespaces;

  namespaces.push_back(BuildPostInstallNamespace(ctx));

  return std::make_shared<Section>("postinstall", kBinaryPath, namespaces);
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android