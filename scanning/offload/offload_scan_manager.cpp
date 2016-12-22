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
#include "wificond/scanning/offload/offload_scan_utils.h"
#include "wificond/scanning/scan_result.h"

#include <vector>

using ::android::hardware::hidl_vec;
using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using android::hardware::wifi::offload::V1_0::ScanResult;
using android::wificond::OnNativeScanResultsReadyHandler;

namespace android {
namespace wificond {

NativeScanResultHandler::NativeScanResultHandler(OnNativeScanResultsReadyHandler handler) 
	: handler_(handler) {	
}

void NativeScanResultHandler::callback(const std::vector<ScanResult> &scanResult) {
  std::vector<::com::android::server::wifi::wificond::NativeScanResult> scanResult_;
  scanResult_ = OffloadScanUtils::convertToNativeScanResults(scanResult);
  handler_(scanResult_);
}

NativeScanResultHandler::~NativeScanResultHandler() {}

OffloadScanManager::OffloadScanManager(OnNativeScanResultsReadyHandler handler)
    : scan_result_handler_(new NativeScanResultHandler(handler)) {
  wifi_offload_hal_ = OffloadScanUtils::getOffloadService();
  if (wifi_offload_hal_ == nullptr) {
      service_available_ = false;
      LOG(ERROR) << "Failed to get Offload service";
      return;
  }
  service_available_ = true;
  wifi_offload_callback_ = OffloadScanUtils::getOffloadCallback(
      ([this] (std::vector<ScanResult> scanResult) -> void {
          this->reportScanResults(scanResult);
      }));
  wifi_offload_hal_->setEventCallback(wifi_offload_callback_);
}

OffloadScanManager::~OffloadScanManager() {}

void OffloadScanManager::reportScanResults(const std::vector<ScanResult> scanResult) {
  scan_result_handler_->callback(scanResult);
}

bool OffloadScanManager::get_service_available() {
  return service_available_;
}

}  // namespace wificond
}  // namespace android
