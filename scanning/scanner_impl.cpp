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

#include "wificond/scanning/scanner_impl.h"

#include <vector>

#include <android-base/logging.h>

#include "wificond/scanning/scan_utils.h"

using android::binder::Status;
using android::net::wifi::IScanEventCallback;
using android::sp;
using com::android::server::wifi::wificond::NativeScanResult;
using com::android::server::wifi::wificond::SingleScanSettings;
using std::vector;

using namespace std::placeholders;

namespace android {
namespace wificond {

ScannerImpl::ScannerImpl(uint32_t interface_index,
                         ScanUtils* scan_utils)
    : valid_(true),
      is_scanning_(false),
      interface_index_(interface_index),
      scan_utils_(scan_utils) {
}

ScannerImpl::~ScannerImpl() {
}

void ScannerImpl::StartMonitoringScanResult() {
  scan_utils_->SubscribeScanResultNotification(
      interface_index_,
      std::bind(&ScannerImpl::OnScanResultsReadyInternal,
      this,
      _1, _2, _3, _4));
}

void ScannerImpl::StopMonitoringScanResult() {
  scan_utils_->UnsubscribeScanResultNotification(interface_index_);
}

Status ScannerImpl::startSingleScan(
    const SingleScanSettings& settings,
    const sp<IScanEventCallback>& scan_events) {
  if (!valid_) {
    return Status::ok();
  }
  // Ignore scan request because scan requests should be rescheduled by
  // WifiScanningService.
  if (is_scanning_) {
    LOG(ERROR) << "Wifi is already scanning";
    return Status::ok();
  }

  LOG(INFO) << "start single scan request";

  // Start a full scan.
  if (settings.is_full_scan) {
    LOG(INFO) << "request a full scan";
    if (!scan_utils_->StartFullScan(interface_index_)) {
      // Failed to start a full scan.
      scan_events->OnScanFailed();
      return Status::ok();
    }
  // Start a partial scan.
  } else {
    LOG(INFO) << "request a partial scan";
    // SSIDs for hidden networks.
    vector<vector<uint8_t>> ssids;
    for (auto& network : settings.hidden_networks) {
      ssids.push_back(network.ssid);
    }
    // Scan Channels.
    vector<uint32_t> freqs;
    for (auto& channel : settings.channel_settings) {
      freqs.push_back(channel.frequency);
    }
    if (!scan_utils_->Scan(interface_index_, ssids, freqs)) {
      // Failed to start a partial scan.
      scan_events->OnScanFailed();
      return Status::ok();
    }
  }
  is_scanning_ = true;
  StartMonitoringScanResult();
  if (scan_events != nullptr) {
    scan_events_ = scan_events;
  }

  return Status::ok();
}

void ScannerImpl::OnScanResultsReadyInternal(
    uint32_t interface_index,
    bool aborted,
    vector<vector<uint8_t>>& ssids,
    vector<uint32_t>& frequencies) {
  LOG(INFO) << __func__;
  // Ignore external scan result.
  if (!is_scanning_) {
    return;
  }
  is_scanning_ = false;

  StopMonitoringScanResult();

  // Underlying interface object was destroyed.
  // Ingore the scan result.
  if (!valid_) {
    return;
  }
  // No callback is required for this scan request.
  if (scan_events_ == nullptr) {
    return;
  }

  if (aborted) {
    LOG(ERROR) << "Scan aborted";
    scan_events_->OnScanFailed();
  }

  vector<NativeScanResult> scan_results;
  scan_utils_->GetScanResult(interface_index_, &scan_results);

  scan_events_->OnScanResult(scan_results);
  scan_events_ = nullptr;
}

}  // namespace wificond
}  // namespace android
