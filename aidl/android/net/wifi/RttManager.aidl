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

// This is actually a list of RttParams.
// We make it identical to ParcelableRttParams class in RttManager.java.
parcelable RttManager.ParcelableRttParams cpp_header "wificond/rtt/rtt_params.h";
// This is actually a list of RttResults.
// We make it identical to ParcelableRttResults class in RttManager.java.
parcelable RttManager.ParcelableRttResults cpp_header "wificond/rtt/rtt_results.h";
parcelable RttManager.ResponderConfig cpp_header "wificond/rtt/responder_config.h";
parcelable RttManager.RttCapabilities cpp_header "wificond/rtt/rtt_capabilities.h";
