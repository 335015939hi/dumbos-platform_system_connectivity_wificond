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

#include <android-base/logging.h>
#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/offload/offload_callback.h"

using std::unique_ptr;
using std::vector;

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using android::hardware::hidl_vec;

namespace android {
namespace wificond {

namespace {
  std::vector<uint8_t> kSsid { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
} // namespace

class OffloadCallbackTest: public ::testing::Test {
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
      scanResultSize = 0;
    }

    void TearDown() override {
      dummyScanResults.erase(dummyScanResults.begin(),
        dummyScanResults.begin() + dummyScanResults.size());
      scanResultSize = 0;
    }

    void onOffloadScanResultHandler(std::vector<ScanResult> scanResult) {
      scanResultSize = scanResult.size();
    }

    std::vector<ScanResult> dummyScanResults;
    std::unique_ptr<OffloadCallback> dut;
    unsigned long scanResultSize;
};

TEST_F(OffloadCallbackTest, checkScanResultSize) {
  dut = unique_ptr<OffloadCallback>(new OffloadCallback(
    [this] (std::vector<ScanResult> scanResult) -> void {
        this->onOffloadScanResultHandler(scanResult);
    }));
  hidl_vec<ScanResult> offloadScanResult(dummyScanResults);
  dut->onScanResult(offloadScanResult); 
  EXPECT_EQ(dummyScanResults.size(), scanResultSize);
}

} // wificond
} //android
