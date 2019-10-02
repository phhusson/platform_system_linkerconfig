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

#include "linkerconfig/common.h"

#include <string>

#include "linkerconfig/context.h"
#include "linkerconfig/section.h"

namespace android {
namespace linkerconfig {
namespace contents {

using android::linkerconfig::modules::Namespace;
using android::linkerconfig::modules::Section;

void AddStandardSystemLinks(const Context& ctx, Section* section) {
  std::string system_ns_name = ctx.GetSystemNamespaceName();
  Namespace* system_ns = section->GetNamespace(system_ns_name);
  for (Namespace& ns : section->GetNamespaces()) {
    if (&ns != system_ns) {
      ns.GetLink(system_ns_name)
          .AddSharedLib({"libc.so",
                         "libm.so",
                         "libdl.so",
                         "@{SANITIZER_RUNTIME_LIBRARIES}"});
    }
  }
}

}  // namespace contents
}  // namespace linkerconfig
}  // namespace android
