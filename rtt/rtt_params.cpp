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


#include "wificond/rtt/rtt_params.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::status_t;
using android::OK;

namespace android {
namespace net {
namespace wifi {

status_t RttParams::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(params_list.size());
  if (status != OK) return status;
  for (auto& params : params_list) {
    status = parcel->writeInt32(params.device_type);
    if (status != OK) return status;
    status = parcel->writeInt32(params.request_type);
    if (status != OK) return status;
    status = parcel->writeBool(params.secure);
    if (status != OK) return status;
    status = parcel->writeString16(params.bssid);
    if (status != OK) return status;
    status = parcel->writeInt32(params.channel_width);
    if (status != OK) return status;
    status = parcel->writeInt32(params.frequency);
    if (status != OK) return status;
    status = parcel->writeInt32(params.center_freq0);
    if (status != OK) return status;
    status = parcel->writeInt32(params.center_freq1);
    if (status != OK) return status;
    status = parcel->writeInt32(params.number_burst);
    if (status != OK) return status;
    status = parcel->writeInt32(params.interval);
    if (status != OK) return status;
    status = parcel->writeInt32(params.num_samples_per_burst);
    if (status != OK) return status;
    status = parcel->writeInt32(params.num_retries_per_measurement_frame);
    if (status != OK) return status;
    status = parcel->writeInt32(params.number_retries_per_ftmr);
    if (status != OK) return status;
    status = parcel->writeBool(params.lci_request);
    if (status != OK) return status;
    status = parcel->writeBool(params.lcr_request);
    if (status != OK) return status;
    status = parcel->writeInt32(params.burst_timeout);
    if (status != OK) return status;
    status = parcel->writeInt32(params.preamble);
    if (status != OK) return status;
    status = parcel->writeInt32(params.bandwidth);
    if (status != OK) return status;
  }
  return OK;
}

status_t RttParams::readFromParcel(const Parcel* parcel) {
  int num = parcel->readInt32();
  if (num < 0) {
    return BAD_VALUE;
  }
  for (int i = 0; i < num; i++) {
    SingleRttParams params;
    params.device_type = parcel->readInt32();
    params.request_type = parcel->readInt32();
    params.secure = parcel->readBool();
    params.bssid = parcel->readString16();
    params.channel_width = parcel->readInt32();
    params.frequency = parcel->readInt32();
    params.center_freq0 = parcel->readInt32();
    params.center_freq1 = parcel->readInt32();
    params.number_burst = parcel->readInt32();
    params.interval = parcel->readInt32();
    params.num_samples_per_burst = parcel->readInt32();
    params.num_retries_per_measurement_frame = parcel->readInt32();
    params.number_retries_per_ftmr = parcel->readInt32();
    params.lci_request = parcel->readBool();
    params.lcr_request = parcel->readBool();
    params.burst_timeout = parcel->readInt32();
    params.preamble = parcel->readInt32();
    params.bandwidth = parcel->readInt32();
    params_list.push_back(params);
  }
  return OK;
}

}  // namespace wifi
}  // namespace net
}  // namespace android
