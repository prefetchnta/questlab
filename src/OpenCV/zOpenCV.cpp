
#include "xOpenCV.h"
#include "../CrH_FMTZ/fmtint.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPICTURE    pics;

        /* 个性部分 */
        camera_t    m_cam;

} iPIC_OCV;

/*
---------------------------------------
    OpenCV 图片信息
---------------------------------------
*/
static bool_t
opencv_info (
  __CR_OT__ sFMT_FRAME*     frame,
  __CR_IN__ const ipls_t*   image
    )
{
    frame->pic = ilab_ipl2img_dup(image);
    if (frame->pic == NULL)
        return (FALSE);
    switch (frame->pic->fmt)
    {
        default:
            ilab_img_del(frame->pic);
            return (FALSE);

        case CR_INDEX8:
            frame->bpp = 8;
            frame->fmt = CR_PIC_GREY;
            frame->clr = "I";
            frame->wh[0] = 8;
            break;

        case CR_ARGB888:
            frame->bpp = 24;
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "RGB";
            frame->wh[0] = 8;
            frame->wh[1] = 8;
            frame->wh[2] = 8;
            break;

        case CR_ARGB8888:
            frame->bpp = 32;
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "ARGB";
            frame->wh[0] = 8;
            frame->wh[1] = 8;
            frame->wh[2] = 8;
            frame->wh[3] = 8;
            break;
    }
    return (TRUE);
}

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPIC_OCV_release (
  __CR_IN__ iPICTURE*   that
    )
{
    iPIC_OCV*   real;

    real = (iPIC_OCV*)that;
    ilab_camera_del(real->m_cam);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPIC_OCV_getMore (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPICTURE::OCV_CAM") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    获取图片帧
---------------------------------------
*/
static sFMT_PIC*
iPIC_OCV_get (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ int32u      index
    )
{
    ipls_t*     ipls;
    iPIC_OCV*   real;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 获取摄像头一帧图像 */
    real = (iPIC_OCV*)that;
    ipls = ilab_camera_get(real->m_cam);
    if (ipls == NULL)
        return (NULL);

    /* 复制到 sIMAGE 结构并填充信息 */
    if (!opencv_info(&temp, ipls)) {
        ilab_ipl_del(ipls);
        return (NULL);
    }
    ilab_ipl_del(ipls);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        image_del(temp.pic);
        return (NULL);
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        image_del(temp.pic);
        mem_free(rett);
        return (NULL);
    }
    CR_NOUSE(index);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Image captured from camera by OpenCV";
    return (rett);
}

/* 接口虚函数表 */
static const iPICTURE_vtbl _rom_ s_pics_vtbl =
{
    iPIC_OCV_release, iPIC_OCV_getMore, iPIC_OCV_get,
};

/* 摄像头设备名 */
#define OCV_DEV_CAMA    CR_AS(":opencv:camera")
#define OCV_DEV_CAMW    CR_WS(":opencv:camera")

/*
=======================================
    OpenCV 摄像头读取
=======================================
*/
CR_API sFMT_PRT*
load_ocv_cam (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    uint_t      cid;
    leng_t      len;
    camera_t    ocam;
    sFMT_PRT*   rett;
    iPIC_OCV*   port;

    /* 根据设备名称创建摄像头对象 */
    switch (param->type)
    {
        case CR_LDR_ANSI:
            len = str_lenA(OCV_DEV_CAMA);
            if (chr_cmpA(param->name.ansi, OCV_DEV_CAMA, len) != 0)
                return (NULL);
            cid = str2intxA(param->name.ansi + len);
            break;

        case CR_LDR_WIDE:
            len = str_lenW(OCV_DEV_CAMW);
            if (chr_cmpW(param->name.wide, OCV_DEV_CAMW, len) != 0)
                return (NULL);
            cid = str2intxW(param->name.wide + len);
            break;

        default:
            return (NULL);
    }
    ocam = ilab_camera_new(cid);
    if (ocam == NULL)
        return (NULL);

    /* 生成多帧图片接口对象 */
    port = struct_new(iPIC_OCV);
    if (port == NULL) {
        ilab_camera_del(ocam);
        return (NULL);
    }
    port->m_cam = ocam;
    port->pics.__count__ = 256;
    port->pics.__vptr__ = &s_pics_vtbl;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PRT);
    if (rett == NULL) {
        iPIC_OCV_release((iPICTURE*)port);
        return (NULL);
    }
    CR_NOUSE(datin);
    rett->type = CR_FMTZ_PRT;
    rett->port = (iPORT*)port;
    rett->more = "iPICTURE";
    rett->infor = "Image captured from camera by OpenCV";
    return (rett);
}

/*
=======================================
    OpenCV 图片文件读取
=======================================
*/
CR_API sFMT_PIC*
load_ocv_pic (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    ipls_t*     ipls;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 加载图片到 IplImage 结构 */
    switch (param->type)
    {
        case CR_LDR_ANSI:
            ipls = ilab_load_fileA(param->name.ansi);
            break;

        case CR_LDR_WIDE:
            ipls = ilab_load_fileW(param->name.wide);
            break;

        case CR_LDR_BUFF:
            ipls = ilab_load_mem(param->buff.data, param->buff.size);
            break;

        default:
            return (NULL);
    }
    if (ipls == NULL)
        return (NULL);

    /* 复制到 sIMAGE 结构并填充信息 */
    if (!opencv_info(&temp, ipls)) {
        ilab_ipl_del(ipls);
        return (NULL);
    }
    ilab_ipl_del(ipls);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL) {
        image_del(temp.pic);
        return (NULL);
    }
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        image_del(temp.pic);
        mem_free(rett);
        return (NULL);
    }
    CR_NOUSE(datin);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Image file supported by OpenCV";
    return (rett);
}

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_ocv_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    sFMTZ*  fmtz;

    /* 过滤加载类型 */
    if (!(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);

    /* 优先判定摄像头 */
    fmtz = (sFMTZ*)load_ocv_cam(NULL, loader);
    if (fmtz == NULL)
        fmtz = (sFMTZ*)load_ocv_pic(NULL, loader);
    return (fmtz);
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_opencv (void_t)
{
    sENGINE*    engine;

    engine = engine_init(NULL, NULL, 0);
    if (engine == NULL)
        return (NULL);
    engine->fmtz_load = engine_ocv_load;
    engine->info = "OpenCV FMTz Engine (Done by CrHackOS)";
    return (engine);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取引擎插件接口 (同名)
=======================================
*/
CR_API sENGINE*
engine_get (void_t)
{
    return (engine_opencv());
}
#endif  /* _CR_BUILD_DLL_ */
