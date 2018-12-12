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

/**
 *    @file
 *          Provides the implementation of the Device Layer ConfigurationManager object
 *          for the ESP32.
 */
#include "ESP32Config.h"

#include <Weave/Core/WeaveKeyIds.h>
#include <Weave/Core/WeaveVendorIdentifiers.hpp>
#include <Weave/DeviceLayer/ConfigurationManager.h>
#include <Weave/DeviceLayer/internal/GenericConfigurationManagerImpl.ipp>
#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>
#include <Weave/Profiles/security/WeaveApplicationKeys.h>

#include "GroupKeyStoreImpl.h"

namespace nl {
namespace Weave {
namespace DeviceLayer {

using namespace ::nl::Weave::Profiles::Security::AppKeys;
using namespace ::nl::Weave::Profiles::DeviceDescription;
using namespace ::nl::Weave::DeviceLayer::Internal;

using ::nl::Weave::kWeaveVendor_NestLabs;

namespace {

enum
{
    kNestWeaveProduct_Connect = 0x0016
};

// Singleton instance of Weave Group Key Store for the ESP32
//
// NOTE: This is declared as a private global variable, rather than a static
// member of ConfigurationManagerImpl, to reduce the number of headers that
// must be included by the application when using the ConfigurationManager API.
//
GroupKeyStoreImpl gGroupKeyStore;

} // unnamed namespace

// Fully instantiate the template classes on which the ESP32 ConfigurationManager depends.
template class Internal::GenericConfigurationManagerImpl<ConfigurationManagerImpl>;

/** Singleton instance of the ConfigurationManager implementation object for the ESP32.
 */
ConfigurationManagerImpl ConfigurationManagerImpl::sInstance;

static uint8_t sTestMac[]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x28};
static uint8_t sTestEui64[] = {0x18, 0xb4, 0x30, 0x00, 0x00, 0x00, 0x00, 0x28};

WEAVE_ERROR ConfigurationManagerImpl::_Init()
{
    WEAVE_ERROR err;
    bool        failSafeArmed;

    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<ConfigurationManagerImpl>::_Init();
    SuccessOrExit(err);

    // Initialize the global GroupKeyStore object.
    err = gGroupKeyStore.Init();
    SuccessOrExit(err);

    err = WEAVE_NO_ERROR;

exit:
    return err;
}

WEAVE_ERROR ConfigurationManagerImpl::_GetPrimaryWiFiMACAddress(uint8_t *buf)
{
    memcpy(buf, sTestMac, sizeof(sTestMac));
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ConfigurationManagerImpl::_GetDeviceDescriptor(
    ::nl::Weave::Profiles::DeviceDescription::WeaveDeviceDescriptor &deviceDesc)
{
    WEAVE_ERROR err;
    size_t      outLen;

    deviceDesc.Clear();

    deviceDesc.DeviceId = FabricState.LocalNodeId;

    deviceDesc.FabricId = FabricState.FabricId;

    deviceDesc.VendorId = kWeaveVendor_NestLabs;

    deviceDesc.ProductId = kNestWeaveProduct_Connect;

    deviceDesc.ProductRevision = 0;

    deviceDesc.ManufacturingDate.Year  = 2018;
    deviceDesc.ManufacturingDate.Month = 1;
    deviceDesc.ManufacturingDate.Day   = 1;

    memcpy(deviceDesc.PrimaryWiFiMACAddress, sTestMac, sizeof(sTestMac));

    memcpy(deviceDesc.Primary802154MACAddress, sTestEui64, sizeof(sTestEui64));

    deviceDesc.RendezvousWiFiESSID[0] = '\0';

    strcpy(deviceDesc.SerialNumber, "00112233445566778899aabbccddeeff");

    strcpy(deviceDesc.SoftwareVersion, "1");

exit:
    return WEAVE_NO_ERROR;
}

::nl::Weave::Profiles::Security::AppKeys::GroupKeyStoreBase *ConfigurationManagerImpl::_GetGroupKeyStore()
{
    return &gGroupKeyStore;
}

bool ConfigurationManagerImpl::_CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::_InitiateFactoryReset()
{
}

WEAVE_ERROR ConfigurationManagerImpl::_ReadPersistedStorageValue(::nl::Weave::Platform::PersistedStorage::Key key,
                                                                 uint32_t &                                   value)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ConfigurationManagerImpl::_WritePersistedStorageValue(::nl::Weave::Platform::PersistedStorage::Key key,
                                                                  uint32_t                                     value)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ConfigurationManagerImpl::GetWiFiStationSecurityType(
    Profiles::NetworkProvisioning::WiFiSecurityType &secType)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ConfigurationManagerImpl::UpdateWiFiStationSecurityType(
    Profiles::NetworkProvisioning::WiFiSecurityType secType)
{
    return WEAVE_NO_ERROR;
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
}

} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
