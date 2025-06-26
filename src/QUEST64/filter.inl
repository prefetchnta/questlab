
/*
---------------------------------------
    HelloWorld
---------------------------------------
*/
static bool_t
quest64_helloworld (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sFILL   fill;
    cpix_t  clrs;
    sIMAGE  dest;
    ansi_t* mess;

    /* 图片反色 */
    quest64_set_image(&dest, image);
    fill.dx = fill.dy = 0;
    fill.dw = dest.position.ww;
    fill.dh = dest.position.hh;
    clrs.val = 0x00FFFFFF;
    fill_xor32_c(&dest, &fill, clrs, NULL);
    quest64_setup_return("quest64_helloworld()");

    /* 信息提示 */
    mess = xml_attr_stringU("message", param);
    if (mess != NULL) {
        MessageBoxA(NULL, mess, "HelloWorld", MB_OK | MB_ICONINFORMATION);
        mem_free(mess);
    }
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    OpenCV 级联分类器
---------------------------------------
*/
static bool_t
quest64_ocv_cascade (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    ansi_t* file;

    /* 模型文件 */
    file = xml_attr_stringU("model", param);
    if (file == NULL)
        goto _func_out1;

    cascade_ocv_t   cscd;

    /* 创建级联分类器 */
    cscd = imglab_ocv_cascade_new(file);
    mem_free(file);
    if (cscd == NULL)
        goto _func_out1;

    fp32_t  fscale;
    sint_t  min_nghbrs, flags;
    uint_t  min_width, min_height;
    uint_t  max_width, max_height;

    /* 参数解析 */
    fscale = xml_attr_fp32U("scale", 1.1f, param);
    min_nghbrs = xml_attr_intxU("min_neighbors", 3, param);
    flags = xml_attr_intxU("flags", 2, param);
    min_width = xml_attr_intxU("min_width", 24, param);
    max_width = xml_attr_intxU("max_width", 0, param);
    min_height = xml_attr_intxU("min_height", 24, param);
    max_height = xml_attr_intxU("max_height", 0, param);

    sIMAGE      dest;
    ximage_t    cvmat;
    xrect_lst_t boxes;

    /* 执行级联分类器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out2;
    boxes = imglab_ocv_cascade_doit(cscd, cvmat, fscale, min_nghbrs, flags,
                            min_width, min_height, max_width, max_height);
    if (boxes == NULL)
        goto _func_out3;

    uint_t  count;

    /* 显示分类结果 */
    count = imglab_rects_count(boxes);
    if (count != 0)
    {
        cpix_t  color;

        color.val = 0xFF00FF00;
        imglab_draw_rects(cvmat, boxes, color, 3, 3, 3, -1);
    }
    imglab_rects_del(boxes);
_func_out3:
    imglab_mat_del(cvmat);
_func_out2:
    imglab_ocv_cascade_del(cscd);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    OpenCV 绘制图码结果
---------------------------------------
*/
static void_t
quest64_draw_codes (
  __CR_IN__ ximage_t    cvmat,
  __CR_IN__ str_lstA_t  texts,
  __CR_IN__ xpoly_lst_t polys,
  __CR_IN__ uint_t      count
    )
{
    cpix_t  color, bkcolor;

    bkcolor.val = 0x800000FF;
    for (uint_t idx = 0; idx < count; idx++)
    {
        bool    okay;
        ansi_t* dtype = texts[idx * 2];
        ansi_t* dinfo = texts[idx * 2 + 1];

        if (dtype == NULL || dinfo == NULL) {
            okay = false;
            color.val = 0xFFFF0000;
        }
        else {
            okay = true;
            color.val = 0xFF00FF00;
        }
        imglab_draw_ploys(cvmat, polys, color, 3, idx);
        if (okay)
        {
            ansi_t* shw;
            xpoly_t ply;

            shw = str_fmtA("[%s] %s", dtype, dinfo);
            if (shw != NULL)
            {
                sPNT2   pnt[4];

                color.val = 0xFFFFFF00;
                ply = imglab_polys_idx(polys, idx);
                imglab_xpoly_get(&pnt[0], imglab_xpoly_idx(ply, 0));
                imglab_xpoly_get(&pnt[1], imglab_xpoly_idx(ply, 1));
                imglab_xpoly_get(&pnt[2], imglab_xpoly_idx(ply, 2));
                imglab_xpoly_get(&pnt[3], imglab_xpoly_idx(ply, 3));
                pnt[0].x += pnt[1].x + pnt[2].x + pnt[3].x;
                pnt[0].y += pnt[1].y + pnt[2].y + pnt[3].y;
                imglab_calc_gb2312(&pnt[1], shw, 16);
                pnt[0].x = pnt[0].x / 4 - pnt[1].x / 2;
                pnt[0].y = pnt[0].y / 4 - pnt[1].y / 2;
                imglab_draw_gb2312(cvmat, shw, pnt->x, pnt->y, 16,
                                   CR_BLT_ALP, color, bkcolor);
                mem_free(shw);
            }
        }
    }
}

/*
---------------------------------------
    OpenCV 绘制目标结果
---------------------------------------
*/
static void_t
quest64_draw_objects (
  __CR_IN__ ximage_t            cvmat,
  __CR_IN__ const sIMAGE*       image,
  __CR_IN__ const sRECT_OBJECT* tango,
  __CR_IN__ size_t              count,
  __CR_IN__ const sINIu*        labels
    )
{
    cpix_t  color, bkcolor;

    bkcolor.val = 0x800000FF;
    for (size_t idx = 0; idx < count; idx++)
    {
        sRECT   rct;
        ansi_t* shw;

        color.val = 0xFF00FF00;
        struct_cpy(&rct, &tango[idx].rect, sRECT);
        rct.x1 -= 4; rct.y1 -= 4;
        rct.x2 += 4; rct.y2 += 4;
        rct.ww += 8; rct.hh += 8;
        draw_rect(image, &rct, color, pixel_set32z);
        rct.x1 += 1; rct.y1 += 1;
        rct.x2 -= 1; rct.y2 -= 1;
        rct.ww -= 2; rct.hh -= 2;
        draw_rect(image, &rct, color, pixel_set32z);
        rct.x1 += 1; rct.y1 += 1;
        rct.x2 -= 1; rct.y2 -= 1;
        rct.ww -= 2; rct.hh -= 2;
        draw_rect(image, &rct, color, pixel_set32z);
        rct.x1 += 1; rct.y1 += 1;
        if (labels != NULL &&
            tango[idx].type >= 0 &&
            tango[idx].type < (int)labels->count) {
            shw = str_fmtA("%s %.1f%%", labels->lines[tango[idx].type],
                                        tango[idx].prob * 100);
        }
        else {
            shw = str_fmtA("%d %.1f%%", tango[idx].type,
                                        tango[idx].prob * 100);
        }
        if (shw != NULL) {
            color.val = 0xFFFFFF00;
            imglab_draw_gb2312(cvmat, shw, rct.x1, rct.y1, 16,
                               CR_BLT_ALP, color, bkcolor);
            mem_free(shw);
        }
    }
}

/*
---------------------------------------
    OpenCV 一维码识别器
---------------------------------------
*/
static bool_t
quest64_ocv_barcode (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    ansi_t* sr_model;
    ansi_t* sr_prototxt;

    /* 超分模型文件 */
    sr_model = xml_attr_stringU("sr_model", param);
    sr_prototxt = xml_attr_stringU("sr_prototxt", param);

    barcode_ocv_t   bar1;

    /* 创建一维码识别器 */
    bar1 = imglab_ocv_barcode_new(sr_model, sr_prototxt);
    TRY_FREE(sr_prototxt); TRY_FREE(sr_model);
    if (bar1 == NULL)
        goto _func_out1;

    leng_t  count;
    fp32_t  dnsp_lmt, grad_thr;

    /* 参数解析 */
    dnsp_lmt = xml_attr_fp32U("dnsp_gate", -1, param);
    grad_thr = xml_attr_fp32U("grad_gate", -1, param);
    sr_model = xml_attr_stringU("det_scales", param);
    if (sr_model != NULL)
    {
        uint_t  size_count;
        fp32_t* box_sizes = str2vec2A(&count, sr_model, "[],");

        mem_free(sr_model);
        if (box_sizes != NULL) {
            size_count = (uint_t)count;
            imglab_ocv_barcode_param(bar1, dnsp_lmt, grad_thr,
                                     box_sizes, size_count);
            mem_free(box_sizes);
        }
    }

    sIMAGE      dest;
    ximage_t    cvmat;
    str_lstA_t  texts;
    xpoly_lst_t polys;

    /* 执行一维码识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out2;
    count = (leng_t)imglab_ocv_barcode_doit(bar1, cvmat, &texts, &polys);
    if (count == 0)
        goto _func_out3;

    /* 显示识别结果 */
    quest64_draw_codes(cvmat, texts, polys, (uint_t)count);
    strlst_freeA(texts, count * 2);
    imglab_polys_del(polys);
_func_out3:
    imglab_mat_del(cvmat);
_func_out2:
    imglab_ocv_barcode_del(bar1);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    OpenCV 二维码识别器 (常规)
---------------------------------------
*/
static bool_t
quest64_ocv_qrcode (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    qr2code_ocv_t   qr2d;

    /* 创建二维码识别器 */
    qr2d = imglab_ocv_qr2code_new(QR2D_OCV_TYPE_NORMAL, NULL);
    if (qr2d == NULL)
        goto _func_out1;

    sOCV_QRCodeParam    prms;

    /* 参数解析 */
    prms.epsX = xml_attr_fp64U("eps_x", -1, param);
    prms.epsY = xml_attr_fp64U("eps_y", -1, param);
    prms.useAlignmentMarkers = xml_attr_intxU("use_ali_mrks", TRUE, param);
    imglab_ocv_qr2code_param(qr2d, &prms, sizeof(sOCV_QRCodeParam));

    sIMAGE      dest;
    leng_t      count;
    ximage_t    cvmat;
    str_lstA_t  texts;
    xpoly_lst_t polys;

    /* 执行二维码识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out2;
    count = (leng_t)imglab_ocv_qr2code_doit(qr2d, cvmat, &texts, &polys);
    if (count == 0)
        goto _func_out3;

    /* 显示识别结果 */
    quest64_draw_codes(cvmat, texts, polys, (uint_t)count);
    strlst_freeA(texts, count * 2);
    imglab_polys_del(polys);
_func_out3:
    imglab_mat_del(cvmat);
_func_out2:
    imglab_ocv_qr2code_del(qr2d);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/* 代码简化 */
#define CJSON_INTG(_name, _def) \
    if (!cjson_int(root, #_name, &prms._name)) \
        prms._name = (_def)
#define CJSON_FP32(_name, _def) \
    if (!cjson_fp32(root, #_name, &prms._name)) \
        prms._name = (_def)
#define CJSON_FP64(_name, _def) \
    if (!cjson_fp64(root, #_name, &prms._name)) \
        prms._name = (_def)
#define CJSON_BOOL(_name, _def) \
    if (!cjson_int(root, #_name, &btmp)) \
        prms._name = (_def); \
    else \
        prms._name = (bool_t)btmp
#define CJSON_CSTR(_name, _def) \
    prms._name = cjson_str_dup(root, #_name); \
    if (prms._name == NULL) \
        prms._name = str_dupA(_def)
#define CJSON_STRING(_name) \
    (prms._name = cjson_str_dup(root, #_name))
#define CJSON_VECTOR3(_name) \
    cjson_fvec(root, #_name, prms._name, 3)
/*
---------------------------------------
    OpenCV ARUCO 参数解析
---------------------------------------
*/
static void_t
quest64_ocv_qrcode_load_aruco_params (
  __CR_IN__ qr2code_ocv_t   qr2d,
  __CR_IN__ const ansi_t*   json
    )
{
    cJSON*  root;

    root = cJSON_Parse(json);
    if (root != NULL)
    {
        sOCV_QRCodeArucoParam   prms;

        CJSON_FP32(minModuleSizeInPyramid, 4.0f);
        CJSON_FP32(maxRotation, CR_PI / 12);
        CJSON_FP32(maxModuleSizeMismatch, 1.75f);
        CJSON_FP32(maxTimingPatternMismatch, 2.0f);
        CJSON_FP32(maxPenalties, 0.4f);
        CJSON_FP32(maxColorsMismatch, 0.2f);
        CJSON_FP32(scaleTimingPatternScore, 0.9f);
        imglab_ocv_qr2code_param(qr2d, &prms, sizeof(sOCV_QRCodeArucoParam));
        cJSON_Delete(root);
    }
}

/*
---------------------------------------
    OpenCV ARUCO 检测参数解析
---------------------------------------
*/
static void_t
quest64_ocv_qrcode_load_aruco_det_params (
  __CR_IN__ qr2code_ocv_t   qr2d,
  __CR_IN__ const ansi_t*   json
    )
{
    cJSON*  root;

    root = cJSON_Parse(json);
    if (root != NULL)
    {
        sint_t                      btmp;
        sOCV_QRCodeArucoDetector    prms;

        CJSON_INTG(adaptiveThreshWinSizeMin, 3);
        CJSON_INTG(adaptiveThreshWinSizeMax, 23);
        CJSON_INTG(adaptiveThreshWinSizeStep, 10);
        CJSON_FP64(adaptiveThreshConstant, 7.0);
        CJSON_FP64(minMarkerPerimeterRate, 0.03);
        CJSON_FP64(maxMarkerPerimeterRate, 4.0);
        CJSON_FP64(polygonalApproxAccuracyRate, 0.03);
        CJSON_FP64(minCornerDistanceRate, 0.05);
        CJSON_INTG(minDistanceToBorder, 3);
        CJSON_FP64(minMarkerDistanceRate, 0.125);
        CJSON_FP32(minGroupDistance, 0.21f);
        CJSON_INTG(cornerRefinementMethod, QR2D_OCV_CORNER_REFINE_NONE);
        CJSON_INTG(cornerRefinementWinSize, 5);
        CJSON_FP32(relativeCornerRefinmentWinSize, 0.3f);
        CJSON_INTG(cornerRefinementMaxIterations, 30);
        CJSON_FP64(cornerRefinementMinAccuracy, 0.1);
        CJSON_INTG(markerBorderBits, 1);
        CJSON_INTG(perspectiveRemovePixelPerCell, 4);
        CJSON_FP64(perspectiveRemoveIgnoredMarginPerCell, 0.13);
        CJSON_FP64(maxErroneousBitsInBorderRate, 0.35);
        CJSON_FP64(minOtsuStdDev, 5.0);
        CJSON_FP64(errorCorrectionRate, 0.6);
        CJSON_FP32(aprilTagQuadDecimate, 0.0f);
        CJSON_FP32(aprilTagQuadSigma, 0.0f);
        CJSON_INTG(aprilTagMinClusterPixels, 5);
        CJSON_INTG(aprilTagMaxNmaxima, 10);
        CJSON_FP32(aprilTagCriticalRad, 10 * CR_PI / 180);
        CJSON_FP32(aprilTagMaxLineFitMse, 10.0f);
        CJSON_INTG(aprilTagMinWhiteBlackDiff, 5);
        CJSON_INTG(aprilTagDeglitch, 0);
        CJSON_BOOL(detectInvertedMarker, FALSE);
        CJSON_BOOL(useAruco3Detection, FALSE);
        CJSON_INTG(minSideLengthCanonicalImg, 32);
        CJSON_FP32(minMarkerLengthRatioOriginalImg, 0.0f);
        imglab_ocv_qr2code_param(qr2d, &prms, sizeof(sOCV_QRCodeArucoDetector));
        cJSON_Delete(root);
    }
}

/*
---------------------------------------
    OpenCV 二维码识别器 (ARUCO)
---------------------------------------
*/
static bool_t
quest64_ocv_qrcode_aruco (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    qr2code_ocv_t   qr2d;

    /* 创建二维码识别器 */
    qr2d = imglab_ocv_qr2code_new(QR2D_OCV_TYPE_ARUCOX, NULL);
    if (qr2d == NULL)
        goto _func_out1;

    /* 参数解析 */
    ansi_t* file = xml_attr_stringU("params", param);
    if (file != NULL) {
        ansi_t* json = file_load_as_strA(file);
        mem_free(file);
        if (json != NULL) {
            quest64_ocv_qrcode_load_aruco_params(qr2d, json);
            mem_free(json);
        }
    }
    file = xml_attr_stringU("det_params", param);
    if (file != NULL) {
        ansi_t* json = file_load_as_strA(file);
        mem_free(file);
        if (json != NULL) {
            quest64_ocv_qrcode_load_aruco_det_params(qr2d, json);
            mem_free(json);
        }
    }

    sIMAGE      dest;
    leng_t      count;
    ximage_t    cvmat;
    str_lstA_t  texts;
    xpoly_lst_t polys;

    /* 执行二维码识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out2;
    count = (leng_t)imglab_ocv_qr2code_doit(qr2d, cvmat, &texts, &polys);
    if (count == 0)
        goto _func_out3;

    /* 显示识别结果 */
    quest64_draw_codes(cvmat, texts, polys, (uint_t)count);
    strlst_freeA(texts, count * 2);
    imglab_polys_del(polys);
_func_out3:
    imglab_mat_del(cvmat);
_func_out2:
    imglab_ocv_qr2code_del(qr2d);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    OpenCV 二维码识别器 (微信)
---------------------------------------
*/
static bool_t
quest64_ocv_qrcode_wechat (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sOCV_QRCodeWeChatParam  prms;

    /* 参数解析 */
    prms.fscale = xml_attr_fp32U("scale", -1, param);
    prms.sr_model = xml_attr_stringU("sr_model", param);
    prms.sr_prototxt = xml_attr_stringU("sr_prototxt", param);
    prms.det_model = xml_attr_stringU("det_model", param);
    prms.det_prototxt = xml_attr_stringU("det_prototxt", param);

    qr2code_ocv_t   qr2d;

    /* 创建二维码识别器 */
    if (prms.sr_model != NULL && prms.sr_prototxt != NULL &&
        prms.det_model != NULL && prms.det_prototxt != NULL)
        qr2d = imglab_ocv_qr2code_new(QR2D_OCV_TYPE_WECHAT, &prms);
    else
        qr2d = imglab_ocv_qr2code_new(QR2D_OCV_TYPE_WECHAT, NULL);
    TRY_FREE(prms.det_prototxt); TRY_FREE(prms.det_model);
    TRY_FREE(prms.sr_prototxt); TRY_FREE(prms.sr_model);
    if (qr2d == NULL)
        goto _func_out1;
    imglab_ocv_qr2code_param(qr2d, &prms, sizeof(sOCV_QRCodeWeChatParam));

    sIMAGE      dest;
    leng_t      count;
    ximage_t    cvmat;
    str_lstA_t  texts;
    xpoly_lst_t polys;

    /* 执行二维码识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out2;
    count = (leng_t)imglab_ocv_qr2code_doit(qr2d, cvmat, &texts, &polys);
    if (count == 0)
        goto _func_out3;

    /* 显示识别结果 */
    quest64_draw_codes(cvmat, texts, polys, (uint_t)count);
    strlst_freeA(texts, count * 2);
    imglab_polys_del(polys);
_func_out3:
    imglab_mat_del(cvmat);
_func_out2:
    imglab_ocv_qr2code_del(qr2d);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    ZXing 参数解析
---------------------------------------
*/
static void_t
quest64_zxi_grpcode_load_options (
  __CR_OT__ sZXI_ReaderOptions& prms,
  __CR_IN__ const ansi_t*       json
    )
{
    cJSON*  root;

    root = cJSON_Parse(json);
    if (root != NULL)
    {
        sint_t  btmp;

        CJSON_BOOL(tryHarder, TRUE);
        CJSON_BOOL(tryRotate, TRUE);
        CJSON_BOOL(tryInvert, TRUE);
        CJSON_BOOL(tryDownscale, TRUE);
        CJSON_BOOL(isPure, FALSE);
        CJSON_BOOL(tryCode39ExtendedMode, FALSE);
        CJSON_BOOL(returnErrors, FALSE);
        CJSON_INTG(downscaleFactor, 3);
        CJSON_INTG(eanAddOnSymbol, ZXI_EAN_IGNORE);
        CJSON_INTG(binarizer, ZXI_BIN_LOCAL_AVERAGE);
        CJSON_INTG(textMode, ZXI_TEXT_HRI);
        CJSON_CSTR(characterSet, "Unknown");
        CJSON_INTG(minLineCount, 2);
        CJSON_INTG(maxNumberOfSymbols, 255);
        CJSON_INTG(downscaleThreshold, 500);
        CJSON_INTG(formats, ZXI_TYPE_NONE);
        cJSON_Delete(root);
    }
}

/*
---------------------------------------
    ZXing 图形码识别器
---------------------------------------
*/
static bool_t
quest64_zxi_grpcode (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sZXI_ReaderOptions  prms;

    /* 参数解析 */
    prms.characterSet = NULL;
    ansi_t* file = xml_attr_stringU("params", param);
    if (file != NULL) {
        ansi_t* json = file_load_as_strA(file);
        mem_free(file);
        if (json != NULL) {
            quest64_zxi_grpcode_load_options(prms, json);
            mem_free(json);
        }
    }

    sIMAGE      dest;
    leng_t      count;
    ximage_t    cvmat;
    str_lstA_t  texts;
    xpoly_lst_t polys;

    /* 执行图形码识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out1;
    count = (leng_t)imglab_zxi_grpcode_doit(cvmat, &texts, &polys, &prms);
    if (count == 0)
        goto _func_out2;

    /* 显示识别结果 */
    quest64_draw_codes(cvmat, texts, polys, (uint_t)count);
    strlst_freeA(texts, count * 2);
    imglab_polys_del(polys);
_func_out2:
    imglab_mat_del(cvmat);
_func_out1:
    TRY_FREE(prms.characterSet);
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    NCNN NanoDet 参数解析
---------------------------------------
*/
static bool_t
quest64_ncnn_nanodet_load_params (
  __CR_OT__ sNCNN_NanoDetParam& prms,
  __CR_IN__ const ansi_t*       json
    )
{
    cJSON*  root;
    sint_t  btmp;

    root = cJSON_Parse(json);
    if (root == NULL)
        return (FALSE);
    if (CJSON_STRING(input_layer) == NULL)
        goto _failure1;
    if (CJSON_STRING(cls_pred8) == NULL)
        goto _failure2;
    if (CJSON_STRING(dis_pred8) == NULL)
        goto _failure3;
    if (CJSON_STRING(cls_pred16) == NULL)
        goto _failure4;
    if (CJSON_STRING(dis_pred16) == NULL)
        goto _failure5;
    if (CJSON_STRING(cls_pred32) == NULL)
        goto _failure6;
    if (CJSON_STRING(dis_pred32) == NULL)
        goto _failure7;
    CJSON_INTG(thread_num, 0);
    CJSON_BOOL(light_mode, FALSE);
    CJSON_INTG(target_size, 320);
    CJSON_FP32(prob_threshold, 0.4f);
    CJSON_FP32(nms_threshold, 0.5f);
    if (!CJSON_VECTOR3(mean_vals))
        prms.mean_vals[0] = prms.mean_vals[1] = prms.mean_vals[2] = -1;
    if (!CJSON_VECTOR3(norm_vals))
        prms.norm_vals[0] = prms.norm_vals[1] = prms.norm_vals[2] = -1;
    cJSON_Delete(root);
    return (TRUE);

_failure7:
    mem_free(prms.cls_pred32);
_failure6:
    mem_free(prms.dis_pred16);
_failure5:
    mem_free(prms.cls_pred16);
_failure4:
    mem_free(prms.dis_pred8);
_failure3:
    mem_free(prms.cls_pred8);
_failure2:
    mem_free(prms.input_layer);
_failure1:
    cJSON_Delete(root);
    return (FALSE);
}

/*
---------------------------------------
    NCNN NanoDet 识别器
---------------------------------------
*/
static bool_t
quest64_ncnn_nanodet (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sINIu*  line;
    uint_t  ngpu;
    uint_t  bprm;
    uint_t  vlkn;
    uint_t  bf16;
    ansi_t* text;
    ansi_t* file;
    ansi_t* name;

    /* 参数解析 */
    name = xml_attr_stringU("model", param);
    if (name == NULL)
        goto _func_out1;
    line = NULL;
    ngpu = xml_attr_intxU("vk_gpu", 0-1UL, param);
    bprm = xml_attr_intxU("bparam", FALSE, param);
    vlkn = xml_attr_intxU("vulkan", TRUE, param);
    bf16 = xml_attr_intxU("float16", FALSE, param);

    sNCNN_NanoDetParam  prms;

    file = xml_attr_stringU("params", param);
    if (file == NULL)
        goto _func_out2;
    text = file_load_as_strA(file);
    mem_free(file);
    if (text == NULL)
        goto _func_out2;
    if (!quest64_ncnn_nanodet_load_params(prms, text)) {
        mem_free(text);
        goto _func_out2;
    }
    mem_free(text);

    /* 加载标签文件 */
    file = str_fmtA("%s.label", name);
    if (file != NULL) {
        text = file_load_as_strA(file);
        mem_free(file);
        if (text != NULL) {
            line = ini_parseU(text);
            mem_free(text);
        }
    }

    nanodet_ncnn_t  nndt;

    nndt = imglab_ncnn_nanodet_new(ngpu);
    if (nndt == NULL)
        goto _func_out3;
    if (!imglab_ncnn_nanodet_load(nndt, name, !!bprm, !!vlkn, !!bf16))
        goto _func_out4;

    sIMAGE      dest;
    ximage_t    cvmat;

    /* 执行 NanoDet 识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out4;

    size_t          cnts;
    sRECT_OBJECT*   objs;

    objs = imglab_ncnn_nanodet_doit(nndt, cvmat, &prms, &cnts);
    if (objs == NULL)
        goto _func_out5;

    /* 显示识别结果 */
    quest64_draw_objects(cvmat, &dest, objs, cnts, line);
    mem_free(objs);
_func_out5:
    imglab_mat_del(cvmat);
_func_out4:
    imglab_ncnn_nanodet_del(nndt);
_func_out3:
    if (line != NULL)
        ini_closeU(line);
    mem_free(prms.input_layer);
    mem_free(prms.cls_pred8);
    mem_free(prms.cls_pred16);
    mem_free(prms.cls_pred32);
    mem_free(prms.dis_pred8);
    mem_free(prms.dis_pred16);
    mem_free(prms.dis_pred32);
_func_out2:
    mem_free(name);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    NCNN NanoDetPlus 参数解析
---------------------------------------
*/
static bool_t
quest64_ncnn_nanodet_plus_load_params (
  __CR_OT__ sNCNN_NanoDetPlusParam& prms,
  __CR_IN__ const ansi_t*           json
    )
{
    cJSON*  root;
    sint_t  btmp;

    root = cJSON_Parse(json);
    if (root == NULL)
        return (FALSE);
    if (CJSON_STRING(input_layer) == NULL)
        goto _failure1;
    if (CJSON_STRING(pred8) == NULL)
        goto _failure2;
    if (CJSON_STRING(pred16) == NULL)
        goto _failure3;
    if (CJSON_STRING(pred32) == NULL)
        goto _failure4;
    if (CJSON_STRING(pred64) == NULL)
        goto _failure5;
    CJSON_INTG(thread_num, 0);
    CJSON_BOOL(light_mode, FALSE);
    CJSON_INTG(target_size, 416);
    CJSON_INTG(num_class, 80);
    CJSON_BOOL(have_sigmoid, TRUE);
    CJSON_FP32(prob_threshold, 0.4f);
    CJSON_FP32(nms_threshold, 0.5f);
    if (!CJSON_VECTOR3(mean_vals))
        prms.mean_vals[0] = prms.mean_vals[1] = prms.mean_vals[2] = -1;
    if (!CJSON_VECTOR3(norm_vals))
        prms.norm_vals[0] = prms.norm_vals[1] = prms.norm_vals[2] = -1;
    cJSON_Delete(root);
    return (TRUE);

_failure5:
    mem_free(prms.pred32);
_failure4:
    mem_free(prms.pred16);
_failure3:
    mem_free(prms.pred8);
_failure2:
    mem_free(prms.input_layer);
_failure1:
    cJSON_Delete(root);
    return (FALSE);
}

/*
---------------------------------------
    NCNN NanoDetPlus 识别器
---------------------------------------
*/
static bool_t
quest64_ncnn_nanodet_plus (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sINIu*  line;
    uint_t  ngpu;
    uint_t  bprm;
    uint_t  vlkn;
    uint_t  bf16;
    ansi_t* text;
    ansi_t* file;
    ansi_t* name;

    /* 参数解析 */
    name = xml_attr_stringU("model", param);
    if (name == NULL)
        goto _func_out1;
    line = NULL;
    ngpu = xml_attr_intxU("vk_gpu", 0-1UL, param);
    bprm = xml_attr_intxU("bparam", FALSE, param);
    vlkn = xml_attr_intxU("vulkan", TRUE, param);
    bf16 = xml_attr_intxU("float16", FALSE, param);

    sNCNN_NanoDetPlusParam  prms;

    file = xml_attr_stringU("params", param);
    if (file == NULL)
        goto _func_out2;
    text = file_load_as_strA(file);
    mem_free(file);
    if (text == NULL)
        goto _func_out2;
    if (!quest64_ncnn_nanodet_plus_load_params(prms, text)) {
        mem_free(text);
        goto _func_out2;
    }
    mem_free(text);

    /* 加载标签文件 */
    file = str_fmtA("%s.label", name);
    if (file != NULL) {
        text = file_load_as_strA(file);
        mem_free(file);
        if (text != NULL) {
            line = ini_parseU(text);
            mem_free(text);
        }
    }

    nanodet_ncnn_t  nndt;

    nndt = imglab_ncnn_nanodet_new(ngpu);
    if (nndt == NULL)
        goto _func_out3;
    if (!imglab_ncnn_nanodet_load(nndt, name, !!bprm, !!vlkn, !!bf16))
        goto _func_out4;

    sIMAGE      dest;
    ximage_t    cvmat;

    /* 执行 NanoDet 识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out4;

    size_t          cnts;
    sRECT_OBJECT*   objs;

    objs = imglab_ncnn_nanodet_plus_doit(nndt, cvmat, &prms, &cnts);
    if (objs == NULL)
        goto _func_out5;

    /* 显示识别结果 */
    quest64_draw_objects(cvmat, &dest, objs, cnts, line);
    mem_free(objs);
_func_out5:
    imglab_mat_del(cvmat);
_func_out4:
    imglab_ncnn_nanodet_del(nndt);
_func_out3:
    if (line != NULL)
        ini_closeU(line);
    mem_free(prms.input_layer);
    mem_free(prms.pred8);
    mem_free(prms.pred16);
    mem_free(prms.pred32);
    mem_free(prms.pred64);
_func_out2:
    mem_free(name);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    NCNN MobileNetSSD 参数解析
---------------------------------------
*/
static bool_t
quest64_ncnn_mbntssd_load_params (
  __CR_OT__ sNCNN_MobileNetSSD_Param&   prms,
  __CR_IN__ const ansi_t*               json
    )
{
    cJSON*  root;
    sint_t  btmp;

    root = cJSON_Parse(json);
    if (root == NULL)
        return (FALSE);
    if (CJSON_STRING(input_layer) == NULL)
        goto _failure1;
    if (CJSON_STRING(output_layer) == NULL)
        goto _failure2;
    CJSON_INTG(thread_num, 0);
    CJSON_BOOL(light_mode, FALSE);
    CJSON_INTG(target_size, 300);
    CJSON_INTG(net_version, 1);
    CJSON_FP32(prob_threshold, 0.6f);
    if (!CJSON_VECTOR3(mean_vals))
        prms.mean_vals[0] = prms.mean_vals[1] = prms.mean_vals[2] = -1;
    if (!CJSON_VECTOR3(norm_vals))
        prms.norm_vals[0] = prms.norm_vals[1] = prms.norm_vals[2] = -1;
    cJSON_Delete(root);
    return (TRUE);

_failure2:
    mem_free(prms.input_layer);
_failure1:
    cJSON_Delete(root);
    return (FALSE);
}

/*
---------------------------------------
    NCNN MobileNetSSD 识别器
---------------------------------------
*/
static bool_t
quest64_ncnn_mbntssd (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sINIu*  line;
    uint_t  ngpu;
    uint_t  bprm;
    uint_t  vlkn;
    uint_t  bf16;
    ansi_t* text;
    ansi_t* file;
    ansi_t* name;
    ansi_t* noop;

    /* 参数解析 */
    name = xml_attr_stringU("model", param);
    if (name == NULL)
        goto _func_out1;
    line = NULL;
    noop = xml_attr_stringU("silence", param);
    ngpu = xml_attr_intxU("vk_gpu", 0-1UL, param);
    bprm = xml_attr_intxU("bparam", FALSE, param);
    vlkn = xml_attr_intxU("vulkan", TRUE, param);
    bf16 = xml_attr_intxU("float16", FALSE, param);

    sNCNN_MobileNetSSD_Param    prms;

    file = xml_attr_stringU("params", param);
    if (file == NULL)
        goto _func_out2;
    text = file_load_as_strA(file);
    mem_free(file);
    if (text == NULL)
        goto _func_out2;
    if (!quest64_ncnn_mbntssd_load_params(prms, text)) {
        mem_free(text);
        goto _func_out2;
    }
    mem_free(text);

    /* 加载标签文件 */
    file = str_fmtA("%s.label", name);
    if (file != NULL) {
        text = file_load_as_strA(file);
        mem_free(file);
        if (text != NULL) {
            line = ini_parseU(text);
            mem_free(text);
        }
    }

    mbntssd_ncnn_t  nndt;

    nndt = imglab_ncnn_mbntssd_new(ngpu);
    if (nndt == NULL)
        goto _func_out3;
    if (noop == NULL) {
        if (!imglab_ncnn_mbntssd_load(nndt, name, "silence",
                                    !!bprm, !!vlkn, !!bf16))
            goto _func_out4;
    }
    else {
        if (!imglab_ncnn_mbntssd_load(nndt, name, noop,
                                    !!bprm, !!vlkn, !!bf16))
            goto _func_out4;
    }

    sIMAGE      dest;
    ximage_t    cvmat;

    /* 执行 MobileNetSSD 识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out4;

    size_t          cnts;
    sRECT_OBJECT*   objs;

    objs = imglab_ncnn_mbntssd_doit(nndt, cvmat, &prms, &cnts);
    if (objs == NULL)
        goto _func_out5;

    /* 显示识别结果 */
    quest64_draw_objects(cvmat, &dest, objs, cnts, line);
    mem_free(objs);
_func_out5:
    imglab_mat_del(cvmat);
_func_out4:
    imglab_ncnn_mbntssd_del(nndt);
_func_out3:
    if (line != NULL)
        ini_closeU(line);
    mem_free(prms.input_layer);
    mem_free(prms.output_layer);
_func_out2:
    TRY_FREE(noop);
    mem_free(name);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}

/*
---------------------------------------
    NCNN YOLO 参数解析
---------------------------------------
*/
static bool_t
quest64_ncnn_yolo_load_params (
  __CR_OT__ sNCNN_YOLO_Param&   prms,
  __CR_IN__ const ansi_t*       json
    )
{
    cJSON*  root;
    sint_t  btmp;

    root = cJSON_Parse(json);
    if (root == NULL)
        return (FALSE);
    if (CJSON_STRING(input_layer) == NULL)
        goto _failure1;
    if (CJSON_STRING(output_layer) == NULL)
        goto _failure2;
    CJSON_INTG(thread_num, 0);
    CJSON_BOOL(light_mode, FALSE);
    CJSON_INTG(target_size, 416);
    CJSON_INTG(yolo_version, 2);
    CJSON_FP32(prob_threshold, 0.6f);
    if (!CJSON_VECTOR3(mean_vals))
        prms.mean_vals[0] = prms.mean_vals[1] = prms.mean_vals[2] = -1;
    if (!CJSON_VECTOR3(norm_vals))
        prms.norm_vals[0] = prms.norm_vals[1] = prms.norm_vals[2] = -1;
    cJSON_Delete(root);
    return (TRUE);

_failure2:
    mem_free(prms.input_layer);
_failure1:
    cJSON_Delete(root);
    return (FALSE);
}

/*
---------------------------------------
    NCNN YOLO 识别器
---------------------------------------
*/
static bool_t
quest64_ncnn_yolo (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sINIu*  line;
    uint_t  ngpu;
    uint_t  bprm;
    uint_t  vlkn;
    uint_t  bf16;
    ansi_t* text;
    ansi_t* file;
    ansi_t* name;
    ansi_t* noop;

    /* 参数解析 */
    name = xml_attr_stringU("model", param);
    if (name == NULL)
        goto _func_out1;
    line = NULL;
    noop = xml_attr_stringU("v5focus", param);
    ngpu = xml_attr_intxU("vk_gpu", 0-1UL, param);
    bprm = xml_attr_intxU("bparam", FALSE, param);
    vlkn = xml_attr_intxU("vulkan", TRUE, param);
    bf16 = xml_attr_intxU("float16", FALSE, param);

    sNCNN_YOLO_Param    prms;

    file = xml_attr_stringU("params", param);
    if (file == NULL)
        goto _func_out2;
    text = file_load_as_strA(file);
    mem_free(file);
    if (text == NULL)
        goto _func_out2;
    if (!quest64_ncnn_yolo_load_params(prms, text)) {
        mem_free(text);
        goto _func_out2;
    }
    mem_free(text);

    /* 加载标签文件 */
    file = str_fmtA("%s.label", name);
    if (file != NULL) {
        text = file_load_as_strA(file);
        mem_free(file);
        if (text != NULL) {
            line = ini_parseU(text);
            mem_free(text);
        }
    }

    yolo_ncnn_t nndt;

    nndt = imglab_ncnn_yolo_new(ngpu);
    if (nndt == NULL)
        goto _func_out3;
    if (noop == NULL) {
        if (!imglab_ncnn_yolo_load(nndt, name, "YoloV5Focus",
                                    !!bprm, !!vlkn, !!bf16))
            goto _func_out4;
    }
    else {
        if (!imglab_ncnn_yolo_load(nndt, name, noop, !!bprm, !!vlkn, !!bf16))
            goto _func_out4;
    }

    sIMAGE      dest;
    ximage_t    cvmat;

    /* 执行 YOLO 识别器 */
    quest64_set_image(&dest, image);
    cvmat = imglab_crh2mat_set(&dest);
    if (cvmat == NULL)
        goto _func_out4;

    size_t          cnts;
    sRECT_OBJECT*   objs;

    objs = imglab_ncnn_yolo_doit(nndt, cvmat, &prms, &cnts);
    if (objs == NULL)
        goto _func_out5;

    /* 显示识别结果 */
    quest64_draw_objects(cvmat, &dest, objs, cnts, line);
    mem_free(objs);
_func_out5:
    imglab_mat_del(cvmat);
_func_out4:
    imglab_ncnn_yolo_del(nndt);
_func_out3:
    if (line != NULL)
        ini_closeU(line);
    mem_free(prms.input_layer);
    mem_free(prms.output_layer);
_func_out2:
    TRY_FREE(noop);
    mem_free(name);
_func_out1:
    CR_NOUSE(netw);
    return (TRUE);
}
