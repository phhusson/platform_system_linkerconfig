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
#include <utility>
#include <vector>

#include <android-base/file.h>
#include <android-base/result.h>
#include <android-base/stringprintf.h>
#include <apex_manifest.pb.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/namespace.h"
#include "linkerconfig/section.h"

using ::android::base::Result;
using ::android::base::StringPrintf;
using ::android::base::WriteStringToFile;
using ::android::linkerconfig::modules::ConfigWriter;
using ::android::linkerconfig::modules::InitializeWithApex;
using ::android::linkerconfig::modules::Namespace;
using ::android::linkerconfig::modules::Section;
using ::apex::proto::ApexManifest;
using ::testing::Contains;

namespace {

void PrepareTestApexDir(const std::string& path, const std::string& name,
                        std::vector<std::string> provided_libs,
                        std::vector<std::string> required_libs) {
  ApexManifest manifest;
  manifest.set_name(name);
  for (auto lib : provided_libs) {
    manifest.add_providenativelibs(lib);
  }
  for (auto lib : required_libs) {
    manifest.add_requirenativelibs(lib);
  }
  std::string content = manifest.SerializeAsString();
  std::string manifest_path = path + "/apex_manifest.pb";
  if (!WriteStringToFile(content, manifest_path)) {
    LOG(ERROR) << "Failed to write a file: " << manifest_path;
  }
}

}  // namespace

TEST(apex_namespace, build_namespace) {
  TemporaryDir foo_dir;
  PrepareTestApexDir(foo_dir.path, "foo", {}, {});

  Namespace ns("foo");
  auto result = InitializeWithApex(ns, foo_dir.path);
  ASSERT_TRUE(result);

  ConfigWriter writer;
  ns.WriteConfig(writer);
  ASSERT_EQ(
      StringPrintf("namespace.foo.isolated = false\n"
                   "namespace.foo.search.paths = %s/${LIB}\n"
                   "namespace.foo.permitted.paths = /system/${LIB}\n"
                   "namespace.foo.asan.search.paths = %s/${LIB}\n"
                   "namespace.foo.asan.permitted.paths = /system/${LIB}\n",
                   foo_dir.path,
                   foo_dir.path),
      writer.ToString());
}

TEST(apex_namespace, resolve_between_apex_namespaces) {
  TemporaryDir foo_dir, bar_dir;
  PrepareTestApexDir(foo_dir.path, "foo", {"foo.so"}, {"bar.so"});
  PrepareTestApexDir(bar_dir.path, "bar", {"bar.so"}, {});

  Namespace foo("foo"), bar("bar");
  InitializeWithApex(foo, foo_dir.path);
  InitializeWithApex(bar, bar_dir.path);

  std::vector<Namespace> namespaces;
  namespaces.push_back(std::move(foo));
  namespaces.push_back(std::move(bar));
  Section section("section", std::move(namespaces));

  auto result = section.Resolve();
  ASSERT_TRUE(result) << result.error();

  // See if two namespaces are linked correctly
  ASSERT_THAT(section.GetNamespace("foo")->GetLink("bar").GetSharedLibs(),
              Contains("bar.so"));
}