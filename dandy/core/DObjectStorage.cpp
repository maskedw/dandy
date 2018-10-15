/**
 *       @file  DObjectStorage.cpp
 *      @brief
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

#include <dandy/core/DObjectStorage.hpp>
#include <utility>


static SingletonPtr<DObjectStorage> _shared_instance;


DObjectStorage* DObjectStorage::shared()
{
    return _shared_instance.get();
}

void DObjectStorage::put(const char* name, DObject* obj)
{
    m_map.insert(std::make_pair(name, obj));
}

bool DObjectStorage::has(const char* name)
{
    return m_map.find(name) != m_map.end();
}

DObject* DObjectStorage::get(const char* name)
{
    DObjectMap::iterator ret = m_map.find(name);
    if (ret == m_map.end())
        return NULL;
    return ret->second;
}

void DObjectStorage::remove(const char* name)
{
    DObjectMap::iterator ret = m_map.find(name);
    if (ret == m_map.end())
        return;
    m_map.erase(ret);
}

void DObjectStorage::walk(DObjectStorageWalkCallback callback)
{
    D_FOREACH(DObjectMap::const_iterator, ite, m_map)
        callback(ite->first.c_str(), ite->second);
}
