
#include "../QstLibs/QstLibs.h"

#include <string>
#include <exception>
#pragma warning (disable: 4245)
#pragma warning (disable: 4512)

#include "zxing/common/Counted.h"
#include "zxing/Binarizer.h"
#include "zxing/MultiFormatReader.h"
#include "zxing/Result.h"
#include "zxing/ReaderException.h"
#include "zxing/common/GlobalHistogramBinarizer.h"
#include "zxing/common/HybridBinarizer.h"
#include "zxing/Exception.h"
#include "zxing/common/IllegalArgumentException.h"
#include "zxing/BinaryBitmap.h"
#include "zxing/DecodeHints.h"
#include "zxing/qrcode/QRCodeReader.h"
#include "zxing/multi/qrcode/QRCodeMultiReader.h"
#include "zxing/multi/ByQuadrantReader.h"
#include "zxing/multi/MultipleBarcodeReader.h"
#include "zxing/multi/GenericMultipleBarcodeReader.h"

using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;

/********************/
/* 灰度图片接口实现 */
/********************/
class GrayImage : public LuminanceSource
{
private:
    const char* m_image;

public:
    /* ==================================================== */
    GrayImage (const char *image, uint_t width, uint_t height)
        : LuminanceSource(width, height)
    {
        m_image = image;
    }

    /* =============== */
    virtual ~GrayImage ()
    {
        mem_free(m_image);
    }

    /* ================================================= */
    ArrayRef<char> getRow (int y, ArrayRef<char> row) const
    {
        const char* line = m_image + y * this->getWidth();

        if (!row)
            row = ArrayRef<char>(this->getWidth());
        for (int x = 0; x < this->getWidth(); x++)
            row[x] = line[x];
        return (row);
    }

    /* =========================== */
    ArrayRef<char> getMatrix () const
    {
        const char*     p = m_image;
        ArrayRef<char>  matrix(this->getWidth() * this->getHeight());
        char*           m = &matrix[0];

        for (int y = 0; y < this->getHeight(); y++)
        for (int x = 0; x < this->getWidth();  x++)
            *m++ = *p++;
        return (matrix);
    }

    /* ======================= */
    bool isCropSupported () const
    {
        return (true);
    }

    /* ========================================================== */
    Ref<LuminanceSource> crop (int x1, int y1, int ww, int hh) const
    {
        char*   dat;

        if (x1 >= this->getWidth() || y1 >= this->getHeight() ||
            x1 + ww > this->getWidth() || y1 + hh > this->getHeight())
            throw IllegalArgumentException("crop: invalid param!");
        dat = (char*)mem_malloc((size_t)ww * hh);
        if (dat == NULL)
            throw IllegalArgumentException("crop: mem_malloc() failure!");
        for (int yy = 0; yy < hh; yy++)
        for (int xx = 0; xx < ww; xx++) {
            dat[yy * ww + xx] = m_image[(y1 + yy) * this->getWidth() +
                                        (x1 + xx)];
        }
        return (Ref<LuminanceSource>(new GrayImage (dat, ww, hh)));
    }

    /* ========================= */
    bool isRotateSupported () const
    {
        return (true);
    }

    /* ============================================== */
    Ref<LuminanceSource> rotateCounterClockwise () const
    {
        int     ww = this->getHeight();
        int     hh = this->getWidth();
        char*   dat = (char*)mem_malloc((size_t)ww * hh);

        if (dat == NULL)
            throw IllegalArgumentException("rotate: mem_malloc() failure!");
        for (int yy = 0; yy < hh; yy++)
        for (int xx = 0; xx < ww; xx++) {
            dat[yy * ww + xx] = m_image[xx * hh + (hh - yy - 1)];
        }
        return (Ref<LuminanceSource>(new GrayImage (dat, ww, hh)));
    }
};

#if 0
/*
---------------------------------------
    ZXing 识别单个码
---------------------------------------
*/
static vector<Ref<Result> >
decode (
  __CR_IN__ Ref<BinaryBitmap>   image,
  __CR_IN__ DecodeHints         hints
    )
{
    Ref<Reader> reader(new MultiFormatReader);

    return (vector<Ref<Result> >(1, reader->decode(image, hints)));
}
#endif

/*
---------------------------------------
    ZXing 识别多个码
---------------------------------------
*/
static vector<Ref<Result> >
decode_multi (
  __CR_IN__ Ref<BinaryBitmap>   image,
  __CR_IN__ DecodeHints         hints
    )
{
    MultiFormatReader               delegate;
    GenericMultipleBarcodeReader    reader(delegate);

    return (reader.decodeMultiple(image, hints));
}

