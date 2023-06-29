
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
    uint_t      count;
    cpix_t      color;
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

    /* 显示分类结果 */
    count = imglab_rects_count(boxes);
    if (count != 0) {
        color.val = 0xFF00FF00;
        imglab_draw_rects(cvmat, boxes, color, 3, 3, 3);
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
