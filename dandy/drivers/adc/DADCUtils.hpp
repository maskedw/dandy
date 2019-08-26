/**
 *       @file  DADCUtils.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/07/27
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

#ifndef dandy_DADCUtils_hpp_
#define dandy_DADCUtils_hpp_


class DADCUtils
{
public:
    template <typename T>
    static uint16_t readU12(T* adc)
    {
        return (adc->read_u16() >> 4) & 0x0FFF;
    }

    template <typename T, int N=10>
    static uint16_t medianFilteredReadU16(T* adc)
    {
        static_assert(N > 0, "required N > 0");
        uint16_t values[N];
        for (int i = 0; i < N; i++)
            values[i] = adc->read_u16();
        std::sort(values, values + N);
        const uint16_t center = values[N / 2];
        return center;
    }

    template <typename T, int N=10>
    static uint16_t medianFilteredReadU12(T* adc)
    {
        static_assert(N > 0, "required N > 0");
        uint16_t values[N];
        for (int i = 0; i < N; i++)
            values[i] = DADCUtils::readU12(adc);
        std::sort(values, values + N);
        const uint16_t center = values[N / 2];
        return center;
    }

    template <typename T, int N=10>
    static float medianFilteredRead(T* adc)
    {
        static_assert(N > 0, "required N > 0");
        float values[N];
        for (int i = 0; i < N; i++)
            values[i] = adc->read();
        std::sort(values, values + N);
        const float center = values[N / 2];
        return center;
    }
};


#endif /* end of include guard: dandy_DADCUtils_hpp_ */
