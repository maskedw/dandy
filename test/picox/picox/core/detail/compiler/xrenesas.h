/**
 *       @file  xrenesas.h
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2016/03/27
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2016> <MaskedW [maskedw00@gmail.com]>
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

#ifndef picox_core_detail_compiler_xrenesas_h_
#define picox_core_detail_compiler_xrenesas_h_


#if !defined(__RX) && !defined(__CA78K0R__) && !defined(__CCRL__)
    #error This compiler does not support
#endif


#if defined(__CCRL__)
    #define X_FUNC                  NULL
    #define X_COMPILER_NO_BOOL      (1)
    #define X_INLINE                static __inline
#endif


#if defined(__CA78K0R__)
    #define X_INLINE                static
    #define X_FUNC                  NULL
    #define X_COMPILER_NO_INLINE    (1)
    #define X_COMPILER_NO_LONGLONG  (1)
    #define X_COMPILER_NO_BOOL      (1)
    #define X_COMPILER_NO_STDINT    (1)
    #define X_COMPILER_NO_64BIT_INT (1)
#endif


#if defined(__RX)
    #define X_PACKED_PRE_BEGIN  _Pragma("pack")
    #define X_PACKED_POST_BEGIN
    #define X_PACKED_PRE_END
    #define X_PACKED_POST_END   _Pragma("unpack")
#endif


#endif /* picox_core_detail_compiler_xrenesas_h_ */
