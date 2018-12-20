/**
 *       @file  DRGB.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/09/26
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


#ifndef dandy_DRGB_hpp_
#define dandy_DRGB_hpp_


#include <dandy/core/DCore.hpp>


/** 32bitのRGB型です。
 */
typedef uint32_t DRGB;


static inline DRGB d_red(DRGB rgb)
{
    return ((rgb >> 16) & 0xff);
}


static inline DRGB d_green(DRGB rgb)
{ return ((rgb >> 8) & 0xff);
}


static inline DRGB d_blue(DRGB rgb)
{
    return (rgb & 0xff);
}

static inline DRGB d_alpha(DRGB rgb)
{
    return ((rgb >> 24) & 0xff);
}

static inline DRGB d_rgb(DRGB r, DRGB g, DRGB b)
{
    return (static_cast<DRGB>(0xff) << 24) |
           ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

static inline DRGB d_rgba(DRGB r, DRGB g, DRGB b, DRGB a)
{
    return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

static inline DRGB d_gray(DRGB r, DRGB g, DRGB b)
{
    return (r*11+g*16+b*5)/32;
}


static inline DRGB d_gray(DRGB rgb)
{
    return d_gray(d_red(rgb), d_green(rgb), d_blue(rgb));
}


static inline bool d_is_gray(DRGB rgb)
{
    return d_red(rgb) == d_green(rgb) && d_red(rgb) == d_blue(rgb);
}


static inline uint32_t d_to_rgb565(DRGB rgb)
{
    const uint_fast16_t r = d_red(rgb);
    const uint_fast16_t g = d_green(rgb);
    const uint_fast16_t b = d_blue(rgb);
    const uint_fast16_t rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    return rgb565;
}


static inline uint32_t d_to_argb1555(DRGB rgb)
{
    const uint_fast16_t a = d_alpha(rgb);
    const uint_fast16_t r = d_red(rgb);
    const uint_fast16_t g = d_green(rgb);
    const uint_fast16_t b = d_blue(rgb);
    const uint_fast16_t argb1555 = ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);

    /* bit7がセットされていたらA1をセットするというルールにする */
    return (a & 80) ? (1UL << 15) | argb1555 : argb1555;
}


static inline uint32_t d_to_rgba5551(DRGB rgb)
{
    const uint_fast16_t a = d_alpha(rgb);
    const uint_fast16_t r = d_red(rgb);
    const uint_fast16_t g = d_green(rgb);
    const uint_fast16_t b = d_blue(rgb);
    const uint_fast16_t argb5551 = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1);

    return (a & 80) ? (1UL << 0) | argb5551 : argb5551;
}


static inline uint32_t d_to_argb8888(DRGB rgb)
{
    const uint_fast32_t a = d_alpha(rgb);
    const uint_fast32_t r = d_red(rgb);
    const uint_fast32_t g = d_green(rgb);
    const uint_fast32_t b = d_blue(rgb);
    const uint_fast32_t argb8888 = (a << 24) | (r << 16) | (g << 8) | (b << 0);

    return argb8888;
}


static inline uint32_t d_to_rgba8888(DRGB rgb)
{
    const uint_fast32_t a = d_alpha(rgb);
    const uint_fast32_t r = d_red(rgb);
    const uint_fast32_t g = d_green(rgb);
    const uint_fast32_t b = d_blue(rgb);
    const uint_fast32_t rgba8888 = (r << 24) | (g << 16) | (b << 8) | (a << 0);

    return rgba8888;
}


static inline uint32_t d_rgb565_red(uint32_t rgb565)
{
    return (rgb565 >> 11) & 0x1F;
}


static inline uint32_t d_rgb565_green(uint32_t rgb565)
{
    return (rgb565 >> 5) & 0x3F;
}


static inline uint32_t d_rgb565_blue(uint32_t rgb565)
{
    return (rgb565) & 0x1F;
}


static inline uint32_t d_argb1555_red(uint32_t argb1555)
{
    return (argb1555 >> 11) & 0x1F;
}


static inline uint32_t d_argb1555_green(uint32_t argb1555)
{
    return (argb1555 >> 5) & 0x1F;
}


static inline uint32_t d_argb1555_blue(uint32_t argb1555)
{
    return (argb1555) & 0x1F;
}


static inline uint32_t d_argb1555_alpha(uint32_t argb1555)
{
    return (argb1555) & 0x1F;
}


static inline uint32_t d_rgba8888_alpha(uint32_t rgba8888)
{
    return (rgba8888 & 0xFF);
}


static inline uint32_t d_rgba8888_red(uint32_t rgba8888)
{
    return (rgba8888 >> 24) & 0xFF;
}


static inline uint32_t d_rgba8888_green(uint32_t rgba8888)
{
    return (rgba8888 >> 16) & 0xFF;
}


static inline uint32_t d_rgba8888_blue(uint32_t rgba8888)
{
    return (rgba8888 >> 8) & 0x1F;
}


static inline uint32_t d_rgb565_to_argb8888(uint32_t rgb565)
{
    const uint_fast32_t r = d_rgb565_red(rgb565);
    const uint_fast32_t g = d_rgb565_green(rgb565);
    const uint_fast32_t b = d_rgb565_blue(rgb565);

    return (r << 19) | ((r << 14) & 0x00070000) |
           (g << 10) | ((g <<  4) & 0x00000300) |
           (b <<  3) | ((b >>  2) & 0x00000007) | 0xFF000000;
}

