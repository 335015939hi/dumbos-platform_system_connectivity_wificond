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
#include "wificond/scanning/offload/offload_scan_utils.h"
#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/tests/mock_offload_callback.h"
#include "wificond/tests/mock_offload.h"

using testing::NiceMock;
using std::unique_ptr;
using std::vector;

using com::android::server::wifi::wificond::NativeScanResult;
using android::hardware::wifi::offload::V1_0::ScanResult;

namespace android {
namespace wificond {

namespace {
  std::vector<uint8_t> kSsid { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
} // namespace

class OffloadScanUtilsTest: public ::testing::Test {
  protected:
    virtual void SetUp() {
      callbackInvoked = false;
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
      callbackInvoked = false;
      dut = nullptr;
      dummyScanResults.erase(dummyScanResults.begin(), dummyScanResults.begin() + dummyScanResults.size());
    }

    void onOffloadScanResultHandler(std::vector<NativeScanResult> scanResult) {
      callbackInvoked = true;
    }

    bool callbackInvoked;
    std::unique_ptr<OffloadScanUtils> dut;
    std::vector<ScanResult> dummyScanResults;
    NiceMock<MockOffload> *mockIOffload;
    NiceMock<MockOffloadCallback> *mockOffloadCallback;
};

TEST_F(OffloadScanUtilsTest, ServiceAvailableTest) {
  mockIOffload = new NiceMock<MockOffload>();
  dut = unique_ptr<OffloadScanUtils>(new OffloadScanUtils(
      mockIOffload, nullptr));
  EXPECT_EQ(true, dut->get_service_available());
}

TEST_F(OffloadScanUtilsTest, ServiceNotAvailableTest) {
  dut = unique_ptr<OffloadScanUtils>(new OffloadScanUtils(
	    nullptr, nullptr));
  EXPECT_EQ(false, dut->get_service_available());
}

TEST_F(OffloadScanUtilsTest, CallbackInvokedTest) {
  mockIOffload = new NiceMock<MockOffload>();
  mockOffloadCallback = new NiceMock<MockOffloadCallback>(
      [this] (std::vector<NativeScanResult> scanResult) -> void {
        this->onOffloadScanResultHandler(scanResult);
    });
  dut = unique_ptr<OffloadScanUtils>(new OffloadScanUtils(mockIOffload,
            mockOffloadCallback));
  mockOffloadCallback->injectScanResults(dummyScanResults);
  EXPECT_EQ(true, callbackInvoked);
}

} // wificond
} //android
