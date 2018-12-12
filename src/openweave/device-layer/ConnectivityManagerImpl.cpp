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

#include <Warm/Warm.h>

namespace nl {
namespace Weave {
namespace DeviceLayer {

class PlatformManagerImpl;
class WeaveDeviceEvent;

} // namespace DeviceLayer
} // namespace Weave
} // namespace nl

#include <Weave/DeviceLayer/ConnectivityManager.h>
#include <Weave/DeviceLayer/WeaveDeviceEvent.h>
#include <Weave/DeviceLayer/internal/BLEManager.h>
#include <Weave/DeviceLayer/internal/NetworkInfo.h>
#include <Weave/DeviceLayer/internal/NetworkProvisioningServer.h>
#include <Weave/DeviceLayer/internal/ServiceTunnelAgent.h>
#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>
#include <Weave/Profiles/WeaveProfiles.h>
#include <Weave/Profiles/common/CommonProfile.h>

#include <openthread/instance.h>
#include <openthread/link.h>
#include <openthread/thread.h>

extern otInstance *gInstance;

#include <new>

using namespace ::nl;
using namespace ::nl::Weave;
using namespace ::nl::Weave::TLV;
using namespace ::nl::Weave::Profiles::Common;
using namespace ::nl::Weave::Profiles::NetworkProvisioning;
using namespace ::nl::Weave::Profiles::WeaveTunnel;
using namespace ::nl::Weave::DeviceLayer::Internal;

using Profiles::kWeaveProfile_Common;
using Profiles::kWeaveProfile_NetworkProvisioning;

namespace nl {
namespace Weave {
namespace DeviceLayer {

namespace {

inline ConnectivityChange GetConnectivityChange(bool prevState, bool newState)
{
    if (prevState == newState)
        return kConnectivity_NoChange;
    else if (newState)
        return kConnectivity_Established;
    else
        return kConnectivity_Lost;
}

} // unnamed namespace

ConnectivityManagerImpl ConnectivityManagerImpl::sInstance;

ConnectivityManager::WiFiStationMode ConnectivityManagerImpl::_GetWiFiStationMode(void)
{
    return mWiFiStationMode;
}

bool ConnectivityManagerImpl::_IsWiFiStationEnabled(void)
{
    return GetWiFiStationMode() == kWiFiStationMode_Enabled;
}

WEAVE_ERROR ConnectivityManagerImpl::_SetWiFiStationMode(WiFiStationMode val)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    return err;
}

bool ConnectivityManagerImpl::_IsWiFiStationProvisioned(void)
{
    return false;
}

void ConnectivityManagerImpl::_ClearWiFiStationProvision(void)
{
}

WEAVE_ERROR ConnectivityManagerImpl::_SetWiFiAPMode(WiFiAPMode val)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    return err;
}

void ConnectivityManagerImpl::_DemandStartWiFiAP(void)
{
}

void ConnectivityManagerImpl::_StopOnDemandWiFiAP(void)
{
}

void ConnectivityManagerImpl::_MaintainOnDemandWiFiAP(void)
{
}

void ConnectivityManagerImpl::_SetWiFiAPIdleTimeoutMS(uint32_t val)
{
}

WEAVE_ERROR ConnectivityManagerImpl::_SetServiceTunnelMode(ServiceTunnelMode val)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    return err;
}

bool ConnectivityManagerImpl::_IsServiceTunnelConnected(void)
{
    return false;
}

bool ConnectivityManagerImpl::_IsServiceTunnelRestricted(void)
{
    return false;
}

bool ConnectivityManagerImpl::_HaveServiceConnectivity(void)
{
    return false;
}

ConnectivityManager::WoBLEServiceMode ConnectivityManagerImpl::_GetWoBLEServiceMode(void)
{
    return kWoBLEServiceMode_NotSupported;
}

WEAVE_ERROR ConnectivityManagerImpl::_SetWoBLEServiceMode(WoBLEServiceMode val)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

bool ConnectivityManagerImpl::_IsBLEAdvertisingEnabled(void)
{
    return false;
}

WEAVE_ERROR ConnectivityManagerImpl::_SetBLEAdvertisingEnabled(bool val)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

bool ConnectivityManagerImpl::_IsBLEFastAdvertisingEnabled(void)
{
    return false;
}

WEAVE_ERROR ConnectivityManagerImpl::_SetBLEFastAdvertisingEnabled(bool val)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

