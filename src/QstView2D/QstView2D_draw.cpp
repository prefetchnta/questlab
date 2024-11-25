
#include "QstView2D.h"

/* 日志函数 */
#include "../logit.inl"

/* 内部函数的声明 */
CR_API void_t   qst_clear (sQstView2D *parm);

/*
---------------------------------------
    释放信息列表
---------------------------------------
*/
static void_t
finfo_free (
  __CR_IN__ void_t* obj
    )
{
    ansi_t**    unit;

    unit = (ansi_t**)obj;
    mem_free(*unit);
}

/*****************************************************************************/
/*                                 内部函数                                  */
/*****************************************************************************/

/*
---------------------------------------
    发送索引命令
---------------------------------------
*/
static void_t
qst_send_index (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          idx,
  __CR_IN__ socket_t        netw
    )
{
    ansi_t  buf[64];

    sprintf(buf, "%s %u", name, idx);
    cmd_shl_send(netw, buf);
}

/*
---------------------------------------
    发送文件信息
---------------------------------------
*/
static void_t
qst_send_finfo (
  __CR_IN__ sARRAY*     list,
  __CR_IN__ socket_t    netw
    )
{
    leng_t  cnts, idx;

    /* 清除前面的信息 */
    cmd_shl_send(netw, "txt:clear 0 1");

    /* 逐条发送文件信息 */
    cnts = array_get_sizeT(list, ansi_t*);
    for (idx = 0; idx < cnts; idx++)
    {
        ansi_t*     temp;
        ansi_t*     info;
        ansi_t**    unit;

        /* 转义转换后发给别人 */
        unit = array_get_unitT(list, ansi_t*, idx);
        temp = str_esc_makeU(*unit);
        if (temp == NULL)
            continue;
        info = str_fmtA("info::main=\"1> %s\"", temp);
        mem_free(temp);
        if (info != NULL) {
            cmd_ini_send(netw, info);
            mem_free(info);
        }
    }
}

/*
---------------------------------------
    发送图片信息
---------------------------------------
*/
static void_t
qst_send_image (
  __CR_IN__ const sFMT_FRAME*   pict,
  __CR_IN__ socket_t            netw
    )
{
    ansi_t* temp;
    ansi_t* info;

    /* 清除前面的信息 */
    cmd_shl_send(netw, "txt:clear 1 1");

    /* 发送像素格式信息 */
    switch (pict->fmt)
    {
        default:          temp = "Unknown"; break;
        case CR_PIC_GREY: temp = "Grey";    break;
        case CR_PIC_PALS: temp = "Palette"; break;
        case CR_PIC_ARGB: temp = "HiColor"; break;
        case CR_PIC_SPEC: temp = "Special"; break;
        case CR_PIC_CMYK: temp = "CMYK";    break;
        case CR_PIC_CMPX: temp = "Complex"; break;
        case CR_PIC_IEEE: temp = "Float";   break;
    }
    info = str_fmtA("info::subs=\"1> Type: %s\"", temp);
    if (info != NULL) {
        cmd_ini_send(netw, info);
        mem_free(info);
    }

    /* 发送像素位数信息 */
    info = str_fmtA("info::subs=\"1> Bits: %u\"", pict->bpp);
    if (info != NULL) {
        cmd_ini_send(netw, info);
        mem_free(info);
    }

    ansi_t  buf[128];
    leng_t  idx, cnts, next;

    /* 检查分量的有效性 */
    cnts = str_lenA(pict->clr);
    if (cnts > sizeof(pict->wh))
        cnts = sizeof(pict->wh);
    for (idx = 0; idx < cnts; idx++) {
        if (pict->wh[idx] == 0)
            break;
    }

    /* 发送颜色分量信息 */
    if (idx < cnts) {
        info = str_fmtA("info::subs=\"1> Mask: %s\"", pict->clr);
    }
    else {
        for (next = 0, idx = 0; idx < cnts; idx++) {
            next += sprintf(&buf[next], "%c%u",
                pict->clr[idx], pict->wh[idx]);
        }
        info = str_fmtA("info::subs=\"1> Mask: %s\"", buf);
    }
    if (info != NULL) {
        cmd_ini_send(netw, info);
        mem_free(info);
    }

    /* 发送图片宽高信息 */
    info = str_fmtA("info::subs=\"1> Size: %u x %u\"",
            pict->pic->position.ww, pict->pic->position.hh);
    if (info != NULL) {
        cmd_ini_send(netw, info);
        mem_free(info);
    }

    /* 发送图片位置信息 */
    info = str_fmtA("info::subs=\"1> Rect: [%d, %d, %d, %d]\"",
                pict->pic->position.x1, pict->pic->position.y1,
                pict->pic->position.x2, pict->pic->position.y2);
    if (info != NULL) {
        cmd_ini_send(netw, info);
        mem_free(info);
    }
}

