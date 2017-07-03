/*
 *    Copyright (c) 2017, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRouterAdvertiserNTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRouterAdvertiserNTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRouterAdvertiserCT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements definition for RouterAdvertiser service.
 */

#include "router_advertiser.hpp"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common/code_utils.hpp"
#include "common/logging.hpp"

namespace ot {

namespace BorderRouter {

static const char kRadvdThreadConf[] = "/etc/radvd.d/thread.conf";

/**
 * RLOC16 consts.
 */
enum
{
    kRloc16Invalid = 0xfffe, ///< Invalid RLOC16.
};

RouterAdvertiser::RouterAdvertiser(const char *aThreadIfName, Ncp::Controller *aNcp) :
    mNcp(aNcp),
    mRloc16(kRloc16Invalid)
{
    strncpy(mThreadIfName, aThreadIfName, sizeof(mThreadIfName));
    mThreadIfName[sizeof(mThreadIfName) - 1] = '\0';
}

otbrError RouterAdvertiser::Start(void)
{
    otbrError ret = OTBR_ERROR_NONE;

    otbrLog(OTBR_LOG_INFO, "Starting router advertiser...");
    mNcp->On(Ncp::kEventRloc16, HandleNcpEvent, this);
    mNcp->On(Ncp::kEventMeshLocalAddress, HandleNcpEvent, this);
    SuccessOrExit(ret = mNcp->RequestEvent(Ncp::kEventRloc16));

    if (mRloc16 != kRloc16Invalid)
    {
        SuccessOrExit(ret = mNcp->RequestEvent(Ncp::kEventMeshLocalAddress));
    }

exit:
    return ret;
}

void RouterAdvertiser::Stop(void)
{
    mNcp->Off(Ncp::kEventMeshLocalAddress, HandleNcpEvent, this);
    mNcp->Off(Ncp::kEventRloc16, HandleNcpEvent, this);

    Disable();
}

void RouterAdvertiser::Disable(void)
{
    // Empty this file to disable.
    FILE *file = NULL;

    VerifyOrExit((file = fopen(kRadvdThreadConf, "w")) != NULL,
                 otbrLog(OTBR_LOG_ERR, "Failed to open radvd configuration file for thread: %s!",
                         otbrErrorString(OTBR_ERROR_ERRNO)));
    fclose(file);

exit:
    return;
}

void RouterAdvertiser::HandleNcpEvent(void *aContext, int aEvent, va_list aArguments)
{
    switch (aEvent)
    {
    case Ncp::kEventMeshLocalAddress:
    {
        uint8_t *addr = va_arg(aArguments, uint8_t *);
        static_cast<RouterAdvertiser *>(aContext)->HandleMeshLocalAddress(addr);
        break;
    }

    case Ncp::kEventRloc16:
    {
        uint16_t rloc16 = va_arg(aArguments, unsigned int);
        static_cast<RouterAdvertiser *>(aContext)->HandleRloc16(rloc16);
        break;
    }

    default:
        assert(false);
        break;
    }
}

void RouterAdvertiser::HandleRloc16(uint16_t aRloc16)
{
    if (aRloc16 == kRloc16Invalid)
    {
        Disable();
        mRloc16 = aRloc16;
    }
    else if (mRloc16 == kRloc16Invalid)
    {
        mRloc16 = aRloc16;
        // TODO Better to make this request delayed.
        mNcp->RequestEvent(Ncp::kEventMeshLocalAddress);
    }
}

void RouterAdvertiser::HandleMeshLocalAddress(const uint8_t *aAddress)
{
    otbrError error = OTBR_ERROR_ERRNO;
    char      ip6str[INET6_ADDRSTRLEN];
    FILE     *file = NULL;
    in6_addr  prefix;

    VerifyOrExit(mRloc16 != kRloc16Invalid, errno = EAGAIN);

    memset(prefix.s6_addr, 0, sizeof(prefix));
    memcpy(prefix.s6_addr, aAddress, 8);

    VerifyOrExit(inet_ntop(AF_INET6, prefix.s6_addr, ip6str, sizeof(ip6str)) != NULL);
    otbrLog(OTBR_LOG_INFO, "Thread mesh local prefix changed: %s.", ip6str);

    file = fopen(kRadvdThreadConf, "w");
    VerifyOrExit(file != NULL);

    fprintf(file,
            "interface %s {\n"
            "    AdvSendAdvert on;\n"
            "    route %s/64\n"
            "    {\n"
            "    };\n"
            "};\n", mThreadIfName, ip6str);

    fclose(file);

    error = OTBR_ERROR_NONE;

exit:
    if (error)
    {
        otbrLog(OTBR_LOG_ERR, "Failed to update route information: %s!", otbrErrorString(error));
    }
}

} // namespace BorderRouter

} // namespace ot
