/**
 *       @file  DCoreUtils.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/08/25
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

#ifndef dandy_DCoreUtils_hpp_
#define dandy_DCoreUtils_hpp_


#include <dandy/core/DCore.hpp>


template <typename T>
inline const T& d_min(const T& a, const T& b) { return (a < b) ? a : b; }

template <typename T>
inline const T& d_max(const T& a, const T& b) { return (a < b) ? b : a; }

template <typename T>
inline const T& d_bound(const T& min, const T& val, const T& max)
{ return d_max(min, d_min(max, val)); }

template <typename T>
inline T d_abs(const T& t) { return t >= 0 ? t : -t; }

inline int d_round(double d)
{ return d >= 0.0 ? int(d + 0.5) : int(d - double(int(d-1)) + 0.5) + int(d-1); }

inline int d_round(float d)
{ return d >= 0.0f ? int(d + 0.5f) : int(d - float(int(d-1)) + 0.5f) + int(d-1); }

inline int64_t d_round64(double d)
{ return d >= 0.0 ? int64_t(d + 0.5) : int64_t(d - double(int64_t(d-1)) + 0.5) + int64_t(d-1); }

inline int64_t d_round64(float d)
{ return d >= 0.0f ? int64_t(d + 0.5f) : int64_t(d - float(int64_t(d-1)) + 0.5f) + int64_t(d-1); }

static inline bool d_fuzzy_compare(double p1, double p2)
{
    return (d_abs(p1 - p2) * 1000000000000. <= X_MIN(d_abs(p1), d_abs(p2)));
}

static inline bool d_fuzzy_compare(float p1, float p2)
{
    return (d_abs(p1 - p2) * 100000.f <= X_MIN(d_abs(p1), d_abs(p2)));
}

static inline bool d_fuzzy_is_null(double d)
{
    return d_abs(d) <= 0.000000000001;
}

static inline bool d_fuzzy_is_null(float f)
{
    return d_abs(f) <= 0.00001f;
}

static inline bool d_is_null(double d)
{
    union U {
        double d;
        uint64_t u;
    };
    U val;
    val.d = d;
    return (val.u & X_UINT64_C(0x7fffffffffffffff)) == 0;
}

static inline bool d_is_null(float f)
{
    union U {
        float f;
        uint32_t u;
    };
    U val;
    val.f = f;
    return (val.u & 0x7fffffff) == 0;
}

#endif /* end of include guard: dandy_DCoreUtils_hpp_ */
