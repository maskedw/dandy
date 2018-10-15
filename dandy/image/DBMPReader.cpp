/**
 *       @file  DBMPReader.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/09/18
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


#include <dandy/image/DBMPReader.hpp>

/* bmpヘッダのバイトオフセット */
#define D__OFFSETOF_BF_TYPE             0
#define D__OFFSETOF_BF_SIZE             2
#define D__OFFSETOF_BF_RESERVED1        6
#define D__OFFSETOF_BF_RESERVED2        8
#define D__OFFSETOF_BF_OFFBITS          10
#define D__OFFSETOF_BI_SIZE             14
#define D__OFFSETOF_BI_WIDTH            18
#define D__OFFSETOF_BI_HEIGHT           22
#define D__OFFSETOF_BI_PLANES           26
#define D__OFFSETOF_BI_BITCOUNT         28
#define D__OFFSETOF_BI_COMPRESSION      30
#define D__OFFSETOF_BI_SIZEIMAGE        34
#define D__OFFSETOF_BI_XPIXPER_METER    38
#define D__OFFSETOF_BI_YPIXPER_METER    42
#define D__OFFSETOF_BI_CLRUSED          46
#define D__OFFSETOF_BI_CLRIMPORANT      50

#define D__SIZEOF_BMP_HEADER            54


DBMPReader::DBMPReader()
    : m_in(nullptr)
    , m_header(nullptr)
    , m_prevRowNumber(-1)
{
}

DBMPReader::~DBMPReader()
{
}

void DBMPReader::setStream(DStream* in)
{
    m_in = in;
}

void DBMPReader::setHeader(const DBMPHeader* header)
{
    m_header = header;
}

const DBMPHeader* DBMPReader::getHeader() const
{
    return m_header;
}

bool DBMPReader::readHeader(DBMPHeader* header)
{
    uint8_t bmphdr[D__SIZEOF_BMP_HEADER];

    if (m_in->seek(0) < 0)
        return false;

    m_prevRowNumber = -1;
    if (m_in->read(bmphdr, sizeof(bmphdr)) != sizeof(bmphdr))
        return false;

    /* unpacking */
    std::memcpy(header->bfType, bmphdr + D__OFFSETOF_BF_TYPE, sizeof(header->bfType));
    header->bfSize         = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BF_SIZE);
    header->bfReserved1    = X_LOAD_U16_LIT(bmphdr + D__OFFSETOF_BF_RESERVED1);
    header->bfReserved2    = X_LOAD_U16_LIT(bmphdr + D__OFFSETOF_BF_RESERVED2);
    header->bfOffBits      = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BF_OFFBITS);
    header->biSize         = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_SIZE);
    header->biWidth        = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_WIDTH);
    header->biHeight       = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_HEIGHT);
    header->biPlanes       = X_LOAD_U16_LIT(bmphdr + D__OFFSETOF_BI_PLANES);
    header->biBitCount     = X_LOAD_U16_LIT(bmphdr + D__OFFSETOF_BI_BITCOUNT);
    header->biCompression  = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_COMPRESSION);
    header->biSizeImage    = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_SIZEIMAGE);
    header->biXPixPerMeter = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_XPIXPER_METER);
    header->biYPixPerMeter = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_YPIXPER_METER);
    header->biClrUsed      = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_CLRUSED);
    header->biClrImporant  = X_LOAD_U32_LIT(bmphdr + D__OFFSETOF_BI_CLRIMPORANT);

    return true;
}

bool DBMPReader::readPalette(DBMPRGBQuad* palette)
{
    X_ASSERT(m_in);
    X_ASSERT(m_header);

    if (m_in->seek(D__SIZEOF_BMP_HEADER) < 0)
        return false;

    m_prevRowNumber = -1;
    const size_t nRead = sizeof(DBMPRGBQuad) * m_header->getNumberOfColors();
    if (m_in->read(palette, nRead) != nRead)
        return false;

    return true;
}

bool DBMPReader::readRow(unsigned int rowNumber, void* rowBuffer)
{
    X_ASSERT(m_in);
    X_ASSERT(m_header);

    if (rowNumber >= m_header->biHeight)
        return false;

    const int lineSize = m_header->getLineSize();
    if (lineSize <= 0)
        return false;

    const int roundedLineSize = X_ROUNDUP_MULTIPLE(lineSize, 4);
    if ((m_prevRowNumber >= 0) && ((m_prevRowNumber - 1) == rowNumber))
    {
        const int remain = roundedLineSize % lineSize;
        if (remain)
        {
            uint8_t nullBuffer[3];
            if (m_in->read(nullBuffer, remain) != remain)
                return false;
        }
    }
    else
    {
        const off_t pos = m_header->bfOffBits + (roundedLineSize * (m_header->biHeight - 1 - rowNumber));
        if (m_in->seek(pos) < 0)
            return false;
    }

    if (m_in->read(rowBuffer, lineSize) != lineSize)
        return false;

    m_prevRowNumber = rowNumber;

    return true;
}

bool DBMPHeader::isValidType() const
{
    return (this->bfType[0] == 'B') && (this->bfType[1] == 'M');
}

bool DBMPHeader::isNoCompression() const
{
    return this->biCompression == 0;
}

bool DBMPHeader::hasPalette() const
{
    return this->biBitCount <= 8;
}

int DBMPHeader::getLineSize() const
{
    const int w = this->biWidth;
    int result = -1;

    switch (this->biBitCount)
    {
        case 1:
            result = DBMP_COLOR_DEPTH1_LINE_SIZE(w);
            break;
        case 4:
            result = DBMP_COLOR_DEPTH4_LINE_SIZE(w);
            break;
       case 8:
            result= DBMP_COLOR_DEPTH8_LINE_SIZE(w);
            break;
       case 16:
            result= DBMP_COLOR_DEPTH16_LINE_SIZE(w);
            break;
       case 32:
            result= DBMP_COLOR_DEPTH32_LINE_SIZE(w);
            break;
    }

    return result;
}

int DBMPHeader::getNumberOfColors() const
{
    return 1U << biBitCount;
}
