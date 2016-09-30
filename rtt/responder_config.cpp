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


#include "wificond/rtt/responder_config.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::status_t;
using android::OK;

namespace android {
namespace net {
namespace wifi {
namespace RttManager {

status_t ResponderConfig::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeString16(mac_address);
  if (status != OK) return status;
  status = parcel->writeInt32(frequency);
  if (status != OK) return status;
  status = parcel->writeInt32(center_freq0);
  if (status != OK) return status;
  status = parcel->writeInt32(center_freq1);
  if (status != OK) return status;
  status = parcel->writeInt32(channel_width);
  if (status != OK) return status;
  status = parcel->writeInt32(preamble);
  if (status != OK) return status;
  return OK;
}

status_t ResponderConfig::readFromParcel(const Parcel* parcel) {
  int num = parcel->readInt32();
  if (num < 0) {
    return BAD_VALUE;
  }
  mac_address = parcel->readString16();
  frequency = parcel->readInt32();
  center_freq0 = parcel->readInt32();
  center_freq1 = parcel->readInt32();
  channel_width = parcel->readInt32();
  preamble = parcel->readInt32();
  return OK;
}

}  // namespace RttManager
}  // namespace wifi
}  // namespace net
}  // namespace android
