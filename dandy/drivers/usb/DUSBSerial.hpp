/**
 *       @file  DUSBSerial.hpp
 *      @brief
 *
 *    @details
 *    mbedのUSBSerialクラスがもひとつ使い勝手が悪いので
 *    mbed-os/features/unsupported/USBDevice/USBSerial/USBSerial.hをベースに再実装しました。
 *
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/01/21
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

#ifndef dandy_DUSBSerial_hpp_
#define dandy_DUSBSerial_hpp_


#include <dandy/core/DCore.hpp>
#include <dandy/core/stream/DStream.hpp>
#include <picox/container/xfifo_buffer.h>
#include "USBCDC.h"


class DUSBSerial: public USBCDC, public DStream {
public:
    /**
    *   Constructor
    *
    * @param connect_blocking define if the connection must be blocked if USB not plugged in
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    DUSBSerial(size_t rx_buffer_size,
               bool connect_blocking = true,
               uint16_t vendor_id = 0x1f00,
               uint16_t product_id = 0x2012,
               uint16_t product_release = 0x0001);
    virtual ~DUSBSerial() override;

    virtual ssize_t read(void *dst, size_t size) override;
    virtual ssize_t write(const void *buffer, size_t size) override;
    virtual off_t seek(off_t offset, int whence = SEEK_SET) override;
    virtual int set_blocking(bool blocking) override
    {
        m_blocking = blocking;
        return 0;
    }
    virtual bool is_blocking() const override
    {
        return m_blocking;
    }
    virtual off_t size() override
    {
        return this->available();
    }


    void clear();

    /**
    * Check the number of bytes available.
    *
    * @returns the number of bytes available
    */
    size_t available();

     /**
    * Check if the terminal is connected.
    *
    * @returns connection status
    */
    bool connected();

    /** Determine if there is a character available to read
     *
     *  @returns
     *    1 if there is a character available to read,
     *    0 otherwise
     */
    int readable() { return available() ? 1 : 0; }

    /** Determine if there is space available to write a character
     *
     *  @returns
     *    1 if there is space to write a character,
     *    0 otherwise
     */
    int writeable() { return 1; } // always return 1, for write operation is blocking

    /**
    * Write a block of data.
    *
    * For more efficiency, a block of size 64 (maximum size of a bulk endpoint) has to be written.
    *
    * @param buf pointer on data which will be written
    * @param size size of the buffer. The maximum size of a block is limited by the size of the endpoint (64 bytes)
    *
    * @returns true if successfull
    */
    bool writeBlock(const uint8_t * buf, uint16_t size);

protected:
    virtual void data_rx() override;

private:

    XFifoBuffer m_rxFifo;
    bool m_blocking;
};

#endif /* end of include guard: dandy_DUSBSerial_hpp_ */
