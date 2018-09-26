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
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   The file implements the CoAP service.
 */

#include "coap_otcoap.hpp"

#include <errno.h>
#include <stdio.h>

#include "common/code_utils.hpp"
#include "common/logging.hpp"
#include "common/types.hpp"

namespace ot {

namespace BorderRouter {

namespace Coap {

static void CoapAddressInit(coap_address_t &aAddress, const uint8_t *aIp6, uint16_t aPort)
{
    coap_address_init(&aAddress);
    aAddress.addr.sin6.sin6_family = AF_INET6;
    aAddress.addr.sin6.sin6_port   = htons(aPort);

    VerifyOrExit(aIp6 != NULL);
    memcpy(&aAddress.addr.sin6.sin6_addr, aIp6, sizeof(aAddress.addr.sin6.sin6_addr));

exit:
    return;
}

MessageOtCoap::MessageOtCoap(Type aType, Code aCode, const uint8_t *aToken, uint8_t aTokenLength)
{
    otCoapHeaderInit(&mHeader, aType, aCode);
    otCoapHeaderSetToken(&mHeader, aToken, aTokenLength);
}

const uint8_t *MessageOtCoap::GetToken(uint8_t &aLength) const
{
    aLength = otCoapHeaderGetTokenLength(&mHeader);
    return otCoapHeaderGetToken(&mHeader);
}

Code MessageOtCoap::GetCode(void) const
{
    return otCoapHeaderGetCode(&mHeader);
}

void MessageOtCoap::SetCode(Code aCode)
{
    mHeader.mHeader.mCode = aCode;
}

Type MessageOtCoap::GetType(void) const
{
    return otCoapHeaderGetType(&mHeader);
}

void MessageOtCoap::SetType(Type aType)
{
    mPdu->hdr->type = aType;
}

void MessageOtCoap::SetToken(const uint8_t *aToken, uint8_t aLength)
{
    otCoapHeaderSetToken(&mHeader, aToken, aLength);
}

void MessageOtCoap::Free(void)
{
}

void MessageOtCoap::SetPath(const char *aPath)
{
    if (otCoapHeaderAppendUriPathOptions(&mHeader, aPath) != OT_ERROR_NONE)
    {
        assert(false);
    }
}

void MessageOtCoap::SetPayload(const uint8_t *aPayload, uint16_t aLength)
{
    mLength = aLength;
    memcpy(mPayload, aPayload, aLength);
}

const uint8_t *MessageOtCoap::GetPayload(uint16_t &aLength) const
{
    return mPayload;
}

otMessage *ToMessage(void) const
{
    otMessage *message = NULL;
    if (mLength > 0)
    {
        otCoapHeaderSetPayloadMarker(&mHeader);
        message = otCoapNewMessage(NULL, &mHeader);
        otMessageAppend(aPayload, aLength);
    }
    else
    {
        message = otCoapNewMessage(NULL, &mHeader);
    }

    return message;
}

Message *AgentOtCoap::NewMessage(Type aType, Code aCode, const uint8_t *aToken, uint8_t aTokenLength)
{
    return new MessageOtCoap(aType, aCode, aToken, aTokenLength);
}

void AgentOtCoap::FreeMessage(Message *aMessage)
{
    delete static_cast<Message *>(aMessage);
}

otbrError AgentOtCoap::Send(Message &       aMessage,
                            const uint8_t * aIp6,
                            uint16_t        aPort,
                            ResponseHandler aHandler,
                            void *          aContext)
{
    otbrError      ret     = OTBR_ERROR_ERRNO;
    MessageOtCoap &message = static_cast<MessageOtCoap &>(aMessage);

    if (aHandler)
    {
        ot_coap_send_request(message.GetMessage(), aIp6, aPort, aHandler, aContext);
    }
    else
    {
        ot_coap_send_response(message, aIp6, aPort);
    }

    ret = OTBR_ERROR_NONE;

exit:
    aMessage.Free();

    if (ret != OTBR_ERROR_NONE)
    {
        if (message != NULL)
        {
            otMessageFree(message);
        }
    }

    return ret;
}

otbrError AgentOtCoap::AddResource(const Resource &aResource)
{
    otbrError ret = OTBR_ERROR_ERRNO;

    for (Resources::iterator it = mResources.begin(); it != mResources.end(); ++it)
    {
        if (it->second == &aResource)
        {
            otbrLog(OTBR_LOG_ERR, "CoAP resource already added!");
            ExitNow(errno = EEXIST);
            break;
        }
    }

exit:
    return ret;
}

otbrError AgentOtCoap::RemoveResource(const Resource &aResource)
{
    otbrError ret = OTBR_ERROR_ERRNO;

    for (Resources::iterator it = mResources.begin(); it != mResources.end(); ++it)
    {
        if (it->second == &aResource)
        {
            coap_delete_resource(&mCoap, it->first->key);
            mResources.erase(it);
            ret = OTBR_ERROR_NONE;
            break;
        }
    }

    if (ret)
    {
        errno = ENOENT;
    }

    return ret;
}

AgentOtCoap::AgentOtCoap(NetworkSender aNetworkSender, void *aContext)
{
    mContext       = aContext;
    mNetworkSender = aNetworkSender;

    ot_coap_init();
}

AgentOtCoap::Input(const void *aBuffer, uint16_t aLength, const uint8_t *aIp6, uint16_t aPort)
{
    ot_coap_input(aBuffer, aLength, aIp6, aPort);
}

Agent *Agent::Create(NetworkSender aNetworkSender, void *aContext)
{
    return new AgentOtCoap(aNetworkSender, aContext);
}

void Agent::Destroy(Agent *aAgent)
{
    delete static_cast<AgentOtCoap *>(aAgent);
}

} // namespace Coap

} // namespace BorderRouter

} // namespace ot
