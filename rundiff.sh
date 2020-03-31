#!/bin/bash

# Copyright (C) 2020 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e
shopt -s extglob

# to use relative paths
cd $(dirname $0)

# when executed directly from commandline, build dependencies
if [[ $(basename $0) == "rundiff.sh" ]]; then
  if [ -z $ANDROID_BUILD_TOP ]; then
    echo "You need to source and lunch before you can use this script"
    exit 1
  fi
  $ANDROID_BUILD_TOP/build/soong/soong_ui.bash --make-mode linkerconfig conv_apex_manifest
fi

# $1: tmp root
# $2: apex
function activate() {
  cp -r ./testdata/root/apex/$2 $1/apex
}

# $1: target output directory
function run_linkerconfig_to {
  # delete old output
  rm -rf $1

  # prepare root with no apexes
  TMP_ROOT=$(mktemp -d -t linkerconfig-root-XXXXXXXX)
  cp -R ./testdata/root/!(apex) $TMP_ROOT

  mkdir -p $1/stage0
  linkerconfig -v R -r $TMP_ROOT -t $1/stage0

  # activate bootstrap apexes
  mkdir -p $TMP_ROOT/apex
  activate $TMP_ROOT com.android.art
  activate $TMP_ROOT com.android.i18n
  # activate $TMP_ROOT com.android.os.statsd
  activate $TMP_ROOT com.android.runtime
  activate $TMP_ROOT com.android.tzdata
  activate $TMP_ROOT com.android.vndk.vR

  find $TMP_ROOT -name apex_manifest.json -exec sh -c '$2 proto $1 -o ${1%.json}.pb' sh  {} conv_apex_manifest \;
  find $TMP_ROOT -name apex_manifest.json -exec sh -c 'mkdir `dirname $1`/lib' sh  {}  \;

  mkdir -p $1/stage1
  linkerconfig -v R -r $TMP_ROOT -t $1/stage1

  # clean up testdata root
  rm -iRf $TMP_ROOT

  # prepare root with all apexes
  TMP_ROOT=$(mktemp -d -t linkerconfig-root-XXXXXXXX)
  cp -R ./testdata/root/* $TMP_ROOT
  find $TMP_ROOT -name apex_manifest.json -exec sh -c '$2 proto $1 -o ${1%.json}.pb' sh  {} conv_apex_manifest \;
  find $TMP_ROOT -name apex_manifest.json -exec sh -c 'mkdir `dirname $1`/lib' sh  {}  \;

  mkdir -p $1/stage2
  linkerconfig -v R -r $TMP_ROOT -t $1/stage2

  mkdir -p $1/product-enabled
  linkerconfig -v R -p R -r $TMP_ROOT -t $1/product-enabled

  mkdir -p $1/vndk-lite
  linkerconfig -v R -e -r $TMP_ROOT -t $1/vndk-lite

  # clean up testdata root
  rm -rf $TMP_ROOT
}

# update golden_output
if [[ $1 == "--update" ]]; then
  run_linkerconfig_to ./testdata/golden_output
  echo "Updated"
  exit 0
fi

echo "Running linkerconfig diff test..."

run_linkerconfig_to ./testdata/output
if diff -ruN ./testdata/golden_output ./testdata/output ; then
  echo "No changes."
else
  echo
  echo "------------------------------------------------------------------------------------------"
  echo "if change looks fine, run following:"
  echo "  \$ANDROID_BUILD_TOP/system/linkerconfig/rundiff.sh --update"
  echo "------------------------------------------------------------------------------------------"
  # fail
  exit 1
fi