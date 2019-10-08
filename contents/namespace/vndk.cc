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
Namespace BuildVndkNamespace([[maybe_unused]] const Context& ctx) {
  bool is_system_section = ctx.IsSystemSection();
  Namespace ns("vndk",
               /*is_isolated=*/is_system_section,
               /*is_visible=*/is_system_section);

  ns.AddSearchPath("/odm/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/vendor/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/system/${LIB}/vndk-sp-@{VNDK_VER}",
                   AsanPath::WITH_DATA_ASAN);

  if (!is_system_section) {
    ns.AddSearchPath("/odm/${LIB}/vndk", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/vendor/${LIB}/vndk", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/system/${LIB}/vndk-@{VNDK_VER}",
                     AsanPath::WITH_DATA_ASAN);
  }

  if (is_system_section) {
    ns.AddPermittedPath("/odm/${LIB}/hw", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/odm/${LIB}/egl", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/vendor/${LIB}/hw", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/vendor/${LIB}/egl", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/system/vendor/${LIB}/hw", AsanPath::NONE);
    ns.AddPermittedPath("/system/vendor/${LIB}/egl", AsanPath::NONE);
    ns.AddPermittedPath("/system/${LIB}/vndk-sp-@{VNDK_VER}/hw",
                        AsanPath::WITH_DATA_ASAN);
  }

  ns.GetLink(ctx.GetSystemNamespaceName()).AddSharedLib({"@{LLNDK_LIBRARIES}"});

  if (is_system_section) {
    ns.GetLink("sphal").AllowAllSharedLibs();
  } else {
    ns.GetLink("default").AllowAllSharedLibs();

    if (android::linkerconfig::modules::IsVndkInSystemNamespace()) {
      ns.GetLink("vndk_in_system")
          .AddSharedLib("@{VNDK_USING_CORE_VARIANT_LIBRARIES}");
    }
  }

  ns.GetLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
