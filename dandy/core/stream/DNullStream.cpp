/**
 *       @file  DNullStream.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/05/31
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2015> <MaskedW [maskedw00@gmail.com]>
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

#include <dandy/core/stream/DNullStream.hpp>


DNullStream::DNullStream()
    : m_totalWrite(0)
    , m_totalRead(0)
{
}

ssize_t DNullStream::read(void *buffer, size_t size)
{
    memset(buffer, 0, size);
    m_totalRead += size;
    return size;
}

ssize_t DNullStream::write(const void *buffer, size_t size)
{
    m_totalWrite += size;
    return size;
}

off_t DNullStream::seek(off_t offset, int whence)
{
    return 0;
}

int DNullStream::close()
{
    return 0;
}

off_t DNullStream::size()
{
    return 0;
}

off_t DNullStream::tell()
{
    return 0;
}

size_t DNullStream::getTotalWrite() const
{
    return m_totalWrite;
}

size_t DNullStream::getTotalRead() const
{
    return m_totalRead;
}
