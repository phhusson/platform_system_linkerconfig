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

// This namespace is for libraries within the NNAPI APEX.

#include "linkerconfig/namespacebuilder.h"

#include <android-base/strings.h>

#include "linkerconfig/apex.h"
#include "linkerconfig/environment.h"
#include "linkerconfig/namespace.h"
#include "linkerconfig/variables.h"

using android::linkerconfig::modules::ApexInfo;
using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildApexPlatformNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("system", /*is_isolated=*/true, /*is_visible=*/false);

  ns.AddSearchPath("/system/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddPermittedPath("/apex/com.android.runtime/${LIB}", AsanPath::SAME_PATH);

  auto stub_libraries_var =
      android::linkerconfig::modules::Variables::GetValue("STUB_LIBRARIES");
  ns.AddProvides(android::base::Split(stub_libraries_var.value_or(""), ":"));

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