WEAVE_ERROR ConnectivityManagerImpl::_GetBLEDeviceName(char *buf, size_t bufSize)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

WEAVE_ERROR ConnectivityManagerImpl::_SetBLEDeviceName(const char *deviceName)
{
    return WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE;
}

uint16_t ConnectivityManagerImpl::_NumBLEConnections(void)
{
    return 0;
}

// ==================== ConnectivityManager Platform Internal Methods ====================

WEAVE_ERROR ConnectivityManagerImpl::_Init()
{
    return 0;
}

void ConnectivityManagerImpl::_OnPlatformEvent(const WeaveDeviceEvent *event)
{
}

void ConnectivityManagerImpl::_OnWiFiScanDone()
{
}

void ConnectivityManagerImpl::_OnWiFiStationProvisionChange()
{
}

// ==================== ConnectivityManager Private Methods ====================

void ConnectivityManagerImpl::DriveStationState()
{
}

void ConnectivityManagerImpl::OnStationConnected()
{
}

void ConnectivityManagerImpl::OnStationDisconnected()
{
}

void ConnectivityManagerImpl::ChangeWiFiStationState(WiFiStationState newState)
{
}

void ConnectivityManagerImpl::DriveStationState(nl::Weave::System::Layer *aLayer,
                                                void *                    aAppState,
                                                nl::Weave::System::Error  aError)
{
}

void ConnectivityManagerImpl::DriveAPState()
{
}

WEAVE_ERROR ConnectivityManagerImpl::ConfigureWiFiAP()
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    return err;
}

void ConnectivityManagerImpl::ChangeWiFiAPState(WiFiAPState newState)
{
}

void ConnectivityManagerImpl::DriveAPState(nl::Weave::System::Layer *aLayer,
                                           void *                    aAppState,
                                           nl::Weave::System::Error  aError)
{
}

void ConnectivityManagerImpl::UpdateInternetConnectivityState(void)
{
}

void ConnectivityManagerImpl::OnStationIPv4AddressLost(void)
{
}

void ConnectivityManagerImpl::DriveServiceTunnelState(void)
{
}

void ConnectivityManagerImpl::DriveServiceTunnelState(nl::Weave::System::Layer *aLayer,
                                                      void *                    aAppState,
                                                      nl::Weave::System::Error  aError)
{
}

const char *ConnectivityManagerImpl::_WiFiStationModeToStr(WiFiStationMode mode)
{
    switch (mode)
    {
    case kWiFiStationMode_NotSupported:
        return "NotSupported";
    case kWiFiStationMode_ApplicationControlled:
        return "AppControlled";
    case kWiFiStationMode_Enabled:
        return "Enabled";
    case kWiFiStationMode_Disabled:
        return "Disabled";
    default:
        return "(unknown)";
    }
}

const char *ConnectivityManagerImpl::_WiFiAPModeToStr(WiFiAPMode mode)
{
    switch (mode)
    {
    case kWiFiAPMode_NotSupported:
        return "NotSupported";
    case kWiFiAPMode_ApplicationControlled:
        return "AppControlled";
    case kWiFiAPMode_Disabled:
        return "Disabled";
    case kWiFiAPMode_Enabled:
        return "Enabled";
    case kWiFiAPMode_OnDemand:
        return "OnDemand";
    case kWiFiAPMode_OnDemand_NoStationProvision:
        return "OnDemand_NoStationProvision";
    default:
        return "(unknown)";
    }
}

const char *ConnectivityManagerImpl::_ServiceTunnelModeToStr(ServiceTunnelMode mode)
{
    switch (mode)
    {
    case kServiceTunnelMode_NotSupported:
        return "NotSupported";
    case kServiceTunnelMode_Disabled:
        return "Disabled";
    case kServiceTunnelMode_Enabled:
        return "Enabled";
    default:
        return "(unknown)";
    }
}

const char *ConnectivityManagerImpl::_WoBLEServiceModeToStr(WoBLEServiceMode mode)
{
    switch (mode)
    {
    case kWoBLEServiceMode_NotSupported:
        return "NotSupported";
    case kWoBLEServiceMode_Enabled:
        return "Disabled";
    case kWoBLEServiceMode_Disabled:
        return "Enabled";
    default:
        return "(unknown)";
    }
}

