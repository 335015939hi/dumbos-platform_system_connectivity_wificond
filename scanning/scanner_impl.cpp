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

#include <string>
#include <vector>

#include <android-base/logging.h>

#include "wificond/scanning/scan_utils.h"

using android::binder::Status;
using android::net::wifi::IScanEvent;
using android::String16;
using android::sp;
using com::android::server::wifi::wificond::NativeScanResult;
using com::android::server::wifi::wificond::PnoSettings;
using std::string;
using std::vector;

using namespace std::placeholders;

namespace android {
namespace wificond {

ScannerImpl::ScannerImpl(uint32_t interface_index,
                         const BandInfo& band_info,
                         const ScanCapabilities& scan_capabilities,
                         const WiphyFeatures& wiphy_features,
                         ScanUtils* scan_utils)
    : valid_(true),
      interface_index_(interface_index),
      band_info_(band_info),
      scan_capabilities_(scan_capabilities),
      wiphy_features_(wiphy_features),
      scan_utils_(scan_utils),
      scan_event_handler_(nullptr) {
  scan_utils_->SubscribeScanResultNotification(
      interface_index,
      std::bind(&ScannerImpl::OnScanResultsReady,
                this,
                _1, _2, _3, _4));

  scan_utils_->SubscribeSchedScanResultNotification(
      interface_index,
      std::bind(&ScannerImpl::OnSchedScanResultsReady,
                this, _1));
}

ScannerImpl::~ScannerImpl() {
  scan_utils_->UnsubscribeScanResultNotification(interface_index_);
  scan_utils_->UnsubscribeSchedScanResultNotification(interface_index_);
}

bool ScannerImpl::CheckIsValid() {
  if (!valid_) {
    LOG(DEBUG) << "Calling on a invalid scanner object."
               << "Underlying client interface object was destroyed.";
  }
  return valid_;
}

Status ScannerImpl::getAvailable2gChannels(vector<int32_t>* out_frequencies) {
  if (!CheckIsValid()) {
    return Status::ok();
  }
  *out_frequencies = vector<int32_t>(band_info_.band_2g.begin(),
                                     band_info_.band_2g.end());
  return Status::ok();
}

Status ScannerImpl::getAvailable5gNonDFSChannels(
    vector<int32_t>* out_frequencies) {
  if (!CheckIsValid()) {
    return Status::ok();
  }
  *out_frequencies = vector<int32_t>(band_info_.band_5g.begin(),
                                     band_info_.band_5g.end());
  return Status::ok();
}

Status ScannerImpl::getAvailableDFSChannels(vector<int32_t>* out_frequencies) {
  if (!CheckIsValid()) {
    return Status::ok();
  }
  *out_frequencies = vector<int32_t>(band_info_.band_dfs.begin(),
                                     band_info_.band_dfs.end());
  return Status::ok();
}

Status ScannerImpl::getScanResults(vector<NativeScanResult>* out_scan_results) {
  if (!scan_utils_->GetScanResult(interface_index_, out_scan_results)) {
    LOG(ERROR) << "Failed to get scan results from NL80211";
  }
  return Status::ok();
}

Status ScannerImpl::scan(const vector<int32_t>& freqs,
                         const vector<String16>& ssids,
                         bool request_random_mac,
                         bool* out_success) {
  vector<vector<uint8_t>> raw_ssids;
  // Add an empty ssid for a wild card scan.
  raw_ssids.push_back({0});
  for (auto& ssid : ssids) {
    string ssid_str = String16::std_string(ssid);
    raw_ssids.emplace_back(vector<uint8_t>(ssid_str.begin(), ssid_str.end()));
  }
  vector<uint32_t> freqs_converted(freqs.begin(), freqs.end());
  bool random_mac = wiphy_features_.supports_random_mac_oneshot_scan &&
                    request_random_mac;
  if (!scan_utils_->Scan(interface_index_, random_mac, raw_ssids, freqs_converted)) {
    *out_success = false;
    LOG(ERROR) << "Failed to start scan";
    return Status::ok();
  }
  *out_success = true;
  return Status::ok();
}

Status ScannerImpl::StartPnoScan(const PnoSettings& pno_settings,
                                 bool request_random_mac,
                                 bool* out_success) {
  // An empty ssid for a wild card scan.
  vector<vector<uint8_t>> scan_ssids = {{0}};
  vector<vector<uint8_t>> match_ssids;
  // Empty frequemcy list: scan all frequencies.
  vector<uint32_t> freqs;
  for (auto& network : pno_settings.pno_networks_) {
    match_ssids.push_back(network.ssid_);
  }
  bool random_mac = wiphy_features_.supports_random_mac_sched_scan &&
                    request_random_mac;
  if (!scan_utils_->StartScheduledScan(interface_index_,
                                       pno_settings.interval_ms_,
                                       // TODO: honor both rssi thresholds.
                                       pno_settings.min_2g_rssi_,
                                       random_mac,
                                       scan_ssids,
                                       match_ssids,
                                       freqs)) {
    *out_success = false;
    LOG(ERROR) << "Failed to start scheduled scan";
    return Status::ok();
  }
  *out_success = true;
  return Status::ok();
}

Status ScannerImpl::StopPnoScan(bool* out_success) {
  *out_success = scan_utils_->StopScheduledScan(interface_index_);
  return Status::ok();
}

Status ScannerImpl::SubscribeScanResultReadyNotification(
    const sp<::android::net::wifi::IScanEvent>& handler) {
  scan_event_handler_ = handler;
  return Status::ok();
}

Status ScannerImpl::UnsubscribeScanResultReadyNotification() {
  scan_event_handler_ = nullptr;
  return Status::ok();
}

void ScannerImpl::OnScanResultsReady(
    uint32_t interface_index,
    bool aborted,
    std::vector<std::vector<uint8_t>>& ssids,
    std::vector<uint32_t>& frequencies) {
  if (scan_event_handler_ != nullptr) {
    // TODO: Pass other parameters back if framework needs them.
    scan_event_handler_->OnScanResultReady();
  }
}

void ScannerImpl::OnSchedScanResultsReady(uint32_t interface_index) {
  if (scan_event_handler_ != nullptr) {
    scan_event_handler_->OnScanResultReady();
  }
}

}  // namespace wificond
}  // namespace android
