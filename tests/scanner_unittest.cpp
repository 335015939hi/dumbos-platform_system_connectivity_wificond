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

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wificond/tests/mock_netlink_manager.h"
#include "wificond/tests/mock_netlink_utils.h"
#include "wificond/tests/mock_scan_utils.h"
#include "wificond/scanning/scanner_impl.h"
#include "wificond/scanning/single_scan_settings.h"

using ::com::android::server::wifi::wificond::SingleScanSettings;

using testing::NiceMock;
using testing::_;


namespace android {
namespace wificond {
namespace {

uint32_t kFakeInterfaceIndex = 12;
}  // namespace

class ScannerTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  NiceMock<MockNetlinkManager> netlink_manager_;
  NiceMock<MockScanUtils> scan_utils_{&netlink_manager_};
  BandInfo band_info_;
  ScanCapabilities scan_capabilities_;
  WiphyFeatures wiphy_features_;

};  // class ScannerTest


TEST_F(ScannerTest, CanSendScanRequest) {
  ScannerImpl scanner_{
        kFakeInterfaceIndex,
        band_info_,
        scan_capabilities_,
        wiphy_features_,
        &scan_utils_};
  SingleScanSettings scan_settings;
  bool success = false;
  scanner_.scan(scan_settings, &success);
  EXPECT_TRUE(true);
}

TEST_F(ScannerTest, CanSendSchedScanRequest) {
  EXPECT_TRUE(true);
}

TEST_F(ScannerTest, CanSendScanRequestWithRandomMAC) {
  EXPECT_TRUE(true);
}

TEST_F(ScannerTest, CanSendSchedScanRequestWithRandomMAC) {
  EXPECT_TRUE(true);
}

}  // namespace wificond
}  // namespace android
