#
#  Copyright (c) 2017, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

sbin_PROGRAMS=otbr-weave

override CFLAGS          := $(filter-out -pedantic-errors -Wall -Werror,$(CFLAGS)) -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-format-truncation
override CXXFLAGS        := $(filter-out -std=gnu++98 -Wall -Werror,$(CXXFLAGS)) -std=c++11 -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-format-truncation
# Always package (e.g. for 'make dist') these subdirectories.

otbr_weave_SOURCES = \
    main.cpp         \
    $(NULL)

otbr_weave_LDFLAGS = \
    -static \
    $(NULL)

otbr_weave_LIBTOOLFLAGS = \
    --preserve-dup-deps \
    $(NULL)

otbr_weave_CPPFLAGS    = \
    -DEXTERNAL_PLATFORMMANAGERIMPL_HEADER=\"PlatformManagerImpl.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMCONFIG_HEADER=\"WeaveDevicePlatformConfig.h\" \
    -DEXTERNAL_WEAVEDEVICEPLATFORMEVENT_HEADER=\"WeaveDevicePlatformEvent.h\" \
    -DEXTERNAL_CONFIGURATIONMANAGERIMPL_HEADER=\"ConfigurationManagerImpl.h\" \
    -DEXTERNAL_CONNECTIVITYMANAGERIMPL_HEADER=\"ConnectivityManagerImpl.h\" \
    -DEXTERNAL_NETWORKPROVISIONINGSERVERIMPL_HEADER=\"NetworkProvisioningServerImpl.h\" \
    -I$(top_srcdir)/src/openweave/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/repo/src/adaptations/device-layer/include \
    -I$(top_srcdir)/third_party/openweave/otbr/output/include \
    -I$(top_srcdir)/third_party/openthread/repo/src/posix/platform \
    -I$(top_srcdir)/third_party/openthread/otbr/output/posix/include \
    $(NULL)

otbr_weave_LDADD = \
    $(top_builddir)/src/openweave/device-layer/libDeviceLayer.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libWeave.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libInetLayer.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libWarm.a \
    $(top_builddir)/src/openweave/device-layer/libWeaveSupport.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libnlfaultinjection.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libSystemLayer.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libmincrypt.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libopenssl-jpake.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libnlunit-test.a \
    $(top_builddir)/third_party/openweave/otbr/output/x86_64-unknown-linux-gnu/lib/libRADaemon.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-cli-ftd.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-posix.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-cli-ftd.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-diag.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-ftd.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libopenthread-posix.a \
    $(top_builddir)/third_party/openthread/otbr/output/posix/x86_64-unknown-linux-gnu/lib/libmbedcrypto.a \
    -lpthread \
    -lssl \
    -lcrypto \
    -lutil \
    $(NULL)

DIST_SUBDIRS                                         = \
    config                                             \
    device-layer                                       \
    $(NULL)

# Always build (e.g. for 'make all') these subdirectories.

SUBDIRS                                              = \
    device-layer                                       \
    $(NULL)


# Always pretty (e.g. for 'make pretty') these subdirectories.

PRETTY_SUBDIRS                                       = \
    config                                             \
    device-layer                                       \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
