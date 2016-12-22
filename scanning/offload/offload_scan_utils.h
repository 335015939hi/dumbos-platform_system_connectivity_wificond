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
#ifndef WIFICOND_OFFLOAD_SCAN_UTILS_H_
#define WIFICOND_OFFLOAD_SCAN_UTILS_H_

#include <android/hardware/wifi/offload/1.0/IOffload.h>
#include "wificond/scanning/offload/OffloadCallback.h"
#include <android-base/logging.h>

#include <vector>

using android::hardware::hidl_death_recipient;
using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using android::hardware::wifi::offload::V1_0::ScanResult;

namespace com {
namespace android {
namespace server {
namespace wifi {
namespace wificond {

class NativeScanStats;

}  // namespace wificond
}  // namespace wifi
}  // namespace server
}  // namespace android
}  // namespace com

namespace android {
namespace wificond {

// Provides methods to interact with Offload HAL
class OffloadScanUtils {
 public:
  explicit OffloadScanUtils(android::sp<IOffload>,
      android::sp<OffloadCallback> offload_callback);
  virtual ~OffloadScanUtils();
  bool get_service_available();

 private:

  android::sp<IOffload> wifi_offload_hal_;
  bool service_available_;
};

}  // namespace wificond
}  // namespace android

#endif // WIFICOND_OFFLOAD_SCAN_UTILS_H
