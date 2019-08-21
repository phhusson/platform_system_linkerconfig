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

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildVndkInSystemNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("vndk_in_system", /*is_isolated=*/true,
               /*is_visible=*/true);

  ns.AddSearchPath("/system/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/@{SYSTEM_EXT:system_ext}/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/@{PRODUCT:product}/${LIB}", AsanPath::WITH_DATA_ASAN);

  if (android::linkerconfig::modules::IsVndkInSystemNamespace()) {
    ns.AddWhitelisted("@{VNDK_USING_CORE_VARIANT_LIBRARIES}");
  }

  ns.CreateLink("system").AddSharedLib(
      {"@{LLNDK_LIBRARIES}", "@{SANITIZER_RUNTIME_LIBRARIES}"});
  ns.CreateLink("vndk", true);
  ns.CreateLink("runtime").AddSharedLib("@{SANITIZER_RUNTIME_LIBRARIES}");
  ns.CreateLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android