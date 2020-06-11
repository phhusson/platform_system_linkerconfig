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

#include <gtest/gtest.h>

#include "linkerconfig/apex.h"
#include "linkerconfig/baseconfig.h"
#include "linkerconfig/variables.h"
#include "testbase.h"

using android::linkerconfig::contents::Context;
using android::linkerconfig::modules::ApexInfo;

struct linkerconfig_vndklite_backward_compatibility : ::testing::Test {
  void SetUp() override {
    MockVariables();
    MockVnkdLite();
    ApexInfo vndk_apex;
    vndk_apex.name = "com.android.vndk.vQ";
    ctx.AddApexModule(vndk_apex);
  }
  Context ctx;
};

TEST_F(linkerconfig_vndklite_backward_compatibility, system_section) {
  auto config = android::linkerconfig::contents::CreateBaseConfiguration(ctx);

  auto system_section = config.GetSection("system");
  ASSERT_TRUE(system_section);

  auto default_namespace = system_section->GetNamespace("default");
  ASSERT_TRUE(default_namespace);

  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/vendor/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/odm/${LIB}"));

  auto sphal_namespace = system_section->GetNamespace("sphal");
  ASSERT_TRUE(sphal_namespace);

  EXPECT_TRUE(ContainsSearchPath(sphal_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(sphal_namespace, "/vendor/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(sphal_namespace, "/vendor/${LIB}/hw"));

  EXPECT_TRUE(ContainsPermittedPath(sphal_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsPermittedPath(sphal_namespace, "/vendor/${LIB}"));
  EXPECT_TRUE(ContainsPermittedPath(sphal_namespace, "/system/vendor/${LIB}"));

  auto rs_namespace = system_section->GetNamespace("rs");
  ASSERT_TRUE(rs_namespace);

  EXPECT_TRUE(ContainsSearchPath(rs_namespace, "/odm/${LIB}/vndk-sp"));
  EXPECT_TRUE(ContainsSearchPath(rs_namespace, "/vendor/${LIB}/vndk-sp"));
  EXPECT_TRUE(ContainsSearchPath(rs_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(rs_namespace, "/vendor/${LIB}"));

  EXPECT_TRUE(ContainsPermittedPath(rs_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsPermittedPath(rs_namespace, "/vendor/${LIB}"));
  EXPECT_TRUE(ContainsPermittedPath(rs_namespace, "/system/vendor/${LIB}"));

  auto vndk_namespace = system_section->GetNamespace("vndk");
  ASSERT_TRUE(vndk_namespace);

  EXPECT_TRUE(ContainsSearchPath(vndk_namespace, "/odm/${LIB}/vndk-sp"));
  EXPECT_TRUE(ContainsSearchPath(vndk_namespace, "/vendor/${LIB}/vndk-sp"));

  EXPECT_TRUE(ContainsPermittedPath(vndk_namespace, "/odm/${LIB}/hw"));
  EXPECT_TRUE(ContainsPermittedPath(vndk_namespace, "/odm/${LIB}/egl"));
  EXPECT_TRUE(ContainsPermittedPath(vndk_namespace, "/vendor/${LIB}/hw"));
  EXPECT_TRUE(ContainsPermittedPath(vndk_namespace, "/vendor/${LIB}/egl"));
}

TEST_F(linkerconfig_vndklite_backward_compatibility, vendor_section) {
  auto config = android::linkerconfig::contents::CreateBaseConfiguration(ctx);

  auto vendor_section = config.GetSection("vendor");
  ASSERT_TRUE(vendor_section);

  auto default_namespace = vendor_section->GetNamespace("default");
  ASSERT_TRUE(default_namespace);

  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/odm/${LIB}/vndk"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/odm/${LIB}/vndk-sp"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/vendor/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/vendor/${LIB}/vndk"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/vendor/${LIB}/vndk-sp"));
}

TEST_F(linkerconfig_vndklite_backward_compatibility, unrestricted_section) {
  auto config = android::linkerconfig::contents::CreateBaseConfiguration(ctx);

  auto unrestricted_section = config.GetSection("unrestricted");
  ASSERT_TRUE(unrestricted_section);

  auto default_namespace = unrestricted_section->GetNamespace("default");
  ASSERT_TRUE(default_namespace);

  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/odm/${LIB}"));
  EXPECT_TRUE(ContainsSearchPath(default_namespace, "/vendor/${LIB}"));
}