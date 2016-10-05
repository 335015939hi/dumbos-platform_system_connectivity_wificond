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

#include <android-base/macros.h>

namespace android {
namespace wificond {

class NL80211Packet;

class MlmeEvent {
 public:
  MlmeEvent() = default;
  static bool InitFromPacket(const NL80211Packet* packet,
                             MlmeEvent* out_mlme_event);
  void DebugLog();

 private:
  uint32_t event_type_;
  uint32_t interface_index_;

  DISALLOW_COPY_AND_ASSIGN(MlmeEvent);
};


}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_MLME_EVENT_H_
