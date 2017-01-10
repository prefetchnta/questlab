
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
  __CR_IN__ uint_t                  cpage,
  __CR_OT__ sPNT2**                 pnts,
  __CR_OT__ leng_t*                 count
    )
{
    sPNT2                   pt;
    sARRAY                  loc;
    uint_t                  cnt =  0;
    sint_t                  res = -1;
    vector<Ref<Result> >    results;
    ansi_t                  *str, *tmp;
    string                  cell_result;

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
        cell_result = "zxing::ReaderException: " + string(e.what());
        res = -2;
    }
    catch (const zxing::IllegalArgumentException& e) {
        cell_result = "zxing::IllegalArgumentException: " + string(e.what());
        res = -3;
    }
    catch (const zxing::Exception& e) {
        cell_result = "zxing::Exception: " + string(e.what());
        res = -4;
    }
    catch (const std::exception& e) {
        cell_result = "std::exception: " + string(e.what());
        res = -5;
    }

    /* 输出结果 */
    cmd_shl_send(netw, "txt:clear 0 0");
    if (res != 0) {
        str = str_esc_makeU(cell_result.c_str());
        if (str != NULL) {
            tmp = str_fmtA("info::main=\"0> %s\"", str);
            mem_free(str);
            if (tmp != NULL) {
                cmd_ini_send(netw, tmp);
                mem_free(tmp);
            }
        }
        return (0);
    }
    for (size_t ii = 0; ii < results.size(); ii++) {
        for (int jj = 0; jj < results[ii]->getResultPoints()->size(); jj++) {
            pt.x = (sint_t)(results[ii]->getResultPoints()[jj]->getX());
            pt.y = (sint_t)(results[ii]->getResultPoints()[jj]->getY());
            array_push_growT(&loc, sPNT2, &pt);
        }
        cnt += 1;

        /* 打印结果 */
        str = str_fmtA("%s:%s", BarcodeFormat::barcodeFormatNames[
                                results[ii]->getBarcodeFormat()],
                                results[ii]->getText()->getText());
        if (str == NULL)
            continue;
        tmp = utf8_to_local(cpage, str);
        mem_free(str);
        if (tmp == NULL)
            continue;
        str = str_esc_makeU(tmp);
        mem_free(tmp);
        if (str == NULL)
            continue;
        tmp = str_fmtA("info::main=\"0> %s\"", str);
        mem_free(str);
        if (tmp == NULL)
            continue;
        cmd_ini_send(netw, tmp);
        mem_free(tmp);
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
  __CR_IN__ uint_t          cpage,
  __CR_OT__ sPNT2**         pnts,
  __CR_OT__ leng_t*         count
    )
{
    size_t  sz;
    byte_t  *dst, *src, *dat;
    uint_t  ww, hh, yy, cnt;

    *pnts = NULL;
    *count = 0;

    /* 图像数据不需要行对齐 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    sz = ww * hh;
    dat = (byte_t*)mem_malloc(sz);
    if (dat == NULL)
        return (0);
    dst = dat;
    src = gray->data;
    for (yy = 0; yy < hh; yy++) {
        mem_cpy(dst, src, ww);
        dst += ww;
        src += gray->bpl;
    }
    cnt = read_image(netw, source, !!hybrid,
                type, cpage, pnts, count);
    mem_free(dat);
    return (cnt);
}
