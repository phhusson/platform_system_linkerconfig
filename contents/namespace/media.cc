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

// This namespace is for libraries within the media APEX.

#include "linkerconfig/namespacebuilder.h"

#include <string>
#include <vector>

#include "linkerconfig/environment.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Link;
using android::linkerconfig::modules::Namespace;

namespace {
const std::vector<std::string> kLibsFromDefaultLegacy = {"libandroid.so",
                                                         "libbinder_ndk.so",
                                                         "libcgrouprc.so",
                                                         "liblog.so",
                                                         "libmediametrics.so",
                                                         "libmediandk.so",
                                                         "libvndksupport.so"};

const std::vector<std::string> kLibsFromDefault = {"@{LLNDK_LIBRARIES}",
                                                   "libbinder_ndk.so",
                                                   "libmediametrics.so"};

const std::vector<std::string> kLibsFromDefaultSystem = {"libcgrouprc.so"};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildMediaNamespace([[maybe_unused]] const Context& ctx) {
  bool is_legacy = ctx.IsLegacyConfig();
  bool is_vndklite = ctx.IsVndkliteConfig();
  bool is_system_section = ctx.IsSystemSection();

  Namespace ns("media", /*is_isolated=*/true, /*is_visible=*/true);
  ns.AddSearchPath("/apex/com.android.media/${LIB}", AsanPath::SAME_PATH);
  ns.AddPermittedPath("/system/${LIB}");
  ns.AddPermittedPath(
      "/apex/com.android.media/${LIB}/extractors",
      (is_legacy || is_vndklite) ? AsanPath::NONE : AsanPath::SAME_PATH);

  Link& system_link = ns.GetLink(ctx.GetSystemNamespaceName());
  if (is_legacy) {
    system_link.AddSharedLib(kLibsFromDefaultLegacy);
  } else {
    system_link.AddSharedLib(kLibsFromDefault);
    if (is_system_section && !is_vndklite) {
      system_link.AddSharedLib(kLibsFromDefaultSystem);
    }
  }

  ns.GetLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
