/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-12-08  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack FreeImage FMTZ 插件接口实现 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fmtint.h"
#include "msclib.h"
#include "strlib.h"
#include "fmtz/fimage.h"
#include "fi/FreeImage.h"

/* FreeImage 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "FreeImage.lib")
#endif

/* 可能是漏掉的宏 */
#ifndef JNG_DEFAULT
    #define JNG_DEFAULT     0
#endif
#ifndef PBM_DEFAULT
    #define PBM_DEFAULT     0
#endif
#ifndef PGM_DEFAULT
    #define PGM_DEFAULT     0
#endif
#ifndef PPM_DEFAULT
    #define PPM_DEFAULT     0
#endif

/*****************************************************************************/
/*                                 加载引擎                                  */
/*****************************************************************************/

/*
---------------------------------------
    FreeImage 类型信息
---------------------------------------
*/
static const ansi_t*
fimage_type (
  __CR_IN__ FREE_IMAGE_FORMAT   format
    )
{
    const ansi_t*   info;

    switch (format)
    {
        default:
            info = "Unknown format";
            break;

        case FIF_BMP:
            info = "Windows or OS/2 Bitmap File (*.BMP)";
            break;

        case FIF_CUT:
            info = "Dr. Halo (*.CUT)";
            break;

        case FIF_DDS:
            info = "DirectDraw Surface (*.DDS)";
            break;

        case FIF_EXR:
            info = "ILM OpenEXR (*.EXR)";
            break;

        case FIF_FAXG3:
            info = "Raw Fax format CCITT G3 (*.G3)";
            break;

        case FIF_GIF:
            info = "Graphics Interchange Format (*.GIF)";
            break;

        case FIF_HDR:
            info = "High Dynamic Range (*.HDR)";
            break;

        case FIF_ICO:
            info = "Windows Icon (*.ICO)";
            break;

        case FIF_IFF:
            info = "Amiga IFF (*.IFF, *.LBM)";
            break;

        case FIF_J2K:
            info = "JPEG-2000 codestream (*.J2K, *.J2C)";
            break;

        case FIF_JNG:
            info = "JPEG Network Graphics (*.JNG)";
            break;

        case FIF_JP2:
            info = "JPEG-2000 File Format (*.JP2)";
            break;

        case FIF_JPEG:
            info = "Independent JPEG Group (*.JPG, *.JIF, *.JPEG, *.JPE)";
            break;

        case FIF_JXR:
            info = "JPEG XR image format (*.JXR, *.WDP, *.HDP)";
            break;

        case FIF_KOALA:
            info = "Commodore 64 Koala format (*.KOA)";
            break;

        case FIF_MNG:
            info = "Multiple Network Graphics (*.MNG)";
            break;

        case FIF_PBM:
            info = "Portable Bitmap (ASCII) (*.PBM)";
            break;

        case FIF_PBMRAW:
            info = "Portable Bitmap (BINARY) (*.PBM)";
            break;

        case FIF_PCD:
            info = "Kodak PhotoCD (*.PCD)";
            break;

        case FIF_PCX:
            info = "Zsoft Paintbrush PCX bitmap format (*.PCX)";
            break;

        case FIF_PFM:
            info = "Portable Floatmap (*.PFM)";
            break;

        case FIF_PGM:
            info = "Portable Graymap (ASCII) (*.PGM)";
            break;

        case FIF_PGMRAW:
            info = "Portable Graymap (BINARY) (*.PGM)";
            break;

        case FIF_PICT:
            info = "Macintosh PICT (*.PCT, *.PICT, *.PIC)";
            break;

        case FIF_PNG:
            info = "Portable Network Graphics (*.PNG)";
            break;

        case FIF_PPM:
            info = "Portable Pixelmap (ASCII) (*.PPM)";
            break;

        case FIF_PPMRAW:
            info = "Portable Pixelmap (BINARY) (*.PPM)";
            break;

        case FIF_PSD:
            info = "Adobe Photoshop (*.PSD)";
            break;

        case FIF_RAS:
            info = "Sun Rasterfile (*.RAS)";
            break;

        case FIF_RAW:
            info = "RAW camera image (many extensions)";
            break;

        case FIF_SGI:
            info = "Silicon Graphics SGI image format (*.SGI)";
            break;

        case FIF_TARGA:
            info = "Truevision Targa files (*.TGA, *.TARGA)";
            break;

        case FIF_TIFF:
            info = "Tagged Image File Format (*.TIF, *.TIFF)";
            break;

        case FIF_WBMP:
            info = "Wireless Bitmap (*.WBMP)";
            break;

        case FIF_WEBP:
            info = "Google WebP image format (*.WEBP)";
            break;

        case FIF_XBM:
            info = "X11 Bitmap Format (*.XBM)";
            break;

        case FIF_XPM:
            info = "X11 Pixmap Format (*.XPM)";
            break;
    }
    return (info);
}

