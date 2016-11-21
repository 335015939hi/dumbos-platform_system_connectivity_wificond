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

#include "wificond/scanning/bucket_settings.h"
#include "wificond/scanning/channel_settings.h"
#include "wificond/scanning/hidden_network.h"
#include "wificond/scanning/scan_settings.h"

using ::com::android::server::wifi::wificond::ChannelSettings;
using ::com::android::server::wifi::wificond::HiddenNetwork;
using ::com::android::server::wifi::wificond::NativeScanSettings;
using std::vector;

namespace android {
namespace wificond {

namespace {

const uint8_t kFakeSsid[] =
    {'G', 'o', 'o', 'g', 'l', 'e', 'G', 'u', 'e', 's', 't'};
constexpr int32_t kFakeFrequency = 5260;
constexpr int32_t kFakeChannelDwellTimeMs = 40;
constexpr bool kFakeIsChannelPassive = true;

}  // namespace

class ScanSettingsTest : public ::testing::Test {
 public:
  void expect_equal(HiddenNetwork& network1, HiddenNetwork& network2) {
    EXPECT_EQ(network1.ssid, network2.ssid);
  }
};

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

TEST_F(ScanSettingsTest, ChannelSettingsParcelableTest) {
  ChannelSettings channel_settings;
  channel_settings.frequency = kFakeFrequency;
  channel_settings.dwell_time_ms = kFakeChannelDwellTimeMs;
  channel_settings.passive = kFakeIsChannelPassive;
  Parcel parcel;
  EXPECT_EQ(::android::OK, channel_settings.writeToParcel(&parcel));

  ChannelSettings channel_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, channel_settings_copy.readFromParcel(&parcel));

  //EXPECT_EQ(channel_settings, channel_settings_copy);
  EXPECT_EQ(channel_settings.frequency, channel_settings_copy.frequency);
  EXPECT_EQ(channel_settings.dwell_time_ms, channel_settings_copy.dwell_time_ms);
  EXPECT_EQ(channel_settings.passive, channel_settings_copy.passive);
}
/*
TEST_F(ScanSettingsTest, BucketSettingsParcelableTest) {
  BucketSettings bucket_settings;

  Parcel parcel;
  EXPECT_EQ(::android::OK, hidden_network.writeToParcel(&parcel));

  BucketSettings bucket_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, bucket_settings_copy.readFromParcel(&parcel));

  EXPECT_EQ(bucket_settings.bucket, bucket_settings.bucket);
  EXPECT_EQ(bucket_settings.band, bucket_settings.band);
}*/

TEST_F(ScanSettingsTest, ParcelableTest) {
  EXPECT_TRUE(true);
}

}  // namespace wificond
}  // namespace android
