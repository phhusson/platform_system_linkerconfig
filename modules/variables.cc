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

#include "linkerconfig/variables.h"

#include <android-base/properties.h>
#include <regex>
#include <sstream>

#include "linkerconfig/log.h"

namespace {
constexpr const char* kVariableRegex =
    "@\\{([^@\\{\\}:]+)(:([^@\\{\\}:]*))?\\}";
}

namespace android {
namespace linkerconfig {
namespace modules {

std::map<std::string, std::string> Variables::variables_;

std::optional<std::string> Variables::GetValue(const std::string& variable) {
  // If variable is in predefined key-value pair, use this value
  if (variables_.find(variable) != variables_.end() &&
      !variables_[variable].empty()) {
    return {variables_[variable]};
  }

  // If variable is defined as property, use this value
  std::string prop_value = android::base::GetProperty(variable, "");
  if (!prop_value.empty()) {
    return {prop_value};
  }

  // If cannot find variable, return default value
  return std::nullopt;
}

void Variables::AddValue(const std::string& key, const std::string& value) {
  variables_[key] = value;
}

std::string Variables::ResolveVariables(const std::string& str) {
  std::string result = str;
  std::regex variable_regex(kVariableRegex);
  std::smatch sm;

  while (std::regex_search(result, sm, variable_regex)) {
    std::stringstream ss;
    ss << sm.prefix();
    auto resolved_value = GetValue(sm[1]);
    if (resolved_value.has_value()) {
      ss << resolved_value.value();
    } else {
      LOG(WARNING) << "Unable to find value for " << sm[1];
      bool contains_default = sm[2].length() > 0;
      if (contains_default) {
        ss << sm[3];
      } else {
        LOG(FATAL) << "There is no default value defined for " << sm[1];
      }
    }
    ss << ResolveVariables(sm.suffix());
    result = ss.str();
  }

  return result;
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android