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

if [ -z $ANDROID_BUILD_TOP ]; then
  echo "You need to source and lunch before you can use this script"
  exit 1
fi

set -e

$ANDROID_BUILD_TOP/build/soong/soong_ui.bash --make-mode linkerconfig conv_apex_manifest

LINKERCONFIG_DIR=$ANDROID_BUILD_TOP/system/linkerconfig
LINKERCONFIG_BIN=$ANDROID_HOST_OUT/bin/linkerconfig
CONV_APEX_BIN=$ANDROID_HOST_OUT/bin/conv_apex_manifest

GOLDEN_OUT=$LINKERCONFIG_DIR/testdata/golden_output
TEST_OUT=$LINKERCONFIG_DIR/testdata/output

echo "Running linkerconfig "

TMP_ROOT=$(mktemp -d -t linkerconfig-XXXXXXXX)

cp -R $LINKERCONFIG_DIR/testdata/root/* $TMP_ROOT

find $TMP_ROOT -name apex_manifest.json -exec sh -c '$2 proto $1 -o ${1%.json}.pb' sh  {} $CONV_APEX_BIN \;
find $TMP_ROOT -name apex_manifest.json -exec sh -c 'mkdir `dirname $1`/lib' sh  {}  \;

rm -rf $TEST_OUT
mkdir -p $TEST_OUT
$LINKERCONFIG_BIN -v R -r $TMP_ROOT -t $TEST_OUT
rm -rf $TMP_ROOT

if diff -ruN $GOLDEN_OUT $TEST_OUT ; then
  echo "no changes"
else
  echo
  echo "------------------------------------------------------------------------------------------"
  echo "if change looks fine, run following:"
  echo "  rm -iRf $GOLDEN_OUT && cp -R $TEST_OUT $GOLDEN_OUT"
  echo "------------------------------------------------------------------------------------------"
fi
