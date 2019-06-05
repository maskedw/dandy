/**
 *       @file  DStreamUtils.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/06/05
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2019> <MaskedW [maskedw00@gmail.com]>
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


#include <dandy/core/utils/DStreamUtils.hpp>


int DStreamUtils::copy(DStream* dst, DStream* src)
{
    uint8_t buffer[512];
    for (;;)
    {
        ssize_t n_or_error = src->read(buffer, sizeof(buffer));

        if (n_or_error == 0)
            break;
        if (n_or_error < 0)
            return n_or_error;

        n_or_error = dst->write(buffer, sizeof(buffer));
        if (n_or_error == 0)
            return -ENOSPC;
        if (n_or_error < 0)
            return n_or_error;
    }
    dst->sync();

    return 0;
}


bool DStreamUtils::equal(DStream* s1, DStream* s2)
{
    for (;;)
    {
        const int c1 = s1->getc();
        const int c2 = s2->getc();

        if ((c1 == EOF) && (c2 == EOF))
            return true;
        if (c1 != c2)
            break;
    }
    return false;
}
