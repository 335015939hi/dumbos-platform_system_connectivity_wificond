/*
 * Copyright (C) 2016 The Android Open Source Project
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
#include "OffloadCallback.h"
#include "wificond/scanning/offload/offload_scan_utils.h"

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

ScanResultHandler::ScanResultHandler(OnScanResultReadyHandler handler)
    : handler_(handler) {
}

void ScanResultHandler::callback(const hidl_vec<ScanResult>& scanResult) {
  handler_(scanResult);
}

ScanResultHandler::~ScanResultHandler() {}

OffloadCallback::OffloadCallback(OnScanResultReadyHandler handler)
    : scan_result_handler_(new ScanResultHandler(handler)) {
}

// Methods from ::android::hardware::wifi::offload::V1_0::IOffloadCallback follow.
Return<void> OffloadCallback::onScanResult(const hidl_vec<ScanResult>& scanResult) {
  scan_result_handler_->callback(scanResult);
  return Void();
}

OffloadCallback::~OffloadCallback() {}

// Methods from ::android::hidl::base::V1_0::IBase follow.

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
