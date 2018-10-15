/**
 *       @file  DMbedI2C.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/08/25
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


#include <dandy/drivers/i2c/DMbedI2C.hpp>


D_IMPL_RTTI(DMbedI2C, DI2C);

DMbedI2C::DMbedI2C(PinName sda, PinName scl)
    : m_sda(sda)
    , m_scl(scl)
    , m_i2c(sda, scl)
{
}

PinName DMbedI2C::sda() const
{
    return m_sda;
}

PinName DMbedI2C::scl() const
{
    return m_scl;
}

I2C* DMbedI2C::get()
{
    return &m_i2c;
}

void DMbedI2C::frequency(int hz)
{
    m_i2c.frequency(hz);
}

int  DMbedI2C::read(int address, void *data, int length)
{
    return m_i2c.read(address, static_cast<char*>(data), length);
}

int  DMbedI2C::write(int address, const void *data, int length)
{
    return m_i2c.write(address, static_cast<const char*>(data), length);
}
