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

#include "wificond/scanning/offload/offload_scan_utils.h"
#include "wificond/scanning/scan_result.h"

using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::IOffloadCallback;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using android::hardware::wifi::offload::V1_0::ScanFilter;
using android::hardware::wifi::offload::V1_0::ScanParam;
using android::hardware::wifi::offload::V1_0::ScanStats;
using android::hardware::wifi::offload::V1_0::ScanResult;
using com::android::server::wifi::wificond::NativeScanStats;
using com::android::server::wifi::wificond::NativeScanResult;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanStats;

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com


namespace android {
namespace wificond {

NativeScanResultHandler::NativeScanResultHandler(
    OnOffloadScanResultsReadyHandler handler)
    : handler_(handler) {
}

NativeScanResultHandler::~NativeScanResultHandler() {};

void NativeScanResultHandler::callback(
    std::vector<NativeScanResult> scanResult) {
  handler_(scanResult);
}

OffloadScanUtils::OffloadScanUtils(OnOffloadScanResultsReadyHandler handler)
    : native_scan_result_handler(new NativeScanResultHandler(handler)) {
  wifi_offload_hal_ = IOffload::getService("wifiOffload");
  if (wifi_offload_hal_ == nullptr) {
      service_available_ = false;
      LOG(ERROR) << "Failed to get Offload service";
      return;
  }
  service_available_ = true;
  wifi_offload_callback_ =
      new OffloadCallback(
      [this] (std::vector<ScanResult> scanResult) {
        this->reportScanResults(scanResult);
      } );
  wifi_offload_hal_->setEventCallback(wifi_offload_callback_);
}

OffloadScanUtils::~OffloadScanUtils() {}

bool OffloadScanUtils::get_service_available() {
  return service_available_;
}

void OffloadScanUtils::reportScanResults(
    std::vector<ScanResult> offloadScanResult) {
  std::vector<NativeScanResult> scanResult;
  scanResult.reserve(offloadScanResult.size());
  // TODO: convert ScanResults from offload module to Wifi cond format
  if (scanResult.size() > 0) {
    native_scan_result_handler->callback(scanResult);
  }   
}

}  // namespace wificond
}  // namespace android
