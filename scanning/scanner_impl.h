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

#ifndef WIFICOND_SCANNER_IMPL_H_
#define WIFICOND_SCANNER_IMPL_H_

#include <vector>

#include <android-base/macros.h>
#include <binder/Status.h>

#include "android/net/wifi/BnWifiScannerImpl.h"

namespace android {
namespace wificond {

class ScanUtils;

class ScannerImpl : public android::net::wifi::BnWifiScannerImpl {
 public:
  ScannerImpl(uint32_t interface_index_,
              ScanUtils* scan_utils_);
  ~ScannerImpl();
  ::android::binder::Status startSingleScan(
      const ::com::android::server::wifi::wificond::SingleScanSettings&
          settings,
      const ::android::sp<::android::net::wifi::IScanEventCallback>&
          scan_events) override;
  void Invalidate() { valid_ = false; }

 private:
  bool valid_;
  bool is_scanning_;
  uint32_t interface_index_;
  ScanUtils* scan_utils_;
  ::android::sp<::android::net::wifi::IScanEventCallback> scan_events_;

  void StartMonitoringScanResult();
  void StopMonitoringScanResult();

  void OnScanResultsReadyInternal(
      uint32_t interface_index,
      bool aborted,
      std::vector<std::vector<uint8_t>>& ssids,
      std::vector<uint32_t>& frequencies);

  DISALLOW_COPY_AND_ASSIGN(ScannerImpl);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_SCANNER_IMPL_H_
