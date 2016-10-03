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


#include "wificond/rtt/rtt_capabilities.h"

#include <binder/Parcel.h>

using android::Parcelable;
using android::Parcel;
using android::status_t;
using android::OK;

namespace android {
namespace net {
namespace wifi {
namespace RttManager {

status_t RttCapabilities::writeToParcel(Parcel* parcel) const {
  status_t status = parcel->writeInt32(onesided_rtt_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(twosided_11mc_rtt_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(lci_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(lcr_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(preamble_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(bw_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(responder_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(secure_rtt_supported ? 1 : 0);
  if (status != OK) return status;
  status = parcel->writeInt32(mc_version);
  if (status != OK) return status;

  return OK;
}

status_t RttCapabilities::readFromParcel(const Parcel* parcel) {
  onesided_rtt_supported = (parcel->readInt32() != 0);
  twosided_11mc_rtt_supported = (parcel->readInt32() != 0);
  lci_supported = (parcel->readInt32() != 0);
  lcr_supported = (parcel->readInt32() != 0);
  preamble_supported = (parcel->readInt32() != 0);
  bw_supported = (parcel->readInt32() != 0);
  responder_supported = (parcel->readInt32() != 0);
  secure_rtt_supported = (parcel->readInt32() != 0);
  mc_version = parcel->readInt32();

  return OK;
}

}  // namespace RttManager
}  // namespace wifi
}  // namespace net
}  // namespace android
