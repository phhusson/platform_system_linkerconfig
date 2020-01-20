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

#include "linkerconfig/variableloader.h"

#include <android-base/result.h>
#include <climits>
#include <cstdlib>
#include <cstring>

#include "linkerconfig/environment.h"
#include "linkerconfig/librarylistloader.h"
#include "linkerconfig/log.h"
#include "linkerconfig/variables.h"

using android::base::ErrnoErrorf;
using android::base::Result;
using android::linkerconfig::modules::GetProductVndkVersion;
using android::linkerconfig::modules::GetVendorVndkVersion;
using android::linkerconfig::modules::Variables;

namespace {
using namespace android::linkerconfig::generator;

void LoadVndkVersionVariable() {
  Variables::AddValue("VENDOR_VNDK_VERSION", GetVendorVndkVersion());
  Variables::AddValue("PRODUCT_VNDK_VERSION", GetProductVndkVersion());
}

Result<std::string> GetRealPath(std::string target_path) {
  char resolved_path[PATH_MAX];
  if (realpath(target_path.c_str(), resolved_path) != nullptr) {
    int start_index = 0;
    if (resolved_path[0] == '/') {
      start_index = 1;
    }
    return &resolved_path[start_index];
  }

  return ErrnoErrorf("Failed to get realpath from {}", target_path);
}

void LoadVariableFromPartitionPath(std::string variable_name, std::string path) {
  auto real_path = GetRealPath(path);

  if (real_path) {
    Variables::AddValue(variable_name, *real_path);
  } else {
    LOG(WARNING) << real_path.error();
  }
}

void LoadPartitionPathVariables(const std::string& root) {
  // TODO(b/141714913): generalize path handling
  LoadVariableFromPartitionPath("PRODUCT", root + "/product");
  LoadVariableFromPartitionPath("SYSTEM_EXT", root + "/system_ext");
}

void LoadLibraryListVariables(const std::string& root) {
  auto private_library_path_vendor = root +
                                     "/system/etc/vndkprivate.libraries." +
                                     GetVendorVndkVersion() + ".txt";
  auto llndk_library_path_vendor =
      root + "/system/etc/llndk.libraries." + GetVendorVndkVersion() + ".txt";
  auto vndksp_library_path_vendor =
      root + "/system/etc/vndksp.libraries." + GetVendorVndkVersion() + ".txt";
  auto vndkcore_library_path_vendor = root + "/system/etc/vndkcore.libraries." +
                                      GetVendorVndkVersion() + ".txt";

  auto private_library_path_product = root +
                                      "/system/etc/vndkprivate.libraries." +
                                      GetProductVndkVersion() + ".txt";
  auto llndk_library_path_product =
      root + "/system/etc/llndk.libraries." + GetProductVndkVersion() + ".txt";
  auto vndksp_library_path_product =
      root + "/system/etc/vndksp.libraries." + GetProductVndkVersion() + ".txt";
  auto vndkcore_library_path_product = root + "/system/etc/vndkcore.libraries." +
                                       GetProductVndkVersion() + ".txt";

  auto vndkcorevariant_library_path =
      root + "/system/etc/vndkcorevariant.libraries.txt";
  auto sanitizer_library_path = root + "/system/etc/sanitizer.libraries.txt";

  Variables::AddValue("LLNDK_LIBRARIES_VENDOR",
                      GetPublicLibrariesString(llndk_library_path_vendor,
                                               private_library_path_vendor));

  Variables::AddValue("PRIVATE_LLNDK_LIBRARIES_VENDOR",
                      GetPrivateLibrariesString(llndk_library_path_vendor,
                                                private_library_path_vendor));

  Variables::AddValue("VNDK_SAMEPROCESS_LIBRARIES_VENDOR",
                      GetPublicLibrariesString(vndksp_library_path_vendor,
                                               private_library_path_vendor));

  Variables::AddValue("VNDK_CORE_LIBRARIES_VENDOR",
                      GetPublicLibrariesString(vndkcore_library_path_vendor,
                                               private_library_path_vendor));

  if (GetProductVndkVersion() != "") {
    Variables::AddValue("LLNDK_LIBRARIES_PRODUCT",
                        GetPublicLibrariesString(llndk_library_path_product,
                                                 private_library_path_product));

    Variables::AddValue("PRIVATE_LLNDK_LIBRARIES_PRODUCT",
                        GetPrivateLibrariesString(llndk_library_path_product,
                                                  private_library_path_product));

    Variables::AddValue("VNDK_SAMEPROCESS_LIBRARIES_PRODUCT",
                        GetPublicLibrariesString(vndksp_library_path_product,
                                                 private_library_path_product));

    Variables::AddValue("VNDK_CORE_LIBRARIES_PRODUCT",
                        GetPublicLibrariesString(vndkcore_library_path_product,
                                                 private_library_path_product));
  }

  Variables::AddValue("VNDK_USING_CORE_VARIANT_LIBRARIES",
                      GetPublicLibrariesString(vndkcorevariant_library_path,
                                               private_library_path_vendor));

  Variables::AddValue("SANITIZER_RUNTIME_LIBRARIES",
                      GetLibrariesString(sanitizer_library_path));
}
}  // namespace

namespace android {
namespace linkerconfig {
namespace generator {

void LoadVariables(const std::string& root) {
  LoadVndkVersionVariable();
  LoadPartitionPathVariables(root);
  LoadLibraryListVariables(root);
}

}  // namespace generator
}  // namespace linkerconfig
}  // namespace android
