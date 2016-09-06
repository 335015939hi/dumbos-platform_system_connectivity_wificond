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
#include <string>
#include <vector>

#include <linux/netlink.h>
#include <linux/nl80211.h>

#include <gtest/gtest.h>

#include "wificond/net/netlink_utils.h"
#include "wificond/tests/mock_netlink_manager.h"

using std::string;
using std::unique_ptr;
using std::vector;
using testing::DoAll;
using testing::NiceMock;
using testing::Return;
using testing::_;

namespace android {
namespace wificond {

namespace {

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

}  // namespace

class ScanUtilsTest : public ::testing::Test {
 protected:
  std::unique_ptr<NiceMock<MockNetlinkManager>> netlink_manager_;
  std::unique_ptr<ScanUtils> scan_utils_;

  virtual void SetUp() {
    netlink_manager_.reset(new NiceMock<MockNetlinkManager>());
    netlink_utils_.reset(new NetlinkUtils(netlink_manager_.get()));

    ON_CALL(*netlink_manager_,
            GetSequenceNumber()).WillByDefault(Return(kFakeSequenceNumber));
    ON_CALL(*netlink_manager_,
            GetFamilyId()).WillByDefault(Return(kFakeFamilyId));
  }
};

// This mocks the behavior of SendMessageAndGetResponses(), which returns a
// vector of NL80211Packet using passed in pointer.
ACTION_P(MakeupResponse, response) {
  // arg1 is the second parameter: vector<unique_ptr<const NL80211Packet>>* responses.
  for (auto& pkt : response) {
    arg1->push_back(unique_ptr<NL80211Packet>(new NL80211Packet(pkt)));
  }
}

TEST_F(ScanUtilsTest, DummyTest) {
}

}  // namespace wificond
}  // namespace android
