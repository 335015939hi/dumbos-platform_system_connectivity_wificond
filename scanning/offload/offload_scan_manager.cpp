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
#include "wificond/scanning/offload/offload_scan_manager.h"

#include <vector>

#include <android-base/logging.h>

#include "wificond/scanning/offload/offload_scan_utils.h"
#include "wificond/scanning/offload/offload_service_utils.h"
#include "wificond/scanning/scan_result.h"

using ::android::hardware::hidl_vec;
using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using ::com::android::server::wifi::wificond::NativeScanResult;
using android::wificond::OnNativeScanResultsReadyHandler;

namespace android {
namespace wificond {

OffloadScanManager::OffloadScanManager(OffloadServiceUtils *utils,
    OnNativeScanResultsReadyHandler handler)
    : service_available_(false),
      scan_result_handler_(handler) {
  wifi_offload_hal_ = utils->getOffloadService();
  if (wifi_offload_hal_ == nullptr) {
    LOG(ERROR) << "Failed to get Offload service";
    return;
  }
  service_available_ = true;
  wifi_offload_callback_ = utils->getOffloadCallback(
      ([this] (std::vector<ScanResult> scanResult) -> void {
        this->ReportScanResults(scanResult);
      }));
  wifi_offload_hal_->setEventCallback(wifi_offload_callback_);
}

OffloadScanManager::~OffloadScanManager() {}

void OffloadScanManager::ReportScanResults(
    const std::vector<ScanResult> scanResult) {
  std::vector<NativeScanResult> scanResult_;
  scanResult_ = OffloadScanUtils::convertToNativeScanResults(scanResult);
  scan_result_handler_(scanResult_);
}

bool OffloadScanManager::isServiceAvailable() const {
  return service_available_;
}

}  // namespace wificond
}  // namespace android
