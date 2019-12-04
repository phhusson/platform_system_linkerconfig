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

// This namespace exposes externally accessible libraries from the ART APEX.
// Keep in sync with the "art" namespace in art/build/apex/ld.config.txt.

#include "linkerconfig/namespacebuilder.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {

Namespace BuildArtNamespace([[maybe_unused]] const Context& ctx) {
  // Make the namespace visible to allow links to be created at runtime, e.g.
  // through android_link_namespaces in libnativeloader. That is not applicable
  // to the vendor section.
  Namespace ns("art",
               /*is_isolated=*/true,
               /*is_visible=*/!ctx.IsVendorSection());

  ns.AddSearchPath("/apex/com.android.art/${LIB}", AsanPath::SAME_PATH);
  ns.AddPermittedPath("/system/${LIB}");

  // Primary boot image is loaded through dlopen, so pass the primary boot image
  // to the list of paths.
  ns.AddPermittedPath("/apex/com.android.art/javalib", AsanPath::SAME_PATH);

  // Need allow_all_shared_libs to let libart.so dlopen oat files in
  // /system/framework and /data.
  // TODO(b/130340935): Use a dynamically created linker namespace similar to
  // classloader-namespace for oat files, and tighten this up.
  ns.GetLink(ctx.GetSystemNamespaceName()).AllowAllSharedLibs();

  ns.GetLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}

}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