/*
---------------------------------------
    尝试加载目标数据
---------------------------------------
*/
static bool_t
qst_try_load (
  __CR_IO__ sQstView2D* parm,
  __CR_IO__ sLOADER*    ldrs
    )
{
    leng_t  idx;
    leng_t  cnts;
    sFMTZ*  fmtz;
    sFMTZ*  tmpz;
    sARRAY  list;
    ansi_t* info;
    sLOADER copy;

    /* 释放上次加载 */
    qst_clear(parm);

    /* 初始化加载尝试 */
    fmtz = tmpz = NULL;
    array_initT(&list, ansi_t*);
    list.free = finfo_free;
    struct_cpy(&copy, ldrs, sLOADER);
    cnts = array_get_sizeT(&parm->extz, sENGINE*);

    sENGINE**   unit;
    sFMT_DAT*   datz;
    sFMT_PIC*   picz;
    sFMT_PRT*   imgz;

    /* 尝试到有结果为止 */
_retry:
    /* 逐个插件试验加载 */
    for (unit = NULL, idx = 0; idx < cnts; idx++) {
        unit = array_get_unitT(&parm->extz, sENGINE*, idx);
        tmpz = engine_do(*unit, &copy);
        if (tmpz != NULL)
            break;
    }

    /* 释放前次的数据 */
    if (fmtz != NULL)
        fmtz_free(fmtz);

    /* 无法识别的图片文件 */
    if (unit == NULL || tmpz == NULL)
        goto _func_out;

    /* 需要经过二次解码 */
    if (tmpz->type == CR_FMTZ_NOP ||
        tmpz->type == CR_FMTZ_DEC)
    {
        /* 换成内存解析 */
        fmtz = tmpz;
        datz = (sFMT_DAT*)tmpz;
        set_ldrM(&copy, datz->data, datz->unsz, ldrs->aprm, 0, 0);
        copy.nprm = ldrs->nprm;
        copy.page = ldrs->page;

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入文件信息 */
        info = str_fmtA("+-Decoder: %s", datz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入大小信息 */
        info = str_fmtA("+---File size: %I64u Bytes / Real size: %u Bytes",
                                    datz->pksz, datz->unsz);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);
        goto _retry;
    }

    /* 得到可以加载的图片文件 */
    if (tmpz->type == CR_FMTZ_PIC ||
        tmpz->type == CR_FMTZ_PRT)
    {
        int32u      count;
        iPICTURE*   slide;

        if (tmpz->type == CR_FMTZ_PIC) {
            parm->fmtz = tmpz;
            picz = (sFMT_PIC*)tmpz;
            count = picz->count;
        }
        else {
            imgz = (sFMT_PRT*)tmpz;
            if (str_cmpA(imgz->more, "iPICTURE") != 0)
                goto _func_out;
            slide = (iPICTURE*)imgz->port;
            picz = CR_VCALL(slide)->get(slide, 0);
            if (picz == NULL)
                goto _func_out;
            parm->fmtz = (sFMTZ*)picz;
            parm->slide = slide;
            parm->pictz = imgz;
            count = pict_get_count(slide);
        }

        /* 压入插件信息 */
        info = str_fmtA("Plug-in: %s", (*unit)->info);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入图片信息 */
        info = str_fmtA("+-Picture: %s", picz->infor);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 压入帧数信息 */
        info = str_fmtA("+---Image frame count: %u", count);
        if (info != NULL)
            array_push_growT(&list, ansi_t*, &info);

        /* 发送图片帧数信息 */
        parm->index = 0;
        qst_send_index("idx:get_min", 0, parm->netw);
        qst_send_index("idx:get_max", count - 1, parm->netw);

        /* 发送整个信息列表 */
        qst_send_finfo(&list, parm->netw);
        array_freeT(&list, ansi_t*);

        /* 窗口拉到最前面 */
        /* 发送消息的方式防止线程死锁 */
        PostMessageA(parm->hwnd, WM_QV2D_WIN_BRING_TO_TOP, 0, 0);
        return (TRUE);
    }

    /* 释放临时的数据 */
_func_out:
    if (tmpz != NULL)
        fmtz_free(tmpz);
    array_freeT(&list, ansi_t*);
    return (FALSE);
}

