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

#ifndef WIFICOND_SCANNING_BUCKET_SETTINGS_H_
#define WIFICOND_SCANNING_BUCKET_SETTINGS_H_

#include <vector>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

#include "wificond/scanning/channel_settings.h"

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class BucketSettings : public ::android::Parcelable {
 public:
  BucketSettings() = default;
  ::android::status_t writeToParcel(::android::Parcel* parcel) const override;
  ::android::status_t readFromParcel(const ::android::Parcel* parcel) override;

  int32_t bucket;
  int32_t band;
  int32_t period_ms;
  int32_t max_period_ms;
  int32_t step_count;
  int32_t report_events;
  std::vector<ChannelSettings> channel_settings;
};

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

#endif  // WIFICOND_SCANNING_BUCKET_SETTINGS_H_
