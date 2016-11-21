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

#include "wificond/scanning/bucket_settings.h"

#include <android-base/logging.h>

#include "wificond/parcelable_utils.h"

using namespace android::wificond::parcelable_utils;

using android::status_t;
using android::OK;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

status_t BucketSettings::writeToParcel(::android::Parcel* parcel) const {
  RETURN_IF_FAILED(parcel->writeInt32(bucket));
  RETURN_IF_FAILED(parcel->writeInt32(band));
  RETURN_IF_FAILED(parcel->writeInt32(period_ms));
  RETURN_IF_FAILED(parcel->writeInt32(max_period_ms));
  RETURN_IF_FAILED(parcel->writeInt32(step_count));
  RETURN_IF_FAILED(parcel->writeInt32(report_events));
  RETURN_IF_FAILED(parcel->writeInt32(channel_settings.size()));
  for (auto& channel : channel_settings) {
    RETURN_IF_FAILED(channel.writeToParcel(parcel));
  }

  return ::android::OK;
}

status_t BucketSettings::readFromParcel(const ::android::Parcel* parcel) {
  RETURN_IF_FAILED(parcel->readInt32(&bucket));
  RETURN_IF_FAILED(parcel->readInt32(&band));
  RETURN_IF_FAILED(parcel->readInt32(&period_ms));
  RETURN_IF_FAILED(parcel->readInt32(&max_period_ms));
  RETURN_IF_FAILED(parcel->readInt32(&step_count));
  RETURN_IF_FAILED(parcel->readInt32(&report_events));
  int32_t num_channels = 0;
  RETURN_IF_FAILED(parcel->readInt32(&num_channels));
  for (int i = 0; i < num_channels; i++) {
    ChannelSettings channel;
    RETURN_IF_FAILED(channel.readFromParcel(parcel));
    channel_settings.push_back(channel);
  }

  return ::android::OK;
}

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com
