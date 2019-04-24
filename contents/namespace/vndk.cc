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
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
std::shared_ptr<Namespace> BuildVndkNamespace([
    [maybe_unused]] const Context& ctx) {
  bool is_system_section = ctx.IsSystemSection();
  auto ns = CreateNamespace("vndk", true, true);

  ns->AddSearchPath("/odm/${LIB}/vndk-sp", true, true);
  ns->AddSearchPath("/vendor/${LIB}/vndk-sp", true, true);
  ns->AddSearchPath("/system/${LIB}/vndk-sp@{VNDK_VER}", true, true);

  if (!is_system_section) {
    ns->AddSearchPath("/odm/${LIB}/vndk", true, true);
    ns->AddSearchPath("/vendor/${LIB}/vndk", true, true);
    ns->AddSearchPath("/system/${LIB}/vndk@{VNDK_VER}", true, true);
  }

  if (is_system_section) {
    ns->AddPermittedPath("/odm/${LIB}/hw", true, true);
    ns->AddPermittedPath("/odm/${LIB}/egl", true, true);
    ns->AddPermittedPath("/vendor/${LIB}/hw", true, true);
    ns->AddPermittedPath("/vendor/${LIB}/egl", true, true);
    ns->AddPermittedPath("/system/vendor/${LIB}/hw", false, false);
    ns->AddPermittedPath("/system/vendor/${LIB}/egl", false, false);
    ns->AddPermittedPath("/system/${LIB}/vndk-sp@{VNDK_VER}/hw", true, true);
  }

  ns->CreateLink(is_system_section ? "default" : "system")
      ->AddSharedLib({"@{LLNDK_LIBRARIES}", "@{SANITIZER_RUNTIME_LIBRARIES}"});
  if (is_system_section) {
    ns->CreateLink("sphal", true);
  } else {
    ns->CreateLink("default", true);

    if (android::linkerconfig::modules::IsVndkInSystemNamespace()) {
      ns->CreateLink("vndk_in_system")
          ->AddSharedLib("@{VNDK_USING_CORE_VARIANT_LIBRARIES");
    }
  }

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android