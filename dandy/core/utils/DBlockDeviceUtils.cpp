/**
 *       @file  DBlockDeviceUtils.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/08/21
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


#include <dandy/core/utils/DBlockDeviceUtils.hpp>
#include <EASTL/unique_ptr.h>


int DBlockDeviceUtils::replace(BlockDevice* bd, const void* src, bd_addr_t addr, bd_size_t size)
{
    X_ASSERT(bd);
    X_ASSERT(src);

    if (!size)
        return 0;

    const bd_size_t erase_size = bd->get_erase_size();
    int result = 0;

    /* アドレスをeraseサイズの倍数に切り下げる */
    const bd_size_t offset = addr % erase_size;
    addr = addr - offset;

    eastl::unique_ptr<char[]> bufferUniquePtr(D_NEW(char[erase_size]));
    X_ASSERT(bufferUniquePtr);

    char* buffer = bufferUniquePtr.get();
    const char* p = static_cast<const char*>(src);
    const char* pp = nullptr;
    bd_size_t toWrite = 0;

    /* 元のデータを読み出して */
    result = bd->read(buffer, addr, erase_size);
    if (result != 0)
        return result;

    /* 書き込みバイト数の調整 */
    if (size + offset <= erase_size)
        toWrite = size;
    else
        toWrite = erase_size - offset;

    /* 消去して */
    result = bd->erase(addr, erase_size);

    if (result != 0)
        return result;

    /* 新しいデータに置き換えて */
    memcpy(buffer + offset, p, toWrite);

    /* 書き戻す  */
    result = bd->program(buffer, addr, erase_size);
    if (result != 0)
        return result;

    /* この段階で全ての書き込みが終われば正常終了 */
    if (size <= toWrite)
        return result;

    size -= toWrite;
    addr += erase_size;
    p += toWrite;

    /* ここまできたらアドレスはerase_sizeアラインになっているはず */
    X_ASSERT((addr % erase_size) == 0);

    /* 残りのデータの置き換え */
    for (;;)
    {
        toWrite = (erase_size > size) ? size : erase_size;
        if (erase_size > toWrite)
        {
            pp = p;
        }
        else
        {
            result = bd->read(buffer, addr, erase_size);
            if (result != 0)
                return result;
            memcpy(buffer, p, toWrite);
            pp = buffer;
        }

        result = bd->erase(addr, erase_size);
        if (result != 0)
            return result;

        result = bd->program(pp, addr, toWrite);
        if (result != 0)
            return result;

        /* 全部書き込んだら終了 */
        if (size <= toWrite)
            break;

        size -= toWrite;
        addr += toWrite;
        p += toWrite;
    }

    return result;
}
