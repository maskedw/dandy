/**
 *       @file  DObjectStorage.hpp
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

#ifndef dandy_DObjectStorage_hpp_
#define dandy_DObjectStorage_hpp_


#include <dandy/core/DObject.hpp>
#include <map>
#include <string>


typedef Callback<void(const char*, DObject*)> DObjectStorageWalkCallback;

class DObjectStorage
{
public:
    static DObjectStorage* shared();
    void put(const char* name, DObject* obj);
    bool has(const char* name);
    DObject* get(const char* name);
    void remove(const char* name);
    void walk(DObjectStorageWalkCallback callback);

private:
    // TODO uthashに置き換え
    typedef std::map<std::string, DObject*> DObjectMap;

    DObjectMap m_map;
};

#endif /* end of include guard: dandy_DObjectStorage_hpp_ */
