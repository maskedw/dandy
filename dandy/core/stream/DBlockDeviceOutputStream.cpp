/**
 *       @file  DBlockDeviceOutputStream.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/06/04
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


#include <dandy/core/stream/DBlockDeviceOutputStream.hpp>


DBlockDeviceOutputStream::DBlockDeviceOutputStream(BlockDevice* blockDevice,
                                                   bd_addr_t address,
                                                   bd_size_t size)
    : m_blockDevice(blockDevice)
    , m_begginAddress(address)
    , m_size(size)
    , m_pos(0)
{
}

DBlockDeviceOutputStream::~DBlockDeviceOutputStream()
{
}

ssize_t DBlockDeviceOutputStream::write(const void* src, size_t size)
{
    const size_t to_write = ((m_pos + size) <= m_size)
                            ? size : (m_size - m_pos);

    if (to_write)
    {
        if (m_blockDevice->program(src, m_begginAddress + m_pos, to_write) != 0)
            return -EIO;
    }

    m_pos += to_write;
    return to_write;
}

off_t DBlockDeviceOutputStream::seek(off_t offset, int whence)
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
        m_pos = seekpos;
    }

    return m_pos;
}