/*
---------------------------------------
    保存一张图片
---------------------------------------
*/
static bool_t
qst_save_img (
  __CR_IN__ const sIMAGE*       image,
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    leng_t      idx;
    leng_t      cnts;
    ansi_t*     func;
    ansi_t*     fext;
    sENGINE**   unit;

    /* 抽出扩展名 */
    fext = str_allocA(str_sizeA(name));
    if (fext == NULL)
        return (FALSE);
    filext_extractA(fext, name);
    func = str_fmtA("save_img_%s", fext + 1);
    mem_free(fext);
    if (func == NULL)
        return (FALSE);

    save_img_fmtz_t     save_call = NULL;

    /* 根据扩展名获取保存接口 */
    cnts = array_get_sizeT(&parm->extz, sENGINE*);
    for (idx = 0; idx < cnts; idx++) {
        unit = array_get_unitT(&parm->extz, sENGINE*, idx);
        save_call = sbin_exportT(unit[0]->sbin, func, save_img_fmtz_t);
        if (save_call != NULL)
            break;
    }
    mem_free(func);
    if (save_call == NULL)
        return (FALSE);

    /* 保存图片文件 */
    return (save_call(image, name, argc, argv));
}

/*
---------------------------------------
    返回计数的格式串
---------------------------------------
*/
static const ansi_t*
qst_count_fmt (
  __CR_IN__ int32u  count
    )
{
    if      (count > 999999999UL) return ("%s_%010u%s");
    else if (count > 99999999UL) return ("%s_%09u%s");
    else if (count > 9999999UL) return ("%s_%08u%s");
    else if (count > 999999UL) return ("%s_%07u%s");
    else if (count > 99999UL) return ("%s_%06u%s");
    else if (count > 9999UL) return ("%s_%05u%s");
    else if (count > 999UL) return ("%s_%04u%s");
    else if (count > 99UL) return ("%s_%03u%s");
    else if (count > 9UL) return ("%s_%02u%s");
    return ("%s_%u%s");
}

/*****************************************************************************/
/*                                 内容渲染                                  */
/*****************************************************************************/

/*
=======================================
    显示加载的图片
=======================================
*/
CR_API void_t
qst_draw_image (
  __CR_IO__ sQstView2D* parm
    )
{
    sBLIT   blt;
    sRECT*  rect;
    iGFX2*  draw;
    sIMAGE* dest;
    sIMAGE* srce;

    /* 过滤参数 */
    if (parm->draw == NULL)
        return;

    /* 选择绘制哪个通道 */
    if (!parm->trans)
        srce = parm->image;
    else
        srce = parm->alpha;
    draw = parm->draw;
    CR_VCALL(draw)->clear(draw, parm->cfgs.bkcolor, 0);
    if (srce == NULL) {
        CR_VCALL(draw)->flip(draw, FALSE);
        return;
    }
    rect = &draw->__back__.position;

    /* X 绘制坐标计算 */
    if (srce->position.ww > rect->ww) {
        blt.dx = 0;
        blt.sx = parm->win_x;
        blt.sw = srce->position.ww - parm->win_x;
    }
    else {
        parm->win_x = 0;
        if (!parm->cfgs.is_center)
            blt.dx = 0;
        else
            blt.dx = (rect->ww - srce->position.ww) / 2;
        blt.sx = 0;
        blt.sw = srce->position.ww;
    }

    /* Y 绘制坐标计算 */
    if (srce->position.hh > rect->hh) {
        blt.dy = 0;
        blt.sy = parm->win_y;
        blt.sh = srce->position.hh - parm->win_y;
    }
    else {
        parm->win_y = 0;
        if (!parm->cfgs.is_center)
            blt.dy = 0;
        else
            blt.dy = (rect->hh - srce->position.hh) / 2;
        blt.sy = 0;
        blt.sh = srce->position.hh;
    }

    /* 清除后台缓冲并绘制 */
    dest = CR_VCALL(draw)->lock(draw);
    if (!parm->cfgs.is_blends)
        blit_set_c(dest, srce, &blt, NULL);
    else
        blit_alp_c(dest, srce, NULL, &blt, NULL);
    CR_VCALL(draw)->unlock(draw);
    CR_VCALL(draw)->flip(draw, FALSE);
}

