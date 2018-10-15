/**
 *       @file  DRef.cpp
 *      @brief
 *
 *    @details
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


#include <dandy/core/DRef.hpp>


D_IMPL_RTTI_ROOT(DRef);


DRef::DRef()
    :   m_tag(D_INVALID_TAG)
    ,   m_use_count(0)
    ,   m_name(nullptr)
{
}


DRef::~DRef()
{
    X_ASSERT(this->m_use_count == 0);
    D_SAFE_DELETE_ARRAY(m_name);
}


void DRef::retain()
{
    ++m_use_count;
}


void DRef::release()
{
    --m_use_count;
    if (m_use_count == 0)
        D_DELETE(this);
}


void DRef::set_name(const char* name)
{
    if (m_name == name)
        return;

    if ((m_name != nullptr) && (name != nullptr) &&
            (std::strcmp(m_name, name) == 0))
        return;

    if (m_name)
        D_SAFE_DELETE_ARRAY(m_name);

    if (! name)
    {
        m_name = nullptr;
    }
    else
    {
        m_name = D_NEW(char[std::strlen(name) + 1]);
        X_ASSERT(m_name);
        std::strcpy(m_name, name);
    }
}
