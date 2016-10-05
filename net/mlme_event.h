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

#ifndef WIFICOND_NET_MLME_EVENT_H_
#define WIFICOND_NET_MLME_EVENT_H_

#include <memory>
#include <vector>

#include <android-base/macros.h>

namespace android {
namespace wificond {

class NL80211Packet;

class MlmeEvent {
 public:
  // This returns a object of a proper subclass for this packet.
  // Returns nullptr when it fails to parse the pakcet.
  static std::unique_ptr<MlmeEvent> InitFromPacket(const NL80211Packet* packet);
  // Returns the event type of this MLME Event.
  // It should be one of the following defined in nl80211.h:
  // NL80211_CMD_ASSOCIATE
  // NL80211_CMD_DISASSOCIATE
  // NL80211_CMD_AUTHENTICATE
  // NL80211_CMD_DEAUTHENTICATE
  // NL80211_CMD_CONNECT
  // NL80211_CMD_DISCONNECT
  // NL80211_CMD_ROAM
  uint8_t GetEventType() { return event_type_; }
  // Returns the index of the interface which is associated with this MLME
  // event.
  uint32_t GetInterfaceIndex() { return interface_index_; }
  // Log the detail of this object, for debug use only;

 protected:
  // MlmeEvent can only be created by InitFromPacket();
  // We keep this as protected because subclass default constructor needs this.
  MlmeEvent() = default;
  uint8_t event_type_;
  uint32_t interface_index_;

 private:
  DISALLOW_COPY_AND_ASSIGN(MlmeEvent);
};

class MlmeConnectEvent : public MlmeEvent {
 friend class MlmeEvent;
 public:
  // Returns the BSSID of the associated AP.
  std::vector<uint8_t>& GetBSSID() { return bssid_; }
  // Get the status code of this connect event.
  // 0 = success, non-zero = failure.
  uint16_t GetStatusCode() { return status_code_; }

 private:
  MlmeConnectEvent() = default;
  static std::unique_ptr<MlmeConnectEvent> InitFromPacket(const NL80211Packet* packet);

  std::vector<uint8_t> bssid_;
  uint16_t status_code_;

  DISALLOW_COPY_AND_ASSIGN(MlmeConnectEvent);
};

class MlmeRoamEvent : public MlmeEvent {
 friend class MlmeEvent;
 public:
  // Returns the BSSID of the associated AP.
  std::vector<uint8_t>& GetBSSID() { return bssid_; }
  // Get the status code of this connect event.
  // 0 = success, non-zero = failure.
  uint16_t GetStatusCode() { return status_code_; }

 private:
  MlmeRoamEvent() = default;
  static std::unique_ptr<MlmeRoamEvent> InitFromPacket(const NL80211Packet* packet);

  std::vector<uint8_t> bssid_;
  uint16_t status_code_;

  DISALLOW_COPY_AND_ASSIGN(MlmeRoamEvent);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_MLME_EVENT_H_
