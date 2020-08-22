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

#include "linkerconfig/sectionbuilder.h"

#include <functional>
#include <vector>

#include "linkerconfig/common.h"
#include "linkerconfig/log.h"
#include "linkerconfig/namespacebuilder.h"
#include "linkerconfig/section.h"

using android::linkerconfig::contents::SectionType;
using android::linkerconfig::modules::ApexInfo;
using android::linkerconfig::modules::LibProvider;
using android::linkerconfig::modules::LibProviders;
using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

namespace android {
namespace linkerconfig {
namespace contents {

// Builds default section for the apex
// For com.android.foo,
//   dir.com.android.foo = /apex/com.android.foo/bin
//   [com.android.foo]
//   additional.namespaces = system
//   namespace.default....
//   namespace.system...
Section BuildApexDefaultSection(Context& ctx, const ApexInfo& apex_info) {
  std::vector<Namespace> namespaces;

  ctx.SetCurrentSection(SectionType::Other);

  namespaces.emplace_back(BuildApexDefaultNamespace(ctx, apex_info));
  namespaces.emplace_back(BuildApexPlatformNamespace(ctx));

  LibProviders libs_providers;
  libs_providers[":sphal"] = LibProvider{
      "sphal",
      std::bind(BuildSphalNamespace, ctx),
      {},
  };
  if (ctx.IsVndkAvailable()) {
    VndkUserPartition user_partition = VndkUserPartition::Vendor;
    std::string user_partition_suffix = "VENDOR";
    if (apex_info.InProduct()) {
      user_partition = VndkUserPartition::Product;
      user_partition_suffix = "PRODUCT";
    }
    libs_providers[":vndk"] = LibProvider{
        "vndk",
        std::bind(BuildVndkNamespace, ctx, user_partition),
        {Var("VNDK_SAMEPROCESS_LIBRARIES_" + user_partition_suffix),
         Var("VNDK_CORE_LIBRARIES_" + user_partition_suffix)},
    };
    libs_providers[":vndksp"] = LibProvider{
        "vndk",
        std::bind(BuildVndkNamespace, ctx, user_partition),
        {Var("VNDK_SAMEPROCESS_LIBRARIES_" + user_partition_suffix)},
    };
  }
  return BuildSection(
      ctx, apex_info.name, std::move(namespaces), {}, libs_providers);
}

}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
