/**
 *       @file  DRef.hpp
 *      @brief  参照カウントを持つクラスのベースクラスです。
 *
 *    @details
 *    @note
 *      リソースに余裕があるならスマートポインタでいいんですが、そうではない環境
 *      を想定しているので、なんやかんやで手動の参照カウントの方が有利な場面が多
 *      いのです。
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2014/08/23
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2014> <MaskedW [maskedw00@gmail.com]>
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

#ifndef dandy_DRef_hpp_
#define dandy_DRef_hpp_


#include <dandy/core/DRTTI.hpp>



/** nullチェック付きのretainです。
 */
#define D_SAFE_RETAIN(p)  do { if(p) { (p)->retain(); } } while (0)


/** nullチェック + 最後にnull代入を行うreleaseです。
 */
#define D_SAFE_RELEASE(p) do { if(p) { (p)->release(); (p) = 0; } } while(0)


/** tag()の初期値です。
 */
static const int D_INVALID_TAG = -1;


/** 参照カウントを持つ基底クラスです。
 */
class DRef
{
public:
    D_DECLARE_RTTI;


    DRef();

    virtual ~DRef();

    /** 参照カウントを1つ加算します。
     */
    void retain();


    /** 参照カウントを1つ減算します。
     */
    void release();


    /** オブジェクトの名前を返します。
     */
    const char* name() const { return m_name; }


    /** オブジェクトの名前を設定します。
     */
    void set_name(const char* name);


    /** オブジェクトの識別子を返します。
     */
    int tag() const { return m_tag; }


    /** オブジェクトの識別子を設定します。
     */
    void set_tag(int tag) { m_tag = tag; }


    /** 現在の参照カウント値を返します。
     */
    int use_count() const { return m_use_count; }

private:
    int     m_tag;
    int     m_use_count;
    char*   m_name;
};


#endif // maskedw_core_DRef_hpp_
