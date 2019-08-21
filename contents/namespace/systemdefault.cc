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
const std::vector<std::string> kLibsFromRuntimeLegacy = {
    "libart.so:libartd.so",
    "libdexfile_external.so",
    "libnativebridge.so",
    "libnativehelper.so",
    "libnativeloader.so",
    "libandroidicu.so",
    // TODO(b/122876336): Remove libpac.so once it's migrated to Webview
    "libpac.so"};

const std::vector<std::string> kLibsFromRuntime = {
    "libdexfile_external.so",
    "libdexfiled_external.so",
    "libnativebridge.so",
    "libnativehelper.so",
    "libnativeloader.so",
    "libandroidicu.so",
    "libpac.so",
    "@{SANITIZER_RUNTIME_LIBRARIES}",
    // TODO(b/120786417 or b/134659294): libicuuc.so and libicui18n.so are kept
    // for app compat.
    "libicui18n.so",
    "libicuuc.so"};

const std::vector<std::string> kPermittedPaths = {
    "/system/${LIB}/drm",
    "/system/${LIB}/extractors",
    "/system/${LIB}/hw",
    "/@{SYSTEM_EXT:system_ext}/${LIB}",
    "/@{PRODUCT:product}/${LIB}",
    // These are where odex files are located. libart has to be able to
    // dlopen the files
    "/system/framework",
    "/system/app",
    "/system/priv-app",
    "/@{SYSTEM_EXT:system_ext}/framework",
    "/@{SYSTEM_EXT:system_ext}/app",
    "/@{SYSTEM_EXT:system_ext}/priv-app",
    "/vendor/framework",
    "/vendor/app",
    "/vendor/priv-app",
    "/system/vendor/framework",
    "/system/vendor/app",
    "/system/vendor/priv-app",
    "/odm/framework",
    "/odm/app",
    "/odm/priv-app",
    "/oem/app",
    "/@{PRODUCT:product}/framework",
    "/@{PRODUCT:product}/app",
    "/@{PRODUCT:product}/priv-app",
    "/data",
    "/mnt/expand",
    "/apex/com.android.runtime/${LIB}/bionic",
    "/system/${LIB}/bootstrap"};

void BuildPermittedPath(Namespace& ns) {
  for (const auto& path : kPermittedPaths) {
    ns.AddPermittedPath(path, AsanPath::SAME_PATH);
  }
}
}  // namespace

namespace android {
namespace linkerconfig {
namespace contents {
Namespace BuildSystemDefaultNamespace([[maybe_unused]] const Context& ctx) {
  bool is_legacy = android::linkerconfig::modules::IsLegacyDevice();
  Namespace ns("default", /*is_isolated=*/!is_legacy,
               /*is_visible=*/true);

  ns.AddSearchPath("/system/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/@{SYSTEM_EXT:system_ext}/${LIB}", AsanPath::WITH_DATA_ASAN);
  ns.AddSearchPath("/@{PRODUCT:product}/${LIB}", AsanPath::WITH_DATA_ASAN);
  if (is_legacy) {
    ns.AddSearchPath("/vendor/${LIB}", AsanPath::WITH_DATA_ASAN);
    ns.AddSearchPath("/odm/${LIB}", AsanPath::WITH_DATA_ASAN);
  }

  if (!is_legacy) {
    BuildPermittedPath(ns);
  }

  ns.CreateLink("runtime").AddSharedLib(is_legacy ? kLibsFromRuntimeLegacy
                                                  : kLibsFromRuntime);
  ns.CreateLink("resolv").AddSharedLib("libnetd_resolv.so");
  ns.CreateLink("neuralnetworks").AddSharedLib("libneuralnetworks.so");

  return ns;
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android