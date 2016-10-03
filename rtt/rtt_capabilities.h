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

#ifndef WIFICOND_RTT_CAPABILITIES_H_
#define WIFICOND_RTT_CAPABILITIES_H_

#include <binder/Parcel.h>
#include <binder/Parcelable.h>

using android::Parcel;
using android::Parcelable;
using android::status_t;

namespace android {
namespace net {
namespace wifi {

// Additional namespace to match the Java side nested parcelable class
// structure.
namespace RttManager {
  struct RttCapabilities : public Parcelable {
    RttCapabilities() = default;

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

    bool onesided_rtt_supported;
    bool twosided_11mc_rtt_supported;
    bool lci_supported;
    bool lcr_supported;
    bool preamble_supported;
    bool bw_supported;
    bool responder_supported;
    bool secure_rtt_supported;
    int32_t mc_version;
  };
};  // namespcae RttManager

}  // namespace wifi
}  // namespace net
}  // namespace android

#endif  // WIFICOND_RTT_CAPABILITIES_H_
