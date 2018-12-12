CPPFLAGS="-maes -DEXTERNAL_PLATFORMMANAGERIMPL_HEADER=\"PlatformManagerImpl.h\" -DEXTERNAL_WEAVEDEVICEPLATFORMCONFIG_HEADER=\"WeaveDevicePlatformConfig.h\" -DEXTERNAL_WEAVEDEVICEPLATFORMEVENT_HEADER=\"WeaveDevicePlatformEvent.h\" -I$(pwd)/../../../src/openweave/device-layer/include" ../repo/configure \
    --with-openssl=no \
    --disable-tests \
    --disable-tools \
    --disable-docs \
    --disable-java \
    --with-logging-style=external \
    --with-weave-project-includes=$(pwd)/../../../src/openweave/config/no-openssl \
    --with-weave-system-project-includes=$(pwd)/../../../src/openweave/config \
    --with-weave-ble-project-includes=$(pwd)/../../../src/openweave/config \
    ${NULL}

make -j2