/*
---------------------------------------
    FreeImage 图片信息
---------------------------------------
*/
static bool_t
fimage_info (
  __CR_OT__ sFMT_FRAME* frame,
  __CR_IN__ FIBITMAP*   bitmap
    )
{
    uint_t                  na;
    uint_t                  nr;
    uint_t                  ng;
    uint_t                  nb;
    uint_t                  temp;
    leng_t                  line;
    byte_t*                 data;
    FIBITMAP*               cnvt;
    FREE_IMAGE_TYPE         itype;
    FREE_IMAGE_COLOR_TYPE   ctype;

    /* 填充图片格式信息 */
    frame->bpp = FreeImage_GetBPP(bitmap);
    ctype = FreeImage_GetColorType(bitmap);
    switch (ctype)
    {
        default:
            frame->fmt = CR_PIC_NONE;
            break;

        case FIC_PALETTE:
        case FIC_MINISBLACK:
        case FIC_MINISWHITE:
            frame->fmt = CR_PIC_PALS;
            break;

        case FIC_RGB:
        case FIC_RGBALPHA:
            frame->fmt = CR_PIC_ARGB;
            break;

        case FIC_CMYK:
            frame->fmt = CR_PIC_CMYK;
            break;
    }
    mem_zero(frame->wh, sizeof(frame->wh));
    itype = FreeImage_GetImageType(bitmap);
    switch (itype)
    {
        default:
        case FIT_UNKNOWN:
            frame->clr = "C";
            frame->wh[0] = (byte_t)frame->bpp;
            break;

        case FIT_BITMAP:
            temp = FreeImage_GetColorsUsed(bitmap);
            if (temp == 0)
            {
                /* 高彩色模式 */
                nr = FreeImage_GetRedMask(bitmap);
                ng = FreeImage_GetGreenMask(bitmap);
                nb = FreeImage_GetBlueMask(bitmap);
                nr = count_bits32((int32u)nr);
                ng = count_bits32((int32u)ng);
                nb = count_bits32((int32u)nb);
                na = frame->bpp - nr - ng - nb;
                if (na == frame->bpp) {
                    if (frame->bpp == 24) {
                        frame->clr = "RGB";
                        frame->wh[0] = 8;
                        frame->wh[1] = 8;
                        frame->wh[2] = 8;
                    }
                    else
                    if (frame->bpp == 32) {
                        frame->clr = "ARGB";
                        frame->wh[0] = 8;
                        frame->wh[1] = 8;
                        frame->wh[2] = 8;
                        frame->wh[3] = 8;
                    }
                    else {
                        frame->clr = "C";
                        frame->wh[0] = (byte_t)frame->bpp;
                    }
                }
                else {
                    if (ctype == FIC_RGB) {
                        frame->clr = "RGB";
                        frame->wh[0] = (byte_t)nr;
                        frame->wh[1] = (byte_t)ng;
                        frame->wh[2] = (byte_t)nb;
                    }
                    else
                    if (ctype == FIC_RGBALPHA) {
                        frame->clr = "ARGB";
                        frame->wh[0] = (byte_t)na;
                        frame->wh[1] = (byte_t)nr;
                        frame->wh[2] = (byte_t)ng;
                        frame->wh[3] = (byte_t)nb;
                    }
                    else {
                        frame->clr = "C";
                        frame->wh[0] = (byte_t)frame->bpp;
                    }
                }
            }
            else
            {
                /* 调色板模式 */
                frame->clr = "P";
                frame->wh[0] = (byte_t)frame->bpp;
            }
            break;

        case FIT_INT16:
        case FIT_UINT16:
            frame->fmt = CR_PIC_GREY;
            frame->clr = "I";
            frame->wh[0] = 16;
            break;

        case FIT_INT32:
        case FIT_UINT32:
            frame->fmt = CR_PIC_GREY;
            frame->clr = "I";
            frame->wh[0] = 32;
            break;

        case FIT_FLOAT:
            frame->fmt = CR_PIC_IEEE;
            frame->clr = "F";
            frame->wh[0] = 32;
            break;

        case FIT_DOUBLE:
            frame->fmt = CR_PIC_IEEE;
            frame->clr = "F";
            frame->wh[0] = 64;
            break;

        case FIT_COMPLEX:
            frame->fmt = CR_PIC_CMPX;
            frame->clr = "RI";
            frame->wh[0] = 64;
            frame->wh[1] = 64;
            break;

        case FIT_RGB16:
            frame->clr = "RGB";
            frame->wh[0] = 16;
            frame->wh[1] = 16;
            frame->wh[2] = 16;
            break;

        case FIT_RGBA16:
            frame->clr = "ARGB";
            frame->wh[0] = 16;
            frame->wh[1] = 16;
            frame->wh[2] = 16;
            frame->wh[3] = 16;
            break;

        case FIT_RGBF:
            frame->fmt = CR_PIC_IEEE;
            frame->clr = "RGB";
            frame->wh[0] = 32;
            frame->wh[1] = 32;
            frame->wh[2] = 32;
            break;

        case FIT_RGBAF:
            frame->fmt = CR_PIC_IEEE;
            frame->clr = "ARGB";
            frame->wh[0] = 32;
            frame->wh[1] = 32;
            frame->wh[2] = 32;
            frame->wh[3] = 32;
            break;
    }

    /* 转换到 CrHack 位图格式 */
    ng = FreeImage_GetWidth(bitmap);
    nb = FreeImage_GetHeight(bitmap);
    frame->pic = image_new(0, 0, ng, nb, CR_ARGB8888, FALSE, 8);
    if (frame->pic == NULL)
        return (FALSE);

    /* 统一转换到32位图格式 */
    cnvt = FreeImage_ConvertTo32Bits(bitmap);
    if (cnvt == NULL)
    {
        /* 不支持转换的颜色格式返回空白图 */
        mem_zero(frame->pic->data, frame->pic->size);
    }
    else
    {
        /* 逐行复制图片 */
        line = ng * 4;
        data = frame->pic->data;
        for (na = 0; na < nb; na++) {
            ng = nb - na - 1;   /* FI 使用的是 GDI 格式 */
            mem_cpy(data, FreeImage_GetScanLine(cnvt, (int)ng), line);
            data += frame->pic->bpl;
        }
        FreeImage_Unload(cnvt);
    }
    return (TRUE);
}

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPICTURE    pics;

        /* 个性部分 */
        void_t*         m_fdata;
        FIMEMORY*       m_memio;
        const ansi_t*   m_infor;
        FIMULTIBITMAP*  m_multi;

} iPIC_FI;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPIC_FI_release (
  __CR_IN__ iPICTURE*   that
    )
{
    iPIC_FI*    real;

    real = (iPIC_FI*)that;
    FreeImage_CloseMultiBitmap(real->m_multi, 0);
    if (real->m_memio != NULL)
        FreeImage_CloseMemory(real->m_memio);
    TRY_FREE(real->m_fdata);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPIC_FI_getMore (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPICTURE::FIMAGE") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    获取图片帧
---------------------------------------
*/
static sFMT_PIC*
iPIC_FI_get (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ int32u      index
    )
{
    bool_t      okay;
    iPIC_FI*    real;
    sFMT_PIC*   rett;
    FIBITMAP*   imgs;
    sFMT_FRAME  temp;

    /* 帧号过滤 */
    if (index >= that->__count__)
        return (NULL);

    /* 获取指定帧 */
    real = (iPIC_FI*)that;
    imgs = FreeImage_LockPage(real->m_multi, (int)index);
    if (imgs == NULL)
        return (NULL);
    okay = fimage_info(&temp, imgs);
    FreeImage_UnlockPage(real->m_multi, imgs, FALSE);
    if (!okay)
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
    iPIC_FI_release, iPIC_FI_getMore, iPIC_FI_get,
};

/*
---------------------------------------
    FreeImage 加载图片
---------------------------------------
*/
static sFMTZ*
fimage_load (
  __CR_IN__ FREE_IMAGE_FORMAT   format,
  __CR_IN__ const sLOADER*      loader,
  __CR_IN__ sint_t              flags
    )
{
    uint_t      cnts;
    uint_t      size;
    bool_t      okay;
    void_t*     data;
    ansi_t*     path;
    iPIC_FI*    port;
    sFMT_PIC*   rets;
    sFMT_PRT*   retm;
    sFMT_FRAME  temp;
    /* ----------- */
    FIBITMAP*       image;
    FIMEMORY*       memio;
    FIMULTIBITMAP*  multi;

    /* 未知格式自动识别 */
    if (format == FIF_UNKNOWN)
    {
        /* 不支持文件区段功能 */
        switch (loader->type)
        {
            case CR_LDR_ANSI:
                format = FreeImage_GetFileType(loader->name.ansi, 0);
                if (format == FIF_UNKNOWN)
                    format = FreeImage_GetFIFFromFilename(loader->name.ansi);
                break;

            case CR_LDR_WIDE:
#ifndef _CR_OS_MSWIN_
                path = utf16_to_local(CR_LOCAL, loader->name.wide);
                if (path == NULL)
                    return (NULL);
                format = FreeImage_GetFileType(path, 0);
                if (format == FIF_UNKNOWN)
                    format = FreeImage_GetFIFFromFilename(path);
                mem_free(path);
#else
                format = FreeImage_GetFileTypeU(loader->name.wide, 0);
                if (format == FIF_UNKNOWN)
                    format = FreeImage_GetFIFFromFilenameU(loader->name.wide);
#endif
                break;

            case CR_LDR_BUFF:
                if (cut_size(&size, loader->buff.size))
                    return (NULL);
                memio = FreeImage_OpenMemory((BYTE*)loader->buff.data, size);
                if (memio == NULL)
                    return (NULL);
                format = FreeImage_GetFileTypeFromMemory(memio, 0);
                FreeImage_CloseMemory(memio);
                break;

            default:
                return (NULL);
        }

        /* 还是无法确定格式 */
        if (format == FIF_UNKNOWN)
            return (NULL);

        /* 确定格式的加载标志 */
        if (format == FIF_GIF)
            flags = GIF_PLAYBACK;
        else
        if (format == FIF_ICO)
            flags = ICO_MAKEALPHA;
        else
        if (format == FIF_JPEG)
            flags = JPEG_ACCURATE;
    }

    /* 加载确定的文件格式 */
    if (format != FIF_GIF && format != FIF_ICO &&
        format != FIF_MNG && format != FIF_TIFF)
    {
        /* 加载单帧图片文件 */
        switch (loader->type)
        {
            case CR_LDR_ANSI:
                memio = NULL;
                image = FreeImage_Load(format, loader->name.ansi, flags);
                break;

            case CR_LDR_WIDE:
                memio = NULL;
#ifndef _CR_OS_MSWIN_
                path = utf16_to_local(CR_LOCAL, loader->name.wide);
                if (path == NULL)
                    return (NULL);
                image = FreeImage_Load(format, path, flags);
                mem_free(path);
#else
                image = FreeImage_LoadU(format, loader->name.wide, flags);
#endif
                break;

            case CR_LDR_BUFF:
                if (cut_size(&size, loader->buff.size))
                    return (NULL);
                memio = FreeImage_OpenMemory((BYTE*)loader->buff.data, size);
                if (memio == NULL)
                    return (NULL);
                image = FreeImage_LoadFromMemory(format, memio, flags);
                break;

            default:
                return (NULL);
        }

        /* 图片加载失败 */
        if (image == NULL) {
            if (memio != NULL)
                FreeImage_CloseMemory(memio);
            return (NULL);
        }

        /* 加载图片数据 */
        okay = fimage_info(&temp, image);
        FreeImage_Unload(image);
        if (memio != NULL)
            FreeImage_CloseMemory(memio);
        if (!okay)
            return (NULL);

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
        rets->infor = fimage_type(format);
        return ((sFMTZ*)rets);
    }

    /* 加载多帧图片文件 */
    switch (loader->type)
    {
        case CR_LDR_ANSI:
            data = NULL;
            memio = NULL;
            multi = FreeImage_OpenMultiBitmap(format,
                        loader->name.ansi, FALSE, TRUE, FALSE, flags);
            break;

        case CR_LDR_WIDE:
            data = NULL;
            memio = NULL;
            path = utf16_to_local(CR_LOCAL, loader->name.wide);
            if (path == NULL)
                return (NULL);
            multi = FreeImage_OpenMultiBitmap(format,
                            path, FALSE, TRUE, FALSE, flags);
            mem_free(path);
            break;

        case CR_LDR_BUFF:
            if (cut_size(&size, loader->buff.size))
                return (NULL);
            data = mem_dup(loader->buff.data, loader->buff.size);
            if (data == NULL)
                return (NULL);
            memio = FreeImage_OpenMemory((BYTE*)data, size);
            if (memio == NULL)
                goto _failure1;
            multi = FreeImage_LoadMultiBitmapFromMemory(format, memio, flags);
            break;

        default:
            return (NULL);
    }

    /* 图片加载失败 */
    if (multi == NULL)
        goto _failure2;

    /* 单帧图片不使用流接口 */
    cnts = FreeImage_GetPageCount(multi);
    if (cnts == 1) {
        image = FreeImage_LockPage(multi, 0);
        if (image == NULL)
            goto _failure3;
        okay = fimage_info(&temp, image);
        FreeImage_UnlockPage(multi, image, FALSE);
        FreeImage_CloseMultiBitmap(multi, 0);
        if (memio != NULL)
            FreeImage_CloseMemory(memio);
        TRY_FREE(data);
        if (!okay)
            return (NULL);

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
        rets->infor = fimage_type(format);
        return ((sFMTZ*)rets);
    }

    /* 生成多帧图片接口对象 */
    port = struct_new(iPIC_FI);
    if (port == NULL)
        goto _failure3;
    port->m_fdata = data;
    port->m_memio = memio;
    port->m_multi = multi;
    port->m_infor = fimage_type(format);
    port->pics.__count__ = cnts;
    port->pics.__vptr__ = &s_pics_vtbl;

    /* 返回读取的文件数据 */
    retm = struct_new(sFMT_PRT);
    if (retm == NULL) {
        iPIC_FI_release((iPICTURE*)port);
        return (NULL);
    }
    retm->type = CR_FMTZ_PRT;
    retm->port = (iPORT*)port;
    retm->more = "iPICTURE";
    retm->infor = port->m_infor;
    return ((sFMTZ*)retm);

_failure3:
    FreeImage_CloseMultiBitmap(multi, 0);
_failure2:
    if (memio != NULL)
        FreeImage_CloseMemory(memio);
_failure1:
    TRY_FREE(data);
    return (NULL);
}

/*
=======================================
    BMP 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_bmp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_BMP, param, BMP_DEFAULT));
}

/*
=======================================
    CUT 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_cut (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_CUT, param, CUT_DEFAULT));
}

/*
=======================================
    DDS 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_dds (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_DDS, param, DDS_DEFAULT));
}

/*
=======================================
    EXR 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_exr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_EXR, param, EXR_DEFAULT));
}

/*
=======================================
    FAXG3 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_g3 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_FAXG3, param, FAXG3_DEFAULT));
}

/*
=======================================
    GIF 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_gif (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_GIF, param, GIF_PLAYBACK));
}

/*
=======================================
    HDR 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_hdr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_HDR, param, HDR_DEFAULT));
}

/*
=======================================
    ICO 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_ico (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_ICO, param, ICO_MAKEALPHA));
}

/*
=======================================
    IFF 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_iff (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_IFF, param, IFF_DEFAULT));
}

/*
=======================================
    J2K 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_j2k (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_J2K, param, J2K_DEFAULT));
}

/*
=======================================
    JNG 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_jng (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_JNG, param, JNG_DEFAULT));
}

/*
=======================================
    JP2 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_jp2 (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_JP2, param, JP2_DEFAULT));
}

/*
=======================================
    JPG 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_jpg (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_JPEG, param, JPEG_ACCURATE));
}

/*
=======================================
    JXR 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_jxr (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_JXR, param, JXR_DEFAULT));
}

/*
=======================================
    KOA 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_koa (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_KOALA, param, KOALA_DEFAULT));
}

/*
=======================================
    MNG 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_mng (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_MNG, param, MNG_DEFAULT));
}

/*
=======================================
    PBM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pbm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    sFMTZ*  pic;

    CR_NOUSE(datin);
    pic = fimage_load(FIF_PBM, param, PBM_DEFAULT);
    if (pic != NULL) return (pic);
    return (fimage_load(FIF_PBMRAW, param, PBM_DEFAULT));
}

/*
=======================================
    PCD 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pcd (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PCD, param, PCD_DEFAULT));
}

/*
=======================================
    PCX 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pcx (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PCX, param, PCX_DEFAULT));
}

/*
=======================================
    PFM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pfm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PFM, param, PFM_DEFAULT));
}

/*
=======================================
    PGM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pgm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    sFMTZ*  pic;

    CR_NOUSE(datin);
    pic = fimage_load(FIF_PGM, param, PGM_DEFAULT);
    if (pic != NULL) return (pic);
    return (fimage_load(FIF_PGMRAW, param, PGM_DEFAULT));
}

/*
=======================================
    PICT 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_pic (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PICT, param, PICT_DEFAULT));
}

/*
=======================================
    PNG 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_png (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PNG, param, PNG_DEFAULT));
}

/*
=======================================
    PPM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_ppm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    sFMTZ*  pic;

    CR_NOUSE(datin);
    pic = fimage_load(FIF_PPM, param, PPM_DEFAULT);
    if (pic != NULL) return (pic);
    return (fimage_load(FIF_PPMRAW, param, PPM_DEFAULT));
}

/*
=======================================
    PSD 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_psd (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_PSD, param, PSD_DEFAULT));
}

/*
=======================================
    RAS 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_ras (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_RAS, param, RAS_DEFAULT));
}

/*
=======================================
    RAW 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_raw (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_RAW, param, RAW_DEFAULT));
}

/*
=======================================
    SGI 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_sgi (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_SGI, param, SGI_DEFAULT));
}

/*
=======================================
    TGA 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_tga (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_TARGA, param, TARGA_DEFAULT));
}

/*
=======================================
    TIFF 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_tif (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_TIFF, param, TIFF_DEFAULT));
}

/*
=======================================
    WBMP 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_wbmp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_WBMP, param, WBMP_DEFAULT));
}

/*
=======================================
    WEBP 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_webp (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_WEBP, param, WEBP_DEFAULT));
}

/*
=======================================
    XBM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_xbm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_XBM, param, XBM_DEFAULT));
}

/*
=======================================
    XPM 文件读取
=======================================
*/
CR_API sFMTZ*
load_fi_xpm (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_XPM, param, XPM_DEFAULT));
}

