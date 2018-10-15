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

    const int len = vsnprintf(NULL, 0, fmt, args2);
    va_end(args2);

    if (len < 0)
        return std::string("");

    char* buffer = D_NEW(char[len + 1]);
    X_ASSERT(buffer);

    vsnprintf(buffer, len + 1, fmt, args);

    const std::string result(buffer);

    D_DELETE_ARRAY(buffer);

    return result;
}
