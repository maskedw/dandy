/**
 *       @file  DMemoryInputStream.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/10/13
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

#include <dandy/core/stream/DMemoryInputStream.hpp>


DMemoryInputStream::DMemoryInputStream(const void* src, size_t size)
    : m_src(static_cast<const uint8_t*>(src))
    , m_size(size)
    , m_pos(0)
{
}

void DMemoryInputStream::attach(const void* src, size_t size)
{
    m_src = static_cast<const uint8_t*>(src);
    m_size = size;
    m_pos = 0;
}

ssize_t DMemoryInputStream::read(void *dst, size_t size)
{
    const size_t to_read = ((m_pos + size) <= m_size)
                            ? size : (m_size - m_pos);

    if (to_read)
        std::memcpy(dst, m_src + m_pos, to_read);

    m_pos += to_read;
    return to_read;
}

ssize_t DMemoryInputStream::write(const void* /*src*/, size_t /*size*/)
{
    return -EACCES;
}

off_t DMemoryInputStream::seek(off_t offset, int whence)
{
    off_t seekpos = 0;
    switch (whence)
    {
        case SEEK_SET:
            seekpos = offset;
            break;

        case SEEK_CUR:
            seekpos = m_pos + offset;
            break;

        case SEEK_END:
            seekpos = m_size + offset;
            break;
        default:
            return -1;
    }

    if (seekpos < 0)
        return -ERANGE;

    if (seekpos > static_cast<off_t>(m_size))
        return -ERANGE;

    m_pos = seekpos;
    return m_pos;
}

int DMemoryInputStream::close()
{
    return 0;
}

off_t DMemoryInputStream::size()
{
    return m_size;
}
