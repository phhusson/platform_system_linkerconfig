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

#include "linkerconfig/configwriter.h"

#include <cstdarg>
#include <cstdio>

#include "linkerconfig/log.h"
#include "linkerconfig/variables.h"

namespace android {
namespace linkerconfig {
namespace modules {

void ConfigWriter::SetPrefix(const std::string& prefix) {
  prefix_ = prefix;
}

void ConfigWriter::ResetPrefix() {
  prefix_ = "";
}

void ConfigWriter::WriteLine(const char* format, ...) {
  va_list args_for_length, args;

  va_start(args, format);
  va_copy(args_for_length, args);

  int length = vsnprintf(nullptr, 0, format, args_for_length);
  va_end(args_for_length);

  if (length < 0) {
    LOG(ERROR) << "Failed to get length of the string with format " << format;
    va_end(args);
    return;
  }

  std::unique_ptr<char[]> formatted_string(new char[length + 1]);

  int res = vsnprintf(formatted_string.get(), length + 1, format, args);
  va_end(args);

  if (res < 0) {
    LOG(ERROR) << "Failed to write a string with format " << format;
    return;
  }

  WriteLine(std::string(formatted_string.get()));
}

void ConfigWriter::WriteLine(const std::string& line) {
  auto resolved_line = Variables::ResolveVariables(prefix_ + line);
  content_ << resolved_line << std::endl;
}

std::string ConfigWriter::ToString() {
  return content_.str();
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android