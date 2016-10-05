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

#include <vector>

#include <linux/nl80211.h>

#include <android-base/logging.h>

#include "wificond/net/nl80211_packet.h"

using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

unique_ptr<MlmeEvent> MlmeEvent::InitFromPacket(const NL80211Packet* packet) {
  uint32_t interface_index;
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX,
                                 &interface_index)) {
     return nullptr;
  }
  uint8_t event_type = packet->GetCommand();

  unique_ptr<MlmeEvent> event;
  if (event_type == NL80211_CMD_CONNECT) {
    event = MlmeConnectEvent::InitFromPacket(packet);
  } else if (event_type == NL80211_CMD_ROAM){
    event = MlmeRoamEvent::InitFromPacket(packet);
  }

  if (event == nullptr) {
    return nullptr;
  }
  event->interface_index_ = interface_index;
  event->event_type_ = event_type;
  return event;
}

unique_ptr<MlmeConnectEvent> MlmeConnectEvent::InitFromPacket(const NL80211Packet* packet) {
  unique_ptr<MlmeConnectEvent> connect_event(new MlmeConnectEvent());
  if (!packet->GetAttributeValue(NL80211_ATTR_STATUS_CODE, &(connect_event->status_code_))) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_STATUS_CODE";
    connect_event->status_code_ = 0;
  }
  if (!packet->GetAttributeValue(NL80211_ATTR_MAC, &(connect_event->bssid_))) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_MAC";
    return nullptr;
  }

  return connect_event;
}

unique_ptr<MlmeRoamEvent> MlmeRoamEvent::InitFromPacket(const NL80211Packet* packet) {
  unique_ptr<MlmeRoamEvent> roam_event(new MlmeRoamEvent());
  if (!packet->GetAttributeValue(NL80211_ATTR_STATUS_CODE, &(roam_event->status_code_))) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_STATUS_CODE";
    roam_event->status_code_ = 0;
  }
  if (!packet->GetAttributeValue(NL80211_ATTR_MAC, &(roam_event->bssid_))) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_MAC";
    return nullptr;
  }

  return roam_event;
}

}  // namespace wificond
}  // namespace android
