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
  std::vector<uint8_t> kSsid { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
} // namespace

class OffloadScanUtilsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
      ScanResult * pScanResult = new ScanResult();
      pScanResult->tsf = 0;
      pScanResult->rssi = -60;
      pScanResult->frequency = 2412;
      pScanResult->capability = 0;
      memcpy(&pScanResult->bssid[0], &kBssid[0], 6);
      pScanResult->networkInfo.ssid = kSsid;
      pScanResult->networkInfo.flags = 0;
      dummyScanResults.push_back(*pScanResult);
  }

  void TearDown() override {

  }

  std::vector<ScanResult> dummyScanResults;
};

TEST_F(OffloadScanUtilsTest, verifyConversion) {
  std::vector<NativeScanResult> nativeScanResult =
      OffloadScanUtils::convertToNativeScanResults(dummyScanResults);
  EXPECT_EQ(nativeScanResult.size(), dummyScanResults.size());
  for (size_t i = 0; i < nativeScanResult.size(); i++) {
    EXPECT_EQ(nativeScanResult[i].frequency, dummyScanResults[i].frequency);
    EXPECT_EQ(nativeScanResult[i].tsf, dummyScanResults[i].tsf);
    EXPECT_EQ(nativeScanResult[i].signal_mbm, dummyScanResults[i].rssi);
    EXPECT_EQ(nativeScanResult[i].ssid.size(), dummyScanResults[i].networkInfo.ssid.size());
    EXPECT_EQ(nativeScanResult[i].bssid.size(), dummyScanResults[i].bssid.elementCount());
    EXPECT_EQ(nativeScanResult[i].capability, dummyScanResults[i].capability);
  }
}

} // wificond
} // android
