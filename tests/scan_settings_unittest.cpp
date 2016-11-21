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

using ::com::android::server::wifi::wificond::BucketSettings;
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
  // Helper functions for testing if two parcelable objects are the same
  // as expected.
  void expect_equal(HiddenNetwork& network1, HiddenNetwork& network2) {
    EXPECT_EQ(network1.ssid, network2.ssid);
  }

  void expect_equal(ChannelSettings& channel1, ChannelSettings& channel2) {
    EXPECT_EQ(channel1.frequency, channel2.frequency);
    EXPECT_EQ(channel1.dwell_time_ms, channel2.dwell_time_ms);
    EXPECT_EQ(channel1.passive, channel2.passive);
  }

  void expect_equal(BucketSettings& bucket1, BucketSettings& bucket2) {
    EXPECT_EQ(bucket1.bucket, bucket2.bucket);
    EXPECT_EQ(bucket1.band, bucket2.band);
    EXPECT_EQ(bucket1.period_ms, bucket2.period_ms);
    EXPECT_EQ(bucket1.max_period_ms, bucket2.max_period_ms);
    EXPECT_EQ(bucket1.step_count, bucket2.step_count);
    EXPECT_EQ(bucket1.report_events, bucket2.report_events);
    EXPECT_EQ(bucket1.channel_settings.size(), bucket2.channel_settings.size());
    for (unsigned int i = 0; i < bucket1.channel_settings.size(); i++) {
      expect_equal(bucket1.channel_settings[i], bucket2.channel_settings[i]);
    }
  }

  void expect_equal(NativeScanSettings& scan1, NativeScanSettings& scan2) {
    EXPECT_EQ(scan1.base_period_ms, scan2.base_period_ms);
    EXPECT_EQ(scan1.max_ap_per_scan, scan2.max_ap_per_scan);
    EXPECT_EQ(scan1.report_threshold_percent, scan2.report_threshold_percent);
    EXPECT_EQ(scan1.report_threshold_num_scans, scan2.report_threshold_num_scans);
    EXPECT_EQ(scan1.bucket_settings.size(), scan2.bucket_settings.size());
    for (unsigned int i = 0; i < scan1.bucket_settings.size(); i++) {
      expect_equal(scan1.bucket_settings[i], scan2.bucket_settings[i]);
    }
    EXPECT_EQ(scan1.hidden_networks.size(), scan2.hidden_networks.size());
    for (unsigned int i = 0; i < scan1.hidden_networks.size(); i++) {
      expect_equal(scan1.hidden_networks[i], scan2.hidden_networks[i]);
    }
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

  expect_equal(channel_settings, channel_settings_copy);
}

TEST_F(ScanSettingsTest, BucketSettingsParcelableTest) {
  BucketSettings bucket_settings;

  Parcel parcel;
  EXPECT_EQ(::android::OK, bucket_settings.writeToParcel(&parcel));

  BucketSettings bucket_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, bucket_settings_copy.readFromParcel(&parcel));

  expect_equal(bucket_settings, bucket_settings_copy);
}

TEST_F(ScanSettingsTest, NativeScanSettingsParcelableTest) {
  NativeScanSettings scan_settings;

  Parcel parcel;
  EXPECT_EQ(::android::OK, scan_settings.writeToParcel(&parcel));

  NativeScanSettings scan_settings_copy;
  parcel.setDataPosition(0);
  EXPECT_EQ(::android::OK, scan_settings_copy.readFromParcel(&parcel));

  expect_equal(scan_settings, scan_settings_copy);
}


}  // namespace wificond
}  // namespace android
