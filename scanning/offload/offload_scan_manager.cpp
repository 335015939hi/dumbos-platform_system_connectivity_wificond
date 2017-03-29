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
using android::hardware::wifi::offload::V1_0::OffloadStatus;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using ::com::android::server::wifi::wificond::NativeScanResult;
using android::wificond::OnNativeScanResultsReadyHandler;

namespace android {
namespace wificond {

OffloadScanManager::OffloadScanManager(OffloadServiceUtils *utils,
    OnNativeScanResultsReadyHandler handler)
    : wifi_offload_hal_(nullptr),
      wifi_offload_callback_(nullptr),
      scan_result_handler_(handler),
      offload_status_(OffloadScanManager::kError) {
  if (utils == nullptr) {
    LOG(ERROR) << "Invalid arguments for Offload ScanManager";
    return;
  }
  if (scan_result_handler_ == nullptr) {
    LOG(ERROR) << "Invalid Offload scan result handler";
    return;
  }
  wifi_offload_hal_ = utils->GetOffloadService();
  if (wifi_offload_hal_ == nullptr) {
    LOG(ERROR) << "Failed to get Offload service";
    offload_status_ = OffloadScanManager::kNoService;
    return;
  }
  wifi_offload_callback_ = utils->GetOffloadCallback(
      ([this] (std::vector<ScanResult> scanResult) -> void {
        this->ReportScanResults(scanResult);
      }),
      ([this] (OffloadStatus status) -> void {
        this->ReportError(status);
      }));
  if (wifi_offload_callback_ == nullptr) {  
    offload_status_ = OffloadScanManager::kNoService;
    LOG(ERROR) << "Invalid Offload callback object";
    return;
  }
  wifi_offload_hal_->setEventCallback(wifi_offload_callback_);
  offload_status_ = OffloadScanManager::kNoError;  
}

OffloadScanManager::~OffloadScanManager() {}

void OffloadScanManager::ReportScanResults(
    const std::vector<ScanResult> scanResult) {
  std::vector<NativeScanResult> scanResult_;
  scanResult_ = OffloadScanUtils::convertToNativeScanResults(scanResult);
  if (scan_result_handler_ != nullptr) {
    scan_result_handler_(scanResult_);
  } else {
    LOG(ERROR) << "No scan result handler for Offload ScanManager";
  }
}

void OffloadScanManager::ReportError(OffloadStatus status) {
  switch(status) {
    case OffloadStatus::OFFLOAD_STATUS_OK:
      offload_status_ = OffloadScanManager::kNoError;
      break;
    case OffloadStatus::OFFLOAD_STATUS_TIMEOUT:
      offload_status_ = OffloadScanManager::kTimeOut;
      break;
    case OffloadStatus::OFFLOAD_STATUS_NO_CONNECTION:
      offload_status_ = OffloadScanManager::kNotConnected;
      break;
    case OffloadStatus::OFFLOAD_STATUS_ERROR:
      offload_status_ = OffloadScanManager::kError;
      break;
    default:
      break;
  }
  if (offload_status_ != OffloadScanManager::kNoError) {
    LOG(WARNING) << "Offload Error reported " << offload_status_;
  }
}

OffloadScanManager::StatusCode OffloadScanManager::getOffloadStatus() const {
  return offload_status_;
}

}  // namespace wificond
}  // namespace android
