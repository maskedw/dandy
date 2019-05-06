/**
 *       @file  DBlockDeviceInputStream.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/01/20
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


#include <dandy/core/stream/DBlockDeviceInputStream.hpp>


#define UNSPPORTED_OPERATION    "Unspported operation"


DBlockDeviceInputStream::DBlockDeviceInputStream(BlockDevice* blockDevice,
                                                 bd_addr_t address,
                                                 bd_size_t size,
                                                 size_t cacheSize)
    : m_blockDevice(blockDevice)
    , m_buffer(nullptr)
    , m_begginAddress(address)
    , m_size(size)
    , m_pos(0)
    , m_cacheSize(cacheSize)
    , m_cacheIndex(0)
    , m_cached(false)
{
    // X_ASSERT(x_is_within(beginAddress + size, 0, SST25_ADDER_END) + 1);

    X_ASSERT(m_size > 0);
    X_ASSERT(m_cacheSize > 0);
    if (m_cacheSize > m_size)
        m_cacheSize = m_size;

    m_buffer = D_NEW(uint8_t[m_cacheSize]);

}

DBlockDeviceInputStream::~DBlockDeviceInputStream()
{
    D_SAFE_DELETE_ARRAY(m_buffer);
}

int DBlockDeviceInputStream::close()
{
    return 0;
}

ssize_t DBlockDeviceInputStream::read(void *buffer, size_t size)
{
    uint8_t* p = static_cast<uint8_t*>(buffer);
    size_t n;
    int c = 0;
    for (n = 0; n < size; n++)
    {
        c = this->GetChar();
        if (c < 0)
            break;
        p[n] = c;
    }

    if ((n == size) || (c == -ENODATA))
        return n;
    return c;
}

ssize_t DBlockDeviceInputStream::write(const void *buffer, size_t size)
{
    X_UNUSED(buffer);
    X_UNUSED(size);
    X_ASSERT(!UNSPPORTED_OPERATION);
    return -ENOSYS;
}

off_t DBlockDeviceInputStream::seek(off_t offset, int whence)
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

    if (m_pos != seekpos)
    {
        /* 本当はシーク位置によってキャッシュを破棄するかどうかを判断すべきだけ
         * ど、別に今の所そこまでの機能はいらんので、シーク時は無条件でキャッシ
         * ュを破棄する。
         */
        this->ResetCache();
        m_pos = seekpos;
    }

    return m_pos;
}

void DBlockDeviceInputStream::ResetCache()
{
    m_cached = false;
    m_cacheIndex = 0;
}

int DBlockDeviceInputStream::GetChar()
{
    if (m_pos >= m_size)
        return -ENODATA;

    if (m_cacheIndex >= m_cacheSize)
        this->ResetCache();

    if (!m_cached)
    {
        m_blockDevice->read(m_buffer, m_begginAddress + m_pos, m_cacheSize);
        m_cached = true;
    }

    const int c = m_buffer[m_cacheIndex++];
    m_pos++;

    return c;
}
