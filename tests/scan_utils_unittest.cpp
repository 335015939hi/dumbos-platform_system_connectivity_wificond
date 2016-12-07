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

#include <linux/netlink.h>
#include <linux/nl80211.h>

#include <gtest/gtest.h>

#include "wificond/scanning/scan_result.h"
#include "wificond/scanning/scan_utils.h"
#include "wificond/tests/mock_netlink_manager.h"

using std::unique_ptr;
using std::vector;
using testing::DoAll;
using testing::NiceMock;
using testing::Return;
using testing::_;

using com::android::server::wifi::wificond::NativeScanResult;

namespace android {
namespace wificond {

namespace {

constexpr uint32_t kFakeInterfaceIndex = 12;
constexpr uint32_t kFakeIntervalMs = 20000;
constexpr uint32_t kFakeSequenceNumber = 1984;
constexpr int32_t kFakeRssiThreshold = -80;

// Currently, control messages are only created by the kernel and sent to us.
// Therefore NL80211Packet doesn't have corresponding constructor.
// For test we manually create control messages using this helper function.
NL80211Packet CreateControlMessageError(int error_code) {
  vector<uint8_t> data;
  data.resize(NLMSG_HDRLEN + NLA_ALIGN(sizeof(int)), 0);
  // Initialize length field.
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data.data());
  nl_header->nlmsg_len = data.size();
  nl_header->nlmsg_type = NLMSG_ERROR;
  nl_header->nlmsg_seq = kFakeSequenceNumber;
  nl_header->nlmsg_pid = getpid();
  int* error_field = reinterpret_cast<int*>(data.data() + NLMSG_HDRLEN);
  *error_field = -error_code;

  return NL80211Packet(data);
}

NL80211Packet CreateControlMessageAck() {
  return CreateControlMessageError(0);
}

}  // namespace

class ScanUtilsTest : public ::testing::Test {
 protected:
  std::unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_;
  std::unique_ptr<ScanUtils> scan_utils_;

 virtual void SetUp() {
    netlink_manager_.reset(new NiceMock<MockNetlinkManager>());
    scan_utils_.reset(new ScanUtils(netlink_manager_.get()));
    ON_CALL(*netlink_manager_,
            SendMessageAndGetResponses(_, _)).WillByDefault(Return(true));
  }
};

MATCHER_P(DoesNL80211PacketMatchCommand, command,
          "Check if the netlink packet matches |command|") {
  return arg.GetCommand() == command;
}

ACTION_P(MakeupResponse, response) {
  // arg1 is the second parameter: vector<unique_ptr<const NL80211Packet>>* responses.
  for (auto& pkt : response) {
    arg1->push_back(unique_ptr<NL80211Packet>(new NL80211Packet(pkt)));
  }
}

TEST_F(ScanUtilsTest, CanGetScanResult) {
  vector<NativeScanResult> scan_results;
  // This makes sure that ScanUtils requests NetlinkManager to send
  // NL80211_CMD_GET_SCAN.
  EXPECT_CALL(
      *netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_GET_SCAN), _));

  // We don't use EXPECT_TRUE here because we need to mock a complete
  // response for NL80211_CMD_GET_SCAN to satisfy the parsing code called
  // by GetScanResult.
  // TODO: Mock response for NL80211_CMD_GET_SCAN.
  scan_utils_->GetScanResult(kFakeInterfaceIndex, &scan_results);
}

TEST_F(ScanUtilsTest, CanSendScanRequest) {
  vector<NL80211Packet> response = {CreateControlMessageAck()};
  // This makes sure that ScanUtils requests NetlinkManager to send
  // NL80211_CMD_TRIGGER_SCAN, and returns true upon receiving a Ack
  // response.
  EXPECT_CALL(
      *netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN), _)).
              WillOnce(DoAll(MakeupResponse(response), Return(true)));
  EXPECT_TRUE(scan_utils_->Scan(kFakeInterfaceIndex, {}, {}));
}

TEST_F(ScanUtilsTest, CanHandleScanRequestFailure) {
  vector<NL80211Packet> response = {CreateControlMessageAck()};
  // This makes sure that ScanUtils requests NetlinkManager to send
  // NL80211_CMD_TRIGGER_SCAN, and returns true upon receiving an ERROR
  // response.
  EXPECT_CALL(
      *netlink_manager_,
      SendMessageAndGetResponses(
          DoesNL80211PacketMatchCommand(NL80211_CMD_TRIGGER_SCAN), _)).
              WillOnce(DoAll(MakeupResponse(response), Return(true)));
  EXPECT_TRUE(scan_utils_->Scan(kFakeInterfaceIndex, {}, {}));
}

TEST_F(ScanUtilsTest, CanSendSchedScanRequest) {
  vector<NL80211Packet> response = {CreateControlMessageAck()};
  // This makes sure that ScanUtils requests NetlinkManager to send
  // NL80211_CMD_START_SCHED_SCAN, and returns true upon receiving a Ack
  // response.
  EXPECT_CALL(
      *netlink_manager_,
       SendMessageAndGetResponses(
           DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN), _)).
              WillOnce(DoAll(MakeupResponse(response), Return(true)));
  EXPECT_FALSE(scan_utils_->StartScheduledScan(
      kFakeInterfaceIndex, kFakeIntervalMs, kFakeRssiThreshold, {}, {}, {}));
}

TEST_F(ScanUtilsTest, CanHandleSchedScanRequestFailure) {
  vector<NL80211Packet> response = {CreateControlMessageAck()};
  // This makes sure that ScanUtils requests NetlinkManager to send
  // NL80211_CMD_START_SCHED_SCAN, and returns false upon receiving am ERROR
  // response.
  EXPECT_CALL(
      *netlink_manager_,
       SendMessageAndGetResponses(
           DoesNL80211PacketMatchCommand(NL80211_CMD_START_SCHED_SCAN), _)).
              WillOnce(DoAll(MakeupResponse(response), Return(true)));
  EXPECT_FALSE(scan_utils_->StartScheduledScan(
      kFakeInterfaceIndex, kFakeIntervalMs, kFakeRssiThreshold, {}, {}, {}));
}

}  // namespace wificond
}  // namespace android
