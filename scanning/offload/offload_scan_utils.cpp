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

#include "wificond/scanning/offload/offload_scan_utils.h"

using android::hardware::wifi::offload::V1_0::IOffload;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;

namespace android {
namespace wificond {

OffloadScanUtils::OffloadScanUtils(android::sp<IOffload> offloadService,
    android::sp<OffloadCallback> offloadCallback)
    : wifi_offload_hal_(offloadService) {
  if (wifi_offload_hal_ == nullptr) {
      service_available_ = false;
      LOG(ERROR) << "Failed to get Offload service";
      return;
  }
  service_available_ = true;
  wifi_offload_hal_->setEventCallback(offloadCallback);
}

OffloadScanUtils::~OffloadScanUtils() {}

bool OffloadScanUtils::get_service_available() {
  return service_available_;
}

}  // namespace wificond
}  // namespace android
