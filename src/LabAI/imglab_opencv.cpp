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
 *             ##       CREATE: 2018-03-05
 *              #
 ================================================
        图像 LAB OpenCV 基础函数库
 ================================================
 */

#include "imglab_int.hpp"

/*****************************************************************************/
/*                                 结构转换                                  */
/*****************************************************************************/

/*
=======================================
    释放 OpenCV 矩阵
=======================================
*/
CR_API void_t
imglab_mat_del (
  __CR_IN__ ximage_t    mat
    )
{
    cv::Mat*    mm;

    mm = (cv::Mat*)mat;
    delete mm;
}

/*
=======================================
    释放 sIMAGE 图片
=======================================
*/
CR_API void_t
imglab_crh_del (
  __CR_IN__ const sIMAGE*   img
    )
{
    image_del(img);
}

/*
=======================================
    OpenCV 矩阵生成 sIMAGE
=======================================
*/
CR_API sIMAGE*
imglab_mat2crh_dup (
  __CR_IN__ ximage_t    mat
    )
{
    uint_t      ww;
    uint_t      hh;
    uint_t      tt;
    cv::Mat*    mm;

    // 抽取图片参数
    mm = (cv::Mat*)mat;
    tt = mm->depth();
    if (tt != CV_8U && tt != CV_8S)
        return (NULL);
    switch (mm->channels())
    {
        default: return (NULL);
        case 1: tt = CR_INDEX8; break;
        case 3: tt = CR_ARGB888; break;
        case 4: tt = CR_ARGB8888; break;
    }
    ww = mm->cols;
    hh = mm->rows;

    sIMAGE* tp;

    // 创建接收图片
    tp = image_new(0, 0, ww, hh, tt);
    if (tp == NULL)
        return (NULL);

    leng_t  bpl;
    byte_t* lne;

    // 接收图片数据
    lne = tp->data;
    bpl = tp->bpc * ww;
    for (uint_t yy = 0; yy < hh; yy++) {
        mem_cpy(lne, mm->ptr<byte_t>(yy), bpl);
        lne += tp->bpl;
    }
    if (tt == CR_INDEX8)
        pal_set_gray8(tp->pal);
    return (tp);
}

/*
=======================================
    OpenCV 矩阵填充 sIMAGE (浅复制)
=======================================
*/
CR_API bool_t
imglab_mat2crh_set (
  __CR_OT__ sIMAGE*     img,
  __CR_IN__ ximage_t    mat
    )
{
    uint_t      ww;
    uint_t      hh;
    uint_t      tt;
    cv::Mat*    mm;

    // 抽取图片参数
    mm = (cv::Mat*)mat;
    tt = mm->depth();
    if (tt != CV_8U && tt != CV_8S)
        return (FALSE);
    switch (mm->channels())
    {
        default: return (FALSE);
        case 1: tt = CR_INDEX8; break;
        case 3: tt = CR_ARGB888; break;
        case 4: tt = CR_ARGB8888; break;
    }
    ww = mm->cols;
    hh = mm->rows;

    // 设置图片结构
    struct_zero(img, sIMAGE);
    img->data = (uchar*)mm->data;
    img->bpl  = (leng_t)mm->step;
    img->size = img->bpl * hh;
    img->bpc  = tt / 8;
    img->fmt  = tt;
    if      ((img->bpl % 32) == 0) img->align = 32;
    else if ((img->bpl % 16) == 0) img->align = 16;
    else if ((img->bpl %  8) == 0) img->align =  8;
    else if ((img->bpl %  4) == 0) img->align =  4;
    else if ((img->bpl %  2) == 0) img->align =  2;
    rect_set_wh(&img->clip_win, 0, 0, ww, hh);
    rect_set_wh(&img->position, 0, 0, ww, hh);
    if (tt == CR_INDEX8)
        pal_set_gray8(img->pal);
    return (TRUE);
}

