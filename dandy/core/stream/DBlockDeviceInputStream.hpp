/**
 *       @file  DBlockDeviceInputStream.hpp
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


#ifndef dandy_DBlockDeviceInputStream_hpp_
#define dandy_DBlockDeviceInputStream_hpp_


#include <dandy/core/stream/DStream.hpp>


class DBlockDeviceInputStream : public DStream
{
public:
    DBlockDeviceInputStream(BlockDevice* blockDevice,
                            bd_addr_t address,
                            bd_size_t size,
                            size_t cacheSize = 1024);

    ~DBlockDeviceInputStream() override;
    virtual int close() override;
    virtual ssize_t read(void *buffer, size_t size) override;
    virtual ssize_t write(const void *buffer, size_t size) override;
    virtual off_t seek(off_t offset, int whence = SEEK_SET) override;

private:
    D_DISALLOW_COPY_AND_ASSIGN(DBlockDeviceInputStream);

    void ResetCache();
    int GetChar();

    BlockDevice* m_blockDevice;
    uint8_t* m_buffer;
    bd_addr_t m_begginAddress;
    size_t m_size;
    size_t m_pos;
    size_t m_cacheSize;
    size_t m_cacheIndex;
    bool m_cached;
};


#endif /* end of include guard: dandy_DBlockDeviceInputStream_hpp_ */
