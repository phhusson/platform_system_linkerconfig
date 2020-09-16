/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "linkerconfig/apexlinkerconfig.h"

#include <android-base/file.h>
#include <gtest/gtest.h>

using ::android::linkerconfig::modules::ParseApexLinkerConfig;

const std::string kBaseDir =
    android::base::GetExecutableDirectory() + "/modules/tests/data/";

const std::vector<std::string> kPermittedPaths = {"/a", "/b/c", "/d/e/f"};

TEST(apexlinkerconfig, file_not_exist) {
  auto result = ParseApexLinkerConfig(kBaseDir + "linker.config.noexist");

  ASSERT_FALSE(result.ok());
}

TEST(apexlinkerconfig, unknown_section) {
  auto result =
      ParseApexLinkerConfig(kBaseDir + "linker.config.unknown_section");

  ASSERT_FALSE(result.ok());
}

TEST(apexlinkerconfig, unknown_property) {
  auto result =
      ParseApexLinkerConfig(kBaseDir + "linker.config.unknown_property");

  ASSERT_FALSE(result.ok());
}

TEST(apexlinkerconfig, permitted_paths) {
  auto result =
      ParseApexLinkerConfig(kBaseDir + "linker.config.permitted_path");

  ASSERT_TRUE(result.ok());
  ASSERT_EQ(result->permitted_paths, kPermittedPaths);
}

TEST(apexlinkerconfig, properties) {
  auto result = ParseApexLinkerConfig(kBaseDir + "linker.config.properties");

  ASSERT_TRUE(result.ok());
  ASSERT_TRUE(result->visible);
}

TEST(apexlinkerconfig, all_contents) {
  auto result = ParseApexLinkerConfig(kBaseDir + "linker.config.all");

  ASSERT_TRUE(result.ok());
  ASSERT_EQ(result->permitted_paths, kPermittedPaths);
  ASSERT_TRUE(result->visible);
}
