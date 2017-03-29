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
#include <memory>
#include <vector>

#include <android-base/logging.h>

#include "wificond/scanning/offload/offload_callback.h"
#include "wificond/scanning/offload/offload_scan_manager.h"
#include "wificond/scanning/scan_result.h"

using ::android::hardware::wifi::offload::V1_0::ScanResult;
using ::android::hardware::wifi::offload::V1_0::OffloadStatus;
using ::android::hardware::hidl_vec;

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

OffloadCallback::OffloadCallback(OnOffloadScanResultsReadyHandler scanResultHandler,
    OnErrorHandler errorHandler) :
    scan_result_handler_(scanResultHandler),
    error_handler_(errorHandler){
}

// Methods from ::android::hardware::wifi::offload::V1_0::IOffloadCallback follow.
Return<void> OffloadCallback::onScanResult(const hidl_vec<ScanResult>& scan_result) {
  if (scan_result_handler_ != nullptr) {
    scan_result_handler_(std::vector<ScanResult>(scan_result));
  } else {
    LOG(WARNING) << "No handler available for Offload Scan results";
  }
  return Void();
}

Return<void> OffloadCallback::onError(OffloadStatus status) {
    // TODO implement
    if (error_handler_ != nullptr) {
      error_handler_(status);
    } else {
      LOG(WARNING) << "No error handler for Offload";
    }
    return Void();
}

OffloadCallback::~OffloadCallback() {}

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android
