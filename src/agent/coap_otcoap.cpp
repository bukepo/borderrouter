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
    assert(false);
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
    if (mMessage)
    {
        otMessageFree(mMessage);
        mMessage = NULL;
    }
}

void MessageOtCoap::SetPath(const char *aPath)
{
    if (otCoapHeaderAppendUriPathOptions(&mHeader, aPath))
    {
        assert(false);
    }
}

void MessageOtCoap::SetPayload(const uint8_t *aPayload, uint16_t aLength)
{
    otCoapHeaderSetPayloadMarker(&mHeader);
    mMessage = otCoapNewMessage(NULL, &mHeader);
    otMessageAppend(aPayload, aLength);
}

const uint8_t *MessageOtCoap::GetPayload(uint16_t &aLength) const
{
    // TODO read
    return NULL;
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
        otCoapSendRequest(aMessage);
    }
    else
    {
        otCoapSendResponse(aMessage);
    }

    ret = OTBR_ERROR_NONE;

exit:
    if (ret != OTBR_ERROR_NONE)
    {
        otbrLog(OTBR_LOG_ERR, "CoAP no memory for callback!");
        message.Free();
    }

    return ret;
}

void AgentOtCoap::HandleRequest(coap_context_t *        aCoap,
                                struct coap_resource_t *aResource,
                                const coap_endpoint_t * aEndPoint,
                                coap_address_t *        aAddress,
                                coap_pdu_t *            aRequest,
                                str *                   aToken,
                                coap_pdu_t *            aResponse)
{
    AgentOtCoap *agent = reinterpret_cast<AgentOtCoap *>(CONTAINING_RECORD(aCoap, AgentOtCoap, mCoap));

    agent->HandleRequest(aResource, aRequest, aResponse, aAddress->addr.sin6.sin6_addr.s6_addr,
                         ntohs(aAddress->addr.sin6.sin6_port));

    (void)aEndPoint;
    (void)aToken;
}

void AgentOtCoap::HandleRequest(struct coap_resource_t *aResource,
                                coap_pdu_t *            aRequest,
                                coap_pdu_t *            aResponse,
                                const uint8_t *         aAddress,
                                uint16_t                aPort)
{
    VerifyOrExit(mResources.count(aResource), otbrLog(OTBR_LOG_WARNING, "CoAP received unexpected request!"));

    {
        const Resource &resource = *mResources[aResource];
        MessageOtCoap   req(aRequest);
        MessageOtCoap   res(aResponse);

        assert(!strncmp(reinterpret_cast<const char *>(aResource->uri.s), resource.mPath, aResource->uri.length));

        // Set code to kCoapEmpty to use separate response if no response set by handler.
        // Handler should later respond an Non-ACK response.
        res.SetCode(kCodeEmpty);
        resource.mHandler(resource, req, res, aAddress, aPort, resource.mContext);
    }

exit:
    return;
}

void AgentOtCoap::Input(const void *aBuffer, uint16_t aLength, const uint8_t *aIp6, uint16_t aPort)
{
    mPacket.length    = aLength;
    mPacket.interface = mCoap.endpoint;
    mPacket.dst       = mCoap.endpoint->addr;
    // memset(mPacket.dst.addr.sin6.sin6_addr.s6_addr, 0, sizeof(mPacket.dst.addr.sin6.sin6_addr));
    CoapAddressInit(mPacket.src, aIp6, aPort);
    memcpy(mPacket.payload, aBuffer, aLength);
    coap_handle_message(&mCoap, &mPacket);
}

void AgentOtCoap::HandleResponse(coap_context_t *       aCoap,
                                 const coap_endpoint_t *aLocalInterface,
                                 const coap_address_t * aRemote,
                                 coap_pdu_t *           aSent,
                                 coap_pdu_t *           aReceived,
                                 const coap_tid_t       aId)
{
    MessageMeta meta;

    VerifyOrExit(aSent != NULL, otbrLog(OTBR_LOG_ERR, "request not found!"));

    memcpy(&meta, aSent->hdr + aSent->length, sizeof(meta));

    if (meta.mHandler)
    {
        MessageOtCoap message(aReceived);
        meta.mHandler(message, meta.mContext);
    }

exit:
    (void)aCoap;
    (void)aLocalInterface;
    (void)aRemote;
    (void)aId;

    return;
}

otbrError AgentOtCoap::AddResource(const Resource &aResource)
{
    otbrError        ret      = OTBR_ERROR_ERRNO;
    coap_resource_t *resource = NULL;

    for (Resources::iterator it = mResources.begin(); it != mResources.end(); ++it)
    {
        if (it->second == &aResource)
        {
            otbrLog(OTBR_LOG_ERR, "CoAP resource already added!");
            ExitNow(errno = EEXIST);
            break;
        }
    }

    resource = coap_resource_init(reinterpret_cast<const unsigned char *>(aResource.mPath), strlen(aResource.mPath), 0);
    coap_register_handler(resource, COAP_REQUEST_POST, AgentOtCoap::HandleRequest);
    coap_add_resource(&mCoap, resource);
    mResources[resource] = &aResource;
    ret                  = OTBR_ERROR_NONE;

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
    coap_clock_init();

    time_t clock_offset = time(NULL);
    memset(&mCoap, 0, sizeof(mCoap));
    prng_init(reinterpret_cast<unsigned long>(aNetworkSender) ^ static_cast<unsigned long>(clock_offset));
    prng(reinterpret_cast<unsigned char *>(&mCoap.message_id), sizeof(unsigned short));

    coap_address_t addr;
    coap_address_init(&addr);
    addr.addr.sin6.sin6_family = AF_INET6;
    mCoap.endpoint             = coap_new_endpoint(&addr, COAP_ENDPOINT_NOSEC);
    mCoap.network_send         = AgentOtCoap::NetworkSend;

    coap_register_response_handler(&mCoap, AgentOtCoap::HandleResponse);
}

ssize_t AgentOtCoap::NetworkSend(coap_context_t *       aCoap,
                                 const coap_endpoint_t *aLocalInterface,
                                 const coap_address_t * aDestination,
                                 unsigned char *        aBuffer,
                                 size_t                 aLength)
{
    (void)aLocalInterface;

    AgentOtCoap *agent = static_cast<AgentOtCoap *>(CONTAINING_RECORD(aCoap, AgentOtCoap, mCoap));

    return agent->mNetworkSender(aBuffer, static_cast<uint16_t>(aLength),
                                 reinterpret_cast<const uint8_t *>(&aDestination->addr.sin6.sin6_addr),
                                 ntohs(aDestination->addr.sin6.sin6_port), agent->mContext);
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
