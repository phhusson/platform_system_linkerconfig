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
#pragma once

#include "linkerconfig/context.h"
#include "linkerconfig/namespace.h"

typedef android::linkerconfig::modules::Namespace NamespaceBuilder(
    const android::linkerconfig::contents::Context& ctx);

namespace android {
namespace linkerconfig {
namespace contents {
NamespaceBuilder BuildSystemDefaultNamespace;
NamespaceBuilder BuildMediaNamespace;
NamespaceBuilder BuildRuntimeNamespace;
NamespaceBuilder BuildConscryptNamespace;
NamespaceBuilder BuildResolvNamespace;
NamespaceBuilder BuildSphalNamespace;
NamespaceBuilder BuildRsNamespace;
NamespaceBuilder BuildVndkNamespace;
NamespaceBuilder BuildVendorDefaultNamespace;
NamespaceBuilder BuildSystemNamespace;
NamespaceBuilder BuildVndkInSystemNamespace;
NamespaceBuilder BuildUnrestrictedDefaultNamespace;
NamespaceBuilder BuildPostInstallNamespace;
}  // namespace contents
}  // namespace linkerconfig
}  // namespace android