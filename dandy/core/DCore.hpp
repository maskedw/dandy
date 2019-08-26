/**
 *       @file  DCore.hpp
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


#ifndef dandy_DCore_hpp_
#define dandy_DCore_hpp_


#include <picox/core/xcore.h>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <string>
#include <dandy/core/DPlatform.hpp>
#include <EABase/eabase.h>
#include <EABase/nullptr.h>


// C++11 constexpr
#if defined(EA_COMPILER_NO_CONSTEXPR)
    #if !defined(constexpr)
        #define constexpr
    #endif
#endif


// C++11 override
#if defined(EA_COMPILER_NO_OVERRIDE)
    #if !defined(override)
        #define override
    #endif
#endif


// C++11 final
#if defined(EA_COMPILER_NO_FINAL)
    #if !defined(final)
        #define final
    #endif
#endif


/** @def    D_THROW_EXCEPTION
 *  @brief  例外スローのラッパーです
 *
 *  マイコンファームウェアでは基本的に例外は使用しません
 */
#if !defined(D_THROW_EXCEPTION)
    #if defined(EA_COMPILER_NO_EXCEPTIONS)
        #define D_THROW_EXCEPTION(e) X_ASSERT(0 && (#e))
    #else
        #define D_THROW_EXCEPTION(e) throw e
    #endif
#endif


/** @def    D_NEW
 *  @brief  newのラッパーマクロです
 */
#ifndef D_NEW
    #define D_NEW(constructor) ::new constructor
#endif


/** @def    D_DELETE
 *  @brief  deleteのラッパーマクロです
 */
#ifndef D_DELETE
    #define D_DELETE(x)        delete x
#endif


/** @def    D_DELETE_ARRAY
 *  @brief  delete[]のラッパーマクロです。
 */
#ifndef D_DELETE_ARRAY
    #define D_DELETE_ARRAY(x)  delete[] x
#endif


/** ポインタ引数pをdeleteした後に0を代入します。
 */
#define D_SAFE_DELETE(p)       do { D_DELETE(p); (p) = 0; } while(0)


/** D_SAFE_DELETE()のdelete[]版です。
 */
#define D_SAFE_DELETE_ARRAY(p) do { if(p) { D_DELETE_ARRAY(p); (p) = 0; } } while(0)


/** イテレータを走査する時の糞長いfor文をちょっとだけ楽に書くためのマクロです。
 *
 *  @note
 *  BOOST_FOREACH()に比べて、イテレータの型を書いたり、イテレータをデリファレン
 *  スする必要があったり、配列には使えなかったりと不便はありますが、その分実装が
 *  超シンプルですみますし、機能もまあこれで十分でしょう。
 */
#define D_FOREACH(IteratorType, ite, container)          \
            for (IteratorType ite = (container).begin(), \
                              end = (container).end();   \
                              ite != end;                \
                              ++ite)

/** 逆走査版 D_FOREACH()です。
 */
#define D_RFOREACH(IteratorType, ite, container)          \
            for (IteratorType ite = (container).rbegin(), \
                              end = (container).rend();   \
                              ite != end;                 \
                              ++ite)

/** const版 D_FOREACHです
 */
#define D_CONST_FOREACH(IteratorType, ite, container)     \
            for (IteratorType ite = (container).cbegin(), \
                              end = (container).cend();   \
                              ite != end;                 \
                              ++ite)

/** const版 D_RFOREACHです
 */
#define D_CONST_RFOREACH(IteratorType, ite, container)    \
            for (IteratorType ite = (container).crbegin(), \
                              end = (container).crend();   \
                              ite != end;                  \
                              ++ite)
/// @cond IGNORE
namespace dandy_detail {
    // http://blogs.wankuma.com/melt/archive/2008/03/08/126828.aspx
    template <typename T, size_t DSize>
    char (*countof_helper(T (&array)[DSize]))[DSize];
}
/// @endcond IGNORE


/** 配列の要素数を返します。
 *
 *  @note
 *  配列の要素数を求める時に、sizeof(x) / sizeof(x[0])というのをよく使いますが、
 *  xがポインタ型である時に問題があります。D_COUNT_OF()ではxが配列型でない時はコ
 *  ンパイルエラーとなるので、より信頼性があります。
 *  @code
 *
 *  char foo[10];
 *  assert(D_COUNT_OF(foo) == 10);
 *  int
 *
 *  @endcode
 */
#define D_COUNT_OF(array) (sizeof(*dandy_detail::countof_helper(array)))


/// @cond IGNORE
namespace dandy_detail {
    // http://d.hatena.ne.jp/setuna-kanata/20100219/1266597128
    template <typename T>
    struct alignment_of_hack
    {
        char c;
        T t;
        alignment_of_hack();
    };
}
/// @endcond IGNORE


/** 型typeのアライメントをバイト数で返します。
 *
 * @code
 * size_t align_double = D_ALIGNMENT_OF(double);
 * @endcode
 */
#define D_ALIGNMENT_OF(type)  (sizeof(dandy_detail::alignment_of_hack<type>) - sizeof(type))


union DMaxAlign
{
    char c; int i; long l; long long ll; void* p;
    float f; double d; long double ld; void(*fp)(void);
    struct dummy {};
    int dummy::*mp; void (dummy::*mfp)();
};


/** classインスタンスのコピーと代入を禁止する時に使用します
 *
 *  @note
 *  C++でclassのコピーと代入を禁止したい時はコピーコンストラ
 *  クタ代入演算子の宣言だけを行い、定義はしないことで、誤っ
 *  たコピーをコンパイルエラーで検出できます。
 *
 *  @code
 *  class Foo
 *  {
 *      D_DISALLOW_COPY_AND_ASSIGN(Foo);
 *  };
 *  Foo a;
 *  Foo b;
 *  a = b;      // コンパイルエラー!!
 *  Foo c(a)    // コンパイルエラー!!
 *  @endcode
 */
#define D_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);              \
    TypeName &operator =(const TypeName &)


/** QtのQ_DECLARE_PRIVATE()と同じです
 *  @see https://wiki.qt.io/D-Pointer
 */
#define D_DECLARE_PRIVATE(ClassName)                                        \
    inline ClassName##Private* d_func()                                     \
    {                                                                       \
        return reinterpret_cast<ClassName##Private*>(d_ptr);                \
    }                                                                       \
    inline const ClassName##Private* d_func() const                         \
    {                                                                       \
        return reinterpret_cast<const ClassName##Private*>(d_ptr);          \
    }                                                                       \
    friend class        ClassName##Private;                                 \
    ClassName##Private* d_ptr;


/** QtのQ_DECLARE_PUBLIC()と同じです
 *  @see https://wiki.qt.io/D-Pointer
 */
#define D_DECLARE_PUBLIC(ClassName)                                         \
    inline ClassName* q_func()                                              \
    {                                                                       \
        return reinterpret_cast<ClassName*>(q_ptr);                         \
    }                                                                       \
    inline const ClassName* q_func() const                                  \
    {                                                                       \
        return reinterpret_cast<const ClassName*>(q_ptr);                   \
    }                                                                       \
    friend class    ClassName;                                              \
    ClassName*      q_ptr;


/** QtのQ_D()と同じです
 *  @see https://wiki.qt.io/D-Pointer
 */
#define D_D(ClassName) ClassName##Private* const d = d_func()


/** QtのQ_Q()と同じです
 *  @see https://wiki.qt.io/D-Pointer
 */
#define D_Q(ClassName) ClassName* const q = q_func()


#endif /* end of include guard: dandy_DCore_hpp_ */
