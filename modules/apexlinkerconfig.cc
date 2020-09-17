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
#include <android-base/strings.h>

#include "linkerconfig/log.h"

using android::base::ReadFileToString;
using android::base::Result;

namespace android {
namespace linkerconfig {
namespace modules {

Result<void> ParsePropertiesSection(ApexLinkerConfig& config,
                                    const std::string& line) {
  auto assign = line.find('=');
  if (assign != std::string::npos) {
    auto name = android::base::Trim(line.substr(0, assign));
    auto value = android::base::Trim(line.substr(assign + 1));
    if (name == "visible") {
      config.visible = (value == "true");
    } else {
      return Errorf("Unexpected property name : {}", name);
    }
  } else {
    return Errorf("Invalid property format. Expected \"name = property\" : {}",
                  line);
  }

  return {};
}

// Format of the file can be found from linker.config.format.md file
Result<ApexLinkerConfig> ParseApexLinkerConfig(const std::string& config_path) {
  ApexLinkerConfig result;
  std::string file_content;

  if (!ReadFileToString(config_path, &file_content)) {
    return ErrnoErrorf("Cannot read file {}", config_path);
  }

  std::stringstream content_stream(file_content);
  std::string line;
  std::string current_section = "";

  while (std::getline(content_stream, line)) {
    line = android::base::Trim(line);
    if (line.empty() || line[0] == '#') {
      continue;
    }

    if (line.front() == '[' && line.back() == ']') {
      current_section = android::base::Trim(line.substr(1, line.length() - 2));
    } else if (current_section == "permitted_path") {
      result.permitted_paths.emplace_back(line);
    } else if (current_section == "properties") {
      auto parse_result = ParsePropertiesSection(result, line);
      if (!parse_result.ok()) {
        return parse_result.error();
      }
    } else {
      LOG(ERROR) << "Wrong configuration format in file " << config_path
                 << " : " << line;

      return Errorf("Failed to parse line {}", line);
    }
  }

  return result;
}

}  // namespace modules
}  // namespace linkerconfig
}  // namespace android
