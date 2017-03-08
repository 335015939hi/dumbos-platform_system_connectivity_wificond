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
#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/tests/mock_offload_callback.h"
#include "wificond/tests/mock_offload.h"
#include "wificond/tests/mock_offload_service_utils.h"
#include "wificond/scanning/offload/offload_scan_manager.h"

using testing::NiceMock;
using std::unique_ptr;
using std::vector;

using com::android::server::wifi::wificond::NativeScanResult;
using android::hardware::wifi::offload::V1_0::ScanResult;

namespace android {
namespace wificond {

/*namespace {
  std::vector<uint8_t> kSsid { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
} // namespace
*/
class OffloadScanManagerTest: public ::testing::Test {
  protected:
    virtual void SetUp() {
      mockOffloadServiceUtils = new NiceMock<MockOffloadServiceUtils>();
  /*    callbackInvoked = false;
      ScanResult * pScanResult = new ScanResult();
      pScanResult->tsf = 0;
      pScanResult->rssi = -60;
      pScanResult->frequency = 2412;
      pScanResult->capability = 0;
      memcpy(&pScanResult->bssid[0], &kBssid[0], 6);
      pScanResult->networkInfo.ssid = kSsid;
      pScanResult->networkInfo.flags = 0;
      dummyScanResults.push_back(*pScanResult);*/
    }

    void TearDown() override {
      dut =  nullptr;
      delete mockOffloadServiceUtils;
/*      callbackInvoked = false;
      dut = nullptr;
      dummyScanResults.erase(dummyScanResults.begin(), dummyScanResults.begin() + dummyScanResults.size());*/
    }

    void onNativeScanResultHandler(std::vector<NativeScanResult> scanResult) {
//      callbackInvoked = true;
    }

/*    bool callbackInvoked;
    std::unique_ptr<OffloadScanManager> dut;
    std::vector<ScanResult> dummyScanResults;
*/  NiceMock<MockOffload> *mockIOffload;
    NiceMock<MockOffloadCallback> *mockOffloadCallback;
    NiceMock<MockOffloadServiceUtils> *mockOffloadServiceUtils;
    std::unique_ptr<OffloadScanManager> dut;
};

TEST_F(OffloadScanManagerTest, ServiceUtilsNotAvailableTest) {
  dut = unique_ptr<OffloadScanManager>(new OffloadScanManager(
            nullptr, nullptr));
  EXPECT_EQ(false, dut->isServiceAvailable());
}

TEST_F(OffloadScanManagerTest, ServiceNotAvailableTest) {
  ON_CALL(*mockOffloadServiceUtils, GetOffloadService())
      .WillByDefault(testing::Return(nullptr));
  dut = std::unique_ptr<OffloadScanManager>(new OffloadScanManager(
    mockOffloadServiceUtils, nullptr));
  EXPECT_EQ(false, dut->isServiceAvailable());
}

TEST_F(OffloadScanManagerTest, ServiceAvailableTest) {
  mockIOffload = new NiceMock<MockOffload>();
  mockOffloadCallback = new NiceMock<MockOffloadCallback>(
    [] (std::vector<ScanResult> scanResult) -> void {});
  EXPECT_CALL(*mockOffloadServiceUtils, GetOffloadService());
  ON_CALL(*mockOffloadServiceUtils, GetOffloadService())
      .WillByDefault(testing::Return(mockIOffload));
  ON_CALL(*mockOffloadServiceUtils, GetOffloadCallback(testing::_))
      .WillByDefault(testing::Return(mockOffloadCallback));
  dut = std::unique_ptr<OffloadScanManager>(new OffloadScanManager(
    mockOffloadServiceUtils,   
    [this] (std::vector<NativeScanResult> scanResult) -> void {
        this->onNativeScanResultHandler(scanResult);
    }));
  EXPECT_EQ(true, dut->isServiceAvailable());
}


/*
TEST_F(OffloadScanManagerTest, CallbackInvokedTest) {
  mockIOffload = new NiceMock<MockOffload>();
  mockOffloadCallback = new NiceMock<MockOffloadCallback>(
      [this] (std::vector<ScanResult> scanResult) -> void {
          std::vector<NativeScanResult> scanResult_ = 
	      OffloadScanUtils::convertToNativeScanResult(scanResult);
      });
  dut = unique_ptr<OffloadScanManager>(
            mockOffloadCallback));
  mockOffloadCallback->injectScanResults(dummyScanResults);
  EXPECT_EQ(true, callbackInvoked);
}
*/
} // wificond
} //android
