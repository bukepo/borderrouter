/*
 *    Copyright (c) 2020, The OpenThread Authors.
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

#ifndef OTBR_OPENTHREAD_CONFIG_H_
#define OTBR_OPENTHREAD_CONFIG_H_

#define OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE 1
#define OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE 1
#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE 1
#define OPENTHREAD_CONFIG_JOINER_ENABLE 1
#define OPENTHREAD_CONFIG_LOG_PLATFORM 1
#define OPENTHREAD_CONFIG_MAX_STATECHANGE_HANDLERS 2
#define OPENTHREAD_CONFIG_PLATFORM_INFO "OTBR-AGENT"
#define OPENTHREAD_CONFIG_PLATFORM_PLATFORM_NETIF 1
#define OPENTHREAD_CONFIG_PLATFORM_PLATFORM_UDP 1
#define OPENTHREAD_CONFIG_PLATFORM_RADIO_COEX_ENABLE 1
#define OPENTHREAD_CONFIG_UART_CLI_RAW 1
#define OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE 0
#define OPENTHREAD_POSIX_RCP_UART_ENABLE 1
#define OPENTHREAD_CONFIG_NCP_UART_ENABLE 1

#endif // define OTBR_OPENTHREAD_CONFIG_H_
