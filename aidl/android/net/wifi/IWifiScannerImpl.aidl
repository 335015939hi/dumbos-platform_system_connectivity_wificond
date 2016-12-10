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

package android.net.wifi;

import android.net.wifi.IScanEvent;
import com.android.server.wifi.wificond.NativeScanResult;

interface IWifiScannerImpl {
  // Returns an array of available frequencies for 2.4GHz channels.
  int[] getAvailable2gChannels();
  // Returns an array of available frequencies for 5GHz non-DFS channels.
  int[] getAvailable5gNonDFSChannels();
  // Returns an array of available frequencies for DFS channels.
  int[] getAvailableDFSChannels();
  // Get the latest scan results from kernel.
  NativeScanResult[] getScanResults();
  // Request a one-shot scan using a frequency list and a ssid list.
  // |random_mac| is used for requesting scan with randomized mac address.
  boolean scan(in int[] freqs, in String[] ssids, boolean random_mac);
  // Subscribe the scanning events.
  // This assumes there is only one subscriber.
  void SubscribeScanEvents(IScanEvent handler);
  // Unsubscribe the scanning events .
  void UnsubscribeScanEvents();
  // TODO(nywang) add more interfaces.
}