/*
=======================================
    图片文件读取 (自动识别)
=======================================
*/
CR_API sFMTZ*
load_fi_auto (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    CR_NOUSE(datin);
    return (fimage_load(FIF_UNKNOWN, param, 0));
}

/* 引擎常数表 */
#undef  _CR_FMTZ_WIDE_
#include "e_fimage.inl"
#define _CR_FMTZ_WIDE_
#include "e_fimage.inl"
#undef  _CR_FMTZ_WIDE_

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_fimage_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    if (!(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
    return (fmtz_find(engine, loader));
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_fimage (void_t)
{
    sENGINE*    engine;

    engine = engine_init(s_finda, s_findw, s_loada, s_loadw);
    if (engine == NULL)
        return (NULL);
    engine->fmtz_load = engine_fimage_load;
    engine->info = "FreeImage FMTz Engine (Done by CrHackOS)";
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
    return (engine_fimage());
}
#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 文件保存                                  */
/*****************************************************************************/

/*
---------------------------------------
    FreeImage 图片保存
---------------------------------------
*/
static bool_t
fimage_save (
  __CR_IN__ FREE_IMAGE_FORMAT   format,
  __CR_IN__ const sIMAGE*       image,
  __CR_IN__ const ansi_t*       name,
  __CR_IN__ uint_t              argc,
  __CR_IN__ ansi_t*             argv[]
    )
{
    leng_t      bpl;
    byte_t*     ptr;
    RGBQUAD*    pal;
    FIBITMAP*   src;
    FIBITMAP*   tmp;
    /* ---------- */
    uint_t  xx, yy, ww, hh;
    uint_t  bpp, type, flags;
    uint_t  rmsk, gmsk, bmsk;

    /* 生成 FI 的位图 */
    yy = rmsk = gmsk = bmsk = 0;
    switch (image->fmt)
    {
        default:
            return (FALSE);

        case CR_INDEX1: type = 1; yy = 2; break;
        case CR_INDEX4: type = 4; yy = 16; break;
        case CR_INDEX8: type = 8; yy = 256; break;

        case CR_ARGBX555:
        case CR_ARGB1555:
            type = 16;
            rmsk = 0x7C00;
            gmsk = 0x03E0;
            bmsk = 0x001F;
            break;

        case CR_ARGB565:
            type = 16;
            rmsk = 0xF800;
            gmsk = 0x07E0;
            bmsk = 0x001F;
            break;

        case CR_ARGB888:
        case CR_ARGB8888:
            type = image->fmt;
            break;
    }
    ww = image->position.ww;
    hh = image->position.hh;
    src = FreeImage_Allocate(ww, hh, type, rmsk, gmsk, bmsk);
    if (src == NULL)
        return (FALSE);

    /* 填充调色板 */
    if (type <= 8) {
        pal = FreeImage_GetPalette(src);
        if (pal == NULL)
            goto _failure;
        ptr = (byte_t*)image->pal;
        for (xx = 0; xx < yy; xx++, ptr += 4) {
            pal[xx].rgbRed   = ptr[2];
            pal[xx].rgbGreen = ptr[1];
            pal[xx].rgbBlue  = ptr[0];
        }
    }

    /* 填充图片数据 */
    ptr = image->data;
    bpl = FreeImage_GetLine(src);
    if (image->gdi) {
        for (yy = 0; yy < hh; yy++, ptr += image->bpl)
            mem_cpy(FreeImage_GetScanLine(src, yy), ptr, bpl);
    }
    else {
        for (; hh != 0; hh--, ptr += image->bpl)
            mem_cpy(FreeImage_GetScanLine(src, hh - 1), ptr, bpl);
    }

    /* 解析参数 [FI 保存标志] [目标格式] */
    bpp = flags = 0;
    if (argc > 0) {
        flags = str2intxA(argv[0], NULL);
        if (argc > 1)
            bpp = str2intxA(argv[1], NULL);
    }

    /* 转换到指定的格式 */
    if (bpp != 0)
    {
        switch (bpp & 0xFF)
        {
            default:
                goto _failure;

            case 1:
                bpp >>= 8;
                tmp = FreeImage_Dither(src, (FREE_IMAGE_DITHER)bpp);
                break;

            case 4:
                tmp = FreeImage_ConvertTo4Bits(src);
                break;

            case 6:
                if (type != 24) {
                    tmp = FreeImage_ConvertTo24Bits(src);
                    if (tmp == NULL)
                        goto _failure;
                    FreeImage_Unload(src);
                    src = tmp;
                }
                tmp = FreeImage_ColorQuantize(src, FIQ_LFPQUANT);
                break;

            case 7:
                if (type != 24) {
                    tmp = FreeImage_ConvertTo24Bits(src);
                    if (tmp == NULL)
                        goto _failure;
                    FreeImage_Unload(src);
                    src = tmp;
                }
                tmp = FreeImage_ColorQuantize(src, FIQ_WUQUANT);
                break;

            case 8:
                tmp = FreeImage_ConvertTo8Bits(src);
                break;

            case 9:
                if (type != 24) {
                    tmp = FreeImage_ConvertTo24Bits(src);
                    if (tmp == NULL)
                        goto _failure;
                    FreeImage_Unload(src);
                    src = tmp;
                }
                tmp = FreeImage_ColorQuantize(src, FIQ_NNQUANT);
                break;

            case 15:
                tmp = FreeImage_ConvertTo16Bits555(src);
                break;

            case 16:
                tmp = FreeImage_ConvertTo16Bits565(src);
                break;

            case 17:
                tmp = FreeImage_ConvertToUINT16(src);
                break;

            case 24:
                tmp = FreeImage_ConvertTo24Bits(src);
                break;

            case 32:
                tmp = FreeImage_ConvertTo32Bits(src);
                break;

            case 33:
                tmp = FreeImage_ConvertToFloat(src);
                break;

            case 64:
                tmp = FreeImage_ConvertToRGB16(src);
                break;

            case 96:
                tmp = FreeImage_ConvertToRGBF(src);
                break;
        }
        if (tmp == NULL)
            goto _failure;
        FreeImage_Unload(src);
        src = tmp;
    }

    /* 保存成指定的格式 */
    if (!FreeImage_Save(format, src, name, flags)) {
        file_deleteA(name);
        goto _failure;
    }
    FreeImage_Unload(src);
    return (TRUE);

_failure:
    FreeImage_Unload(src);
    return (FALSE);
}

/*
=======================================
    BMP 文件保存
=======================================
*/
CR_API bool_t
save_img_bmp (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_BMP, img, name, argc, argv));
}

