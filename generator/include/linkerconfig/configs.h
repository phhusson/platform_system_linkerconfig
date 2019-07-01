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

// TODO : Create this file in build time
#include <map>
#include <string>

inline const std::map<std::string, std::string> kPredefinedConfigs = {
    {"PRODUCT", "product"},
    {"SYSTEM_EXT", "system_ext"},
    {"LLNDK_LIBRARIES",
     "libEGL.so:libGLESv1_CM.so:libGLESv2.so:libGLESv3.so:libRS.so:libandroid_"
     "net.so:libc.so:libcgrouprc.so:libclang_rt.asan-aarch64-android.so:"
     "libclang_rt.asan-arm-android.so:libclang_rt.hwasan-aarch64-android.so:"
     "libdl.so:libft2.so:liblog.so:libm.so:libmediandk.so:libnativewindow.so:"
     "libneuralnetworks.so:libsync.so:libvndksupport.so:libvulkan.so"},
    {"SANITIZER_RUNTIME_LIBRARIES",
     "libclang_rt.asan-aarch64-android.so:libclang_rt.hwasan-aarch64-android."
     "so:libclang_rt.ubsan_standalone-aarch64-android.so:libclang_rt.tsan-"
     "aarch64-android.so:libclang_rt.asan-arm-android.so:libclang_rt.hwasan-"
     "arm-android.so:libclang_rt.ubsan_standalone-arm-android.so:libclang_rt."
     "tsan-arm-android.so"},
    {"PRIVATE_LLNDK_LIBRARIES", ""},
    {"VNDK_SAMEPROCESS_LIBRARIES",
     "android.hardware.graphics.common@1.0.so:android.hardware.graphics.common@"
     "1.1.so:android.hardware.graphics.common@1.2.so:android.hardware.graphics."
     "mapper@2.0.so:android.hardware.graphics.mapper@2.1.so:android.hardware."
     "graphics.mapper@3.0.so:android.hardware.renderscript@1.0.so:android.hidl."
     "memory.token@1.0.so:android.hidl.memory@1.0.so:android.hidl.memory@1.0-"
     "impl.so:android.hidl.safe_union@1.0.so:libRSCpuRef.so:libRSDriver.so:"
     "libRS_internal.so:libbacktrace.so:libbase.so:libbcinfo.so:"
     "libbinderthreadstate.so:libblas.so:libc++.so:libcompiler_rt.so:libcutils."
     "so:libhardware.so:libhidlbase.so:libhidlmemory.so:libhidltransport.so:"
     "libhwbinder.so:libhwbinder_noltopgo.so:libion.so:libjsoncpp.so:liblzma."
     "so:libprocessgroup.so:libunwindstack.so:libutils.so:libutilscallstack.so:"
     "libz.so"},
    {"VNDK_USING_CORE_VARIANT_LIBRARIES", ""},
    {"VNDK_CORE_LIBRARIES",
     "android.frameworks.cameraservice.common@2.0.so:android.frameworks."
     "cameraservice.device@2.0.so:android.frameworks.cameraservice.service@2.0."
     "so:android.frameworks.displayservice@1.0.so:android.frameworks."
     "schedulerservice@1.0.so:android.frameworks.sensorservice@1.0.so:android."
     "frameworks.stats@1.0.so:android.frameworks.vr.composer@1.0.so:android."
     "hardware.atrace@1.0.so:android.hardware.audio.common@2.0.so:android."
     "hardware.audio.common@4.0.so:android.hardware.audio.common@5.0.so:"
     "android.hardware.audio.effect@2.0.so:android.hardware.audio.effect@4.0."
     "so:android.hardware.audio.effect@5.0.so:android.hardware.audio@2.0.so:"
     "android.hardware.audio@4.0.so:android.hardware.audio@5.0.so:android."
     "hardware.authsecret@1.0.so:android.hardware.automotive.audiocontrol@1.0."
     "so:android.hardware.automotive.evs@1.0.so:android.hardware.automotive."
     "vehicle@2.0.so:android.hardware.biometrics.face@1.0.so:android.hardware."
     "biometrics.fingerprint@2.1.so:android.hardware.bluetooth.a2dp@1.0.so:"
     "android.hardware.bluetooth.audio@2.0.so:android.hardware.bluetooth@1.0."
     "so:android.hardware.boot@1.0.so:android.hardware.broadcastradio@1.0.so:"
     "android.hardware.broadcastradio@1.1.so:android.hardware.broadcastradio@2."
     "0.so:android.hardware.camera.common@1.0.so:android.hardware.camera."
     "device@1.0.so:android.hardware.camera.device@3.2.so:android.hardware."
     "camera.device@3.3.so:android.hardware.camera.device@3.4.so:android."
     "hardware.camera.device@3.5.so:android.hardware.camera.metadata@3.2.so:"
     "android.hardware.camera.metadata@3.3.so:android.hardware.camera.metadata@"
     "3.4.so:android.hardware.camera.provider@2.4.so:android.hardware.camera."
     "provider@2.5.so:android.hardware.cas.native@1.0.so:android.hardware.cas@"
     "1.0.so:android.hardware.cas@1.1.so:android.hardware.configstore-utils.so:"
     "android.hardware.configstore@1.0.so:android.hardware.configstore@1.1.so:"
     "android.hardware.confirmationui-support-lib.so:android.hardware."
     "confirmationui@1.0.so:android.hardware.contexthub@1.0.so:android."
     "hardware.drm@1.0.so:android.hardware.drm@1.1.so:android.hardware.drm@1.2."
     "so:android.hardware.dumpstate@1.0.so:android.hardware.fastboot@1.0.so:"
     "android.hardware.gatekeeper@1.0.so:android.hardware.gnss.measurement_"
     "corrections@1.0.so:android.hardware.gnss.visibility_control@1.0.so:"
     "android.hardware.gnss@1.0.so:android.hardware.gnss@1.1.so:android."
     "hardware.gnss@2.0.so:android.hardware.graphics.allocator@2.0.so:android."
     "hardware.graphics.allocator@3.0.so:android.hardware.graphics.bufferqueue@"
     "1.0.so:android.hardware.graphics.bufferqueue@2.0.so:android.hardware."
     "graphics.composer@2.1.so:android.hardware.graphics.composer@2.2.so:"
     "android.hardware.graphics.composer@2.3.so:android.hardware.health."
     "storage@1.0.so:android.hardware.health@1.0.so:android.hardware.health@2."
     "0.so:android.hardware.input.classifier@1.0.so:android.hardware.input."
     "common@1.0.so:android.hardware.ir@1.0.so:android.hardware.keymaster@3.0."
     "so:android.hardware.keymaster@4.0.so:android.hardware.light@2.0.so:"
     "android.hardware.media.bufferpool@1.0.so:android.hardware.media."
     "bufferpool@2.0.so:android.hardware.media.c2@1.0.so:android.hardware."
     "media.omx@1.0.so:android.hardware.media@1.0.so:android.hardware.memtrack@"
     "1.0.so:android.hardware.neuralnetworks@1.0.so:android.hardware."
     "neuralnetworks@1.1.so:android.hardware.neuralnetworks@1.2.so:android."
     "hardware.nfc@1.0.so:android.hardware.nfc@1.1.so:android.hardware.nfc@1.2."
     "so:android.hardware.oemlock@1.0.so:android.hardware.power.stats@1.0.so:"
     "android.hardware.power@1.0.so:android.hardware.power@1.1.so:android."
     "hardware.power@1.2.so:android.hardware.power@1.3.so:android.hardware."
     "radio.config@1.0.so:android.hardware.radio.config@1.1.so:android."
     "hardware.radio.config@1.2.so:android.hardware.radio.deprecated@1.0.so:"
     "android.hardware.radio@1.0.so:android.hardware.radio@1.1.so:android."
     "hardware.radio@1.2.so:android.hardware.radio@1.3.so:android.hardware."
     "radio@1.4.so:android.hardware.secure_element@1.0.so:android.hardware."
     "secure_element@1.1.so:android.hardware.sensors@1.0.so:android.hardware."
     "sensors@2.0.so:android.hardware.soundtrigger@2.0.so:android.hardware."
     "soundtrigger@2.0-core.so:android.hardware.soundtrigger@2.1.so:android."
     "hardware.soundtrigger@2.2.so:android.hardware.tetheroffload.config@1.0."
     "so:android.hardware.tetheroffload.control@1.0.so:android.hardware."
     "thermal@1.0.so:android.hardware.thermal@1.1.so:android.hardware.thermal@"
     "2.0.so:android.hardware.tv.cec@1.0.so:android.hardware.tv.cec@2.0.so:"
     "android.hardware.tv.input@1.0.so:android.hardware.usb.gadget@1.0.so:"
     "android.hardware.usb@1.0.so:android.hardware.usb@1.1.so:android.hardware."
     "usb@1.2.so:android.hardware.vibrator@1.0.so:android.hardware.vibrator@1."
     "1.so:android.hardware.vibrator@1.2.so:android.hardware.vibrator@1.3.so:"
     "android.hardware.vr@1.0.so:android.hardware.weaver@1.0.so:android."
     "hardware.wifi.hostapd@1.0.so:android.hardware.wifi.hostapd@1.1.so:"
     "android.hardware.wifi.offload@1.0.so:android.hardware.wifi.supplicant@1."
     "0.so:android.hardware.wifi.supplicant@1.1.so:android.hardware.wifi."
     "supplicant@1.2.so:android.hardware.wifi@1.0.so:android.hardware.wifi@1.1."
     "so:android.hardware.wifi@1.2.so:android.hardware.wifi@1.3.so:android."
     "hidl.allocator@1.0.so:android.hidl.memory.block@1.0.so:android.hidl."
     "token@1.0.so:android.hidl.token@1.0-utils.so:android.system.net.netd@1.0."
     "so:android.system.net.netd@1.1.so:android.system.suspend@1.0.so:android."
     "system.wifi.keystore@1.0.so:libadf.so:libaudioroute.so:libaudioutils.so:"
     "libbinder.so:libcamera_metadata.so:libcap.so:libclang_rt.scudo-aarch64-"
     "android.so:libclang_rt.scudo-arm-android.so:libclang_rt.scudo_minimal-"
     "aarch64-android.so:libclang_rt.scudo_minimal-arm-android.so:libclang_rt."
     "ubsan_standalone-aarch64-android.so:libclang_rt.ubsan_standalone-arm-"
     "android.so:libcn-cbor.so:libcodec2.so:libcrypto.so:libcrypto_utils.so:"
     "libcurl.so:libdiskconfig.so:libdumpstateutil.so:libevent.so:libexif.so:"
     "libexpat.so:libfmq.so:libgatekeeper.so:libgui.so:libhardware_legacy.so:"
     "libhidlallocatorutils.so:libhidlcache.so:libjpeg.so:libkeymaster_"
     "messages.so:libkeymaster_portable.so:libldacBT_abr.so:libldacBT_enc.so:"
     "liblz4.so:libmedia_helper.so:libmedia_omx.so:libmemtrack.so:libminijail."
     "so:libmkbootimg_abi_check.so:libnetutils.so:libnl.so:libopus.so:libpcre2."
     "so:libpiex.so:libpng.so:libpower.so:libprocinfo.so:libprotobuf-cpp-full."
     "so:libprotobuf-cpp-lite.so:libpuresoftkeymasterdevice.so:libradio_"
     "metadata.so:libselinux.so:libsoftkeymasterdevice.so:libspeexresampler.so:"
     "libsqlite.so:libssl.so:libstagefright_amrnb_common.so:libstagefright_"
     "bufferpool@2.0.so:libstagefright_bufferqueue_helper.so:libstagefright_"
     "enc_common.so:libstagefright_flacdec.so:libstagefright_foundation.so:"
     "libstagefright_omx.so:libstagefright_omx_utils.so:libstagefright_"
     "xmlparser.so:libsysutils.so:libtinyalsa.so:libtinyxml2.so:libui.so:"
     "libusbhost.so:libvorbisidec.so:libwifi-system-iface.so:libxml2.so:libyuv."
     "so:libziparchive.so"}};