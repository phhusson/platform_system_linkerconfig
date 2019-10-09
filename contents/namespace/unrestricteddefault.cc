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
#include "linkerconfig/namespace.h"

using android::linkerconfig::modules::AsanPath;
using android::linkerconfig::modules::Namespace;

namespace {
const std::vector<std::string> kLibsFromArt = {
    "libdexfile_external.so",
    "libdexfiled_external.so",
    "libnativebridge.so",
    "libnativehelper.so",
    "libnativeloader.so",
    "libandroidicu.so",
    "libpac.so",
    // TODO(b/120786417 or b/134659294): libicuuc.so and libicui18n.so are kept
    // for app compat.
    "libicui18n.so",
    "libicuuc.so"};
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildUnrestrictedDefaultNamespace([[maybe_unused]] const Context& ctx) {
  Namespace ns("default", /*is_isolated=*/false, /*is_visible=*/true);

  ns.AddSearchPath("/system/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/odm/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/vendor/${LIB}", AsanPath::WITH_DATA_ASAN);

  ns.GetLink("art").AddSharedLib(kLibsFromArt);
  ns.GetLink("resolv").AddSharedLib("libnetd_resolv.so");
  ns.GetLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
