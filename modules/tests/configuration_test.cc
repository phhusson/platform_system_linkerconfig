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
#include <vector>

#include "linkerconfig/configuration.h"
#include "modules_testbase.h"

using namespace android::linkerconfig::modules;

constexpr const char* kExpectedConfiguration =
    R"(dir.system = /system/bin
dir.system = /system/xbin
dir.vendor = /odm/bin
dir.vendor = /vendor/bin
dir.vendor = /data/nativetest/odm
dir.system = /product/bin
[system]
additional.namespaces = namespace1,namespace2
namespace.default.isolated = false
namespace.default.search.paths = /search_path1
namespace.default.search.paths += /search_path2
namespace.default.search.paths += /search_path3
namespace.default.permitted.paths = /permitted_path1
namespace.default.permitted.paths += /permitted_path2
namespace.default.permitted.paths += /permitted_path3
namespace.default.asan.search.paths = /search_path1
namespace.default.asan.search.paths += /data/asan/search_path1
namespace.default.asan.search.paths += /search_path2
namespace.default.asan.permitted.paths = /permitted_path1
namespace.default.asan.permitted.paths += /data/asan/permitted_path1
namespace.default.asan.permitted.paths += /permitted_path2
namespace.default.links = namespace1,namespace2
namespace.default.link.namespace1.shared_libs = lib1.so
namespace.default.link.namespace1.shared_libs += lib2.so
namespace.default.link.namespace1.shared_libs += lib3.so
namespace.default.link.namespace2.allow_all_shared_libs = true
namespace.namespace1.isolated = false
namespace.namespace1.search.paths = /search_path1
namespace.namespace1.search.paths += /search_path2
namespace.namespace1.search.paths += /search_path3
namespace.namespace1.permitted.paths = /permitted_path1
namespace.namespace1.permitted.paths += /permitted_path2
namespace.namespace1.permitted.paths += /permitted_path3
namespace.namespace1.asan.search.paths = /search_path1
namespace.namespace1.asan.search.paths += /data/asan/search_path1
namespace.namespace1.asan.search.paths += /search_path2
namespace.namespace1.asan.permitted.paths = /permitted_path1
namespace.namespace1.asan.permitted.paths += /data/asan/permitted_path1
namespace.namespace1.asan.permitted.paths += /permitted_path2
namespace.namespace2.isolated = false
namespace.namespace2.search.paths = /search_path1
namespace.namespace2.search.paths += /search_path2
namespace.namespace2.search.paths += /search_path3
namespace.namespace2.permitted.paths = /permitted_path1
namespace.namespace2.permitted.paths += /permitted_path2
namespace.namespace2.permitted.paths += /permitted_path3
namespace.namespace2.asan.search.paths = /search_path1
namespace.namespace2.asan.search.paths += /data/asan/search_path1
namespace.namespace2.asan.search.paths += /search_path2
namespace.namespace2.asan.permitted.paths = /permitted_path1
namespace.namespace2.asan.permitted.paths += /data/asan/permitted_path1
namespace.namespace2.asan.permitted.paths += /permitted_path2
[vendor]
namespace.default.isolated = false
namespace.default.search.paths = /search_path1
namespace.default.search.paths += /search_path2
namespace.default.search.paths += /search_path3
namespace.default.permitted.paths = /permitted_path1
namespace.default.permitted.paths += /permitted_path2
namespace.default.permitted.paths += /permitted_path3
namespace.default.asan.search.paths = /search_path1
namespace.default.asan.search.paths += /data/asan/search_path1
namespace.default.asan.search.paths += /search_path2
namespace.default.asan.permitted.paths = /permitted_path1
namespace.default.asan.permitted.paths += /data/asan/permitted_path1
namespace.default.asan.permitted.paths += /permitted_path2
)";

TEST(linkerconfig_configuration, generate_configuration) {
  std::vector<std::shared_ptr<Section>> sections;

  std::vector<std::shared_ptr<Namespace>> system_namespaces;
  BinaryPathList system_binary_path = {{"/system/bin", kDefaultPriority},
                                       {"/system/xbin", kDefaultPriority},
                                       {"/product/bin", kLowPriority + 10}};

  system_namespaces.push_back(CreateNamespaceWithLinks(
      "default", false, false, "namespace1", "namespace2"));
  system_namespaces.push_back(
      CreateNamespaceWithPaths("namespace1", false, false));
  system_namespaces.push_back(
      CreateNamespaceWithPaths("namespace2", false, false));

  sections.push_back(std::make_shared<Section>("system", system_binary_path,
                                               system_namespaces));

  std::vector<std::shared_ptr<Namespace>> vendor_namespaces;
  BinaryPathList vendor_binary_path = {{"/odm/bin", kLowPriority},
                                       {"/vendor/bin", kLowPriority},
                                       {"/data/nativetest/odm", kLowPriority}};

  vendor_namespaces.push_back(CreateNamespaceWithPaths("default", false, false));

  sections.push_back(std::make_shared<Section>("vendor", vendor_binary_path,
                                               vendor_namespaces));

  Configuration conf(sections);

  android::linkerconfig::modules::ConfigWriter writer;
  conf.WriteConfig(writer);

  ASSERT_EQ(writer.ToString(), kExpectedConfiguration);
}