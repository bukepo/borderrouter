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
 *          Defines platform-specific event types and data for the
 *          Weave Device Layer on the POSIX.
 */

#ifndef WEAVE_DEVICE_PLATFORM_EVENT_H
#define WEAVE_DEVICE_PLATFORM_EVENT_H

#include <SystemLayer/SystemError.h>
#include <SystemLayer/SystemLayer.h>

namespace nl {
namespace Weave {

namespace DeviceLayer {

struct WeaveDeviceEvent;

namespace DeviceEventType {

/**
 * Enumerates platform-specific event types that are visible to the application.
 */
enum
{
    kPosixSystemEvent = kRange_PublicPlatformSpecific,
};

} // namespace DeviceEventType

/**
 * Represents platform-specific event information for the POSIX platform.
 */
struct WeaveDevicePlatformEvent final
{
    union
    {
        int PosixSystemEvent;
    };
};

} // namespace DeviceLayer

namespace System {

typedef int                                                      EventType;
typedef const struct ::nl::Weave::DeviceLayer::WeaveDeviceEvent *Event;

namespace Platform {
namespace Layer {

extern Error PostEvent(Layer &aLayer, void *aContext, Object &aTarget, EventType aType, uintptr_t aArgument);
extern Error DispatchEvents(Layer &aLayer, void *aContext);
extern Error DispatchEvent(Layer &aLayer, void *aContext, Event aEvent);
extern Error StartTimer(Layer &aLayer, void *aContext, uint32_t aMilliseconds);

} // namespace Layer
} // namespace Platform
} // namespace System

} // namespace Weave
} // namespace nl

#endif // WEAVE_DEVICE_PLATFORM_EVENT_H
