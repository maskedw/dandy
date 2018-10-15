/**
 *       @file  DBMPReader.hpp
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


#ifndef dandy_DBMP_hpp_
#define dandy_DBMP_hpp_

#include <dandy/core/stream/DStream.hpp>


/* https://ja.wikipedia.org/wiki/Windows_bitmap
 * > ビットマップデータ
 * > このブロックは、イメージを各ピクセルごとに記述する。ピクセルは通常、左下から右下
 * > へ、これを下から上に向かって保存する。各ピクセルは1バイト以上で記述されている。
 * > 直接RGBデータが置かれる場合のデータ順は、上項カラーパレットに準ずる。水平方向の
 * > バイト数が4の倍数ではないときは、0x00で埋めて4の倍数にする。
 */
#define DBMP_COLOR_DEPTH1_LINE_SIZE(w)    (X_ROUNDUP_MULTIPLE(w, 8) / 8)
#define DBMP_COLOR_DEPTH4_LINE_SIZE(w)    (X_ROUNDUP_MULTIPLE(w, 8) / 2)
#define DBMP_COLOR_DEPTH8_LINE_SIZE(w)    (w)
#define DBMP_COLOR_DEPTH16_LINE_SIZE(w)   ((w) * 2)
#define DBMP_COLOR_DEPTH24_LINE_SIZE(w)   ((w) * 3)
#define DBMP_COLOR_DEPTH32_LINE_SIZE(w)   ((w) * 4)


struct DBMPHeader
{
    // BITMAPFILEHEADER
    char     bfType[2];       //!< ファイルタイプ'BM'固定
    uint32_t bfSize;          //!< ファイルサイズ[byte]
    uint16_t bfReserved1;     //!< 予約領域１
    uint16_t bfReserved2;     //!< 予約領域２
    uint32_t bfOffBits;       //!< ファイル先頭から画像データまでのオフセット
    // BITMAPINFOHEADER
    uint32_t biSize;          //!< BITMAPINDOHEADERのサイズ(40固定)
    int32_t  biWidth;         //!< 画像の幅(ピクセル)
    int32_t  biHeight;        //!< 画像の高さ(ピクセル)
    uint16_t biPlanes;        //!< プレーン数(1固定)
    uint16_t biBitCount;      //!< 1画素あたりのビット数
    uint32_t biCompression;   //!< 圧縮形式
    uint32_t biSizeImage;     //!< 画像データ部のサイズ
    int32_t  biXPixPerMeter;  //!< 横方向解像度(1mあたりの画素数)
    int32_t  biYPixPerMeter;  //!< 縦方向解像度(1mあたりの画素数)
    uint32_t biClrUsed;       //!< 格納されているパレット数(使用色数)
    uint32_t biClrImporant;   //!< 重要なパレットのインデックス

    bool isValidType() const;
    bool isNoCompression() const;
    bool hasPalette() const;
    int getLineSize() const;
    int getNumberOfColors() const;
};


/** bmpカラーパレットのデータ型
 */
struct DBMPRGBQuad
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
};


class DBMPReader
{
public:
    DBMPReader();
    ~DBMPReader();
    void setStream(DStream* in);
    void setHeader(const DBMPHeader* header);
    const DBMPHeader* getHeader() const;
    bool readHeader(DBMPHeader* header);
    bool readPalette(DBMPRGBQuad* palette);
    bool readRow(unsigned int rowNumber, void* rowBuffer);

private:
    DStream* m_in;
    const DBMPHeader* m_header;
    int m_prevRowNumber;
};


#endif /* end of include guard: dandy_DBMP_hpp_ */
