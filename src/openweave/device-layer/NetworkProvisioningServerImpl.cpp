/*
 *
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <Weave/Core/WeaveTLV.h>
#include <Weave/DeviceLayer/internal/GenericNetworkProvisioningServerImpl.ipp>
#include <Weave/DeviceLayer/internal/NetworkInfo.h>
#include <Weave/DeviceLayer/internal/NetworkProvisioningServer.h>
#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>
#include <Weave/Profiles/WeaveProfiles.h>
#include <Weave/Profiles/common/CommonProfile.h>

#include <openthread/instance.h>
#include <openthread/thread.h>

extern otInstance *gInstance;

using namespace ::nl;
using namespace ::nl::Weave;
using namespace ::nl::Weave::TLV;
using namespace ::nl::Weave::Profiles::Common;
using namespace ::nl::Weave::Profiles::NetworkProvisioning;

using Profiles::kWeaveProfile_Common;

namespace nl {
namespace Weave {
namespace DeviceLayer {
namespace Internal {

// Explicitly instantiate the GenericNetworkProvisioningServerImpl<> template for use by the
// ESP32 NetworkProvisioningServerImpl class.
template class GenericNetworkProvisioningServerImpl<NetworkProvisioningServerImpl>;

NetworkProvisioningServerImpl NetworkProvisioningServerImpl::sInstance;

void NetworkProvisioningServerImpl::_OnPlatformEvent(const WeaveDeviceEvent *event)
{
    WEAVE_ERROR err;

    // Handle ESP system events...

    // Handle a change in WiFi connectivity...

    // Propagate the event to the GenericNetworkProvisioningServerImpl<> base class so
    // that it can take action on specific events.
    GenericImplClass::_OnPlatformEvent(event);

exit:
    return;
}

WEAVE_ERROR NetworkProvisioningServerImpl::GetWiFiStationProvision(NetworkInfo &netInfo, bool includeCredentials)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    return err;
}

WEAVE_ERROR NetworkProvisioningServerImpl::SetWiFiStationProvision(const NetworkInfo &netInfo)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    return err;
}

WEAVE_ERROR NetworkProvisioningServerImpl::SetThreadProvision(const NetworkInfo &netInfo)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    otThreadSetExtendedPanId(gInstance, reinterpret_cast<const otExtendedPanId *>(netInfo.mThread.mExtendedPANId));
    otThreadSetNetworkName(gInstance, netInfo.mThread.mNetworkName);
    otThreadSetNetworkName(gInstance, netInfo.mThread.mNetworkName);
    otThreadSetMasterKey(gInstance, reinterpret_cast<const otMasterKey *>(netInfo.mThread.mNetworkKey));
    return err;
}

WEAVE_ERROR NetworkProvisioningServerImpl::ClearWiFiStationProvision(void)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    // Clear the persisted WiFi station security type.
    ConfigurationMgrImpl().UpdateWiFiStationSecurityType(kWiFiSecurityType_NotSpecified);

    return err;
}

WEAVE_ERROR NetworkProvisioningServerImpl::InitiateWiFiScan(void)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

#if WEAVE_DEVICE_CONFIG_WIFI_SCAN_COMPLETION_TIMEOUT
    // Arm timer in case we never get the scan done event.
    SystemLayer.StartTimer(WEAVE_DEVICE_CONFIG_WIFI_SCAN_COMPLETION_TIMEOUT, HandleScanTimeOut, NULL);
#endif // WEAVE_DEVICE_CONFIG_WIFI_SCAN_COMPLETION_TIMEOUT

exit:
    return err;
}

void NetworkProvisioningServerImpl::HandleScanDone()
{
    ConnectivityMgr().OnWiFiScanDone();
}

#if WEAVE_DEVICE_CONFIG_WIFI_SCAN_COMPLETION_TIMEOUT

void NetworkProvisioningServerImpl::HandleScanTimeOut(::nl::Weave::System::Layer *aLayer,
                                                      void *                      aAppState,
                                                      ::nl::Weave::System::Error  aError)
{
    WeaveLogError(DeviceLayer, "WiFi scan timed out");

    // Reset the state.
    sInstance.mState = kState_Idle;

    // Verify that the ScanNetworks request is still outstanding; if so, send a
    // Common:InternalError StatusReport to the client.
    if (sInstance.GetCurrentOp() == kMsgType_ScanNetworks)
    {
        sInstance.SendStatusReport(kWeaveProfile_Common, kStatus_InternalError, WEAVE_ERROR_TIMEOUT);
    }

    // Tell the ConnectivityManager that the WiFi scan is now done.
    ConnectivityMgr().OnWiFiScanDone();
}

#endif // WEAVE_DEVICE_CONFIG_WIFI_SCAN_COMPLETION_TIMEOUT

} // namespace Internal
} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