/*
---------------------------------------
    ZXing 识别图片
---------------------------------------
*/
static uint_t
read_image (
  __CR_IN__ socket_t                netw,
  __CR_IN__ Ref<LuminanceSource>    source,
  __CR_IN__ bool                    hybrid,
  __CR_IN__ DecodeHintType          type,
  __CR_OT__ sPNT2**                 pnts,
  __CR_OT__ leng_t*                 count
    )
{
    sPNT2                   pt;
    sARRAY                  loc;
    ansi_t*                 str;
    ansi_t*                 tmp;
    ansi_t*                 hex;
    uint_t                  cnt =  0;
    sint_t                  res = -1;
    string                  err_txt = "zxing::error";
    vector<Ref<Result> >    results;

    /* 创建对象 */
    array_initT(&loc, sPNT2);

    try {
        Ref<Binarizer>  binarizer;

        /* 两种不同的二值化方法 */
        if (hybrid)
            binarizer = new HybridBinarizer (source);
        else
            binarizer = new GlobalHistogramBinarizer (source);

        DecodeHints hints(type);

        if (type == 0)
            hints = DecodeHints::DEFAULT_HINT;
        hints.setTryHarder(true);

        Ref<BinaryBitmap>   binary(new BinaryBitmap (binarizer));

        /* 搜索多个码 */
        results = decode_multi(binary, hints);
        res = 0;
    }
    catch (const ReaderException& e) {
        err_txt = "zxing::ReaderException: " + string(e.what());
        res = -2;
    }
    catch (const zxing::IllegalArgumentException& e) {
        err_txt = "zxing::IllegalArgumentException: " + string(e.what());
        res = -3;
    }
    catch (const zxing::Exception& e) {
        err_txt = "zxing::Exception: " + string(e.what());
        res = -4;
    }
    catch (const std::exception& e) {
        err_txt = "std::exception: " + string(e.what());
        res = -5;
    }

    /* 输出结果 */
    if (res != 0) {
        if (netw != NULL)
            netw_cmd_send(netw, err_txt.c_str());
        return (0);
    }
    for (size_t ii = 0; ii < results.size(); ii++) {
        cnt += 1;
        for (int jj = 0; jj < results[ii]->getResultPoints()->size(); jj++) {
            pt.x = (sint_t)(results[ii]->getResultPoints()[jj]->getX());
            pt.y = (sint_t)(results[ii]->getResultPoints()[jj]->getY());
            array_push_growT(&loc, sPNT2, &pt);
        }

        /* 打印结果 */
        if (netw == NULL)
            continue;
        tmp = (ansi_t*)results[ii]->getText()->getText().c_str();
        hex = misc_str2hex(tmp);
        if (hex == NULL) {
            str = str_fmtA("|ZXing| %s: %s",
                            BarcodeFormat::barcodeFormatNames[
                                results[ii]->getBarcodeFormat()], tmp);
        }
        else {
            str = str_fmtA("|ZXing| %s: %s (%s)",
                            BarcodeFormat::barcodeFormatNames[
                                results[ii]->getBarcodeFormat()], tmp, hex);
            mem_free(hex);
        }
        if (str != NULL) {
            netw_cmd_send(netw, str);
            mem_free(str);
        }
    }
    *pnts  = array_get_dataT(&loc, sPNT2);
    *count = array_get_sizeT(&loc, sPNT2);
    return (cnt);
}

/*
=======================================
    ZXing 识别部分代码
=======================================
*/
extern uint_t
zxing_do_decode (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const sIMAGE*   gray,
  __CR_IN__ bool_t          hybrid,
  __CR_IN__ uint_t          type,
  __CR_OT__ sPNT2**         pnts,
  __CR_OT__ leng_t*         count
    )
{
    uint_t  ww, hh, yy;
    byte_t  *dst, *src, *dat;

    *pnts = NULL;
    *count = 0;

    /* 图像数据不需要行对齐 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    dat = (byte_t*)mem_malloc((size_t)ww * hh);
    if (dat == NULL)
        return (0);
    dst = dat;
    src = gray->data;
    for (yy = 0; yy < hh; yy++) {
        mem_cpy(dst, src, ww);
        dst += ww;
        src += gray->bpl;
    }

    Ref<LuminanceSource> source = Ref<LuminanceSource>(
            new GrayImage ((char*)dat, ww, hh));

    return (read_image(netw, source, !!hybrid, type, pnts, count));
}
