/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <functional>
#include <memory>
#include <vector>
#include <string>

#include <gtest/gtest.h>

#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/offload/offload_scan_utils.h"

using android::hardware::wifi::offload::V1_0::ScanResult;
using ::com::android::server::wifi::wificond::NativeScanResult;

namespace android {
namespace wificond {

namespace {
  uint8_t kSsid[] = { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
  int16_t kRssi = -60;
  uint32_t kFrequency = 2412;
  uint8_t kBssidSize = 6;
} // namespace

class OffloadScanUtilsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    ScanResult scanResult;
    std::vector<uint8_t> ssid(kSsid, kSsid + sizeof(kSsid));
    scanResult.tsf = 0;
    scanResult.rssi = kRssi;
    scanResult.frequency = kFrequency;
    scanResult.capability = 0;
    memcpy(&scanResult.bssid[0], &kBssid[0], kBssidSize);
    scanResult.networkInfo.ssid = ssid;
    scanResult.networkInfo.flags = 0;
    dummy_scan_results.push_back(scanResult);
  }

  void TearDown() override {
    dummy_scan_results.clear();
  }

  std::vector<ScanResult> dummy_scan_results;
};

TEST_F(OffloadScanUtilsTest, verifyConversion) {
  std::vector<NativeScanResult> native_scan_results =
      OffloadScanUtils::convertToNativeScanResults(dummy_scan_results);
  EXPECT_EQ(native_scan_results.size(), dummy_scan_results.size());
  for (size_t i = 0; i < native_scan_results.size(); i++) {
    EXPECT_EQ(native_scan_results[i].frequency, dummy_scan_results[i].frequency);
    EXPECT_EQ(native_scan_results[i].tsf, dummy_scan_results[i].tsf);
    EXPECT_EQ(native_scan_results[i].signal_mbm, dummy_scan_results[i].rssi);
    EXPECT_EQ(native_scan_results[i].ssid.size(), dummy_scan_results[i].networkInfo.ssid.size());
    EXPECT_EQ(native_scan_results[i].bssid.size(), dummy_scan_results[i].bssid.elementCount());
    EXPECT_EQ(native_scan_results[i].capability, dummy_scan_results[i].capability);
  }
}

} // namespace wificond
} // namespace android
