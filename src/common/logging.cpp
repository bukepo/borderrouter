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

#include "logging.hpp"
#include "code_utils.hpp"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <syslog.h>

#include "time.hpp"

static int        sLevel      = LOG_INFO;
static const char kHexChars[] = "0123456789abcdef";

static unsigned long sMsecsStart;
static FILE *        sLogFp         = nullptr;
static bool          sSyslogEnabled = true;
static bool          sSyslogOpened  = false;

#define LOGFLAG_syslog 1
#define LOGFLAG_file 2

/** Set/Clear syslog enable flag */
void otbrLogEnableSyslog(bool aEnable)
{
    sSyslogEnabled = aEnable;
}

/** Enable logging to a specific file */
void otbrLogSetFilename(const char *aFilename)
{
    if (sLogFp)
    {
        fclose(sLogFp);
        sLogFp = NULL;
    }
    sLogFp = fopen(aFilename, "w");
    if (sLogFp == NULL)
    {
        fprintf(stderr, "Cannot open log file %s: %s\n", aFilename, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/** Get the current debug log level */
int otbrLogGetLevel(void)
{
    return sLevel;
}

/** Set the debug log level */
void otbrLogSetLevel(int aLevel)
{
    assert(aLevel >= LOG_EMERG && aLevel <= LOG_DEBUG);
    sLevel = aLevel;
}

/** Determine if we should not or not log, and if so where to */
static int LogCheck(int aLevel)
{
    int flags = 0;

    assert(aLevel >= LOG_EMERG && aLevel <= LOG_DEBUG);

    if (sSyslogOpened && sSyslogEnabled && (aLevel <= sLevel))
    {
        flags = flags | LOGFLAG_syslog;
    }

    /* if someone has turned on the seperate file the most likely
     * situation is they are debugging a problem, or need a extra
     * information. In this case, we do not test the log level.
     */
    if (sLogFp != NULL)
    {
        flags = flags | LOGFLAG_file;
    }
    return flags;
}

/** return the time, in milliseconds since application start */
static unsigned long GetMsecsNow(void)
{
    unsigned long now = ot::BorderRouter::GetNow();

    now -= sMsecsStart;
    return now;
}

/** Write this string to the private log file, inserting the timestamp at column 0 */
static void LogString(const char *aString)
{
    static bool sLogCol0 = true; /* we start at col0 */

    int ch;

    while ((ch = *aString++) != 0)
    {
        if (sLogCol0)
        {
            unsigned long now = GetMsecsNow();

            sLogCol0 = false;
            fprintf(sLogFp, "%4lu.%03lu | ", (now / 1000), (now % 1000));
        }

        fputc(ch, sLogFp);

        if (ch == '\n')
        {
            sLogCol0 = true;
            /* force flush (in case something crashes) */
            fflush(sLogFp);
        }
    }
}

/** Print to the private log file */
static void LogVprintf(const char *aFormat, va_list aArguments)
{
    char buf[1024];

    vsnprintf(buf, sizeof(buf), aFormat, aArguments);

    LogString(buf);
}

/** Print to the private log file */
static void LogPrintf(const char *aFormat, ...)
{
    va_list aArguments;

    va_start(aArguments, aFormat);
    LogVprintf(aFormat, aArguments);
    va_end(aArguments);
}

/** Initialize logging */
void otbrLogInit(const char *aIdent, int aLevel, bool aPrintStderr)
{
    assert(aIdent);
    assert(aLevel >= LOG_EMERG && aLevel <= LOG_DEBUG);

    sMsecsStart = ot::BorderRouter::GetNow();

    /* only open the syslog once... */
    if (!sSyslogOpened)
    {
        sSyslogOpened = true;
        openlog(aIdent, (LOG_CONS | LOG_PID) | (aPrintStderr ? LOG_PERROR : 0), LOG_USER);
    }
    sLevel = aLevel;
}

/** log to the syslog or log file */
void otbrLog(int aLevel, const char *aFormat, ...)
{
    va_list aArguments;

    va_start(aArguments, aFormat);
    otbrLogv(aLevel, aFormat, aArguments);
    va_end(aArguments);
}

/** log to the syslog or log file */
void otbrLogv(int aLevel, const char *aFormat, va_list aArguments)
{
    int flags = LogCheck(aLevel);

    assert(aFormat);

    if (flags & LOGFLAG_file)
    {
        va_list cpy;
        va_copy(cpy, aArguments);
        LogVprintf(aFormat, cpy);
        va_end(cpy);

        /* logs do not end with a NEWLINE, we add one here */
        LogString("\n");
    }

    if (flags & LOGFLAG_syslog)
    {
        vsyslog(aLevel, aFormat, aArguments);
    }
}

/** Hex dump data to the log */
void otbrDump(int aLevel, const char *aPrefix, const void *aMemory, size_t aSize)
{
    int addr;
    int flags;

    assert(aPrefix && (aMemory || aSize == 0));

    VerifyOrExit((flags = LogCheck(aLevel)) != 0);

    /* break hex dumps into 16byte lines
     * In the form ADDR: XX XX XX XX ...
     */

    // we pre-increment... so subtract
    addr = -16;

    while (aSize > 0)
    {
        size_t this_size;
        char   hex[16 * 3 + 1];

        addr              = addr + 16;
        const uint8_t *p8 = (const uint8_t *)(aMemory) + addr;

        /* truncate line to max 16 bytes */
        this_size = aSize;
        if (this_size > 16)
        {
            this_size = 16;
        }
        aSize = aSize - this_size;

        char *ch = hex - 1;

        for (const uint8_t *pEnd = p8 + this_size; p8 < pEnd; p8++)
        {
            *++ch = kHexChars[(*p8) >> 4];
            *++ch = kHexChars[(*p8) & 0x0f];
            *++ch = ' ';
        }
        *ch = 0;

        if (flags & LOGFLAG_syslog)
        {
            syslog(aLevel, "%s: %04x: %s", aPrefix, addr, hex);
        }
        if (flags & LOGFLAG_file)
        {
            LogPrintf("%s: %04x: %s\n", aPrefix, addr, hex);
        }
    }

exit:
    return;
}

const char *otbrErrorString(otbrError aError)
{
    const char *error = NULL;

    switch (aError)
    {
    case OTBR_ERROR_NONE:
        error = "OK";
        break;

    case OTBR_ERROR_ERRNO:
        error = strerror(errno);
        break;

    case OTBR_ERROR_DTLS:
        error = "DTLS error";
        break;

    case OTBR_ERROR_DBUS:
        error = "DBUS error";
        break;

    case OTBR_ERROR_MDNS:
        error = "MDNS error";
        break;

    default:
        assert(false);
    }

    return error;
}

void otbrLogResult(const char *aAction, otbrError aError)
{
    otbrLog((aError == OTBR_ERROR_NONE ? OTBR_LOG_INFO : OTBR_LOG_WARNING), "%s: %s", aAction, otbrErrorString(aError));
}

void otbrLogDeinit(void)
{
    sSyslogOpened = false;
    closelog();
}