static inline uint32_t d_rgb565_to_rgba8888(uint32_t rgb565)
{
    const uint_fast32_t r = d_rgb565_red(rgb565);
    const uint_fast32_t g = d_rgb565_green(rgb565);
    const uint_fast32_t b = d_rgb565_blue(rgb565);

    return (r << 27) | ((r << 22) & 0x07000000) |
           (g << 18) | ((g << 12) & 0x00030000) |
           (b << 11) | ((b <<  6) & 0x00000700) | 0xFF;
}


static inline uint32_t d_rgb565_to_argb1555(uint32_t rgb565)
{
    const uint_fast16_t r = d_rgb565_red(rgb565);
    const uint_fast16_t g = d_rgb565_green(rgb565) >> 1;
    const uint_fast16_t b = d_rgb565_blue(rgb565);

    return 0x80 | (r <<  10) | (g << 5) | b;
}


static inline uint32_t d_argb1555_to_argb8888(uint32_t argb1555)
{
    const uint_fast32_t a = d_argb1555_alpha(argb1555) ? 0xFF000000 : 0x00;
    const uint_fast32_t r = d_argb1555_red(argb1555);
    const uint_fast32_t g = d_argb1555_green(argb1555);
    const uint_fast32_t b = d_argb1555_blue(argb1555);

    return (r << 19) | ((r << 14) & 0x00070000) |
           (g << 11) | ((g <<  6) & 0x00000700) |
           (b <<  3) | ((b >>  2) & 0x00000007) | a;
}

static inline uint32_t d_argb1555_to_rgba8888(uint32_t argb1555)
{
    const uint_fast32_t a = d_argb1555_alpha(argb1555) ? 0xFF : 0x00;
    const uint_fast32_t r = d_argb1555_red(argb1555);
    const uint_fast32_t g = d_argb1555_green(argb1555);
    const uint_fast32_t b = d_argb1555_blue(argb1555);

    return (r << 27) | ((r << 22) & 0x07000000) |
           (g << 19) | ((g << 14) & 0x00070000) |
           (b << 11) | ((b <<  6) & 0x00000700) | a;
}


static inline uint32_t d_argb1555_to_rgb565(uint32_t argb1555)
{
    const uint_fast16_t r = d_argb1555_red(argb1555);
    const uint_fast16_t g = d_argb1555_green(argb1555);
    const uint_fast16_t b = d_argb1555_blue(argb1555);

    return (r << 11) | ((g << 6) | ((g >> 4) & 0x01)) | b;
}


static inline uint32_t d_argb8888_to_rgba8888(uint32_t argb8888)
{
    return d_to_rgba8888(argb8888);
}


static inline uint32_t d_argb8888_to_rgb565(uint32_t argb8888)
{
    return d_to_rgb565(argb8888);
}


static inline uint32_t d_argb8888_to_argb1555(uint32_t argb8888)
{
    return d_to_argb1555(argb8888);
}


static inline uint32_t d_rgba8888_to_argb8888(uint32_t rgba8888)
{
    return ((rgba8888 >> 8) & 0x00FFFFFF) |
           ((rgba8888 << 24) & 0xFF000000);
}


static inline uint32_t d_rgba8888_to_rgb565(uint32_t rgba8888)
{
    const uint_fast16_t r = d_rgba8888_red(rgba8888) >> 3;
    const uint_fast16_t g = d_rgba8888_green(rgba8888) >> 2;
    const uint_fast16_t b = d_rgba8888_blue(rgba8888) >> 3;

    return (r << 11) | (g << 5) | b;
}


static inline uint32_t d_rgba8888_to_argb1555(uint32_t rgba8888)
{
    const uint_fast16_t a = d_rgba8888_alpha(rgba8888);
    const uint_fast16_t r = d_rgba8888_red(rgba8888) >> 3;
    const uint_fast16_t g = d_rgba8888_green(rgba8888) >> 3;
    const uint_fast16_t b = d_rgba8888_blue(rgba8888) >> 3;
    const uint_fast16_t argb1555 = (r << 10) | (g << 5) | (b);
    return (a & 0x80) ? (1U << 15) | argb1555 : argb1555;
}


#define D_RGB_WHITE         (0xffffffff)
#define D_RGB_BLACK         (0xff000000)
#define D_RGB_RED           (0xffff0000)
#define D_RGB_DARK_RED      (0xff800000)
#define D_RGB_GREEN         (0xff00ff00)
#define D_RGB_DARK_GREEN    (0xff008000)
#define D_RGB_BLUE          (0xff0000ff)
#define D_RGB_DARK_BLUE     (0xff000080)
#define D_RGB_CYAN          (0xff00ffff)
#define D_RGB_DARK_SYAN     (0xff008080)
#define D_RGB_MAGENTA       (0xffff00ff)
#define D_RGB_DARK_MAGENTA  (0xff800080)
#define D_RGB_YELLOW        (0xffffff00)
#define D_RGB_DARK_YELLOW   (0xff808000)
#define D_RGB_GRAY          (0xffa0a0a4)
#define D_RGB_DARK_GRAY     (0xff808080)
#define D_RGB_LIGHT_GRAY    (0xffc0c0c0)
#define D_RGB_TRANSPARENT   (0x00000000)


#endif /* end of include guard: dandy_DRGB_hpp_ */
