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
 *          General utility methods for the ESP32 platform.
 */

#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>

#include "ESP32Utils.h"

using namespace ::nl::Weave::DeviceLayer::Internal;
using namespace ::nl::Weave::Profiles::NetworkProvisioning;

WEAVE_ERROR ESP32Utils::IsAPEnabled(bool &apEnabled)
{
    return WEAVE_NO_ERROR;
}

bool ESP32Utils::IsStationProvisioned(void)
{
    return false;
}

WEAVE_ERROR ESP32Utils::IsStationConnected(bool &connected)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Utils::StartWiFiLayer(void)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Utils::EnableStationMode(void)
{
    return WEAVE_NO_ERROR;
}

WEAVE_ERROR ESP32Utils::SetAPMode(bool enabled)
{
    return WEAVE_NO_ERROR;
}

int ESP32Utils::OrderScanResultsByRSSI(const void *_res1, const void *_res2)
{
    return 0;
}
