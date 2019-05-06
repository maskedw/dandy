/**
 *       @file  DStringUtils.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/05/31
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2018> <MaskedW [maskedw00@gmail.com]>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <dandy/core/utils/DStringUtils.hpp>


namespace {
    inline uint8_t D__NibbleFromChar(char c)
    {
        if(c >= '0' && c <= '9') return c - '0';
        if(c >= 'a' && c <= 'f') return c - 'a' + 10;
        if(c >= 'A' && c <= 'F') return c - 'A' + 10;
        return 255;
    }
    const char D__HexMap[] = "0123456789ABCDEF";
}


std::string DStringUtils::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const std::string result = DStringUtils::vformat(fmt, args);
    va_end(args);

    return result;
}

std::string DStringUtils::vformat(const char* fmt, va_list args)
{
    va_list args2;
    va_copy(args2, args);

    const int len = x_vsnprintf(NULL, 0, fmt, args2);
    va_end(args2);

    if (len < 0)
        return std::string("");

    char* buffer = D_NEW(char[len + 1]);
    X_ASSERT(buffer);

    x_vsnprintf(buffer, len + 1, fmt, args);

    const std::string result(buffer);

    D_DELETE_ARRAY(buffer);

    return result;
}


std::string DStringUtils::bytesToHex(const void* bytes, size_t len)
{
    if (len == 0)
        return std::string();

    char* buffer = D_NEW(char[len * 2 + 1]);
    X_ASSERT(buffer);

    const uint8_t* p = static_cast<const uint8_t*>(bytes);
    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        buffer[j++] = D__HexMap[(p[i] >> 4) & 0x0F];
        buffer[j++] = D__HexMap[(p[i]) & 0x0F];
    }
    buffer[j] = '\0';

    const std::string result(buffer);

    D_DELETE_ARRAY(buffer);

    return result;
}


void* DStringUtils::hexToBytes(const char* hex, void* dst)
{
    const size_t len = ::strlen(hex) / 2;
    uint8_t* p = static_cast<uint8_t*>(dst);
    for (size_t i = 0; i < len; i++)
    {
        p[i] = D__NibbleFromChar(hex[0]) << 4 | D__NibbleFromChar(hex[1]);
        hex += 2;
    }

    return dst;
}