/*
=======================================
    EXR 文件保存
=======================================
*/
CR_API bool_t
save_img_exr (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_EXR, img, name, argc, argv));
}

/*
=======================================
    GIF 文件保存
=======================================
*/
CR_API bool_t
save_img_gif (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_GIF, img, name, argc, argv));
}

/*
=======================================
    HDR 文件保存
=======================================
*/
CR_API bool_t
save_img_hdr (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_HDR, img, name, argc, argv));
}

/*
=======================================
    ICO 文件保存
=======================================
*/
CR_API bool_t
save_img_ico (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_ICO, img, name, argc, argv));
}

/*
=======================================
    J2K 文件保存
=======================================
*/
CR_API bool_t
save_img_j2k (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_J2K, img, name, argc, argv));
}

/*
=======================================
    JNG 文件保存
=======================================
*/
CR_API bool_t
save_img_jng (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_JNG, img, name, argc, argv));
}

/*
=======================================
    JP2 文件保存
=======================================
*/
CR_API bool_t
save_img_jp2 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_JP2, img, name, argc, argv));
}

/*
=======================================
    JPG 文件保存
=======================================
*/
CR_API bool_t
save_img_jpg (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_JPEG, img, name, argc, argv));
}

/*
=======================================
    JXR 文件保存
=======================================
*/
CR_API bool_t
save_img_jxr (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_JXR, img, name, argc, argv));
}

