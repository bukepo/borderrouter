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
 *          Provides implementations for the Weave entropy sourcing functions
 *          on the ESP32 platform.
 */

#include <assert.h>
#include <stdio.h>

#include <Weave/DeviceLayer/internal/WeaveDeviceLayerInternal.h>
#include <Weave/Support/crypto/WeaveRNG.h>

using namespace ::nl;
using namespace ::nl::Weave;

namespace nl {
namespace Weave {
namespace DeviceLayer {
namespace Internal {

namespace {

int GetEntropy_ESP32(uint8_t *buf, size_t bufSize)
{
    FILE * file = NULL;
    size_t readLength;

    file = fopen("/dev/urandom", "rb");
    assert(file != NULL);

    readLength = fread(buf, 1, bufSize, file);
    assert(readLength == bufSize);

    fclose(file);
    return 0;
}

} // unnamed namespace

WEAVE_ERROR InitEntropy()
{
    WEAVE_ERROR  err;
    unsigned int seed;

    // Initialize the source used by Weave to get secure random data.
    err = ::nl::Weave::Platform::Security::InitSecureRandomDataSource(GetEntropy_ESP32, 64, NULL, 0);
    SuccessOrExit(err);

    // Seed the standard rand() pseudo-random generator with data from the secure random source.
    err = ::nl::Weave::Platform::Security::GetSecureRandomData((uint8_t *)&seed, sizeof(seed));
    SuccessOrExit(err);
    srand(seed);
    printf("srand seed set: %u", seed);

exit:
    if (err != WEAVE_NO_ERROR)
    {
        printf("InitEntropy() failed: %s", ErrorStr(err));
    }
    return err;
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace Weave
} // namespace nl
