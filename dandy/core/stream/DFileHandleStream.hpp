/**
 *       @file  DFileHandleStream.hpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/06/08
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


#ifndef dandy_DFileHandleStream_hpp_
#define dandy_DFileHandleStream_hpp_


#include <dandy/core/stream/DStream.hpp>


class DFileHandleStream : public DStream
{
public:
    DFileHandleStream(FileHandle* fh)
        : m_fh(fh) {}

    virtual ssize_t read(void *buffer, size_t size) override
    {
        return m_fh->read(buffer, size);
    }

    virtual ssize_t write(const void *buffer, size_t size) override
    {
        return m_fh->write(buffer, size);
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET) override
    {
        return m_fh->seek(offset, whence);
    }

    virtual int close() override
    {
        return m_fh->close();
    }

    virtual int sync() override
    {
        return m_fh->sync();
    }

    virtual int isatty() override
    {
        return m_fh->isatty();
    }

    virtual off_t tell() override
    {
        return m_fh->tell();
    }

    virtual void rewind() override
    {
        return m_fh->rewind();
    }

    virtual off_t size() override
    {
        return m_fh->size();
    }

    virtual int truncate(off_t length) override
    {
        return m_fh->truncate(length);
    }

    virtual int set_blocking(bool blocking) override
    {
        return m_fh->set_blocking(blocking);
    }

    virtual bool is_blocking() const override
    {
        return m_fh->is_blocking();
    }

    virtual int enable_input(bool enabled) override
    {
        return m_fh->enable_input(enabled);
    }

    virtual int enable_output(bool enabled) override
    {
        return m_fh->enable_output(enabled);
    }

    virtual short poll(short events) const override
    {
        return m_fh->poll(events);
    }

    virtual void sigio(Callback<void()> func) override
    {
        m_fh->sigio(func);
    }

private:
    D_DISALLOW_COPY_AND_ASSIGN(DFileHandleStream);

    FileHandle* m_fh;
};


#endif /* end of include guard: dandy_DFileHandleStream_hpp_ */
