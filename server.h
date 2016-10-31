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

#ifndef WIFICOND_SERVER_H_
#define WIFICOND_SERVER_H_

#include <memory>
#include <string>
#include <vector>

#include <android-base/macros.h>
#include <android/hardware/wifi/1.0/IWifi.h>
#include <wifi_hal/driver_tool.h>
#include <wifi_system/hal_tool.h>
#include <wifi_system/interface_tool.h>

#include "android/net/wifi/BnWificond.h"
#include "android/net/wifi/IApInterface.h"
#include "android/net/wifi/IClientInterface.h"
#include "android/net/wifi/IInterfaceEventCallback.h"

#include "wificond/ap_interface_impl.h"
#include "wificond/client_interface_impl.h"
#include "wificond/rtt/rtt_controller_impl.h"

namespace android {
namespace wificond {

class NL80211Packet;
class NetlinkUtils;
class ScanUtils;

class Server : public android::net::wifi::BnWificond,
               public android::hardware::wifi::V1_0::IWifiEventCallback,
               public android::hardware::wifi::V1_0::IWifiChipEventCallback {
 public:
  Server(std::unique_ptr<wifi_system::HalTool> hal_tool,
         std::unique_ptr<wifi_system::InterfaceTool> if_tool,
         std::unique_ptr<wifi_hal::DriverTool> driver_tool,
         std::unique_ptr<wifi_system::SupplicantManager> supplicant_man,
         std::unique_ptr<wifi_system::HostapdManager> hostapd_man,
         sp<android::hardware::wifi::V1_0::IWifi> hal_hidl,
         NetlinkUtils* netlink_utils,
         ScanUtils* scan_utils);
  ~Server() override = default;

  android::binder::Status RegisterCallback(
      const android::sp<android::net::wifi::IInterfaceEventCallback>&
          callback) override;
  android::binder::Status UnregisterCallback(
      const android::sp<android::net::wifi::IInterfaceEventCallback>&
          callback) override;

  android::binder::Status registerRttClient(
      const android::sp<android::net::wifi::IRttClient>& rtt_client,
      android::sp<android::net::wifi::IRttController>* out_rtt_controller)
      override;

  android::binder::Status unregisterRttClient(
      const android::sp<android::net::wifi::IRttClient>& rttClient) override;

  android::binder::Status createApInterface(
      android::sp<android::net::wifi::IApInterface>*
          created_interface) override;

  android::binder::Status createClientInterface(
      android::sp<android::net::wifi::IClientInterface>*
          created_interface) override;

  android::binder::Status tearDownInterfaces() override;

  android::binder::Status GetClientInterfaces(
      std::vector<android::sp<android::IBinder>>* out_client_ifs) override;
  android::binder::Status GetApInterfaces(
      std::vector<android::sp<android::IBinder>>* out_ap_ifs) override;

  android::hardware::Return<void> onStart() override;
  android::hardware::Return<void> onStop() override;
  android::hardware::Return<void> onFailure(
      const android::hardware::wifi::V1_0::WifiStatus& reason) override;
  android::hardware::Return<void> onChipReconfigured(uint32_t mode_id) override;
  android::hardware::Return<void>
  onDebugRingBufferConnectivityEventEntriesAvailable(
      const android::hardware::wifi::V1_0::WifiDebugRingBufferStatus& status,
      const android::hardware::hidl_vec<
          android::hardware::wifi::V1_0::WifiDebugRingEntryConnectivityEvent>&
          entries) override;
  android::hardware::Return<void> onDebugRingBufferPowerEventEntriesAvailable(
      const android::hardware::wifi::V1_0::WifiDebugRingBufferStatus& status,
      const android::hardware::hidl_vec<
          android::hardware::wifi::V1_0::WifiDebugRingEntryPowerEvent>& entries)
      override;
  android::hardware::Return<void>
  onDebugRingBufferWakelockEventEntriesAvailable(
      const android::hardware::wifi::V1_0::WifiDebugRingBufferStatus& status,
      const android::hardware::hidl_vec<
          android::hardware::wifi::V1_0::WifiDebugRingEntryWakelockEvent>&
          entries) override;
  android::hardware::Return<void> onDebugRingBufferVendorDataEntriesAvailable(
      const android::hardware::wifi::V1_0::WifiDebugRingBufferStatus& status,
      const android::hardware::hidl_vec<
          android::hardware::wifi::V1_0::WifiDebugRingEntryVendorData>& entries)
      override;

  // Call this once on startup.  It ignores all the invariants held
  // in wificond and tries to restore ourselves to a blank state by
  // killing userspace daemons and cleaning up the interface state.
  void CleanUpSystemState();

 private:
  // Does the actual work of setting up an interface for a particular mode.
  //
  // |mode| is one of WIFI_GET_FW_PATH_* defined in hardware_legacy/wifi.h.
  // |interface_name| is a pointer to a string to store the name of Linux
  //     network interface that has been setup.
  //
  // Returns true on success, false otherwise.
  bool SetupInterfaceForMode(int mode,
                             std::string* interface_name,
                             uint32_t* interface_index,
                             std::vector<uint8_t>* interface_mac_addr);
  bool RefreshWiphyIndex();
  void BroadcastClientInterfaceReady(
      android::sp<android::net::wifi::IClientInterface> network_interface);
  void BroadcastApInterfaceReady(
      android::sp<android::net::wifi::IApInterface> network_interface);
  void BroadcastClientInterfaceTornDown(
      android::sp<android::net::wifi::IClientInterface> network_interface);
  void BroadcastApInterfaceTornDown(
      android::sp<android::net::wifi::IApInterface> network_interface);

  const std::unique_ptr<wifi_system::HalTool> hal_tool_;
  const std::unique_ptr<wifi_system::InterfaceTool> if_tool_;
  const std::unique_ptr<wifi_hal::DriverTool> driver_tool_;
  const std::unique_ptr<wifi_system::SupplicantManager> supplicant_manager_;
  const std::unique_ptr<wifi_system::HostapdManager> hostapd_manager_;
  const sp<android::hardware::wifi::V1_0::IWifi> hal_hidl_;
  sp<android::hardware::wifi::V1_0::IWifiChip> hal_chip_hidl_;
  NetlinkUtils* const netlink_utils_;
  ScanUtils* const scan_utils_;

  uint32_t wiphy_index_;
  std::vector<std::unique_ptr<ApInterfaceImpl>> ap_interfaces_;
  std::vector<std::unique_ptr<ClientInterfaceImpl>> client_interfaces_;
  std::vector<android::sp<android::net::wifi::IInterfaceEventCallback>>
      interface_event_callbacks_;

  std::unique_ptr<RttControllerImpl> rtt_controller_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_SERVER_H_
