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
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wificond/scanning/scanner_impl.h"
#include "wificond/tests/mock_netlink_manager.h"
#include "wificond/tests/mock_netlink_utils.h"
#include "wificond/tests/mock_scan_utils.h"

using std::unique_ptr;
using std::vector;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace android {
namespace wificond {
namespace {

const uint32_t kTestInterfaceIndex = 21;

class ScannerImplTest : public ::testing::Test {
 protected:
  void SetUp() override {
     scanner_.reset(new ScannerImpl{
        kTestInterfaceIndex,
        band_info_,
        scan_capabilities_,
        wiphy_features_,
        scan_utils_.get()});
  }

  void TearDown() override {
  }

 private:
  unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_{
      new NiceMock<MockNetlinkManager>()};
  unique_ptr<NiceMock<MockScanUtils>> scan_utils_{
      new NiceMock<MockScanUtils>(netlink_manager_.get())};
  unique_ptr<ScannerImpl> scanner_;

  BandInfo band_info_;
  ScanCapabilities scan_capabilities_;
  WiphyFeatures wiphy_features_;
};  // class ScannerImplTest

}  // namespace

TEST_F(ScannerImplTest, uccess) {
  EXPECT_TRUE(true);
}

}  // namespace wificond
}  // namespace android
