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


DUSBSerial::DUSBSerial(
        size_t rx_buffer_size,
        bool connect_blocking,
        uint16_t vendor_id,
        uint16_t product_id,
        uint16_t product_release)
    : USBCDC(connect_blocking, vendor_id, product_id, product_release)
    , m_blocking(false)
{
    xfifo_init(&m_rxFifo, nullptr, rx_buffer_size, nullptr);
}

DUSBSerial::~DUSBSerial()
{
    xfifo_deinit(&m_rxFifo);
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
    const bool ok = this->send(static_cast<uint8_t*>(const_cast<void*>(buffer)), size);
    return ok ? size : -EIO;
}


off_t DUSBSerial::seek(off_t offset, int whence)
{
    return 0;
}


void DUSBSerial::data_rx()
{
     uint8_t c[64];
     uint32_t byteRead = 0;

     this->receive(c, sizeof(c), &byteRead);
     X_ASSERT(xfifo_reserve(&m_rxFifo) >= byteRead);
     xfifo_push_back_n(&m_rxFifo, c, byteRead);
}

size_t DUSBSerial::available() {
    return xfifo_size(&m_rxFifo);
}


bool DUSBSerial::connected() {
    return _terminal_connected;
}


void DUSBSerial::clear() {
    core_util_critical_section_enter();
    xfifo_clear(&m_rxFifo);
    core_util_critical_section_exit();
}
