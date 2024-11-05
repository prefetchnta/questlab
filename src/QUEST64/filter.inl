
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
    for (uint_t idx = 0; idx < count; idx++)
    {
        bool    okay;
        cpix_t  color;
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
                imglab_text_size(&pnt[1], shw, FONT_OCV_HERSHEY_DUPLEX, 12, 1);
                pnt[0].x = pnt[0].x / 4 - pnt[1].x / 2;
                pnt[0].y = pnt[0].y / 4 + pnt[1].y / 2;
                imglab_draw_text(cvmat, shw, pnt->x, pnt->y, FONT_OCV_HERSHEY_DUPLEX, 12, color, 1);
                mem_free(shw);
            }
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
    OpenCV ARUCO 监测参数解析
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

    ansi_t* file;
    ansi_t* json;

    /* 参数解析 */
    file = xml_attr_stringU("params", param);
    if (file != NULL) {
        json = file_load_as_strA(file);
        mem_free(file);
        if (json != NULL) {
            quest64_ocv_qrcode_load_aruco_params(qr2d, json);
            mem_free(json);
        }
    }
    file = xml_attr_stringU("det_params", param);
    if (file != NULL) {
        json = file_load_as_strA(file);
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
