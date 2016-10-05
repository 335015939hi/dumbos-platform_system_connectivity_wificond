/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wificond/net/mlme_event.h"

#include <string>
#include <vector>

#include <linux/netlink.h>
#include <linux/nl80211.h>

#include <android-base/logging.h>

#include "wificond/net/nl80211_packet.h"

using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

bool MlmeEvent::InitFromPacket(const NL80211Packet* packet,
                               MlmeEvent* out_mlme_event) {
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX,
                                 &(out_mlme_event->interface_index_))) {
    return false;
  }
  out_mlme_event->event_type_ = packet->GetCommand();
  return true;
}

void MlmeEvent::DebugLog() {
  LOG(INFO) << "Event type: " << event_type_;
  LOG(INFO) << "Interface index: " << interface_index_;
}


}  // namespace wificond
}  // namespace android
