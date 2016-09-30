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

#ifndef WIFICOND_CLIENT_INTERFACE_BINDER_H_
#define WIFICOND_CLIENT_INTERFACE_BINDER_H_

#include <android-base/macros.h>
#include <binder/Status.h>

#include "android/net/wifi/BnClientInterface.h"

namespace android {
namespace wificond {

class ClientInterfaceImpl;

class ClientInterfaceBinder : public android::net::wifi::BnClientInterface {
 public:
  explicit ClientInterfaceBinder(ClientInterfaceImpl* impl);
  ~ClientInterfaceBinder() override;

  // Called by |impl_| its destruction.
  // This informs the binder proxy that no future manipulations of |impl_|
  // by remote processes are possible.
  void NotifyImplDead() { impl_ = nullptr; }

  ::android::binder::Status enableSupplicant(bool* success) override;
  ::android::binder::Status disableSupplicant(bool* success) override;
  ::android::binder::Status getPacketCounters(
      std::vector<int32_t>* out_packet_counters) override;
  ::android::binder::Status signalPoll(
      std::vector<int32_t>* out_signal_poll_results) override;
  ::android::binder::Status getMacAddress(
      std::vector<uint8_t>* out_mac_address) override;
  ::android::binder::Status getInterfaceName(std::string* out_name) override;
  ::android::binder::Status requestRttRange(
      const ::android::net::wifi::RttManager::ParcelableRttParams& rtt_params,
      const ::android::sp<::android::net::wifi::IRttEventCallback>& callback,
      bool* success) override;
  ::android::binder::Status cancelRttRange(bool* success) override;
  ::android::binder::Status enableRttResponder(
      int32_t timeoutSeconds,
      ::android::net::wifi::RttManager::ResponderConfig* _out_responder_config) override;
  ::android::binder::Status disableRttResponder(bool* success) override;

 private:
  ClientInterfaceImpl* impl_;

  DISALLOW_COPY_AND_ASSIGN(ClientInterfaceBinder);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_CLIENT_INTERFACE_BINDER_H_
