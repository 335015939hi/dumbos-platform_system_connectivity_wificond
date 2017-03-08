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

#include "wificond/tests/mock_offload_callback.h"
#include "wificond/tests/mock_offload.h"
#include "wificond/tests/mock_offload_service_utils.h"

#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/offload/offload_scan_manager.h"
#include <android/hardware/wifi/offload/1.0/IOffload.h>

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::implementation::OnOffloadScanResultsReadyHandler;
using com::android::server::wifi::wificond::NativeScanResult;
using testing::NiceMock;

namespace android {
namespace wificond {

namespace {
  std::vector<uint8_t> kSsid { 'G', 'o', 'o', 'g', 'l', 'e' };
  uint8_t kBssid [6] = { 0x12, 0xef, 0xa1, 0x2c, 0x97, 0x8b };
} // namespace

class OffloadScanManagerTest: public ::testing::Test {
  protected:
    virtual void SetUp() {
      mockOffloadServiceUtils = new NiceMock<MockOffloadServiceUtils>();
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
      dut =  nullptr;
      delete mockOffloadServiceUtils;
      callbackInvoked = false;
      dut = nullptr;
      dummyScanResults.erase(dummyScanResults.begin(),
          dummyScanResults.begin() + dummyScanResults.size());
    }

    void onNativeScanResultHandler(std::vector<NativeScanResult> scanResult) {
      callbackInvoked = true;
    }

    NiceMock<MockOffloadCallback> * CaptureHandler(
      OnOffloadScanResultsReadyHandler handler) {
      handler_ = handler;
      mockOffloadCallback = new NiceMock<MockOffloadCallback>(handler_);
      return mockOffloadCallback;
    }

    std::vector<ScanResult> dummyScanResults;
    bool callbackInvoked;
    NiceMock<MockOffload> *mockIOffload;
    NiceMock<MockOffloadCallback> *mockOffloadCallback;
    NiceMock<MockOffloadServiceUtils> *mockOffloadServiceUtils;
    std::unique_ptr<OffloadScanManager> dut;
    OnOffloadScanResultsReadyHandler handler_;
};

/**
 * Testing OffloadScanManager with OffloadServiceUtils null argument
 */
TEST_F(OffloadScanManagerTest, ServiceUtilsNotAvailableTest) {
  dut = std::unique_ptr<OffloadScanManager>(new OffloadScanManager(
            nullptr, nullptr));
  EXPECT_EQ(false, dut->isServiceAvailable());
}

/**
 * Testing OffloadScanManager with no handle on Offloal HAL service
 * and no registered handler for Offload Scan results
 */
TEST_F(OffloadScanManagerTest, ServiceNotAvailableTest) {
  ON_CALL(*mockOffloadServiceUtils, GetOffloadService())
      .WillByDefault(testing::Return(nullptr));
  dut = std::unique_ptr<OffloadScanManager>(new OffloadScanManager(
    mockOffloadServiceUtils, nullptr));
  EXPECT_EQ(false, dut->isServiceAvailable());
}

/**
 * Testing OffloadScanManager when service is available and valid handler
 * registered for Offload Scan results
 */
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
    [] (std::vector<NativeScanResult> scanResult) -> void {}));
  EXPECT_EQ(true, dut->isServiceAvailable());
}

/**
 * Testing OffloadScanManager when service is available and valid handler
 * is registered, test to ensure that registered handler is invoked when
 * scan results are available
 */
TEST_F(OffloadScanManagerTest, CallbackInvokedTest) {
  OnOffloadScanResultsReadyHandler handler;
  mockIOffload = new NiceMock<MockOffload>();
  EXPECT_CALL(*mockOffloadServiceUtils, GetOffloadService());
  ON_CALL(*mockOffloadServiceUtils, GetOffloadService())
      .WillByDefault(testing::Return(mockIOffload));
  ON_CALL(*mockOffloadServiceUtils, GetOffloadCallback(testing::_))
      .WillByDefault(testing::Invoke(
          [this] (OnOffloadScanResultsReadyHandler handler)
              -> NiceMock<MockOffloadCallback> * {
            return this->CaptureHandler(handler);
          }));
  dut = std::unique_ptr<OffloadScanManager>(new OffloadScanManager(
    mockOffloadServiceUtils,
    [this] (std::vector<NativeScanResult> scanResult) -> void {
        this->onNativeScanResultHandler(scanResult);
    }));
  mockOffloadCallback->onScanResult(dummyScanResults);
  EXPECT_EQ(true, callbackInvoked);
}

} // wificond
} //android
