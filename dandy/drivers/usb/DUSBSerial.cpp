/**
 *       @file  DUSBSerial.cpp
 *      @brief
 *
 *    @details
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
 * Copyright (c) 2010-2011 mbed.org, MIT License
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

#include <dandy/drivers/usb/DUSBSerial.hpp>


bool DUSBSerial::writeBlock(const uint8_t * buf, uint16_t size) {
    if(size > MAX_PACKET_SIZE_EPBULK) {
        return false;
    }
    if(!send(const_cast<uint8_t*>(buf), size)) {
        return false;
    }
    return true;
}


ssize_t DUSBSerial::read(void *dst, size_t size)
{
    if (m_blocking && xfifo_empty(&m_rxFifo))
        while (xfifo_empty(&m_rxFifo));

    const size_t available = xfifo_size(&m_rxFifo);
    const size_t to_read = available >= size ? size : available;

    if (to_read > 0)
        xfifo_pop_front_n(&m_rxFifo, dst, to_read);

    return to_read;
}


ssize_t DUSBSerial::write(const void *buffer, size_t size)
{
    const uint8_t* p = static_cast<const uint8_t*>(buffer);
    size_t remain = size;
    while (remain)
    {
        /* ちょっとずつ送信しないとデータが壊れる・・・。原因不明 */
        const size_t MAX_SIZE = 1;
        const size_t to_write = size <= MAX_SIZE ? remain : MAX_SIZE;
        if (!this->writeBlock(p, to_write))
            return -EIO;
        p += to_write;
        remain -= to_write;
    }

    return size;
}


off_t DUSBSerial::seek(off_t offset, int whence)
{
    return 0;
}


bool DUSBSerial::EPBULK_OUT_callback() {
    uint8_t c[65];
    uint32_t size = 0;

    //we read the packet received and put it on the circular buffer
    readEP(c, &size);
    X_ASSERT(xfifo_reserve(&m_rxFifo) >= size);

    xfifo_push_back_n(&m_rxFifo, c, size);

    return true;
}


size_t DUSBSerial::available() {
    return xfifo_size(&m_rxFifo);
}


bool DUSBSerial::connected() {
    return terminal_connected;
}


void DUSBSerial::clear() {
    core_util_critical_section_enter();
    xfifo_clear(&m_rxFifo);
    core_util_critical_section_exit();
}
