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
 *   This file includes definition for RouterAdvertiser service.
 */

#ifndef ROUTER_ADVERTISER_HPP_
#define ROUTER_ADVERTISER_HPP_

#include <net/if.h>

#include "common/types.hpp"
#include "ncp.hpp"

namespace ot {

namespace BorderRouter {

/**
 * @addtogroup border-router-router-advertiser
 *
 * @brief
 *   This module includes definition for RouterAdvertiser service.
 *
 * @{
 */

/**
 * This interface defines the functionality of RouterAdvertiser service.
 *
 */
class RouterAdvertiser
{
public:
    /**
     * The constructor to initialize the router advertiser service.
     *
     * @param[in]   aThreadIfName       The thread interface name.
     * @param[in]   aNcp                A pointer to the NCP controller.
     *
     */
    RouterAdvertiser(const char *aThreadIfName, Ncp::Controller *aNcp);

    ~RouterAdvertiser(void) {}

    /**
     * This method starts the router advertiser service.
     *
     * @retval OTBR_ERROR_NONE      Successfully started router advertiser service;
     * @retval OTBR_ERROR_ERRNO     Failed to start router advertiser service.
     *
     */
    otbrError Start(void);

    /**
     * This function handles NCP events.
     *
     * @param[in]   aContext        A pointer to application-specific context.
     * @param[in]   aEvent          The event happened.
     * @param[in]   aArguments      The event associated arguments.
     *
     */
    static void HandleNcpEvent(void *aContext, int aEvent, va_list aArguments);

    /**
     * This method stops the router advertiser service.
     *
     */
    void Stop(void);

private:
    void HandleMeshLocalAddress(const uint8_t *aAddress);
    void HandleRloc16(uint16_t aRloc16);
    void Disable(void);

    Ncp::Controller *mNcp;
    char             mThreadIfName[IFNAMSIZ];
    uint16_t         mRloc16;
};

/**
 * @}
 */

} // namespace BorderRouter

} // namespace ot

#endif  // ROUTER_ADVERTISER_HPP_
