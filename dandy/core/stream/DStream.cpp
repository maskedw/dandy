/**
 *       @file  DStream.cpp
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


#include <dandy/core/stream/DStream.hpp>


X_IMPL_RTTI_TAG(D__DSTREAM_RTTI_TAG) = 0;
static int D__WriteStream(void* self, const void* src, size_t size, size_t* nwritten);
static int D__ReadStream(void* self, void* dst, size_t size, size_t* nread);

#define D__DECLARE_XSTREAM(name)             \
    XStream name;                            \
    xstream_init(&name);                     \
    name.m_rtti_tag = &D__DSTREAM_RTTI_TAG;  \
    name.m_driver = this;                    \
    name.m_vtable = &X__dstream_vtable

static const XStreamVTable X__dstream_vtable = {
    .m_name = "DStream",
    .m_read_func = D__ReadStream,
    .m_write_func = D__WriteStream,
};

int DStream::printf(const char *fmt, ...)
{
    int len;
    va_list args;
    va_start(args, fmt);
    len = this->vprintf(fmt, args);
    va_end(args);
    return len;
}

int DStream::vprintf(const char *fmt, std::va_list args)
{
    D__DECLARE_XSTREAM(xst);
    return xstream_vprintf(&xst, fmt, args);
}

char* DStream::gets(char* dst, size_t size, bool* overflow)
{
    D__DECLARE_XSTREAM(xst);
    char* line;
    const int error = xstream_gets(&xst, dst, size, &line, overflow);
    return (error == 0) && line ? dst : nullptr;
}


std::string DStream::getline(size_t maxLineSize)
{
    char* p = D_NEW(char[maxLineSize]);
    X_ASSERT(p);

    if (!this->gets(p, maxLineSize, nullptr))
    {
        D_DELETE_ARRAY(p);
        return std::string();
    }

    const std::string result(p);
    D_DELETE_ARRAY(p);

    return result;
}

static int D__WriteStream(void* self, const void* src, size_t size, size_t* nwritten)
{
    const ssize_t ret = static_cast<DStream*>(self)->write(src, size);
    const bool success = ret >= 0;
    *nwritten = success ? ret : 0;

    return success ? 0 : ret;
}

static int D__ReadStream(void* self, void* dst, size_t size, size_t* nread)
{
    const ssize_t ret = static_cast<DStream*>(self)->read(dst, size);
    const bool success = ret >= 0;
    *nread = success ? ret : 0;

    return success ? 0 : ret;
}
