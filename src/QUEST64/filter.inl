
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
    quest64_return_string(netw, "quest64_helloworld()");

    /* 信息提示 */
    mess = xml_attr_stringU("message", param);
    if (mess != NULL) {
        MessageBoxA(NULL, mess, "HelloWorld", MB_OK | MB_ICONINFORMATION);
        mem_free(mess);
    }
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
    quest64_return_string(netw, NULL);
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
            sPNT2   pnt;
            ansi_t* shw;
            xpoly_t ply;

            shw = str_fmtA("[%s] %s", dtype, dinfo);
            if (shw != NULL) {
                color.val = 0xFFFFFF00;
                ply = imglab_polys_idx(polys, idx);
                imglab_xpoly_get(&pnt, imglab_xpoly_idx(ply, 1));
                imglab_draw_text(cvmat, shw, pnt.x, pnt.y,
                    FONT_OCV_HERSHEY_PLAIN, 16, color, 2);
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
    TRY_FREE(sr_prototxt);
    TRY_FREE(sr_model);
    if (bar1 == NULL)
        goto _func_out1;

    leng_t  count;
    fp32_t  dnsp_lmt, grad_thr;

    /* 参数解析 */
    dnsp_lmt = xml_attr_fp32U("dnsp_gate", -1, param);
    grad_thr = xml_attr_fp32U("grad_gate", -1, param);
    sr_model = xml_attr_stringU("dect_scales", param);
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
    quest64_return_string(netw, NULL);
    return (TRUE);
}
