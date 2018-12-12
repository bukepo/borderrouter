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
 *          Utilities for interacting with the the ESP32 "NVS" key-value store.
 */

#include "ESP32Config.h"

#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>

extern uint64_t gNodeId;

namespace nl {
namespace Weave {
namespace DeviceLayer {
namespace Internal {

// NVS namespaces used to store device configuration information.
const char ESP32Config::kConfigNamespace_WeaveFactory[]  = "weave-factory";
const char ESP32Config::kConfigNamespace_WeaveConfig[]   = "weave-config";
const char ESP32Config::kConfigNamespace_WeaveCounters[] = "weave-counters";

// Keys stored in the weave-factory namespace
const ESP32Config::Key ESP32Config::kConfigKey_SerialNum         = {kConfigNamespace_WeaveFactory, "serial-num"};
const ESP32Config::Key ESP32Config::kConfigKey_DeviceId          = {kConfigNamespace_WeaveFactory, "device-id"};
const ESP32Config::Key ESP32Config::kConfigKey_DeviceCert        = {kConfigNamespace_WeaveFactory, "device-cert"};
const ESP32Config::Key ESP32Config::kConfigKey_DevicePrivateKey  = {kConfigNamespace_WeaveFactory, "device-key"};
const ESP32Config::Key ESP32Config::kConfigKey_ManufacturingDate = {kConfigNamespace_WeaveFactory, "mfg-date"};
const ESP32Config::Key ESP32Config::kConfigKey_PairingCode       = {kConfigNamespace_WeaveFactory, "pairing-code"};

// Keys stored in the weave-config namespace
const ESP32Config::Key ESP32Config::kConfigKey_FabricId           = {kConfigNamespace_WeaveConfig, "fabric-id"};
const ESP32Config::Key ESP32Config::kConfigKey_ServiceConfig      = {kConfigNamespace_WeaveConfig, "service-config"};
const ESP32Config::Key ESP32Config::kConfigKey_PairedAccountId    = {kConfigNamespace_WeaveConfig, "account-id"};
const ESP32Config::Key ESP32Config::kConfigKey_ServiceId          = {kConfigNamespace_WeaveConfig, "service-id"};
const ESP32Config::Key ESP32Config::kConfigKey_FabricSecret       = {kConfigNamespace_WeaveConfig, "fabric-secret"};
const ESP32Config::Key ESP32Config::kConfigKey_GroupKeyIndex      = {kConfigNamespace_WeaveConfig, "group-key-index"};
const ESP32Config::Key ESP32Config::kConfigKey_LastUsedEpochKeyId = {kConfigNamespace_WeaveConfig, "last-ek-id"};
const ESP32Config::Key ESP32Config::kConfigKey_FailSafeArmed      = {kConfigNamespace_WeaveConfig, "fail-safe-armed"};
const ESP32Config::Key ESP32Config::kConfigKey_WiFiStationSecType = {kConfigNamespace_WeaveConfig, "sta-sec-type"};

// Prefix used for NVS keys that contain Weave group encryption keys.
const char ESP32Config::kGroupKeyNamePrefix[] = "gk-";

WEAVE_ERROR ESP32Config::ReadConfigValue(Key key, bool &val)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ReadConfigValue(Key key, uint32_t &val)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ReadConfigValue(Key key, uint64_t &val)
{
    if (!strcmp(key.Namespace, kConfigNamespace_WeaveFactory))
    {
        if (!strcmp(key.Name, "device-id"))
        {
            val = gNodeId;
        }
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ReadConfigValueStr(Key key, char *buf, size_t bufSize, size_t &outLen)
{
    static char kPairingCode[]  = "NESTUS";
    static char kSerialNumber[] = "00112233445566778899aabbccddeeff";

    if (!strcmp(key.Namespace, kConfigNamespace_WeaveFactory))
    {
        if (!strcmp(key.Name, "pairing-code"))
        {
            strcpy(buf, kPairingCode);
            outLen = sizeof(kPairingCode);
        }
        else if (!strcmp(key.Name, "serial-num"))
        {
            strcpy(buf, kSerialNumber);
            outLen = sizeof(kSerialNumber);
        }
    }

    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ReadConfigValueBin(Key key, uint8_t *buf, size_t bufSize, size_t &outLen)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValue(Key key, bool val)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValue(Key key, uint32_t val)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValue(Key key, uint64_t val)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValueStr(Key key, const char *str)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValueStr(Key key, const char *str, size_t strLen)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::WriteConfigValueBin(Key key, const uint8_t *data, size_t dataLen)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ClearConfigValue(Key key)
{
    return WEAVE_NO_ERROR;
}

bool ESP32Config::ConfigValueExists(Key key)
{
    return false;
}

WEAVE_ERROR ESP32Config::EnsureNamespace(const char *ns)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Config::ClearNamespace(const char *ns)
{
    return WEAVE_NO_ERROR;
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