/*
=======================================
    PBM 文件保存
=======================================
*/
CR_API bool_t
save_img_pbm (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_PBM, img, name, argc, argv));
}

/*
=======================================
    PFM 文件保存
=======================================
*/
CR_API bool_t
save_img_pfm (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_PFM, img, name, argc, argv));
}

/*
=======================================
    PGM 文件保存
=======================================
*/
CR_API bool_t
save_img_pgm (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_PGM, img, name, argc, argv));
}

/*
=======================================
    PNG 文件保存
=======================================
*/
CR_API bool_t
save_img_png (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_PNG, img, name, argc, argv));
}

/*
=======================================
    PPM 文件保存
=======================================
*/
CR_API bool_t
save_img_ppm (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_PPM, img, name, argc, argv));
}

/*
=======================================
    TIFF 文件保存
=======================================
*/
CR_API bool_t
save_img_tif (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_TIFF, img, name, argc, argv));
}

/*
=======================================
    TGA 文件保存
=======================================
*/
CR_API bool_t
save_img_tga (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_TARGA, img, name, argc, argv));
}

/*
=======================================
    WBMP 文件保存
=======================================
*/
CR_API bool_t
save_img_wbmp (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_WBMP, img, name, argc, argv));
}

/*
=======================================
    WEBP 文件保存
=======================================
*/
CR_API bool_t
save_img_webp (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_WEBP, img, name, argc, argv));
}

/*
=======================================
    XPM 文件保存
=======================================
*/
CR_API bool_t
save_img_xpm (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          argc,
  __CR_IN__ ansi_t*         argv[]
    )
{
    return (fimage_save(FIF_XPM, img, name, argc, argv));
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
