#include "platform-posix.h"

#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/time.h>

#include "openthread-system.h"

#include <openthread/cli.h>
#include <openthread/tasklet.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/radio.h>

#include <Weave/Core/WeaveCore.h>
#include <Weave/DeviceLayer/PlatformManager.h>
#include <Weave/DeviceLayer/internal/DeviceControlServer.h>
#include <Weave/DeviceLayer/internal/DeviceDescriptionServer.h>
#include <Weave/DeviceLayer/internal/FabricProvisioningServer.h>
#include <Weave/DeviceLayer/internal/NetworkProvisioningServer.h>
#include <Weave/Profiles/echo/WeaveEcho.h>
#include <Weave/Support/ErrorStr.h>

#define NETWORK_SLEEP_TIME_MSECS (100 * 1000)

uint64_t            gDestNodeId;
nl::Inet::IPAddress gLocalv6Addr;
nl::Inet::IPAddress gDestv6Addr;
uint16_t            gDestPort;
otInstance *        gInstance;

using namespace nl::Weave;
using namespace nl::Weave::DeviceLayer::Internal;
using namespace nl::Weave::DeviceLayer;
using namespace nl::Weave::System;
using namespace nl::Weave::Profiles;

void otpProcessDrivers(otInstance *aInstance)
{
    fd_set         readFdSet;
    fd_set         writeFdSet;
    fd_set         errorFdSet;
    struct timeval timeout;
    int            maxFd = -1;
    int            rval;

    FD_ZERO(&readFdSet);
    FD_ZERO(&writeFdSet);
    FD_ZERO(&errorFdSet);
}

void DriveIO(otInstance *aInstance)
{
    struct timeval sleepTime;
    fd_set         readFDs, writeFDs, exceptFDs;
    int            numFDs = 0;
    int            selectRes;

    // Use a sleep value of 100 milliseconds
    sleepTime.tv_sec  = 0;
    sleepTime.tv_usec = NETWORK_SLEEP_TIME_MSECS;

    platformAlarmUpdateTimeout(&sleepTime);

    FD_ZERO(&readFDs);
    FD_ZERO(&writeFDs);
    FD_ZERO(&exceptFDs);

    if (SystemLayer.State() == System::kLayerState_Initialized)
        SystemLayer.PrepareSelect(numFDs, &readFDs, &writeFDs, &exceptFDs, sleepTime);

    platformUartUpdateFdSet(&readFDs, &writeFDs, &exceptFDs, &numFDs);
    platformUdpUpdateFdSet(aInstance, &readFDs, &numFDs);
    platformRadioUpdateFdSet(&readFDs, &writeFDs, &numFDs, &sleepTime);

    if (otTaskletsArePending(aInstance))
    {
        sleepTime.tv_sec  = 0;
        sleepTime.tv_usec = 0;
    }

    if (nl::Weave::DeviceLayer::InetLayer.State == InetLayer::kState_Initialized)
        nl::Weave::DeviceLayer::InetLayer.PrepareSelect(numFDs, &readFDs, &writeFDs, &exceptFDs, sleepTime);

    selectRes = select(numFDs, &readFDs, &writeFDs, &exceptFDs, &sleepTime);
    if (selectRes < 0)
    {
        printf("select failed: %s\n", nl::ErrorStr(System::MapErrorPOSIX(errno)));
        return;
    }

    if (SystemLayer.State() == System::kLayerState_Initialized)
    {
        SystemLayer.HandleSelectResult(selectRes, &readFDs, &writeFDs, &exceptFDs);
    }

    if (nl::Weave::DeviceLayer::InetLayer.State == InetLayer::kState_Initialized)
    {
        nl::Weave::DeviceLayer::InetLayer.HandleSelectResult(selectRes, &readFDs, &writeFDs, &exceptFDs);
    }

    platformRadioProcess(aInstance, &readFDs, &writeFDs);
    platformUartProcess(&readFDs, &writeFDs, &exceptFDs);
    platformAlarmProcess(aInstance);
    platformUdpProcess(aInstance, &readFDs);
}

// Dummy Platform implementations for Platform::PersistedStorage functions
namespace nl {
namespace Weave {
namespace Platform {
namespace PersistedStorage {

WEAVE_ERROR Read(const char *aKey, uint32_t &aValue)
{
    return WEAVE_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
}

WEAVE_ERROR Write(const char *aKey, uint32_t aValue)
{
    return WEAVE_NO_ERROR;
}

} // namespace PersistedStorage
} // namespace Platform
} // namespace Weave
} // namespace nl

jmp_buf gResetJump;

int main(int argc, char *argv[])
{
    WEAVE_ERROR err;
    otInstance *instance;

    if (setjmp(gResetJump))
    {
        alarm(0);
        execvp(argv[0], argv);
    }

    // Initialize OpenThread
    instance = otSysInit(argc, argv);
    otSysInitNetif(instance);
    otCliUartInit(instance);

    // Initialize the Weave stack.
    PlatformMgr().InitWeaveStack();

    gInstance = instance;
    while (true)
    {
        otTaskletsProcess(instance);
        DriveIO(instance);
    }

    otInstanceFinalize(instance);
    return 0;
}
