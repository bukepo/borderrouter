#
#    Copyright (c) 2014-2018 Nest Labs, Inc.
#    Copyright (c) 2018 Google LLC
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    Description:
#      This file is the GNU automake template for the OpenWeave Device Layer
#      library.

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

override CFLAGS          := $(filter-out -pedantic-errors -Wall -Werror,$(CFLAGS)) -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-format-truncation
override CXXFLAGS        := $(filter-out -std=gnu++98 -Wall -Werror,$(CXXFLAGS)) -std=c++11 -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-format-truncation

EXTRA_DIST                              = \
    $(NULL)

lib_LIBRARIES                           = libDeviceLayer.a libWeaveSupport.a

libDeviceLayer_a_CPPFLAGS               = \
    -DEXTERNAL_PLATFORMMANAGERIMPL_HEADER=\"PlatformManagerImpl.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMCONFIG_HEADER=\"WeaveDevicePlatformConfig.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMEVENT_HEADER=\"WeaveDevicePlatformEvent.h\" \
    -DEXTERNAL_CONFIGURATIONMANAGERIMPL_HEADER=\"ConfigurationManagerImpl.h\" \
    -DEXTERNAL_CONNECTIVITYMANAGERIMPL_HEADER=\"ConnectivityManagerImpl.h\" \
    -DEXTERNAL_NETWORKPROVISIONINGSERVERIMPL_HEADER=\"NetworkProvisioningServerImpl.h\" \
    -I$(top_srcdir)/src/openweave/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/otbr/output/include \
    -I$(top_srcdir)/third_party/openweave/repo/src/adaptations/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/repo/src/adaptations/device-layer/trait-support \
    -I$(top_srcdir)/third_party/openthread/otbr/output/posix/include \
    $(NULL)

libDeviceLayer_a_SOURCES                = \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/DeviceControlServer.cpp               \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/DeviceDescriptionServer.cpp           \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/DeviceIdentityTraitDataSource.cpp     \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/FabricProvisioningServer.cpp          \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/GeneralUtils.cpp                      \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/NetworkInfo.cpp                       \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/CASEAuth.cpp                          \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/EchoServer.cpp                        \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/Globals.cpp                           \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/PersistedStorage.cpp                  \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/ServiceDirectoryManager.cpp           \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/ServiceProvisioningServer.cpp         \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/ServiceTunnelAgent.cpp                \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/SystemEventSupport.cpp                \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/SystemTimerSupport.cpp                \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/TestDeviceIds.cpp                     \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/TimeSyncManager.cpp                   \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/TraitManager.cpp                      \
    ../../../third_party/openweave/repo/src/adaptations/device-layer/trait-support/weave/trait/description/DeviceIdentityTrait.cpp \
    ConfigurationManagerImpl.cpp          \
    ConnectivityManagerImpl.cpp           \
    NetworkProvisioningServerImpl.cpp     \
    PlatformManagerImpl.cpp               \
    ESP32Config.cpp                       \
    ESP32Utils.cpp                        \
    Entropy.cpp                           \
    BLEManagerImpl.cpp                    \
    GroupKeyStoreImpl.cpp                 \
    Logging.cpp                           \
    SystemTimeSupport.cpp                 \
    $(NULL)

libWeaveSupport_a_CPPFLAGS               = \
    -DEXTERNAL_PLATFORMMANAGERIMPL_HEADER=\"PlatformManagerImpl.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMCONFIG_HEADER=\"WeaveDevicePlatformConfig.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMEVENT_HEADER=\"WeaveDevicePlatformEvent.h\" \
    -DEXTERNAL_CONFIGURATIONMANAGERIMPL_HEADER=\"ConfigurationManagerImpl.h\" \
    -DEXTERNAL_CONNECTIVITYMANAGERIMPL_HEADER=\"ConnectivityManagerImpl.h\" \
    -DEXTERNAL_NETWORKPROVISIONINGSERVERIMPL_HEADER=\"NetworkProvisioningServerImpl.h\" \
    -I$(top_srcdir)/src/openweave/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/otbr/output/include \
    -I$(top_srcdir)/third_party/openweave/repo/src/adaptations/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/repo/src/adaptations/device-layer/trait-support \
    $(NULL)

libWeaveSupport_a_SOURCES               = \
    WarmSupport.cpp                       \
    LoggingSupport.cpp                    \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