/*
=======================================
    处理可渲染的数据
=======================================
*/
CR_API void_t
qst_make_image (
  __CR_IO__ sQstView2D* parm
    )
{
    int32u      idx;
    sIMAGE*     img;
    sIMAGE*     rgb;
    sIMAGE*     alp;
    sFMT_PIC*   pic;

    /* 释放前次的生成 */
    parm->win_x = parm->win_y = 0;
    if (parm->image != NULL) {
        image_del(parm->image);
        parm->image = NULL;
    }
    if (parm->alpha != NULL) {
        image_del(parm->alpha);
        parm->alpha = NULL;
    }

    sFMT_FRAME* frame;

    /* 图片数据源选择 */
    if (parm->fmtz == NULL)
    {
        /* 使用画布的图片数据 */
        if (parm->paint == NULL)
            return;
        img = parm->paint;
        parm->index = 0;
        frame = NULL;
    }
    else
    {
        /* 使用加载的图片数据 */
        idx = parm->index;
        if (parm->slide != NULL)
        {
            /* 使用多帧图片接口 */
            if (idx >= pict_get_count(parm->slide))
                idx = pict_get_count(parm->slide) - 1;
            pic = CR_VCALL(parm->slide)->get(parm->slide, idx);
            if (pic != NULL) {
                fmtz_free(parm->fmtz);
                parm->fmtz = (sFMTZ*)pic;
            }
            else {
                pic = (sFMT_PIC*)parm->fmtz;
            }
            frame = pic->frame;
        }
        else
        {
            /* 使用普通图片对象 */
            pic = (sFMT_PIC*)parm->fmtz;
            if (idx >= pic->count)
                idx = pic->count - 1;
            frame = &pic->frame[idx];
        }
        img = frame->pic;
        parm->index = idx;
    }

    /* 转换到32位色 */
    rgb = img_auto_to_32(NULL, 0, 0, img);
    if (rgb == NULL)
        return;

    /* 处理 Y 轴颠倒的情况 */
    image_fuck_gdi(rgb);

    /* 用关键色设置透明通道 */
    if (parm->cfgs.use_keycolor) {
        if (parm->cfgs.def_keycolor != 0)
            rgb->keycolor = parm->cfgs.def_keycolor;
        image_alptran32(rgb);
    }

    /* 调用滤镜脚本 (如果有的话) */
    if (parm->flt_scr != NULL && parm->flt_lst != NULL)
    {
        /* 判断64位滤镜有没有崩溃, 崩溃则记录 */
        if (misc_is_win64() && !misc_is_running("fQUEST64"))
            logit("fQUEST64.exe fucked, please check it!");
        xmlcall_exec(parm->flt_lst, rgb, "", parm->flt_scr);
    }

    /* 抽出透明色通道 */
    img = image_get_alpha(rgb);
    if (img == NULL) {
        image_del(rgb);
        return;
    }
    alp = img_auto_to_32(NULL, 0, 0, img);
    image_del(img);
    if (alp == NULL) {
        image_del(rgb);
        return;
    }

    /* 刷新最新结果 */
    parm->image = rgb;
    parm->alpha = alp;

    /* 画布结果不发送信息 */
    if (frame != NULL)
    {
        /* 发送当前帧号 */
        if (parm->send)
            qst_send_index("idx:get_now", parm->index, parm->netw);

        /* 发送图片信息 */
        qst_send_image(frame, parm->netw);
    }
}

