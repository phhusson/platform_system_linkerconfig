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

// This namespace is exclusively for vndk-sp libs.

#include "linkerconfig/namespacebuilder.h"

#include "linkerconfig/environment.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildVndkNamespace([[maybe_unused]] const Context& ctx) {
  bool is_system_section = ctx.IsSystemSection();
  bool is_product_section = ctx.IsProductSection();
  bool is_vndklite = ctx.IsVndkliteConfig();
  // Isolated but visible when used in the [system] section to allow links to be
  // created at runtime, e.g. through android_link_namespaces in
  // libnativeloader. Otherwise it isn't isolated, so visibility doesn't matter.
  Namespace ns("vndk",
               /*is_isolated=*/is_system_section,
               /*is_visible=*/is_system_section);

  if (is_system_section) {
    // It is linked from vendor HAL. It must use vendor vndk libs.
    ns.AddSearchPath("/odm/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/vendor/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/apex/com.android.vndk.v@{VENDOR_VNDK_VERSION}/${LIB}",
                     AsanPath::SAME_PATH);
  } else if (is_product_section) {
    ns.AddSearchPath("/product/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/product/${LIB}/vndk", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/apex/com.android.vndk.v@{PRODUCT_VNDK_VERSION}/${LIB}",
                     AsanPath::SAME_PATH);
  } else {
    ns.AddSearchPath("/odm/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/odm/${LIB}/vndk", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/vendor/${LIB}/vndk-sp", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/vendor/${LIB}/vndk", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/apex/com.android.vndk.v@{VENDOR_VNDK_VERSION}/${LIB}",
                     AsanPath::SAME_PATH);
  }

  if (is_system_section) {
    ns.AddPermittedPath("/odm/${LIB}/hw", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/odm/${LIB}/egl", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/vendor/${LIB}/hw", AsanPath::WITH_DATA_ASAN);
    ns.AddPermittedPath("/vendor/${LIB}/egl", AsanPath::WITH_DATA_ASAN);
    if (!is_vndklite) {
      ns.AddPermittedPath("/system/vendor/${LIB}/hw", AsanPath::NONE);
    }
    ns.AddPermittedPath("/system/vendor/${LIB}/egl", AsanPath::NONE);

    // This is exceptionally required since android.hidl.memory@1.0-impl.so is here
    ns.AddPermittedPath(
        "/apex/com.android.vndk.v@{VENDOR_VNDK_VERSION}/${LIB}/hw",
        AsanPath::SAME_PATH);
  }

  // For the [vendor] section, the links should be identical to that of the
  // 'vndk_in_system' namespace, except the links to 'default' and 'vndk_in_system'.

  if (is_product_section) {
    ns.GetLink(ctx.GetSystemNamespaceName())
        .AddSharedLib({"@{LLNDK_LIBRARIES_PRODUCT}"});
  } else {
    ns.GetLink(ctx.GetSystemNamespaceName())
        .AddSharedLib({"@{LLNDK_LIBRARIES_VENDOR}"});
  }

  if (!is_vndklite) {
    if (is_system_section) {
      // The "vndk" namespace links to the system namespace for LLNDK libs above
      // and links to "sphal" namespace for vendor libs. The ordering matters;
      // the system namespace has higher priority than the "sphal" namespace.
      ns.GetLink("sphal").AllowAllSharedLibs();
    } else {
      // [vendor] or [product] section
      ns.GetLink("default").AllowAllSharedLibs();

      if (android::linkerconfig::modules::IsVndkInSystemNamespace()) {
        ns.GetLink("vndk_in_system")
            .AddSharedLib("@{VNDK_USING_CORE_VARIANT_LIBRARIES}");
      }
    }
  }

  ns.GetLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
