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
#ifndef ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_H
#define ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_H

#include <android/hardware/wifi/offload/1.0/IOffloadCallback.h>
#include <hidl/Status.h>
#include <vector>

namespace android {
namespace hardware {
namespace wifi {
namespace offload {
namespace V1_0 {
namespace implementation {

using ::android::hardware::wifi::offload::V1_0::IOffloadCallback;
using ::android::hardware::wifi::offload::V1_0::OffloadStatus;
using ::android::hardware::wifi::offload::V1_0::ScanResult;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;	

typedef std::function<void(
    const std::vector<ScanResult>& scanResult)> OnOffloadScanResultsReadyHandler;

typedef std::function<void(OffloadStatus status)> OnErrorHandler;

class OffloadCallback : public IOffloadCallback {
 public:
  explicit OffloadCallback(OnOffloadScanResultsReadyHandler /* scanResulthandler */,
      OnErrorHandler /* errorHandler */);
  virtual ~OffloadCallback();

  // Methods from ::android::hardware::wifi::offload::V1_0::IOffloadCallback follow.
  Return<void> onScanResult(const hidl_vec<ScanResult>& scanResult) override;
  Return<void> onError(OffloadStatus status) override;
  // Methods from ::android::hidl::base::V1_0::IBase follow.

 private:
  OnOffloadScanResultsReadyHandler scan_result_handler_;
  OnErrorHandler error_handler_;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace offload
}  // namespace wifi
}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_WIFI_OFFLOAD_V1_0_OFFLOADCALLBACK_H