/*
=======================================
    sIMAGE 生成 OpenCV 矩阵
=======================================
*/
CR_API ximage_t
imglab_crh2mat_dup (
  __CR_IN__ const sIMAGE*   img
    )
{
    uint_t      chn;
    bool_t      rel;
    leng_t      bpl;
    byte_t*     lne;
    cv::Mat*    mat;

    rel = FALSE;

    // 图片格式调整
    switch (img->fmt)
    {
        case CR_INDEX8: chn = 1; break;
        case CR_ARGB888: chn = 3; break;
        case CR_ARGB8888: chn = 4; break;

        default:
            img = img_auto_to_32(NULL, 0, 0, img);
            if (img == NULL)
                return (NULL);
            chn = 4;
            rel = TRUE;
            break;
    }

    // 处理颠倒图片
    if (img->gdi) {
        if (!rel) {
            img = image_dup(img);
            if (img == NULL)
                return (NULL);
            rel = TRUE;
        }
        image_flp(img, FALSE);
    }

    // 复制到 OpenCV 矩阵
    if (chn == 4)
        mat = new(std::nothrow) cv::Mat (img->position.hh, img->position.ww, CV_8UC4);
    else if (chn == 3)
        mat = new(std::nothrow) cv::Mat (img->position.hh, img->position.ww, CV_8UC3);
    else
        mat = new(std::nothrow) cv::Mat (img->position.hh, img->position.ww, CV_8UC1);
    if (mat == NULL) {
        if (rel) image_del(img);
        return (NULL);
    }
    lne = img->data;
    bpl = img->position.ww;
    bpl *= chn;
    for (uint_t yy = 0; yy < img->position.hh; yy++) {
        mem_cpy(mat->ptr<byte_t>(yy), lne, bpl);
        lne += img->bpl;
    }
    if (rel) image_del(img);
    return ((ximage_t)mat);
}

/*
=======================================
    sIMAGE 生成 OpenCV 矩阵 (浅复制)
=======================================
*/
CR_API ximage_t
imglab_crh2mat_set (
  __CR_IN__ const sIMAGE*   img
    )
{
    int         typ;
    cv::Mat*    mat;

    // 设置到 OpenCV 矩阵
    switch (img->fmt)
    {
        default: return (NULL);
        case CR_INDEX8: typ = CV_8UC1; break;
        case CR_ARGB888: typ = CV_8UC3; break;
        case CR_ARGB8888: typ = CV_8UC4; break;
    }
    mat = new(std::nothrow) cv::Mat (img->position.hh, img->position.ww,
                       typ, img->data, img->bpl);
    if (mat == NULL)
        return (NULL);
    return ((ximage_t)mat);
}

/*****************************************************************************/
/*                                 图片加载                                  */
/*****************************************************************************/

/*
=======================================
    从文件加载图片
=======================================
*/
CR_API ximage_t
imglab_load_file (
  __CR_IN__ const ansi_t*   name
    )
{
    cv::Mat*    mat;

    mat = new(std::nothrow) cv::Mat ();
    if (mat == NULL)
        return (NULL);
    *mat = cv::imread(name);
    if (mat->data == NULL) {
        delete mat;
        return (NULL);
    }
    return ((ximage_t)mat);
}

/*
=======================================
    从内存加载图片
=======================================
*/
CR_API ximage_t
imglab_load_data (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    cv::Mat*    mat;

    mat = new(std::nothrow) cv::Mat ();
    if (mat == NULL)
        return (NULL);

    int     len = (int)size;
    void*   ptr = (void*)data;
    cv::Mat buf = cv::Mat(1, len, CV_8UC1, ptr);

    *mat = cv::imdecode(buf, cv::IMREAD_COLOR);
    if (mat->data == NULL) {
        delete mat;
        return (NULL);
    }
    return ((ximage_t)mat);
}

