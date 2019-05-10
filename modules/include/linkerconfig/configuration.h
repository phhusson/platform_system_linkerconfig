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

#include <string>
#include <vector>

#include "linkerconfig/configwriter.h"
#include "linkerconfig/section.h"

namespace android {
namespace linkerconfig {
namespace modules {
class Configuration {
 public:
  explicit Configuration(std::vector<Section> sections)
      : sections_(std::move(sections)) {
  }
  Configuration(const Configuration&) = delete;
  Configuration(Configuration&&) = default;

  void WriteConfig(ConfigWriter& writer);

  // For test usage
  Section* GetSection(const std::string& name);

 private:
  std::vector<Section> sections_;
};
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android