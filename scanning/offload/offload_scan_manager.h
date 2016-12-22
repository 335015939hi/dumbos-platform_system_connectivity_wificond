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
#ifndef WIFICOND_OFFLOAD_SCAN_MANAGER_H_
#define WIFICOND_OFFLOAD_SCAN_MANAGER_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/scanning/offload/offload_callback.h"
#include "wificond/scanning/offload/offload_service_utils.h"

#include <vector>

using ::android::hardware::hidl_vec;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;

using std::unique_ptr;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanResult;

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

namespace android {
namespace wificond {

typedef std::function<void(
    std::vector<::com::android::server::wifi::wificond::NativeScanResult>
    scanResult)> OnNativeScanResultsReadyHandler;

class NativeScanResultHandler {
 public:
  NativeScanResultHandler(OnNativeScanResultsReadyHandler);
  ~NativeScanResultHandler();
  void callback(const std::vector<ScanResult>& scanResult);

 private:
  OnNativeScanResultsReadyHandler handler_;
};

// Provides methods to interact with Offload HAL
class OffloadScanManager {
 public:
  explicit OffloadScanManager(OffloadServiceUtils* utils,
      OnNativeScanResultsReadyHandler handler);
  virtual ~OffloadScanManager();
  bool isServiceAvailable();

 private:
  void reportScanResults(const std::vector<ScanResult> scanResult);

  android::sp<IOffload> wifi_offload_hal_;
  android::sp<OffloadCallback> wifi_offload_callback_;
  bool service_available_;
  std::unique_ptr<NativeScanResultHandler> scan_result_handler_;
};

}  // namespace wificond
}  // namespace android

#endif // WIFICOND_OFFLOAD_SCAN_MANAGER_H_