/*
=======================================
    保存 PNG 图片
=======================================
*/
CR_API bool_t
imglab_save_file_png (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ ximage_t        mat
    )
{
    ansi_t  file[MAX_PATHA];

    if (strlen(name) + 5 > sizeof(file))
        return (FALSE);
    sprintf(file, "%s.png", name);

    cv::Mat*            img = (cv::Mat*)mat;
    std::vector<int>    compression_params;

    try {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);
        cv::imwrite(file, *img, compression_params);
    }
    catch (...) {
        file_deleteA(file);
        return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                 视频加载                                  */
/*****************************************************************************/

/*
=======================================
    获取视频文件对象
=======================================
*/
CR_API xvideo_t
imglab_video_new (
  __CR_IN__ const ansi_t*   name
    )
{
    cv::VideoCapture*   cap;

    cap = new(std::nothrow) cv::VideoCapture (name);
    if (cap == NULL)
        return (NULL);
    if (!cap->isOpened() ||
        (cap->get(cv::CAP_PROP_FRAME_COUNT) < 1)) {
        delete cap;
        return (NULL);
    }
    cap->set(cv::CAP_PROP_CONVERT_RGB, 1);
    return ((xvideo_t)cap);
}

/*
=======================================
    释放视频文件对象
=======================================
*/
CR_API void_t
imglab_video_del (
  __CR_IN__ xvideo_t    avi
    )
{
    cv::VideoCapture*   cap;

    cap = (cv::VideoCapture*)avi;
    delete cap;
}

/*
=======================================
    获取一帧图像
=======================================
*/
CR_API ximage_t
imglab_video_get (
  __CR_IN__ xvideo_t    avi
    )
{
    cv::Mat*            img;
    cv::VideoCapture*   cap;

    img = new(std::nothrow) cv::Mat ();
    if (img == NULL)
        return (NULL);
    cap = (cv::VideoCapture*)avi;
    if (cap->grab() && cap->retrieve(*img))
        return ((ximage_t)img);
    delete img;
    return (NULL);
}

/*
=======================================
    返回视频的帧数
=======================================
*/
CR_API int64u
imglab_video_count (
  __CR_IN__ xvideo_t    avi
    )
{
    int64u              cnt;
    cv::VideoCapture*   cap;

    cap = (cv::VideoCapture*)avi;
    cnt = (int64s)cap->get(cv::CAP_PROP_FRAME_COUNT);
    return ((int64u)cnt);
}

/*
=======================================
    视频定位到指定帧
=======================================
*/
CR_API int64u
imglab_video_seek (
  __CR_IN__ xvideo_t    avi,
  __CR_IN__ int64u      frame
    )
{
    int64u              idx;
    cv::VideoCapture*   cap;

    cap = (cv::VideoCapture*)avi;
    cap->set(cv::CAP_PROP_POS_FRAMES, (double)frame);
    idx = (int64s)cap->get(cv::CAP_PROP_POS_FRAMES);
    for (; idx < frame; idx++) cap->grab();
    idx = (int64s)cap->get(cv::CAP_PROP_POS_FRAMES);
    return ((int64u)idx);
}

/*****************************************************************************/
/*                                摄像头操作                                 */
/*****************************************************************************/

/*
=======================================
    获取摄像头对象 (通过序号)
=======================================
*/
CR_API camera_t
imglab_camera_new (
  __CR_IN__ uint_t  id
    )
{
    cv::VideoCapture*   cap;

    cap = new(std::nothrow) cv::VideoCapture ((int)id);
    if (cap == NULL)
        return (NULL);
    if (!cap->isOpened()) {
        delete cap;
        return (NULL);
    }
    cap->set(cv::CAP_PROP_CONVERT_RGB, 1);

    cv::Mat tmp;

    // 需要提前抓一下图
    cap->grab();
    cap->retrieve(tmp);
    return ((camera_t)cap);
}

/*
=======================================
    获取摄像头对象 (通过名字)
=======================================
*/
CR_API camera_t
imglab_camera_new2 (
  __CR_IN__ const ansi_t*   name
    )
{
    uint_t          id, num;
    const ansi_t*   clist[8];

    num = dshow_cam_list(clist, 8);
    if (num == 0)
        return (NULL);
    for (id = 0; id < num; id++) {
        if (clist[id] == NULL)
            continue;
        if (strcmp(clist[id], name) == 0)
            break;
    }
    for (uint_t jj = 0; jj < num; jj++)
        TRY_FREE(clist[jj]);
    if (id >= num)
        return (NULL);
    return (imglab_camera_new(id));
}

/*
=======================================
    释放摄像头对象
=======================================
*/
CR_API void_t
imglab_camera_del (
  __CR_IN__ camera_t    cam
    )
{
    cv::VideoCapture*   cap;

    cap = (cv::VideoCapture*)cam;
    delete cap;
}

/*
=======================================
    获取一帧图像
=======================================
*/
CR_API ximage_t
imglab_camera_get (
  __CR_IN__ camera_t    cam
    )
{
    cv::Mat*            img;
    cv::VideoCapture*   cap;

    img = new(std::nothrow) cv::Mat ();
    if (img == NULL)
        return (NULL);
    cap = (cv::VideoCapture*)cam;
    if (cap->grab() && cap->retrieve(*img))
        return ((ximage_t)img);
    delete img;
    return (NULL);
}

/*****************************************************************************/
/*                                  多边形                                   */
/*****************************************************************************/

/*
=======================================
    释放多边形
=======================================
*/
CR_API void_t
imglab_xpoly_del (
  __CR_IN__ xpoly_t ply
    )
{
    std::vector<cv::Point>* con;

    con = (std::vector<cv::Point>*)ply;
    delete con;
}

/*
=======================================
    返回多边形点数
=======================================
*/
CR_API uint_t
imglab_xpoly_count (
  __CR_IN__ xpoly_t ply
    )
{
    std::vector<cv::Point>* con;

    con = (std::vector<cv::Point>*)ply;
    return ((uint_t)(con->size()));
}

/*
=======================================
    多边形点坐标索引
=======================================
*/
CR_API xpnt2_t
imglab_xpoly_idx (
  __CR_IN__ xpoly_t ply,
  __CR_IN__ uint_t  idx
    )
{
    cv::Point*              pnt;
    std::vector<cv::Point>* con;

    con = (std::vector<cv::Point>*)ply;
    try {
        pnt = &con->at(idx);
    }
    catch (...) {
        return (NULL);
    }
    return ((xpnt2_t)pnt);
}

/*
=======================================
    获取多边形点坐标
=======================================
*/
CR_API void_t
imglab_xpoly_get (
  __CR_OT__ sPNT2*  pnt,
  __CR_IN__ xpnt2_t con,
  __CR_IN__ uint_t  count
    )
{
    cv::Point*  ptr = (cv::Point*)con;

    for (uint_t idx = 0; idx < count; idx++, ptr++) {
        pnt[idx].x = ptr->x;
        pnt[idx].y = ptr->y;
    }
}

/*
=======================================
    设置多边形点坐标
=======================================
*/
CR_API void_t
imglab_xpoly_set (
  __CR_OT__ xpnt2_t         con,
  __CR_IN__ const sPNT2*    pnt,
  __CR_IN__ uint_t          count
    )
{
    cv::Point*  ptr = (cv::Point*)con;

    for (uint_t idx = 0; idx < count; idx++, ptr++) {
        ptr->x = pnt[idx].x;
        ptr->y = pnt[idx].y;
    }
}

/*
=======================================
    绘制多边形调试框 (单个)
=======================================
*/
CR_API void_t
imglab_draw_xploy (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xpoly_t     ply,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         lbold
    )
{
    size_t                  idx, count;
    std::vector<cv::Point>* con = (std::vector<cv::Point>*)ply;

    count = con->size();
    if (count-- <= 2)
        return;

    cv::Mat*    img = (cv::Mat*)mat;
    cv::Scalar  clr = CPIX2SCALAR(color);

    for (idx = 0; idx < count; idx++)
        cv::line(*img, (*con)[idx], (*con)[idx + 1], clr, lbold);
    cv::line(*img, (*con)[0], (*con)[idx], clr, lbold);
}

/*****************************************************************************/
/*                                 矩形列表                                  */
/*****************************************************************************/

/*
=======================================
    释放矩形列表
=======================================
*/
CR_API void_t
imglab_rects_del (
  __CR_IN__ xrect_lst_t list
    )
{
    std::vector<cv::Rect>*  lst;

    lst = (std::vector<cv::Rect>*)list;
    delete lst;
}

/*
=======================================
    返回列表矩形个数
=======================================
*/
CR_API uint_t
imglab_rects_count (
  __CR_IN__ xrect_lst_t list
    )
{
    std::vector<cv::Rect>*  lst;

    lst = (std::vector<cv::Rect>*)list;
    return ((uint_t)(lst->size()));
}

/*
=======================================
    列表矩形坐标索引
=======================================
*/
CR_API xrect_t
imglab_rects_idx (
  __CR_IN__ xrect_lst_t list,
  __CR_IN__ uint_t      idx
    )
{
    cv::Rect*               rct;
    std::vector<cv::Rect>*  lst;

    lst = (std::vector<cv::Rect>*)list;
    try {
        rct = &lst->at(idx);
    }
    catch (...) {
        return (NULL);
    }
    return ((xrect_t)rct);
}

/*
=======================================
    获取列表矩形坐标
=======================================
*/
CR_API void_t
imglab_rects_get (
  __CR_OT__ sRECT*  box,
  __CR_IN__ xrect_t rct,
  __CR_IN__ uint_t  count
    )
{
    cv::Rect*   ptr = (cv::Rect*)rct;

    for (uint_t idx = 0; idx < count; idx++, ptr++)
        rect_set_wh(&box[idx], ptr->x, ptr->y, ptr->width, ptr->height);
}

/*
=======================================
    设置列表矩形坐标
=======================================
*/
CR_API void_t
imglab_rects_set (
  __CR_OT__ xrect_t         rct,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ uint_t          count
    )
{
    cv::Rect*   ptr = (cv::Rect*)rct;

    for (uint_t idx = 0; idx < count; idx++, ptr++) {
        ptr->x      = (int)box[idx].x1;
        ptr->y      = (int)box[idx].y1;
        ptr->width  = (int)box[idx].ww;
        ptr->height = (int)box[idx].hh;
    }
}

/*
=======================================
    绘制矩形调试框 (单个)
=======================================
*/
CR_API void_t
imglab_draw_xrect (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xrect_t     rct,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         xbias,
  __CR_IN__ int         ybias,
  __CR_IN__ int         lbold
    )
{
    int         lww = xbias * 2;
    int         lhh = ybias * 2;
    cv::Mat*    img = (cv::Mat*)mat;
    cv::Rect*   con = (cv::Rect*)rct;
    cv::Scalar  clr = CPIX2SCALAR(color);
    cv::Rect    tmp;

    tmp.x = con->x - xbias;
    tmp.y = con->y - ybias;
    tmp.width  = con->width  + lww;
    tmp.height = con->height + lhh;
    cv::rectangle(*img, tmp, clr, lbold);
}

/*
=======================================
    绘制矩形调试框 (多个)
=======================================
*/
CR_API void_t
imglab_draw_rects (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xrect_lst_t list,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         xbias,
  __CR_IN__ int         ybias,
  __CR_IN__ int         lbold,
  __CR_IN__ int         index
    )
{
    std::vector<cv::Rect>*  lst;

    lst = (std::vector<cv::Rect>*)list;
    if (index >= (int)lst->size())
        return;

    int         lww = xbias * 2;
    int         lhh = ybias * 2;
    cv::Mat*    img = (cv::Mat*)mat;
    cv::Scalar  clr = CPIX2SCALAR(color);
    cv::Rect    tmp;

    if (index < 0)
    {
        for (auto rct : *lst)
        {
            tmp.x = rct.x - xbias;
            tmp.y = rct.y - ybias;
            tmp.width  = rct.width  + lww;
            tmp.height = rct.height + lhh;
            cv::rectangle(*img, tmp, clr, lbold);
        }
    }
    else
    {
        cv::Rect*   rct = &((*lst)[index]);

        tmp.x = rct->x - xbias;
        tmp.y = rct->y - ybias;
        tmp.width  = rct->width  + lww;
        tmp.height = rct->height + lhh;
        cv::rectangle(*img, tmp, clr, lbold);
    }
}

/*****************************************************************************/
/*                                四边形列表                                 */
/*****************************************************************************/

/*
=======================================
    释放四边形列表
=======================================
*/
CR_API void_t
imglab_rocts_del (
  __CR_IN__ xroct_lst_t list
    )
{
    std::vector<cv::RotatedRect>*   lst;

    lst = (std::vector<cv::RotatedRect>*)list;
    delete lst;
}

/*
=======================================
    返回列表四边形个数
=======================================
*/
CR_API uint_t
imglab_rocts_count (
  __CR_IN__ xroct_lst_t list
    )
{
    std::vector<cv::RotatedRect>*   lst;

    lst = (std::vector<cv::RotatedRect>*)list;
    return ((uint_t)(lst->size()));
}

/*
=======================================
    列表四边形坐标索引
=======================================
*/
CR_API xroct_t
imglab_rocts_idx (
  __CR_IN__ xroct_lst_t list,
  __CR_IN__ uint_t      idx
    )
{
    cv::RotatedRect*                rct;
    std::vector<cv::RotatedRect>*   lst;

    lst = (std::vector<cv::RotatedRect>*)list;
    try {
        rct = &lst->at(idx);
    }
    catch (...) {
        return (NULL);
    }
    return ((xroct_t)rct);
}

/*
=======================================
    获取列表四边形坐标
=======================================
*/
CR_API void_t
imglab_rocts_get (
  __CR_OT__ sPNT2*  pnt,
  __CR_IN__ xroct_t rct,
  __CR_IN__ uint_t  count
    )
{
    cv::Point2f         tmp[4];
    cv::RotatedRect*    ptr = (cv::RotatedRect*)rct;

    for (uint_t idx = 0; idx < count * 4; idx += 4, ptr++) {
        ptr->points(tmp);
        pnt[idx + 0].x = (int)tmp[0].x;
        pnt[idx + 0].y = (int)tmp[0].y;
        pnt[idx + 1].x = (int)tmp[1].x;
        pnt[idx + 1].y = (int)tmp[1].y;
        pnt[idx + 2].x = (int)tmp[2].x;
        pnt[idx + 2].y = (int)tmp[2].y;
        pnt[idx + 3].x = (int)tmp[3].x;
        pnt[idx + 3].y = (int)tmp[3].y;
    }
}

/*
=======================================
    获取列表四边形坐标
=======================================
*/
CR_API void_t
imglab_rocts_get2 (
  __CR_OT__ sRECT*  box,
  __CR_OT__ fp32_t* angle,
  __CR_IN__ xroct_t rct,
  __CR_IN__ uint_t  count
    )
{
    cv::RotatedRect*    ptr = (cv::RotatedRect*)rct;

    for (uint_t idx = 0; idx < count; idx++, ptr++) {
        if (box != NULL) {
            box[idx].x1 = (int)ptr->center.x;
            box[idx].y1 = (int)ptr->center.y;
            box[idx].x2 = box[idx].x1;
            box[idx].y2 = box[idx].y1;
            box[idx].ww = (int)ptr->size.width;
            box[idx].hh = (int)ptr->size.height;
        }
        if (angle != NULL)
            angle[idx] = ptr->angle;
    }
}

/*
=======================================
    设置列表四边形坐标
=======================================
*/
CR_API void_t
imglab_rocts_set (
  __CR_OT__ xroct_t         rct,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ const fp32_t*   angle,
  __CR_IN__ uint_t          count
    )
{
    fp32_t              cx, cy, ww, hh, aa;
    cv::RotatedRect*    ptr = (cv::RotatedRect*)rct;

    if (box == NULL && angle == NULL)
        return;
    if (box == NULL && angle != NULL) {
        for (uint_t idx = 0; idx < count; idx++, ptr++)
            ptr->angle = angle[idx];
    }
    else {
        for (uint_t idx = 0; idx < count; idx++, ptr++) {
            cx  = (fp32_t)box[idx].x1;
            cx += (fp32_t)box[idx].x2;
            cy  = (fp32_t)box[idx].y1;
            cy += (fp32_t)box[idx].y2;
            ww  = (fp32_t)box[idx].ww;
            hh  = (fp32_t)box[idx].hh;
            aa  = (angle != NULL) ? angle[idx] : ptr->angle;
            *ptr = cv::RotatedRect(cv::Point2f(cx / 2, cy / 2),
                                   cv::Size2f(ww, hh), aa);
        }
    }
}

/*
=======================================
    设置列表四边形坐标
=======================================
*/
CR_API void_t
imglab_rocts_set2 (
  __CR_OT__ xroct_t         rct,
  __CR_IN__ const sPNT2*    pnt,
  __CR_IN__ uint_t          count
    )
{
    cv::Point2f         tmp[3];
    cv::RotatedRect*    ptr = (cv::RotatedRect*)rct;

    for (uint_t idx = 0; idx < count * 3; idx += 3, ptr++) {
        tmp[0].x = (fp32_t)pnt[idx + 0].x;
        tmp[0].y = (fp32_t)pnt[idx + 0].y;
        tmp[1].x = (fp32_t)pnt[idx + 1].x;
        tmp[1].y = (fp32_t)pnt[idx + 1].y;
        tmp[2].x = (fp32_t)pnt[idx + 2].x;
        tmp[2].y = (fp32_t)pnt[idx + 2].y;
        *ptr = cv::RotatedRect(tmp[0], tmp[1], tmp[2]);
    }
}

/*
=======================================
    绘制四边形调试框 (单个)
=======================================
*/
CR_API void_t
imglab_draw_xroct (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xroct_t     rct,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         lbold
    )
{
    cv::Mat*            img = (cv::Mat*)mat;
    cv::Scalar          clr = CPIX2SCALAR(color);
    cv::RotatedRect*    con = (cv::RotatedRect*)rct;
    cv::Point2f         pnt[4];

    con->points(pnt);
    cv::line(*img, pnt[0], pnt[1], clr, lbold);
    cv::line(*img, pnt[1], pnt[2], clr, lbold);
    cv::line(*img, pnt[2], pnt[3], clr, lbold);
    cv::line(*img, pnt[3], pnt[0], clr, lbold);
}

/*
=======================================
    绘制四边形调试框 (多个)
=======================================
*/
CR_API void_t
imglab_draw_rocts (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xroct_lst_t list,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         lbold,
  __CR_IN__ int         index
    )
{
    std::vector<cv::RotatedRect>*   lst;

    lst = (std::vector<cv::RotatedRect>*)list;
    if (index >= (int)lst->size())
        return;

    cv::Mat*    img = (cv::Mat*)mat;
    cv::Scalar  clr = CPIX2SCALAR(color);
    cv::Point2f pnt[4];

    if (index < 0)
    {
        for (auto rct : *lst)
        {
            rct.points(pnt);
            cv::line(*img, pnt[0], pnt[1], clr, lbold);
            cv::line(*img, pnt[1], pnt[2], clr, lbold);
            cv::line(*img, pnt[2], pnt[3], clr, lbold);
            cv::line(*img, pnt[3], pnt[0], clr, lbold);
        }
    }
    else
    {
        cv::RotatedRect*    rct = &((*lst)[index]);

        rct->points(pnt);
        cv::line(*img, pnt[0], pnt[1], clr, lbold);
        cv::line(*img, pnt[1], pnt[2], clr, lbold);
        cv::line(*img, pnt[2], pnt[3], clr, lbold);
        cv::line(*img, pnt[3], pnt[0], clr, lbold);
    }
}

/*****************************************************************************/
/*                                多边形列表                                 */
/*****************************************************************************/

/*
=======================================
    释放多边形列表
=======================================
*/
CR_API void_t
imglab_polys_del (
  __CR_IN__ xpoly_lst_t list
    )
{
    std::vector<std::vector<cv::Point> >*   lst;

    lst = (std::vector<std::vector<cv::Point> >*)list;
    delete lst;
}

/*
=======================================
    返回列表多边形个数
=======================================
*/
CR_API uint_t
imglab_polys_count (
  __CR_IN__ xpoly_lst_t list
    )
{
    std::vector<std::vector<cv::Point> >*   lst;

    lst = (std::vector<std::vector<cv::Point> >*)list;
    return ((uint_t)(lst->size()));
}

/*
=======================================
    列表多边形对象索引
=======================================
*/
CR_API xpoly_t
imglab_polys_idx (
  __CR_IN__ xpoly_lst_t list,
  __CR_IN__ uint_t      idx
    )
{
    std::vector<cv::Point>*                 con;
    std::vector<std::vector<cv::Point> >*   lst;

    lst = (std::vector<std::vector<cv::Point> >*)list;
    try {
        con = &lst->at(idx);
    }
    catch (...) {
        return (NULL);
    }
    return ((xpoly_t)con);
}

/*
=======================================
    绘制多边形调试框 (多个)
=======================================
*/
CR_API void_t
imglab_draw_ploys (
  __CR_IO__ ximage_t    mat,
  __CR_IN__ xpoly_lst_t list,
  __CR_IN__ cpix_t      color,
  __CR_IN__ int         lbold,
  __CR_IN__ int         index
    )
{
    std::vector<std::vector<cv::Point> >*   lst;

    lst = (std::vector<std::vector<cv::Point> >*)list;
    if (index >= (int)lst->size())
        return;

    cv::Mat*    img = (cv::Mat*)mat;
    cv::Scalar  clr = CPIX2SCALAR(color);

    cv::drawContours(*img, *lst, index, clr, lbold);
}

/*****************************************************************************/
/*                               图形绘制函数                                */
/*****************************************************************************/

/*
=======================================
    绘制文本字符串
=======================================
*/
CR_API void_t
imglab_draw_text (
  __CR_IO__ ximage_t        mat,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ sint_t          dx,
  __CR_IN__ sint_t          dy,
  __CR_IN__ sint_t          font,
  __CR_IN__ sint_t          height,
  __CR_IN__ cpix_t          color,
  __CR_IN__ sint_t          lbold
    )
{
    cv::Mat*    img = (cv::Mat*)mat;
    cv::Scalar  clr = CPIX2SCALAR(color);
    double      scale = cv::getFontScaleFromHeight(font, height, lbold);

    cv::putText(*img, text, cv::Point(dx, dy), font, scale, clr, lbold);
}

/*
=======================================
    返回绘制文本大小
=======================================
*/
CR_API sint_t
imglab_text_size (
  __CR_OT__ sPNT2*          size,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ sint_t          font,
  __CR_IN__ sint_t          height,
  __CR_IN__ sint_t          lbold
    )
{
    cv::Size    box;
    /* ---------- */
    sint_t  baseline = 0;
    double  scale = cv::getFontScaleFromHeight(font, height, lbold);

    box = cv::getTextSize(text, font, scale, lbold, &baseline);
    size->x = box.width;
    size->y = box.height;
    return (baseline);
}
