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

#include "wificond/tests/mock_offload_callback.h"
#include "wificond/scanning/scan_result.h"

using android::hardware::wifi::offload::V1_0::implementation::OnOffloadScanResultsReadyHandler;
using android::hardware::wifi::offload::V1_0::ScanResult;
using ::android::hardware::hidl_vec;

namespace android {
namespace wificond {

MockOffloadCallback::MockOffloadCallback(OnOffloadScanResultsReadyHandler handler) :
    OffloadCallback(handler), handler_(handler) {
    ON_CALL(*this, onScanResult(testing::_)).WillByDefault(testing::Invoke(
      [this] (const hidl_vec<ScanResult> &scanResult) -> ::android::hardware::Return<void> {
        std::vector<ScanResult> scanResult_(scanResult);
        handler_(scanResult_);
        return ::android::hardware::Void();
      }));
}

} // wificond
} // android
