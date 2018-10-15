/**
 *       @file  DNullStream.hpp
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

#ifndef dandy_DNullStream_hpp_
#define dandy_DNullStream_hpp_


#include <dandy/core/stream/DStream.hpp>


class DNullStream : public DStream
{
public:
    DNullStream();
    virtual ssize_t read(void *buffer, size_t size) override;
    virtual ssize_t write(const void *buffer, size_t size) override;
    virtual off_t seek(off_t offset, int whence = SEEK_SET) override;
    virtual int close() override;
    virtual off_t size() override;
    virtual off_t tell() override;
    size_t getTotalWrite() const;
    size_t getTotalRead() const;

private:
    D_DISALLOW_COPY_AND_ASSIGN(DNullStream);
    size_t m_totalWrite;
    size_t m_totalRead;
};


#endif /* end of include guard: dandy_DNullStream_hpp_ */
