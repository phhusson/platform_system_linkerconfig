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

#include <string>
#include <vector>

using android::linkerconfig::modules::Namespace;

namespace {
const std::vector<std::string> kLibsFromDefault = {"libc.so", "libm.so",
                                                   "libdl.so", "liblog.so"};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildConscryptNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("conscrypt", /*is_isolated=*/true,
               /*is_visible=*/true);

  ns.AddSearchPath("/apex/com.android.conscrypt/${LIB}", /*also_in_asan=*/true,
                   /*with_data_asan=*/false);
  ns.CreateLink("runtime").AddSharedLib("libandroidio.so");
  ns.CreateLink("default").AddSharedLib(kLibsFromDefault);

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android