/*
 * Copyright (C) 2020 The Android Open Source Project
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

// This namespace is for libraries within the adbd APEX.

#include "linkerconfig/namespacebuilder.h"

#include "linkerconfig/environment.h"
#include "linkerconfig/namespace.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildAdbdNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("adbd", /*is_isolated=*/true, /*is_visible=*/false);
  ns.AddSearchPath("/apex/com.android.adbd/${LIB}", AsanPath::SAME_PATH);
  ns.AddPermittedPath("/system/${LIB}");

  ns.AddProvides(std::vector{
      "libadbconnection_client.so",
  });
  ns.AddRequires(std::vector{
      "libc.so",
      "libdl.so",
      "liblog.so",
      "libm.so",
  });
  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