/*
=======================================
    渲染目标数据
=======================================
*/
CR_API void_t
qst_render_data (
  __CR_IO__ sQstView2D* parm,
  __CR_IO__ sLOADER*    ldrs
    )
{
    /* 置鼠标忙碌状态 */
    QST_SET_CURSOR(parm->hwnd, parm->cur_busy);

    /* 尝试加载文件 */
    qst_try_load(parm, ldrs);

    /* 渲染获取的结果 */
    qst_make_image(parm);
    qst_draw_image(parm);

    /* 置鼠标空闲状态 */
    QST_SET_CURSOR(parm->hwnd, parm->cur_free);
}

/*
=======================================
    XY 轴漫游图像
=======================================
*/
CR_API void_t
qst_move_xy (
  __CR_IO__ sQstView2D* parm,
  __CR_IN__ sint_t      delta_x,
  __CR_IN__ sint_t      delta_y
    )
{
    sRECT*  rect;
    iGFX2*  draw;
    bool_t  spnt;
    uint_t  step;
    sIMAGE* srce;

    /* 过滤参数 */
    if (parm->draw == NULL)
        return;

    /* 有图像才能漫游 */
    if (!parm->trans)
        srce = parm->image;
    else
        srce = parm->alpha;
    if (srce == NULL)
        return;
    spnt = FALSE;
    draw = parm->draw;
    rect = &draw->__back__.position;

    /* 必须要有宽于视口的宽度 */
    if (srce->position.ww > rect->ww) {
        if (delta_x < 0) {
            step = (uint_t)(0 - delta_x);
            if (parm->win_x <= step)
                parm->win_x = 0;
            else
                parm->win_x -= step;
        }
        else {
            step = (uint_t)(delta_x);
            if (step >= srce->position.ww - rect->ww ||
                parm->win_x >= srce->position.ww - step - rect->ww)
                parm->win_x = srce->position.ww - rect->ww;
            else
                parm->win_x += step;
        }
        spnt = TRUE;
    }
    else {
        parm->win_x = 0;
    }

    /* 必须要有高于视口的高度 */
    if (srce->position.hh > rect->hh) {
        if (delta_y < 0) {
            step = (uint_t)(0 - delta_y);
            if (parm->win_y <= step)
                parm->win_y = 0;
            else
                parm->win_y -= step;
        }
        else {
            step = (uint_t)(delta_y);
            if (step >= srce->position.hh - rect->hh ||
                parm->win_y >= srce->position.hh - step - rect->hh)
                parm->win_y = srce->position.hh - rect->hh;
            else
                parm->win_y += step;
        }
        spnt = TRUE;
    }
    else {
        parm->win_y = 0;
    }

    /* 刷新显示 */
    if ((!spnt) ||
        (delta_x == 0 && delta_y == 0))
        return;
    qst_draw_image(parm);
}

/*
=======================================
    切换多帧图片
=======================================
*/
CR_API void_t
qst_set_index (
  __CR_IO__ sQstView2D* parm,
  __CR_IN__ int32u      index
    )
{
    sFMT_PIC*   pic;

    /* 数据源有效性检查 */
    if (parm->fmtz == NULL)
        return;

    /* 图片帧号有效性检查 */
    if (parm->slide != NULL)
    {
        /* 使用多帧图片接口 */
        if (index >= pict_get_count(parm->slide))
            index = pict_get_count(parm->slide) - 1;
    }
    else
    {
        /* 使用普通图片对象 */
        pic = (sFMT_PIC*)parm->fmtz;
        if (index >= pic->count)
            index = pic->count - 1;
    }
    if (index == parm->index)
        return;
    parm->index = index;
    qst_make_image(parm);
    qst_draw_image(parm);
}

/*****************************************************************************/
/*                                 图片保存                                  */
/*****************************************************************************/

/*
=======================================
    保存当前图片
=======================================
*/
CR_API bool_t
qst_save_now (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    int32u      idx;
    sFMT_PIC*   pic;

    if (parm->paint != NULL)
        return (qst_save_img(parm->paint, parm, name, argc, argv));
    if (parm->fmtz == NULL)
        return (FALSE);
    idx = (parm->slide != NULL) ? 0 : parm->index;
    pic = (sFMT_PIC*)parm->fmtz;
    return (qst_save_img(pic->frame[idx].pic, parm, name, argc, argv));
}

