/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <fuzzbinder/libbinder_driver.h>
#include <fuzzbinder/random_fd.h>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <cutils/properties.h>
#include <wifi_system/interface_tool.h>

#include "wificond/looper_backed_event_loop.h"
#include "wificond/net/netlink_manager.h"
#include "wificond/net/netlink_utils.h"
#include "wificond/scanning/scan_utils.h"
#include "wificond/server.h"

using android::net::wifi::nl80211::IWificond;
using android::wifi_system::InterfaceTool;
using std::unique_ptr;
using android::base::unique_fd;
using namespace android;

void fuzzOnBinderReadReady(int /*fd*/) {}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

    FuzzedDataProvider provider(data, size);
    auto ramdomFds = getRandomFds(&provider);

    auto event_dispatcher = std::make_unique<wificond::LooperBackedEventLoop>();
    event_dispatcher->WatchFileDescriptor(
        ramdomFds[provider.ConsumeIntegralInRange<size_t>(0, ramdomFds.size() - 1)].get(),
        android::wificond::EventLoop::kModeInput,
        &fuzzOnBinderReadReady);

    android::wificond::NetlinkManager netlink_manager(event_dispatcher.get());
    if (!netlink_manager.Start()) {
        LOG(ERROR) << "Failed to start netlink manager";
    }
    android::wificond::NetlinkUtils netlink_utils(&netlink_manager);
    android::wificond::ScanUtils scan_utils(&netlink_manager);

    auto server  = sp<android::wificond::Server>::make(
              std::make_unique<InterfaceTool>(),
              &netlink_utils,
              &scan_utils);
    fuzzService(server, FuzzedDataProvider(data, size));
    return 0;
}
