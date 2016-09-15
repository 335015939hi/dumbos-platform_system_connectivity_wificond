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

#include "wificond/client_interface_binder.h"

#include <vector>

#include <binder/Status.h>

#include "wificond/client_interface_impl.h"

using android::binder::Status;
using std::vector;
using std::unique_ptr;

namespace android {
namespace wificond {

ClientInterfaceBinder::ClientInterfaceBinder(ClientInterfaceImpl* impl)
    : impl_(impl) {
}

ClientInterfaceBinder::~ClientInterfaceBinder() {
}

Status ClientInterfaceBinder::enableSupplicant(bool* success) {
  *success = impl_ && impl_->EnableSupplicant();
  return Status::ok();
}

Status ClientInterfaceBinder::disableSupplicant(bool* success) {
  *success = impl_ && impl_->DisableSupplicant();
  return Status::ok();
}

Status ClientInterfaceBinder::getPacketCounters(
    unique_ptr<vector<int64_t>>* out_packet_counters) {
  out_packet_counters->reset(new vector<int64_t>());
  if (impl_ == nullptr || !impl_->GetPacketCounters(out_packet_counters)) {
    *out_packet_counters = nullptr;
  }
  return Status::ok();
}


}  // namespace wificond
}  // namespace android
