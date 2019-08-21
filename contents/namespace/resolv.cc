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

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace {
const std::vector<std::string> kLibsFromDefault = {"libc.so",
                                                   "libcgrouprc.so",
                                                   "libm.so",
                                                   "libdl.so",
                                                   "libbinder_ndk.so",
                                                   "liblog.so",
                                                   "libvndksupport.so"};

const std::vector<std::string> kLibsFromUnrestrictedDefault =
    {"libc.so", "libm.so", "libdl.so", "libbinder_ndk.so", "liblog.so"};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildResolvNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("resolv", /*is_isolated=*/true, /*is_visible=*/true);
  ns.AddSearchPath("/apex/com.android.resolv/${LIB}", AsanPath::SAME_PATH);
  ns.CreateLink("default").AddSharedLib(
      ctx.IsSystemSection() ? kLibsFromDefault : kLibsFromUnrestrictedDefault);

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android