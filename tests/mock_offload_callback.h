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

#ifndef WIFICOND_TEST_MOCK_OFFLOAD_CALLBACK_H
#define WIFICOND_TEST_MOCK_OFFLOAD_CALLBACK_H

#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wificond/scanning/offload/offload_callback.h"
#include <android/hardware/wifi/offload/1.0/IOffloadCallback.h>

using android::hardware::wifi::offload::V1_0::ScanResult;
using android::hardware::wifi::offload::V1_0::implementation::OffloadCallback;
using android::hardware::wifi::offload::V1_0::implementation::OnOffloadScanResultsReadyHandler;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

namespace android {
namespace wificond {

class MockOffloadCallback : public OffloadCallback {
  public:
    MockOffloadCallback(OnOffloadScanResultsReadyHandler handler);
    ~MockOffloadCallback() override = default;

    MOCK_METHOD1(onScanResult,
        Return<void>(const hidl_vec<ScanResult> &scanResult));

  private:
    OnOffloadScanResultsReadyHandler handler_;
};

} // wificond
} // android

#endif // WIFICOND_TEST_MOCK_OFFLOAD_CALLBACK_H
