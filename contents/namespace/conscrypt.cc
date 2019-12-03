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

// This namespace is for libraries within the conscrypt APEX.
// Keep in sync with the "conscrypt" namespace in art/build/apex/ld.config.txt.

#include "linkerconfig/namespacebuilder.h"

#include <string>
#include <vector>

#include "linkerconfig/environment.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildConscryptNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("conscrypt", /*is_isolated=*/true,
               /*is_visible=*/true);

  ns.AddSearchPath("/apex/com.android.conscrypt/${LIB}", AsanPath::SAME_PATH);
  ns.AddPermittedPath("/system/${LIB}");
  ns.GetLink("art").AddSharedLib("libandroidio.so");
  ns.GetLink(ctx.GetSystemNamespaceName()).AddSharedLib("liblog.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
