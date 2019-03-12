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

#include "linkerconfig/namespace.h"

#include <android-base/logging.h>

#define LOG_TAG "linkerconfig"

namespace android {
namespace linkerconfig {
namespace modules {

constexpr const char* kDataAsanPath = "/data/asan";

std::string Namespace::GetPathString(const std::string& path_type,
                                     const std::vector<std::string>& path_list) {
  std::string prefix = "namespace." + name_ + "." + path_type + ".paths ";
  std::string path_string = "";
  bool is_first = true;
  for (auto& path : path_list) {
    path_string += prefix + (is_first ? "= " : "+= ") + path + "\n";
    is_first = false;
  }

  return path_string;
}

std::shared_ptr<Link> Namespace::CreateLink(const std::string& target_namespace,
                                            bool allow_all_shared_libs) {
  auto new_link =
      std::make_shared<Link>(name_, target_namespace, allow_all_shared_libs);

  if (links_.find(target_namespace) != links_.end()) {
    LOG(INFO) << "Link to " << target_namespace
              << " already exists. Overwriting link.";
  }

  links_[target_namespace] = new_link;
  return new_link;
}

std::string Namespace::GenerateConfig() {
  std::string config = "";
  std::string prefix = "namespace." + name_ + ".";

  config += prefix + "isolated = " + (is_isolated_ ? "true" : "false") + "\n";

  if (is_visible_) {
    config += prefix + "visible = true\n";
  }

  config += GetPathString("search", search_paths_);
  config += GetPathString("permitted", permitted_paths_);
  config += GetPathString("asan.search", asan_search_paths_);
  config += GetPathString("asan.permitted", asan_permitted_paths_);

  if (!links_.empty()) {
    config += prefix + "links = ";

    bool is_first = true;
    for (auto& link : links_) {
      if (!is_first) {
        config += ",";
      }
      config += link.first;
      is_first = false;
    }

    config += "\n";

    for (auto& link : links_) {
      config += link.second->GenerateConfig();
    }
  }

  return config;
}

void Namespace::AddSearchPath(const std::string& path, bool in_asan,
                              bool with_data_asan) {
  search_paths_.push_back(path);

  if (in_asan) {
    asan_search_paths_.push_back(path);
    if (with_data_asan) {
      asan_search_paths_.push_back(kDataAsanPath + path);
    }
  }
}

void Namespace::AddPermittedPath(const std::string& path, bool in_asan,
                                 bool with_data_asan) {
  permitted_paths_.push_back(path);

  if (in_asan) {
    asan_permitted_paths_.push_back(path);
    if (with_data_asan) {
      asan_permitted_paths_.push_back(kDataAsanPath + path);
    }
  }
}

}  // namespace modules
}  // namespace linkerconfig
}  // namespace android