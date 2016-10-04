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


#include "wificond/rtt/rtt_results.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::status_t;
using android::OK;

namespace android {
namespace net {
namespace wifi {
namespace RttManager {

status_t ParcelableRttResults::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(result_list.size());
  if (status != OK) return status;
  for (auto& results : results_list) {
  }
  return OK;
}

status_t ParcelableRttResults::readFromParcel(const Parcel* parcel) {
  int num = parcel->readInt32();
  if (num < 0) {
    return BAD_VALUE;
  }
  for (int i = 0; i < num; i++) {
    RttResults results;
  }
  return OK;
}

}  // namespace RttManager
}  // namespace wifi
}  // namespace net
}  // namespace android
