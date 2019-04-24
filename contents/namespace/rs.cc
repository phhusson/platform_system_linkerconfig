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

using android::linkerconfig::modules::CreateNamespace;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
std::shared_ptr<Namespace> BuildRsNamespace([[maybe_unused]] const Context& ctx) {
  auto ns = CreateNamespace("rs", true, true);

  ns->AddSearchPath("/odm/${LIB}/vndk-sp", true, true);
  ns->AddSearchPath("/vendor/${LIB}/vndk-sp", true, true);
  ns->AddSearchPath("/system/${LIB}/vndk-sp@{VNDK_VER}", true, true);
  ns->AddSearchPath("/odm/${LIB}", true, true);
  ns->AddSearchPath("/vendor/${LIB}", true, true);

  ns->AddPermittedPath("/odm/${LIB}", true, true);
  ns->AddPermittedPath("/vendor/${LIB}", true, true);
  ns->AddPermittedPath("/system/vendor/${LIB}", false, false);
  ns->AddPermittedPath("/data", true, false);

  ns->CreateLink("default")->AddSharedLib({"@{LLNDK_LIBRARIES}",
                                           "@{SANITIZER_RUNTIME_LIBRARIES}",
                                           "@{PRIVATE_LLNDK_LIBRARIES}"});
  ns->CreateLink("vndk")->AddSharedLib("@{VNDK_SAMEPROCESS_LIBRARIES}");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android