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

#include "linkerconfig/namespacebuilder.h"

#include "linkerconfig/environment.h"

using android::linkerconfig::modules::CreateNamespace;
using android::linkerconfig::modules::GetVendorVndkVersion;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
std::shared_ptr<Namespace> BuildVendorDefaultNamespace([
    [maybe_unused]] const Context& ctx) {
  auto ns = CreateNamespace("default", true, true);

  ns->AddSearchPath("/odm/${LIB}", true, true);
  ns->AddSearchPath("/vendor/${LIB}", true, true);

  if (GetVendorVndkVersion() == "27") {
    ns->AddSearchPath("/vendor/${LIB}/hw", true, true);
    ns->AddSearchPath("/vendor/${LIB}/egl", true, true);
  }

  ns->AddPermittedPath("/odm", true, true);
  ns->AddPermittedPath("/vendor", true, true);
  ns->AddPermittedPath("/system/vendor", false, false);

  ns->CreateLink("system")->AddSharedLib("@{LLNDK_LIBRARIES}");
  ns->CreateLink("vndk")->AddSharedLib(
      {"@{VNDK_SAMEPROCESS_LIBRARIES", "@{VNDK_CORE_LIBRARIES}"});
  if (android::linkerconfig::modules::IsVndkInSystemNamespace()) {
    ns->CreateLink("vndk_in_system")
        ->AddSharedLib("@{VNDK_USING_CORE_VARIANT_LIBRARIES}");
  }

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android