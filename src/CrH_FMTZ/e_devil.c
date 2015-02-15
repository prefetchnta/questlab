/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-09-20  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>> CrHack DevIL FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fmtint.h"
#include "strlib.h"
#include "devil/il.h"
#include "fmtz/devil.h"

/* DevIL 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "ResIL.lib")
#endif

/* DevIL 是否已经初始化 */
static bool_t   s_init = FALSE;

/*
=======================================
    初始化 DevIL 库
=======================================
*/
CR_API bool_t
devil_init (void_t)
{
    /* 使用独立加载函数必须调用
       使用引擎接口或者插件则不需要 */
    if (!s_init) {
        ilInit();
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
        s_init = TRUE;
    }
    return (TRUE);
}

/*
=======================================
    释放 DevIL 库
=======================================
*/
CR_API void_t
devil_kill (void_t)
{
    if (s_init) {
        ilShutDown();
        s_init = FALSE;
    }
}

/*
---------------------------------------
    DevIL 类型信息
---------------------------------------
*/
static const ansi_t*
devil_type (
  __CR_IN__ ILenum  format
    )
{
    const ansi_t*   info;

    switch (format)
    {
        default:
            info = "Unknown format";
            break;

        case IL_BMP:
            info = "Microsoft Windows Bitmap - .bmp extension";
            break;

        case IL_CUT:
            info = "Dr. Halo - .cut extension";
            break;

        case IL_DOOM:
            info = "DooM walls - no specific extension";
            break;

        case IL_DOOM_FLAT:
            info = "DooM flats - no specific extension";
            break;

        case IL_ICO:
            info = "Microsoft Windows Icons and Cursors"
                   " - .ico and .cur extensions";
            break;

        case IL_JPG:
            info = "JPEG - .jpg, .jpe and .jpeg extensions";
            break;

        case IL_ILBM:
            info = "Amiga IFF (FORM ILBM) - .iff, .ilbm, .lbm extensions";
            break;

        case IL_PCD:
            info = "Kodak PhotoCD - .pcd extension";
            break;

        case IL_PCX:
            info = "ZSoft PCX - .pcx extension";
            break;

        case IL_PIC:
            info = "PIC - .pic extension";
            break;

        case IL_PNG:
            info = "Portable Network Graphics - .png extension";
            break;

        case IL_PNM:
            info = "Portable Any Map - .pbm, .pgm, .ppm and .pnm extensions";
            break;

        case IL_SGI:
            info = "Silicon Graphics - .sgi, .bw, .rgb and .rgba extensions";
            break;

        case IL_TGA:
            info = "TrueVision Targa File - .tga, .vda, .icb and"
                   " .vst extensions";
            break;

        case IL_TIF:
            info = "Tagged Image File Format - .tif and .tiff extensions";
            break;

        case IL_CHEAD:
            info = "C-Style Header - .h extension";
            break;

        case IL_RAW:
            info = "Raw Image Data - any extension";
            break;

        case IL_MDL:
            info = "Half-Life Model Texture - .mdl extension";
            break;

        case IL_WAL:
            info = "Quake 2 Texture - .wal extension";
            break;

        case IL_LIF:
            info = "Homeworld Texture - .lif extension";
            break;

        case IL_MNG:
            info = "Multiple-image Network Graphics - .mng extension";
            break;

        case IL_GIF:
            info = "Graphics Interchange Format - .gif extension";
            break;

        case IL_DDS:
            info = "DirectDraw Surface - .dds extension";
            break;

        case IL_DCX:
            info = "ZSoft Multi-PCX - .dcx extension";
            break;

        case IL_PSD:
            info = "Adobe PhotoShop - .psd extension";
            break;

        case IL_PSP:
            info = "PaintShop Pro - .psp extension";
            break;

        case IL_PIX:
            info = "PIX - .pix extension";
            break;

        case IL_PXR:
            info = "Pixar - .pxr extension";
            break;

        case IL_XPM:
            info = "X Pixel Map - .xpm extension";
            break;

        case IL_HDR:
            info = "Radiance High Dynamic Range - .hdr extension";
            break;

        case IL_ICNS:
            info = "Macintosh Icon - .icns extension";
            break;

        case IL_JP2:
            info = "Jpeg 2000 - .jp2 extension";
            break;

        case IL_EXR:
            info = "OpenEXR - .exr extension";
            break;

        case IL_WDP:
            info = "Microsoft HD Photo - .wdp and .hdp extension";
            break;

        case IL_VTF:
            info = "Valve Texture Format - .vtf extension";
            break;

        case IL_WBMP:
            info = "Wireless Bitmap - .wbmp extension";
            break;

        case IL_SUN:
            info = "Sun Raster - .sun, .ras, .rs, .im1, .im8, .im24 and"
                   " .im32 extensions";
            break;

        case IL_IFF:
            info = "Interchange File Format - .iff extension";
            break;

        case IL_TPL:
            info = "Gamecube Texture - .tpl extension";
            break;

        case IL_FITS:
            info = "Flexible Image Transport System - .fit and"
                   " .fits extensions";
            break;

        case IL_DICOM:
            info = "Digital Imaging and Communications in Medicine (DICOM)"
                   " - .dcm and .dicom extensions";
            break;

        case IL_IWI:
            info = "Call of Duty Infinity Ward Image - .iwi extension";
            break;

        case IL_BLP:
            info = "Blizzard Texture Format - .blp extension";
            break;

        case IL_FTX:
            info = "Heavy Metal: FAKK2 Texture - .ftx extension";
            break;

        case IL_ROT:
            info = "Homeworld 2 - Relic Texture - .rot extension";
            break;

        case IL_TEXTURE:
            info = "Medieval II: Total War Texture - .texture extension";
            break;

        case IL_DPX:
            info = "Digital Picture Exchange - .dpx extension";
            break;

        case IL_UTX:
            info = "Unreal (and Unreal Tournament) Texture - .utx extension";
            break;

        case IL_MP3:
            info = "MPEG-1 Audio Layer 3 - .mp3 extension";
            break;
    }
    return (info);
}

