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

#ifndef WIFICOND_RTT_PARCELABLE_PARAMS_H_
#define WIFICOND_RTT_PARCELABLE_PARAMS_H_

#include <vector>

#include <binder/Parcel.h>
#include <binder/Parcelable.h>
#include <utils/String16.h>

using android::Parcel;
using android::Parcelable;
using android::String16;
using android::status_t;
using std::vector;

namespace android {
namespace net {
namespace wifi {

struct SingleRttParams {
  SingleRttParams() = default;
  int32_t device_type;
  int32_t request_type;
  bool secure;
  String16 bssid;
  int32_t channel_width;
  int32_t frequency;
  int32_t center_freq0;
  int32_t center_freq1;
  int32_t number_burst;
  int32_t interval;
  int32_t num_samples_per_burst;
  int32_t num_retries_per_measurement_frame;
  int32_t number_retries_per_ftmr;
  bool lci_request;
  bool lcr_request;
  int32_t burst_timeout;
  int32_t preamble;
  int32_t bandwidth;
};

// Additional namespace to match the Java side nested parcelable class
// structure.
namespace RttManager {
  class ParcelableRttParams : public Parcelable {
   public:
    ParcelableRttParams() = default;

    // Write |this| parcelable to the given |parcel|.  Keep in mind that
    // implementations of writeToParcel must be manually kept in sync
    // with readFromParcel and the Java equivalent versions of these methods.
    //
    // Returns android::OK on success and an appropriate error otherwise.
    status_t writeToParcel(Parcel* parcel) const override;

    // Read data from the given |parcel| into |this|.  After readFromParcel
    // completes, |this| should have equivalent state to the object that
    // wrote itself to the parcel.
    //
    // Returns android::OK on success and an appropriate error otherwise.
    status_t readFromParcel(const Parcel* parcel) override;

    private:
    vector<SingleRttParams> params_list;
  };
};  // namespcae RttManager

}  // namespace wifi
}  // namespace net
}  // namespace android

#endif  // WIFICOND_RTT_PARCEABLE_PARAMS_H_
