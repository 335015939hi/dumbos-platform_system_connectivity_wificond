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
#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/offload/OffloadCallback.h"
#include "wificond/scanning/offload/offload_scan_manager.h"
#include <memory>
#include <vector>
using ::android::hardware::wifi::offload::V1_0::ScanResult;
using ::android::hardware::hidl_vec;

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

OffloadScanResultHandler::OffloadScanResultHandler(
    OnOffloadScanResultsReadyHandler handler)
    : handler_(handler) {
}

void OffloadScanResultHandler::callback(const hidl_vec<ScanResult>& scanResult) {
  const std::vector<ScanResult> scanResult_(scanResult);
  handler_(scanResult_);
}

OffloadScanResultHandler::~OffloadScanResultHandler() {}

OffloadCallback::OffloadCallback(OnOffloadScanResultsReadyHandler handler)
    : scan_result_handler_(new OffloadScanResultHandler(handler)) {
}

// Methods from ::android::hardware::wifi::offload::V1_0::IOffloadCallback follow.
Return<void> OffloadCallback::onScanResult(const hidl_vec<ScanResult>& scanResult) {
  scan_result_handler_->callback(scanResult);
  return Void();
}

OffloadCallback::~OffloadCallback() {}

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
