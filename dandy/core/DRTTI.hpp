/**
 *       @file  DRTTI.hpp
 *      @brief  dynamic_castなしでrtti機能を提供します。
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/08/22
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

#ifndef dandy_DRTTI_hpp_
#define dandy_DRTTI_hpp_


#include <dandy/core/DCore.hpp>
#include <EASTL/type_traits.h>


class DRTTI
{
  public:
    explicit DRTTI(const char* classname)
        : m_classname(classname)
        , m_basertti(nullptr)
    {
    }

    DRTTI(const char* classname, const DRTTI& basertti)
        : m_classname(classname)
        , m_basertti(&basertti)
    {
    }


    const char* classname() const
    {
        return m_classname;
    }


    bool is_exactly(const DRTTI& rtti) const
    {
        return (this == &rtti);
    }


    bool derives_from (const DRTTI& rtti) const
    {
        const DRTTI* compare = this;
        while (compare != nullptr)
        {
            if (compare == &rtti)
                return true;
            compare = compare->m_basertti;
        }
        return false;
    }

private:
    EA_NON_COPYABLE(DRTTI);

    const char* const  m_classname;
    const DRTTI* const m_basertti;
};


/** FromからToへの変換が可能な場合、Toをかえします。
 */
template <typename To, typename From>
static inline To d_rtti_cast(From x)
{
    enum
    {
        IS_POINTER_TO    = eastl::is_pointer<To>::value,
        IS_POINTER_FROM  = eastl::is_pointer<From>::value,
    };
    static_assert(IS_POINTER_TO,   "'To' is not a pointer type");
    static_assert(IS_POINTER_FROM, "'From' is not a pointer type");

    typedef typename eastl::remove_pointer<To>::type T;

    if (! x)
        return 0;

    return x->_rtti().derives_from(
        (T::_rtti_obj)) ? static_cast<To>(x) : 0;
}


/** rtti_cast失敗時にプログラムを停止させます。
 */
template <typename To, typename From>
static inline To d_strict_rtti_cast(From x)
{
    To result = d_rtti_cast<To>(x);
    X_ASSERT(result);

    return result;
}


/** RTTIを使用するクラスはこのマクロをclass定義に記述します。
 *
 * @code
 * class Foo
 * {
 *      D_DECLARE_RTTI;
 * };
 * @endcode
 */
#define D_DECLARE_RTTI                                           \
    public:                                                      \
        virtual const DRTTI& _rtti() const { return _rtti_obj; } \
        static const DRTTI _rtti_obj


/** ルートクラスがRTTIをを使用するときはこちらをソースコードに記述します。
 *
 * @code
 * // Foo.hpp
 * class Foo
 * {
 *      D_DECLARE_RTTI;
 * };
 *
 * // Foo.cpp
 * D_IMPL_RTTI_ROOT(Foo);
 * @endcode
 */
#define D_IMPL_RTTI_ROOT(T) \
    const DRTTI T::_rtti_obj(#T)


/** RTTIクラスの派生クラスはこちらをソースコードに記述します。
 *
 * @code
 * // FooBar.hpp
 * class Foo
 * {
 *      D_DECLARE_RTTI;
 * };
 *
 * class Bar : public(Foo)
 * {
 *      D_DECLARE_RTTI;
 * }
 *
 * // FooBar.cpp
 * D_IMPL_RTTI_ROOT(Foo);
 * D_IMPL_RTTI(Bar, Foo);
 * @endcode
 */
#define D_IMPL_RTTI(DerivedClass, BaseClass) \
    const DRTTI DerivedClass::_rtti_obj(#DerivedClass, BaseClass::_rtti_obj)


#endif // dandy_DRTTI_hpp_
