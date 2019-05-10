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

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/namespace.h"

namespace android {
namespace linkerconfig {
namespace modules {

using BinaryPathPriority = unsigned int;
using BinaryPathMap = std::multimap<BinaryPathPriority, std::string>;
using BinaryPathList = std::vector<std::pair<std::string, BinaryPathPriority>>;

constexpr const static BinaryPathPriority kHighPriority = 20;
constexpr const static BinaryPathPriority kDefaultPriority = 50;
constexpr const static BinaryPathPriority kLowPriority = 80;

class Section {
 public:
  Section(const std::string& name, BinaryPathList binary_paths,
          std::vector<Namespace> namespaces)
      : name_(name),
        binary_paths_(binary_paths),
        namespaces_(std::move(namespaces)) {
  }

  Section(const Section&) = delete;
  Section(Section&&) = default;

  void WriteConfig(ConfigWriter& writer);
  void CollectBinaryPaths(BinaryPathMap& binary_paths);

  // For test usage
  Namespace* GetNamespace(const std::string& namespace_name);
  std::string GetName();

 private:
  const std::string name_;
  BinaryPathList binary_paths_;
  std::vector<Namespace> namespaces_;
};
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android