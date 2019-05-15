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

#include "linkerconfig/environment.h"

using android::linkerconfig::modules::Namespace;

namespace {
const std::vector<std::string> kLibsFromDefaultLegacy = {
    "libandroid.so",
    "libbinder_ndk.so",
    "libc.so",
    "libcgrouprc.so",
    "libdl.so",
    "liblog.so",
    "libmediametrics.so",
    "libmediandk.so",
    "libm.so",
    "libvndksupport.so",
    "libclang_rt.asan-aarch64-android.so",
    "libclang_rt.asan-arm-android.so",
    "libclang_rt.asan-i686-android.so",
    "libclang_rt.asan-x86_64-android.so",
    "libclang_rt.hwasan-aarch64-android.so"};

const std::vector<std::string> kLibsFromDefault = {
    "@{LLNDK_LIBRARIES}", "libandroid.so", "libbinder_ndk.so",
    "libmediametrics.so", "@{SANITIZER_RUNTIME_LIBRARIES}"};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildMediaNamespace([[maybe_unused]] const Context& ctx) {
  bool is_legacy = android::linkerconfig::modules::IsLegacyDevice();
  Namespace ns("media", /*is_isolated=*/true, /*is_visible=*/true);
  ns.AddSearchPath("/apex/com.android.media/${LIB}", /*also_in_asan=*/true,
                   /*with_data_asan=*/false);
  ns.AddPermittedPath("/apex/com.android.media/${LIB}/extractors",
                      /*also_in_asan=*/false, /*with_data_asan=*/false);
  ns.CreateLink("default").AddSharedLib(is_legacy ? kLibsFromDefaultLegacy
                                                  : kLibsFromDefault);

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android