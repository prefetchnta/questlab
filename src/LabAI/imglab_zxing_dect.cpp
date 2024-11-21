/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2024-11-12
 *              #
 ================================================
        ImageLAB ZXing 目标识别函数库
 ================================================
 */

#include "zxing/ReadBarcode.h"
#include "imglab_int.hpp"

/*
=======================================
    执行图形码的识别
=======================================
*/
CR_API size_t
imglab_zxi_grpcode_doit (
  __CR_IN__ ximage_t                    mat,
  __CR_OT__ str_lstA_t*                 text,
  __CR_OT__ xpoly_lst_t*                list,
  __CR_IN__ const sZXI_ReaderOptions*   options
    )
{
    ZXing::ReaderOptions    param;

    // 参数配置
    if (options != NULL) {
        param.setTryHarder(!!options->tryHarder);
        param.setTryRotate(!!options->tryRotate);
        param.setTryInvert(!!options->tryInvert);
        param.setTryDownscale(!!options->tryDownscale);
        param.setIsPure(!!options->isPure);
        param.setTryCode39ExtendedMode(!!options->tryCode39ExtendedMode);
        param.setValidateCode39CheckSum(!!options->validateCode39CheckSum);
        param.setValidateITFCheckSum(!!options->validateITFCheckSum);
        param.setReturnCodabarStartEnd(!!options->returnCodabarStartEnd);
        param.setReturnErrors(!!options->returnErrors);
        param.setDownscaleFactor((uint8_t)options->downscaleFactor);
        switch (options->eanAddOnSymbol)
        {
            default: break;
            case ZXI_EAN_IGNORE:
                param.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Ignore);
                break;
            case ZXI_EAN_READ:
                param.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);
                break;
            case ZXI_EAN_REQUIRE:
                param.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Require);
                break;
        }
        switch (options->binarizer)
        {
            default: break;
            case ZXI_BIN_LOCAL_AVERAGE:
                param.setBinarizer(ZXing::Binarizer::LocalAverage);
                break;
            case ZXI_BIN_GLOBAL_HISTOGRAM:
                param.setBinarizer(ZXing::Binarizer::GlobalHistogram);
                break;
            case ZXI_BIN_FIXED_THRESHOLD:
                param.setBinarizer(ZXing::Binarizer::FixedThreshold);
                break;
            case ZXI_BIN_BOOL_CAST:
                param.setBinarizer(ZXing::Binarizer::BoolCast);
                break;
        }
        switch (options->textMode)
        {
            default: break;
            case ZXI_TEXT_PLAIN:
                param.setTextMode(ZXing::TextMode::Plain);
                break;
            case ZXI_TEXT_ECI:
                param.setTextMode(ZXing::TextMode::ECI);
                break;
            case ZXI_TEXT_HRI:
                param.setTextMode(ZXing::TextMode::HRI);
                break;
            case ZXI_TEXT_HEX:
                param.setTextMode(ZXing::TextMode::Hex);
                break;
            case ZXI_TEXT_ESCAPED:
                param.setTextMode(ZXing::TextMode::Escaped);
                break;
        }
        if (options->characterSet != NULL)
            param.setCharacterSet(options->characterSet);
        param.setMinLineCount((uint8_t)options->minLineCount);
        param.setMaxNumberOfSymbols((uint8_t)options->maxNumberOfSymbols);
        param.setDownscaleThreshold((uint16_t)options->downscaleThreshold);
        param.setFormats(static_cast<ZXing::BarcodeFormat>(options->formats));
    }

    cv::Mat*            mm = (cv::Mat*)mat;
    ZXing::ImageFormat  fmt = ZXing::ImageFormat::None;

    // 执行识别
    switch (mm->channels()) {
        default: break;
        case 1: fmt = ZXing::ImageFormat::Lum;  break;
        case 3: fmt = ZXing::ImageFormat::BGR;  break;
        case 4: fmt = ZXing::ImageFormat::BGRX; break;
    }
    if (mm->depth() != CV_8U || fmt == ZXing::ImageFormat::None)
        return (0);

    ZXing::ImageView    img (mm->data, mm->cols, mm->rows, fmt);
    ZXing::Results      rets = ZXing::ReadBarcodes(img, param);

    size_t  cnts = rets.size();

    // 返回结果
    if (cnts != 0)
    {
        // 处理每一个识别结果
        if (list != NULL)
        {
            std::vector<cv::Point>  pnts;
            auto zx2cv = [](ZXing::PointI p) { return cv::Point(p.x, p.y); };

            // 返回位置
            for (size_t idx = 0; idx < cnts; idx++)
            {
                auto    pos = rets[idx].position();

                pnts.push_back(zx2cv(pos[0]));
                pnts.push_back(zx2cv(pos[1]));
                pnts.push_back(zx2cv(pos[2]));
                pnts.push_back(zx2cv(pos[3]));
            }
            *list = imglab_split_to_xpoly(&pnts, 4);
        }

        // 返回文本
        if (text != NULL) {
            *text = mem_talloc(cnts * 2, ansi_t*);
            if (*text != NULL) {
                for (size_t idx = 0; idx < cnts; idx++)
                {
                    auto    dtype = ZXing::ToString(rets[idx].format());
                    auto    dinfo = rets[idx].text();

                    (*text)[idx * 2 + 0] = str_dupA(dtype.c_str());
                    (*text)[idx * 2 + 1] = str_dupA(dinfo.c_str());
                }
            }
        }
    }
    return (cnts);
}
