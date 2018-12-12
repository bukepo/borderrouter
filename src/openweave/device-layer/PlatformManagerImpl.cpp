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
 *          Provides an implementation of the PlatformManager object
 *          for the ESP32 platform.
 */

#include <Weave/DeviceLayer/PlatformManager.h>
#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>
#include <Weave/DeviceLayer/internal/GenericPlatformManagerImpl.ipp>

namespace nl {
namespace Weave {
namespace DeviceLayer {

// Fully instantiate the template classes on which the ESP32 PlatformManager depends.
template class Internal::GenericPlatformManagerImpl<PlatformManagerImpl>;

PlatformManagerImpl PlatformManagerImpl::sInstance;

void PlatformManagerImpl::_PostEvent(const WeaveDeviceEvent * event)
{
    (void)event;
}

//void PlatformManagerImpl::_DispatchEvent(const WeaveDeviceEvent * event)
//{
//    (void)event;
//}

WEAVE_ERROR PlatformManagerImpl::_InitWeaveStack(void)
{
    WEAVE_ERROR err;

    // Call _InitWeaveStack() on the generic implementation base class
    // to finish the initialization process.
    err = Internal::GenericPlatformManagerImpl<PlatformManagerImpl>::_InitWeaveStack();
    SuccessOrExit(err);

exit:
    return err;
}

} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
