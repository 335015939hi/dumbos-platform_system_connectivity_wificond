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

#include <vector>

#include <gtest/gtest.h>

#include "wificond/scanning/channel_settings.h"
#include "wificond/scanning/hidden_network.h"
#include "wificond/scanning/single_scan_settings.h"

using ::com::android::server::wifi::wificond::ChannelSettings;
using ::com::android::server::wifi::wificond::HiddenNetwork;
using ::com::android::server::wifi::wificond::SingleScanSettings;
using std::vector;

namespace android {
namespace wificond {

namespace {

const uint8_t kFakeSsid[] =
    {'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};

const bool kFakeIsFullScan = false;
constexpr uint32_t kFakeFrequency = 5260;
constexpr uint32_t kFakeFrequency1 = 2460;
constexpr uint32_t kFakeFrequency2 = 2500;

}  // namespace

class ScanSettingsTest : public ::testing::Test {
 public:
  // Helper functions for testing if two parcelable objects are the same
  // as expected.
  void expect_equal(ChannelSettings& channel1, ChannelSettings& channel2) {
    EXPECT_EQ(channel1.frequency, channel2.frequency);
  }
  void expect_equal(HiddenNetwork& network1, HiddenNetwork& network2) {
    EXPECT_EQ(network1.ssid, network2.ssid);
  }
  void expect_equal(SingleScanSettings& scan1, SingleScanSettings& scan2) {
    EXPECT_EQ(scan1.is_full_scan, scan2.is_full_scan);
    EXPECT_EQ(scan1.channel_settings.size(), scan2.channel_settings.size());
    for (unsigned int i = 0; i < scan1.channel_settings.size(); i++) {
      expect_equal(scan1.channel_settings[i], scan2.channel_settings[i]);
    }
    EXPECT_EQ(scan1.hidden_networks.size(), scan2.hidden_networks.size());
    for (unsigned int i = 0; i < scan1.hidden_networks.size(); i++) {
      expect_equal(scan1.hidden_networks[i], scan2.hidden_networks[i]);
    }
  }
};

TEST_F(ScanSettingsTest, ChannelSettingsParcelableTest) {
  ChannelSettings channel_settings;
  channel_settings.frequency = kFakeFrequency;

  Parcel parcel;
  EXPECT_EQ(::android::OK, channel_settings.writeToParcel(&parcel));

  ChannelSettings channel_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, channel_settings_copy.readFromParcel(&parcel));

  expect_equal(channel_settings, channel_settings_copy);
}

TEST_F(ScanSettingsTest, HiddenNetworkParcelableTest) {
  HiddenNetwork hidden_network;
  std::vector<uint8_t> ssid(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));
  hidden_network.ssid = ssid;

  Parcel parcel;
  EXPECT_EQ(::android::OK, hidden_network.writeToParcel(&parcel));

  HiddenNetwork hidden_network_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, hidden_network_copy.readFromParcel(&parcel));

  expect_equal(hidden_network, hidden_network_copy);
}

TEST_F(ScanSettingsTest, SingleScanSettingsParcelableTest) {
  SingleScanSettings scan_settings;
  scan_settings.is_full_scan = kFakeIsFullScan;

  ChannelSettings channel, channel1, channel2;
  channel.frequency= kFakeFrequency;
  channel1.frequency= kFakeFrequency1;
  channel2.frequency= kFakeFrequency2;

  HiddenNetwork network;
  std::vector<uint8_t> ssid(kFakeSsid, kFakeSsid + sizeof(kFakeSsid));
  network.ssid = ssid;

  scan_settings.channel_settings.push_back(channel);
  scan_settings.channel_settings.push_back(channel1);
  scan_settings.channel_settings.push_back(channel2);
  scan_settings.hidden_networks.push_back(network);

  Parcel parcel;
  EXPECT_EQ(::android::OK, scan_settings.writeToParcel(&parcel));

  SingleScanSettings scan_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, scan_settings_copy.readFromParcel(&parcel));

  expect_equal(scan_settings, scan_settings_copy);
}

}  // namespace wificond
}  // namespace android
