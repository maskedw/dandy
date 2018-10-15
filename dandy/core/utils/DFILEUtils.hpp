/**
 *       @file  DFILEUtils.hpp
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

#ifndef dandy_DFILEUtils_hpp_
#define dandy_DFILEUtils_hpp_


#include <dandy/core/DCore.hpp>


class DFILEUtils
{
public:
    static int put8(FILE* fp, uint8_t value);
    static int put16(FILE* fp, uint16_t value);
    static int put32(FILE* fp, uint32_t value);
    static int put16Little(FILE* fp, uint16_t value);
    static int put32Little(FILE* fp, uint32_t value);
    static int put16Big(FILE* fp, uint16_t value);
    static int put32Big(FILE* fp, uint32_t value);
    static int putString(FILE* fp, const char* str);
    static bool exists(const char* path);
};


#endif /* end of include guard: dandy_DFILEUtils_hpp_ */
