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

#include "wificond/scanning/scan_result.h"

#include <iomanip>
#include <sstream>

#include <android-base/logging.h>

using std::string;
using std::stringstream;

namespace android {
namespace net {
namespace wifi {

NativeScanResult::NativeScanResult(std::vector<uint8_t>& ssid_,
                                   std::vector<uint8_t>& bssid_,
                                   std::vector<uint8_t>& info_element_,
                                   uint32_t frequency_,
                                   int32_t signal_mbm_,
                                   uint64_t tsf_,
                                   uint16_t capability_,
                                   bool associated_)
    : ssid(ssid_),
      bssid(bssid_),
      info_element(info_element_),
      frequency(frequency_),
      signal_mbm(signal_mbm_),
      tsf(tsf_),
      capability(capability_),
      associated(associated_) {
}

::android::status_t NativeScanResult::writeToParcel(::android::Parcel* parcel) const {
  status_t status = parcel->writeInt32(ssid.size());
  if (status != OK) return status;
  status = parcel->writeByteVector(ssid);
  if (status != OK) return status;
  status = parcel->writeInt32(bssid.size());
  if (status != OK) return status;
  status = parcel->writeByteVector(bssid);
  if (status != OK) return status;
  status = parcel->writeInt32(info_element.size());
  if (status != OK) return status;
  status = parcel->writeByteVector(info_element);
  if (status != OK) return status;
  status = parcel->writeUint32(frequency);
  if (status != OK) return status;
  status = parcel->writeInt32(signal_mbm);
  if (status != OK) return status;
  status = parcel->writeUint64(tsf);
  if (status != OK) return status;
  // There is no writeUint16() avalaible.
  // Use writeUint32() instead;
  status = parcel->writeUint32(capability);
  if (status != OK) return status;
  status = parcel->writeInt32(associated ? 1 : 0);
  if (status != OK) return status;
  return ::android::OK;
}

::android::status_t NativeScanResult::readFromParcel(const ::android::Parcel* parcel) {
  parcel->readInt32();
  status_t status = parcel->readByteVector(&ssid);
  if (status != OK) return status;
  parcel->readInt32();
  status = parcel->readByteVector(&bssid);
  if (status != OK) return status;
  parcel->readInt32();
  status = parcel->readByteVector(&info_element);
  if (status != OK) return status;
  status = parcel->readUint32(&frequency);
  if (status != OK) return status;
  status = parcel->readInt32(&signal_mbm);
  if (status != OK) return status;
  status = parcel->readUint64(&tsf);
  if (status != OK) return status;
  // There is no readUint16() avalaible.
  // Use readUint32() instead;
  capability = static_cast<uint16_t>(parcel->readUint32());
  associated = (parcel->readInt32() != 0);
  return ::android::OK;
}

void NativeScanResult::DebugLog() {
  LOG(INFO) << "Scan result:";
  // |ssid| might be an encoded array but we just print it as ASCII here.
  string ssid_str(ssid.data(), ssid.data() + ssid.size());
  LOG(INFO) << "SSID: " << ssid_str;

  stringstream ss;
  string bssid_str;
  for (uint8_t& b : bssid) {
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    if (&b != &bssid.back()) {
      ss << ":";
    }
  }
  bssid_str = ss.str();
  LOG(INFO) << "BSSID: " << bssid_str;
  LOG(INFO) << "FREQUENCY: " << frequency;
  LOG(INFO) << "SIGNAL: " << signal_mbm/100 << "dBm";
  LOG(INFO) << "TSF: " << tsf;
  LOG(INFO) << "CAPABILITY: " << capability;
  LOG(INFO) << "ASSOCIATED: " << associated;

}

}  // namespace wifi
}  // namespace net
}  // namespace android