/*
---------------------------------------
    DevIL 图片信息
---------------------------------------
*/
static bool_t
devil_info (
  __CR_OT__ sFMT_FRAME* frame
    )
{
    uint_t  ww, hh;

    /* 填充图片格式信息 */
    mem_zero(frame->wh, sizeof(frame->wh));
    frame->bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
    switch (ilGetInteger(IL_IMAGE_FORMAT))
    {
        default:
            frame->fmt = CR_PIC_NONE;
            frame->clr = "C";
            frame->wh[0] = (byte_t)frame->bpp;
            break;

        case IL_COLOR_INDEX:
            frame->fmt = CR_PIC_PALS;
            frame->clr = "P";
            frame->wh[0] = (byte_t)frame->bpp;
            break;

        case IL_ALPHA:
            frame->fmt = CR_PIC_GREY;
            frame->clr = "A";
            frame->wh[0] = (byte_t)frame->bpp;
            break;

        case IL_RGB:
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "BGR";
            frame->wh[0] = (byte_t)(frame->bpp / 3);
            frame->wh[1] = frame->wh[0];
            frame->wh[2] = frame->wh[0];
            break;

        case IL_RGBA:
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "ABGR";
            frame->wh[0] = (byte_t)(frame->bpp / 4);
            frame->wh[1] = frame->wh[0];
            frame->wh[2] = frame->wh[0];
            frame->wh[3] = frame->wh[0];
            break;

        case IL_BGR:
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "RGB";
            frame->wh[0] = (byte_t)(frame->bpp / 3);
            frame->wh[1] = frame->wh[0];
            frame->wh[2] = frame->wh[0];
            break;

        case IL_BGRA:
            frame->fmt = CR_PIC_ARGB;
            frame->clr = "ARGB";
            frame->wh[0] = (byte_t)(frame->bpp / 4);
            frame->wh[1] = frame->wh[0];
            frame->wh[2] = frame->wh[0];
            frame->wh[3] = frame->wh[0];
            break;

        case IL_LUMINANCE:
            frame->fmt = CR_PIC_GREY;
            frame->clr = "I";
            frame->wh[0] = (byte_t)frame->bpp;
            break;

        case IL_LUMINANCE_ALPHA:
            frame->fmt = CR_PIC_GREY;
            frame->clr = "AI";
            frame->wh[0] = (byte_t)(frame->bpp / 2);
            frame->wh[1] = frame->wh[0];
            break;
    }
    switch (ilGetInteger(IL_IMAGE_TYPE))
    {
        default:
            break;

        case IL_HALF:
            frame->fmt = CR_PIC_IEEE;
            break;

        case IL_FLOAT:
            frame->fmt = CR_PIC_IEEE;
            break;

        case IL_DOUBLE:
            frame->fmt = CR_PIC_IEEE;
            break;
    }

    /* 转换到 CrHack 位图格式 */
    ww = ilGetInteger(IL_IMAGE_WIDTH);
    hh = ilGetInteger(IL_IMAGE_HEIGHT);
    frame->pic = image_new(0, 0, ww, hh, CR_ARGB8888, FALSE, 4);
    if (frame->pic == NULL)
        return (FALSE);
    ilCopyPixels(0, 0, 0, ww, hh, 1, IL_BGRA, IL_UNSIGNED_BYTE,
                 frame->pic->data);
    return (TRUE);
}

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPICTURE    pics;

        /* 个性部分 */
        ILuint          m_image;
        void_t*         m_fdata;
        const ansi_t*   m_infor;

} iPIC_IL;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPIC_IL_release (
  __CR_IN__ iPICTURE*   that
    )
{
    iPIC_IL*    real;

    real = (iPIC_IL*)that;
    ilDeleteImages(1, &real->m_image);
    TRY_FREE(real->m_fdata);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPIC_IL_getMore (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPICTURE::DEVIL") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    获取图片帧
---------------------------------------
*/
static sFMT_PIC*
iPIC_IL_get (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ int32u      index
    )
{
    iPIC_IL*    real;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 帧号过滤 */
    if (index >= that->__count__)
        return (NULL);

    /* 获取指定帧 */
    real = (iPIC_IL*)that;
    ilBindImage(real->m_image);
    if (!ilActiveImage((ILuint)index))
        return (NULL);
    if (!devil_info(&temp))
        return (NULL);

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
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = real->m_infor;
    return (rett);
}

/* 接口虚函数表 */
static const iPICTURE_vtbl _rom_ s_pics_vtbl =
{
    iPIC_IL_release, iPIC_IL_getMore, iPIC_IL_get,
};

/*
---------------------------------------
    DevIL 加载图片
---------------------------------------
*/
static sFMTZ*
devil_load (
  __CR_IN__ ILenum          format,
  __CR_IN__ const sLOADER*  loader
    )
{
    uint_t      cnts;
    ILuint      size;
    void_t*     data;
#if defined(UNICODE) || defined(_UNICODE)
    wide_t*     path;
#else
    ansi_t*     path;
#endif
    ILuint      imgs;
    iPIC_IL*    port;
    sFMT_PIC*   rets;
    sFMT_PRT*   retm;
    ILboolean   okay;
    sFMT_FRAME  temp;

    /* 必须先初始化 */
    if (!s_init)
        return (NULL);

    /* 未知格式自动识别 */
    if (format == IL_TYPE_UNKNOWN)
    {
        /* 不支持文件区段功能 */
        switch (loader->type)
        {
            case CR_LDR_ANSI:
#if defined(UNICODE) || defined(_UNICODE)
                path = local_to_utf16(CR_LOCAL, loader->name.ansi);
                if (path == NULL)
                    return (NULL);
                format = ilDetermineType(path);
                if (format == IL_TYPE_UNKNOWN)
                    format = ilTypeFromExt(path);
                mem_free(path);
#else
                format = ilTypeFromExt(loader->name.ansi);
                if (format == IL_TYPE_UNKNOWN)
                    format = ilTypeFromExt(loader->name.ansi);
#endif
                break;

            case CR_LDR_WIDE:
#if defined(UNICODE) || defined(_UNICODE)
                format = ilDetermineType(loader->name.wide);
                if (format == IL_TYPE_UNKNOWN)
                    format = ilTypeFromExt(loader->name.wide);
#else
                path = utf16_to_local(CR_LOCAL, loader->name.wide);
                if (path == NULL)
                    return (NULL);
                format = ilDetermineType(path);
                if (format == IL_TYPE_UNKNOWN)
                    format = ilTypeFromExt(path);
                mem_free(path);
#endif
                break;

            case CR_LDR_BUFF:
                if (cut_size(&size, loader->buff.size))
                    return (NULL);
                format = ilDetermineTypeL(loader->buff.data, size);
                break;

            default:
                return (NULL);
        }

        /* 还是无法确定格式 */
        if (format == IL_TYPE_UNKNOWN)
            return (NULL);
    }

    /* 加载确定的文件格式 */
    switch (loader->type)
    {
        case CR_LDR_ANSI:
            data = NULL;
#if defined(UNICODE) || defined(_UNICODE)
            path = local_to_utf16(CR_LOCAL, loader->name.ansi);
            if (path == NULL)
                return (NULL);
            ilGenImages(1, &imgs);
            ilBindImage(imgs);
            okay = ilLoad(format, path);
            mem_free(path);
#else
            ilGenImages(1, &imgs);
            ilBindImage(imgs);
            okay = ilLoad(format, loader->name.ansi);
#endif
            break;

        case CR_LDR_WIDE:
            data = NULL;
#if defined(UNICODE) || defined(_UNICODE)
            ilGenImages(1, &imgs);
            ilBindImage(imgs);
            okay = ilLoad(format, loader->name.wide);
#else
            path = utf16_to_local(CR_LOCAL, loader->name.wide);
            if (path == NULL)
                return (NULL);
            ilGenImages(1, &imgs);
            ilBindImage(imgs);
            okay = ilLoad(format, path);
            mem_free(path);
#endif
            break;

        case CR_LDR_BUFF:
            if (cut_size(&size, loader->buff.size))
                return (NULL);
            data = mem_dup(loader->buff.data, loader->buff.size);
            if (data == NULL)
                return (NULL);
            ilGenImages(1, &imgs);
            ilBindImage(imgs);
            okay = ilLoadL(format, data, size);
            break;

        default:
            return (NULL);
    }

    /* 图片加载失败 */
    if (!okay)
        goto _failure;

    /* 单帧图片不使用流接口 */
    cnts = ilGetInteger(IL_NUM_IMAGES) + 1;
    if (cnts == 1) {
        if (!devil_info(&temp))
            goto _failure;
        ilDeleteImages(1, &imgs);
        TRY_FREE(data);

        /* 返回读取的文件数据 */
        rets = struct_new(sFMT_PIC);
        if (rets == NULL) {
            image_del(temp.pic);
            return (NULL);
        }
        rets->frame = struct_dup(&temp, sFMT_FRAME);
        if (rets->frame == NULL) {
            image_del(temp.pic);
            mem_free(rets);
            return (NULL);
        }
        rets->type = CR_FMTZ_PIC;
        rets->count = 1;
        rets->infor = devil_type(format);
        return ((sFMTZ*)rets);
    }

    /* 生成多帧图片接口对象 */
    port = struct_new(iPIC_IL);
    if (port == NULL)
        goto _failure;
    port->m_fdata = data;
    port->m_image = imgs;
    port->m_infor = devil_type(format);
    port->pics.__count__ = cnts;
    port->pics.__vptr__ = &s_pics_vtbl;

    /* 返回读取的文件数据 */
    retm = struct_new(sFMT_PRT);
    if (retm == NULL) {
        iPIC_IL_release((iPICTURE*)port);
        return (NULL);
    }
    retm->type = CR_FMTZ_PRT;
    retm->port = (iPORT*)port;
    retm->more = "iPICTURE";
    retm->infor = port->m_infor;
    return ((sFMTZ*)retm);

_failure:
    ilDeleteImages(1, &imgs);
    TRY_FREE(data);
    return (NULL);
}

/*
=======================================
    BMP 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_bmp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_BMP, param));
}

/*
=======================================
    CUT 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_cut (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_CUT, param));
}

/*
=======================================
    DOOM WALLS 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_doomw (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DOOM, param));
}

/*
=======================================
    DOOM FLATS 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_doomf (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DOOM_FLAT, param));
}

/*
=======================================
    ICO 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_ico (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_ICO, param));
}

/*
=======================================
    JPG 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_jpg (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_JPG, param));
}

/*
=======================================
    ILBM 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_ilbm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_ILBM, param));
}

/*
=======================================
    PCD 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pcd (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PCD, param));
}

/*
=======================================
    PCX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pcx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PCX, param));
}

/*
=======================================
    PIC 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pic (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PIC, param));
}

/*
=======================================
    PNG 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_png (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PNG, param));
}

/*
=======================================
    PNM 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pnm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PNM, param));
}

/*
=======================================
    SGI 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_sgi (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_SGI, param));
}

/*
=======================================
    TGA 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_tga (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_TGA, param));
}

/*
=======================================
    TIF 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_tif (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_TIF, param));
}

/*
=======================================
    CHEAD 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_chead (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_CHEAD, param));
}

/*
=======================================
    RAW 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_raw (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_RAW, param));
}

/*
=======================================
    MDL 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_mdl (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_MDL, param));
}

/*
=======================================
    WAL 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_wal (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_WAL, param));
}

/*
=======================================
    LIF 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_lif (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_LIF, param));
}

/*
=======================================
    MNG 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_mng (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_MNG, param));
}

/*
=======================================
    GIF 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_gif (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_GIF, param));
}

/*
=======================================
    DDS 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_dds (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DDS, param));
}

/*
=======================================
    DCX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_dcx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DCX, param));
}

/*
=======================================
    PSD 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_psd (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PSD, param));
}

/*
=======================================
    PSP 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_psp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PSP, param));
}

/*
=======================================
    PIX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pix (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PIX, param));
}

/*
=======================================
    PXR 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_pxr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_PXR, param));
}

/*
=======================================
    XPM 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_xpm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_XPM, param));
}

/*
=======================================
    HDR 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_hdr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_HDR, param));
}

/*
=======================================
    ICNS 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_icns (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_ICNS, param));
}

/*
=======================================
    JP2 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_jp2 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_JP2, param));
}

/*
=======================================
    EXR 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_exr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_EXR, param));
}

/*
=======================================
    WDP 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_wdp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_WDP, param));
}

/*
=======================================
    VTF 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_vtf (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_VTF, param));
}

/*
=======================================
    WBMP 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_wbmp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_WBMP, param));
}

/*
=======================================
    SUN 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_sun (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_SUN, param));
}

/*
=======================================
    IFF 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_iff (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_IFF, param));
}

/*
=======================================
    TPL 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_tpl (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_TPL, param));
}

/*
=======================================
    FITS 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_fits (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_FITS, param));
}

/*
=======================================
    DICOM 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_dicom (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DICOM, param));
}

/*
=======================================
    IWI 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_iwi (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_IWI, param));
}

/*
=======================================
    BLP 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_blp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_BLP, param));
}

/*
=======================================
    FTX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_ftx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_FTX, param));
}

/*
=======================================
    ROT 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_rot (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_ROT, param));
}

/*
=======================================
    TEXTURE 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_tex (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_TEXTURE, param));
}

/*
=======================================
    DPX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_dpx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_DPX, param));
}

/*
=======================================
    UTX 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_utx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_UTX, param));
}

/*
=======================================
    MP3 文件读取
=======================================
*/
CR_API sFMTZ*
load_il_mp3 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_MP3, param));
}

/*
=======================================
    图片文件读取 (自动识别)
=======================================
*/
CR_API sFMTZ*
load_il_auto (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (devil_load(IL_TYPE_UNKNOWN, param));
}

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_devil.inl"
#define _CR_FMTZ_WIDE_
#include "e_devil.inl"
#undef  _CR_FMTZ_WIDE_

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_devil_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
    return (fmtz_find(engine, loader));
}

/*
---------------------------------------
    引擎插件释放回调
---------------------------------------
*/
static void_t
engine_devil_free (
  __CR_IO__ sENGINE*    engine
    )
{
    devil_kill();
    CR_NOUSE(engine);
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_devil (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_finda, s_findw, s_loada, s_loadw);
    if (engine == NULL)
        return (NULL);
    if (!devil_init()) {
        engine_free(engine);
        return (NULL);
    }
    engine->fmtz_load = engine_devil_load;
    engine->engine_free = engine_devil_free;
    engine->info = "DevIL FMTz Engine (Done by CrHackOS)";
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
    return (engine_devil());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
