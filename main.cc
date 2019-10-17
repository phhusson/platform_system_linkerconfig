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

#include <getopt.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "linkerconfig/baseconfig.h"
#include "linkerconfig/environment.h"
#include "linkerconfig/legacy.h"
#include "linkerconfig/log.h"
#include "linkerconfig/variableloader.h"
#include "linkerconfig/variables.h"

namespace {
const static struct option program_options[] = {
    {"target", required_argument, 0, 't'},
#ifndef __ANROID__
    {"root", required_argument, 0, 'r'},
    {"vndk", required_argument, 0, 'v'},
#endif
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

struct ProgramArgs {
  std::string target_file;
  std::string root;
  std::string vndk_version;
};

[[noreturn]] void PrintUsage(int status = EXIT_SUCCESS) {
  std::cerr << "Usage : linkerconfig [--target <target_file>]"
#ifndef __ANDROID__
               " --root <root dir>"
               " --vndk <vndk version>"
#endif
               " [--help]"
            << std::endl;
  exit(status);
}

bool ParseArgs(int argc, char* argv[], ProgramArgs* args) {
  int parse_result;
  while ((parse_result = getopt_long(
              argc, argv, "t:r:v:h", program_options, NULL)) != -1) {
    switch (parse_result) {
      case 't':
        args->target_file = optarg;
        break;
      case 'r':
        args->root = optarg;
        break;
      case 'v':
        args->vndk_version = optarg;
        break;
      case 'h':
        PrintUsage();
      default:
        return false;
    }
  }

  if (optind < argc) {
    return false;
  }

  return true;
}

android::linkerconfig::modules::Configuration GetConfiguration() {
  if (android::linkerconfig::modules::IsLegacyDevice()) {
    return android::linkerconfig::contents::CreateLegacyConfiguration();
  }

  // TODO : Use recovery if needed

  // Use base configuration in default
  return android::linkerconfig::contents::CreateBaseConfiguration();
}

#ifdef __ANDROID__
struct CombinedLogger {
  android::base::LogdLogger logd;

  void operator()(android::base::LogId id, android::base::LogSeverity severity,
                  const char* tag, const char* file, unsigned int line,
                  const char* message) {
    logd(id, severity, tag, file, line, message);
    KernelLogger(id, severity, tag, file, line, message);
  }
};
#endif

}  // namespace

int main(int argc, char* argv[]) {
  android::base::InitLogging(argv
#ifdef __ANDROID__
                             ,
                             CombinedLogger()
#endif
  );

  ProgramArgs args;

  if (!ParseArgs(argc, argv, &args)) {
    PrintUsage(EXIT_FAILURE);
  }

  std::ostream* out = &std::cout;
  std::ofstream file_out;

  if (args.target_file != "") {
    file_out.open(args.target_file);
    if (file_out.fail()) {
      PLOG(FATAL) << "Failed to open file " << args.target_file;
      return EXIT_FAILURE;
    }
    out = &file_out;
  }

#ifndef __ANDROID__
  if (args.root == "" || args.vndk_version == "") {
    PrintUsage();
  }
  android::linkerconfig::modules::Variables::AddValue("ro.vndk.version",
                                                      args.vndk_version);
#endif

  android::linkerconfig::generator::LoadVariables(args.root);
  auto config = GetConfiguration();
  android::linkerconfig::modules::ConfigWriter config_writer;

  config.WriteConfig(config_writer);
  *out << config_writer.ToString();
  if (!out->good()) {
    PLOG(FATAL) << "Failed to write content to " << args.target_file;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}