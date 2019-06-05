/**
 *       @file  DStream.hpp
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

#ifndef DStream_hpp_
#define DStream_hpp_


#include <dandy/core/DCore.hpp>


class DStream : public FileHandle
{
public:
    DStream() {}
    virtual int close() override { return 0; }
    virtual ssize_t read(void *buffer, size_t size) override { return -ENOSYS; }
    virtual ssize_t write(const void *buffer, size_t size) override { return -ENOSYS; }

    int putc(int c)
    {
        const uint8_t b = c;
        const ssize_t result = this->write(&b, 1);
        return result == 1 ? c : EOF;
    }

    int getc()
    {
        uint8_t b;
        const ssize_t result = this->read(&b, 1);
        return result == 1 ? b : EOF;
    }

    int printf(const char *format, ...) X_PRINTF_ATTR(2, 3);
    int vprintf(const char *format, std::va_list args);
    char* gets(char* dst, size_t size, bool* overflow);
    std::string getline(size_t maxLineSize = 1024);

private:
    D_DISALLOW_COPY_AND_ASSIGN(DStream);
};


#endif /* end of include guard: DStream_hpp_ */
