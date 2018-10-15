/**
 *       @file  DFILEStream.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/09/25
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


#include <dandy/core/stream/DFILEStream.hpp>


DFILEStream::DFILEStream(FILE* fp)
    : m_fp(fp)
{
}

ssize_t DFILEStream::read(void *buffer, size_t size)
{
    const size_t nread = fread(buffer, 1, size, m_fp);
    if (nread != size)
    {
        if (ferror(m_fp) != 0)
            return -EIO;
    }
    return nread;
}

ssize_t DFILEStream::write(const void *buffer, size_t size)
{
    const size_t nwritten = fwrite(buffer, 1, size, m_fp);
    if (nwritten != size)
    {
        if (ferror(m_fp) != 0)
            return -EIO;
    }
    return nwritten;
}

off_t DFILEStream::seek(off_t offset, int whence)
{
    long pos = -1;
    if (fseek(m_fp, offset, whence) != 0)
        return -EIO;

    pos = ftell(m_fp);
    if (pos < 0)
        return -EIO;
    return pos;
}

int DFILEStream::close()
{
    if (fclose(m_fp) != 0)
        return -EIO;
    return 0;
}

int DFILEStream::sync()
{
    if (fflush(m_fp) != 0)
        return -EIO;
    return 0;
}

