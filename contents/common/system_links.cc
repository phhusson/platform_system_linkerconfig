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

#include <string>

#include <android-base/strings.h>

#include "linkerconfig/common.h"
#include "linkerconfig/context.h"
#include "linkerconfig/section.h"
#include "linkerconfig/variables.h"

namespace android {
namespace linkerconfig {
namespace contents {

using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

void AddStandardSystemLinks(const Context& ctx, Section* section) {
  std::string system_ns_name = ctx.GetSystemNamespaceName();
  section->ForEachNamespaces([system_ns_name](Namespace& ns) {
    if (ns.GetName() != system_ns_name) {
      ns.GetLink(system_ns_name)
          .AddSharedLib("@{STUB_LIBRARIES}", "@{SANITIZER_RUNTIME_LIBRARIES}");
    }
  });

  Namespace* system_ns = section->GetNamespace(system_ns_name);
  if (system_ns) {
    std::optional<std::string> stub_libraries_var =
        android::linkerconfig::modules::Variables::GetValue("STUB_LIBRARIES");
    system_ns->AddProvides(
        android::base::Split(stub_libraries_var.value_or(""), ":"));
  }
}
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