/*
=======================================
    保存所有图片
=======================================
*/
CR_API bool_t
qst_save_all (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    int32u          idx;
    int32u          cnt;
    ansi_t*         str;
    ansi_t*         fle;
    ansi_t*         ext;
    sFMT_PIC*       pic;
    const ansi_t*   fmt;

    if (parm->paint != NULL)
        return (qst_save_img(parm->paint, parm, name, argc, argv));
    if (parm->fmtz == NULL)
        return (FALSE);
    pic = (sFMT_PIC*)parm->fmtz;
    if (parm->slide == NULL)
        cnt = pic->count;
    else
        cnt = pict_get_count(parm->slide);
    if (cnt == 1)
        return (qst_save_now(parm, name, argc, argv));
    fle = str_dupA(name);
    if (fle == NULL)
        return (FALSE);
    ext = str_allocA(str_sizeA(name));
    if (ext == NULL)
        goto _failure1;
    filext_removeA(fle);
    filext_extractA(ext, name);
    fmt = qst_count_fmt(cnt);
    if (parm->slide == NULL) {
        for (idx = 0; idx < cnt; idx++) {
            str = str_fmtA(fmt, fle, idx, ext);
            if (str == NULL)
                goto _failure2;
            if (!qst_save_img(pic->frame[idx].pic, parm, str, argc, argv))
                goto _failure3;
            mem_free(str);
        }
    }
    else {
        for (idx = 0; idx < cnt; idx++) {
            str = str_fmtA(fmt, fle, idx, ext);
            if (str == NULL)
                goto _failure2;
            pic = CR_VCALL(parm->slide)->get(parm->slide, idx);
            if (pic == NULL)
                goto _failure3;
            if (!qst_save_img(pic->frame->pic, parm, str, argc, argv)) {
                fmtz_free((sFMTZ*)pic);
                goto _failure3;
            }
            fmtz_free((sFMTZ*)pic);
            mem_free(str);
        }
    }
    mem_free(ext);
    mem_free(fle);
    return (TRUE);

_failure3:
    mem_free(str);
_failure2:
    mem_free(ext);
_failure1:
    mem_free(fle);
    return (FALSE);
}

/*
=======================================
    保存最小图片
=======================================
*/
CR_API bool_t
qst_save_min (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    sRECT   box;
    sIMAGE* save;
    byte_t* line;
    bool_t  retc;
    sint_t  xx, yy;
    sint_t  ww, hh;

    if (parm->image == NULL)
        return (FALSE);
    ww = parm->image->position.ww;
    hh = parm->image->position.hh;

    /* 查找 X/Y 坐标的极值 */
    line = parm->image->data;
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++) {
            if (line[xx * 4 + 3] != 0x00) {
                box.x1 = xx;
                box.y1 = yy;
                goto _find_y2;
            }
        }
        line += parm->image->bpl;
    }
    return (FALSE);

_find_y2:
    line  = parm->image->data;
    line += parm->image->size;
    line -= parm->image->bpl;
    for (yy = hh - 1; yy >= 0; yy--) {
        for (xx = ww - 1; xx >= 0; xx--) {
            if (line[xx * 4 + 3] != 0x00) {
                box.x2 = xx;
                box.y2 = yy;
                goto _find_xx;
            }
        }
        line -= parm->image->bpl;
    }
    return (FALSE);

_find_xx:
    line  = parm->image->data;
    line += parm->image->bpl * box.y1;
    for (yy = box.y1; yy <= box.y2; yy++) {
        for (xx = 0; xx < ww; xx++) {
            if (line[xx * 4 + 3] != 0x00) {
                if (box.x1 > xx)
                    box.x1 = xx;
                break;
            }
        }
        if (xx < ww) {
            for (xx = ww - 1; xx >= 0; xx--) {
                if (line[xx * 4 + 3] != 0x00) {
                    if (box.x2 < xx)
                        box.x2 = xx;
                    break;
                }
            }
        }
        line += parm->image->bpl;
    }
    if (box.x1 > box.x2 || box.y1 > box.y2)
        return (FALSE);
    box.ww = box.x2 - box.x1 + 1;
    box.hh = box.y2 - box.y1 + 1;
    save = image_grab(parm->image, &box);
    if (save == NULL)
        return (FALSE);
    retc = qst_save_img(save, parm, name, argc, argv);
    image_del(save);
    return (retc);
}