const char *ConnectivityManagerImpl::WiFiStationStateToStr(WiFiStationState state)
{
    switch (state)
    {
    case kWiFiStationState_NotConnected:
        return "NotConnected";
    case kWiFiStationState_Connecting:
        return "Connecting";
    case kWiFiStationState_Connecting_Succeeded:
        return "Connecting_Succeeded";
    case kWiFiStationState_Connecting_Failed:
        return "Connecting_Failed";
    case kWiFiStationState_Connected:
        return "Connected";
    case kWiFiStationState_Disconnecting:
        return "Disconnecting";
    default:
        return "(unknown)";
    }
}

const char *ConnectivityManagerImpl::WiFiAPStateToStr(WiFiAPState state)
{
    switch (state)
    {
    case kWiFiAPState_NotActive:
        return "NotActive";
    case kWiFiAPState_Activating:
        return "Activating";
    case kWiFiAPState_Active:
        return "Active";
    case kWiFiAPState_Deactivating:
        return "Deactivating";
    default:
        return "(unknown)";
    }
}

void ConnectivityManagerImpl::RefreshMessageLayer(void)
{
    WEAVE_ERROR err = MessageLayer.RefreshEndpoints();
    if (err != WEAVE_NO_ERROR)
    {
        WeaveLogError(DeviceLayer, "MessageLayer.RefreshEndpoints() failed: %s", nl::ErrorStr(err));
    }
}

void ConnectivityManagerImpl::HandleServiceTunnelNotification(WeaveTunnelConnectionMgr::TunnelConnNotifyReasons reason,
                                                              WEAVE_ERROR                                       err,
                                                              void *                                            appCtxt)
{
    bool newTunnelState  = false;
    bool prevTunnelState = GetFlag(sInstance.mFlags, kFlag_ServiceTunnelUp);
    bool isRestricted    = false;

    switch (reason)
    {
    case WeaveTunnelConnectionMgr::kStatus_TunDown:
        WeaveLogProgress(DeviceLayer, "ConnectivityManager: Service tunnel down");
        break;
    case WeaveTunnelConnectionMgr::kStatus_TunPrimaryConnError:
        WeaveLogProgress(DeviceLayer, "ConnectivityManager: Service tunnel connection error: %s", ::nl::ErrorStr(err));
        break;
    case WeaveTunnelConnectionMgr::kStatus_TunPrimaryUp:
        newTunnelState = true;
        isRestricted   = (err == WEAVE_ERROR_TUNNEL_ROUTING_RESTRICTED);
        WeaveLogProgress(DeviceLayer, "ConnectivityManager: %service tunnel established",
                         (isRestricted) ? "RESTRICTED s" : "S");
        break;
    default:
        break;
    }

    // If the tunnel state has changed...
    if (newTunnelState != prevTunnelState)
    {
        // Update the cached copy of the state.
        SetFlag(sInstance.mFlags, kFlag_ServiceTunnelUp, newTunnelState);

        // Alert other components of the change to the tunnel state.
        WeaveDeviceEvent event;
        event.Type                                  = DeviceEventType::kServiceTunnelStateChange;
        event.ServiceTunnelStateChange.Result       = GetConnectivityChange(prevTunnelState, newTunnelState);
        event.ServiceTunnelStateChange.IsRestricted = isRestricted;
        PlatformMgr().PostEvent(&event);

        // If the new tunnel state represents a logical change in connectivity to the service, as it
        // relates to the application, post a ServiceConnectivityChange event.
        // (Note that the establishment of a restricted tunnel to the service does not constitute a
        // logical change in service connectivity from the application's standpoint, as such a tunnel
        // cannot be used for general application interactions, only pairing).
        if (newTunnelState)
        {
            if (!isRestricted)
            {
                event.Type                             = DeviceEventType::kServiceConnectivityChange;
                event.ServiceConnectivityChange.Result = kConnectivity_Established;
                PlatformMgr().PostEvent(&event);
            }
        }

        else
        {
            event.Type                             = DeviceEventType::kServiceConnectivityChange;
            event.ServiceConnectivityChange.Result = kConnectivity_Lost;
            PlatformMgr().PostEvent(&event);
        }
    }
}

WEAVE_ERROR ConnectivityManagerImpl::_SetThreadMode(ThreadMode val)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    if (val == kThreadMode_Enabled)
    {
        otLinkSetPanId(gInstance, 0xface);
        otIp6SetEnabled(gInstance, true);
        otThreadSetEnabled(gInstance, true);
    }

    return err;
}

bool ConnectivityManagerImpl::_IsThreadProvisioned(void)
{
    return true;
}

} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