/*
=======================================
    保存显示图片
=======================================
*/
CR_API bool_t
qst_save_show (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    leng_t  size;
    bool_t  retc;
    ansi_t* path;
    ansi_t* fext;
    ansi_t* full;

    if (parm->image == NULL || parm->alpha == NULL)
        return (FALSE);
    size = str_lenA(name) + 1;
    path = str_allocA(size);
    if (path == NULL)
        return (FALSE);
    mem_cpy(path, name, size);
    filext_removeA(path);
    fext = str_allocA(size);
    if (fext == NULL) {
        mem_free(path);
        return (FALSE);
    }
    filext_extractA(fext, name);
    full = str_allocA(size + 4);
    if (full == NULL) {
        mem_free(fext);
        mem_free(path);
        return (FALSE);
    }
    sprintf(full, "%s_rgb%s", path, fext);
    retc  = qst_save_img(parm->image, parm, full, argc, argv);
    sprintf(full, "%s_alp%s", path, fext);
    retc &= qst_save_img(parm->alpha, parm, full, argc, argv);
    mem_free(full);
    mem_free(fext);
    mem_free(path);
    return (retc);
}

/*
=======================================
    保存显示图片 (局部+旋转+缩放)
=======================================
*/
CR_API bool_t
qst_save_show2 (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[],
  __CR_IN__ const sRECT*        box,
  __CR_IN__ fp32_t              ccw,
  __CR_IN__ fp32_t              scale,
  __CR_IN__ bool_t              lerp
    )
{
    bool_t  retc;
    sIMAGE* save;

    if (parm->image == NULL)
        return (FALSE);
    if (ccw < 0.0f)
        save = image_grab(parm->image, box);
    else
        save = image_rotz(parm->image, box, ccw * CR_DTOR, scale, lerp);
    if (save == NULL)
        return (FALSE);
    retc = qst_save_img(save, parm, name, argc, argv);
    image_del(save);
    return (retc);
}

/*
=======================================
    保存显示图片 (切图)
=======================================
*/
CR_API bool_t
qst_save_show3 (
  __CR_IN__ const sQstView2D*   parm,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[],
  __CR_IN__ uint_t              tilew,
  __CR_IN__ uint_t              tileh,
  __CR_IN__ uint_t              stepx,
  __CR_IN__ uint_t              stepy
    )
{
    sRECT   box;
    bool_t  retc;
    sIMAGE* save;
    ansi_t* path;
    ansi_t* fext;
    ansi_t* full;
    uint_t  xx, yy;
    uint_t  ww, hh;
    uint_t  cw, ch;

    if (parm->image == NULL)
        return (FALSE);
    path = str_dupA(name);
    if (path == NULL)
        return (FALSE);
    retc = FALSE;
    fext = str_allocA(str_sizeA(name));
    if (fext == NULL)
        goto _func_out1;
    filext_removeA(path);
    filext_extractA(fext, name);
    ww = parm->image->position.ww;
    hh = parm->image->position.hh;
    for (yy = 0; yy < hh; yy += stepy) {
        if (tileh > hh - yy)
            ch = hh - yy;
        else
            ch = tileh;
        for (xx = 0; xx < ww; xx += stepx) {
            if (tilew > ww - xx)
                cw = ww - xx;
            else
                cw = tilew;
            rect_set_wh(&box, xx, yy, cw, ch);
            save = image_grab(parm->image, &box);
            if (save == NULL)
                goto _func_out2;
            full = str_fmtA("%s(%04u,%04u)%s", path, xx, yy, fext);
            if (full == NULL) {
                image_del(save);
                goto _func_out2;
            }
            retc = qst_save_img(save, parm, full, argc, argv);
            image_del(save);
            mem_free(full);
            if (!retc)
                goto _func_out2;
        }
    }
_func_out2:
    mem_free(fext);
_func_out1:
    mem_free(path);
    return (retc);
}
