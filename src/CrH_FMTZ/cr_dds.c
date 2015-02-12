/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack DDS 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtz.h"
#include "safe.h"
#include "d3dfmt.h"
#include "pixels.h"

/* DDS 成员标志 */
#define DDSD_CAPS           0x1UL       /* 必需 */
#define DDSD_HEIGHT         0x2UL       /* 必需 */
#define DDSD_WIDTH          0x4UL       /* 必需 */
#define DDSD_PITCH          0x8UL
#define DDSD_PIXELFORMAT    0x1000UL    /* 必需 */
#define DDSD_MIPMAPCOUNT    0x20000UL
#define DDSD_LINEARSIZE     0x80000UL
#define DDSD_DEPTH          0x800000UL

/* DDS 像素类型 */
#define DDPF_ALPHAPIXELS    0x1UL       /* 纹理包含 Alpha 通道数据 */
#define DDPF_ALPHA          0x2UL       /* 只有 Alpha 通道的非压缩数据 */
#define DDPF_FOURCC         0x4UL       /* 包含压缩的 RGB 数据 */
#define DDPF_RGB            0x40UL      /* 包含非压缩的 RGB 数据 */
#define DDPF_YUV            0x200UL     /* 包含非压缩的 YUV 数据 */
#define DDPF_LUMINANCE      0x20000UL   /* 包含单通道亮度数据 */

/* DDS 内部结构 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* DDS 像素格式结构 */
CR_TYPEDEF struct
{
        int32u  dwSize;         /* 结构大小=32 */
        int32u  dwFlags;        /* DDS 像素类型标志 */
        int32u  dwFourCC;       /* 压缩格式类型 */
        int32u  dwRGBBitCount;  /* 像素的总位数 */
        int32u  dwRBitMask;     /* 红色通道的掩码 */
        int32u  dwGBitMask;     /* 绿色通道的掩码 */
        int32u  dwBBitMask;     /* 蓝色通道的掩码 */
        int32u  dwABitMask;     /* 透明通道的掩码 */

} CR_PACKED sDDS_FMT;

/* DDS 文件头结构 */
CR_TYPEDEF struct
{
        int32u      dwMagic;            /* "DDS " 标志 */
        int32u      dwSize;             /* 结构大小=124 */
        int32u      dwFlags;            /* 结构成员标志 */
        int32u      dwHeight;           /* 图片高度 */
        int32u      dwWidth;            /* 图片宽度 */
        int32u      dwLinearSize;       /* 每行字节数 */
        int32u      dwDepth;            /* 卷纹理的深度 */
        int32u      dwMipMapCount;      /* Mipmap 层数 */
        int32u      dwReserved1[11];    /* 保留不用 */
        sDDS_FMT    ddpf;               /* 像素格式 */
        int32u      dwCaps;             /* 复杂性标志 */
        int32u      dwCaps2;            /* 附加细节信息 */
        int32u      dwCaps3;            /* 保留不用 */
        int32u      dwCaps4;            /* 保留不用 */
        int32u      dwReserved2;        /* 保留不用 */

} CR_PACKED sDDS_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    DDS 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_dds (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t  size;
    uint_t  ww, hh;
    int32u  d3dfmt;
    byte_t* imgdat;
    /* --------- */
    sDDS_HDR    head;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (!(CR_VCALL(datin)->geType(datin, &head, sDDS_HDR)))
        return (NULL);
    if (head.dwMagic != mk_tag4("DDS ") ||
        head.dwSize != CDWORD_LE(124) ||
        head.ddpf.dwSize != CDWORD_LE(32))
        return (NULL);
    head.dwFlags = DWORD_LE(head.dwFlags);
    if (head.dwFlags & DDSD_DEPTH)
        return (NULL);
    head.ddpf.dwFlags = DWORD_LE(head.ddpf.dwFlags);
    if (head.ddpf.dwFlags & DDPF_YUV)
        return (NULL);

    /* 对宽高的截断检查 */
    if (cut_int32_u(&ww, DWORD_LE(head.dwWidth)))
        return (NULL);
    if (cut_int32_u(&hh, DWORD_LE(head.dwHeight)))
        return (NULL);

    /* 判断图片像素格式 */
    mem_zero(temp.wh, sizeof(temp.wh));
    head.ddpf.dwRBitMask = DWORD_LE(head.ddpf.dwRBitMask);
    head.ddpf.dwGBitMask = DWORD_LE(head.ddpf.dwGBitMask);
    head.ddpf.dwBBitMask = DWORD_LE(head.ddpf.dwBBitMask);
    head.ddpf.dwABitMask = DWORD_LE(head.ddpf.dwABitMask);
    head.ddpf.dwRGBBitCount = DWORD_LE(head.ddpf.dwRGBBitCount);
    if (head.ddpf.dwFlags & DDPF_FOURCC)
    {
        /* 压缩或特殊位图 */
        if (head.ddpf.dwFourCC == CDWORD_LE(36)) {
            temp.fmt = CR_PIC_ARGB;
            temp.bpp = 64;
            temp.clr = "ABGR";
            temp.wh[0] = 16;
            temp.wh[1] = 16;
            temp.wh[2] = 16;
            temp.wh[3] = 16;
            d3dfmt = D3DFMT_A16B16G16R16;
        }
        else {
            temp.fmt = CR_PIC_SPEC;
            temp.bpp = 24;
            switch (head.ddpf.dwFourCC)
            {
                case D3DFMT_DXT1: temp.clr = "DXT1"; break;
                case D3DFMT_DXT2: temp.clr = "DXT2"; break;
                case D3DFMT_DXT3: temp.clr = "DXT3"; break;
                case D3DFMT_DXT4: temp.clr = "DXT4"; break;
                case D3DFMT_DXT5: temp.clr = "DXT5"; break;

                default:
                    return (NULL);
            }
            d3dfmt = head.ddpf.dwFourCC;
        }
    }
    else
    if (head.ddpf.dwFlags & DDPF_RGB)
    {
        /* 非压缩的彩色位图 */
        temp.fmt = CR_PIC_ARGB;
        if (head.ddpf.dwRGBBitCount == 32) {
            temp.bpp = 32;
            if (head.ddpf.dwRBitMask == 0x00FF0000UL &&
                head.ddpf.dwGBitMask == 0x0000FF00UL &&
                head.ddpf.dwBBitMask == 0x000000FFUL) {
                if (head.ddpf.dwABitMask == 0xFF000000UL) {
                    temp.clr = "ARGB";
                    d3dfmt = D3DFMT_A8R8G8B8;
                }
                else {
                    temp.clr = "XRGB";
                    d3dfmt = D3DFMT_X8R8G8B8;
                }
                temp.wh[0] = 8;
                temp.wh[1] = 8;
                temp.wh[2] = 8;
                temp.wh[3] = 8;
            }
            else
            if (head.ddpf.dwRBitMask == 0x000000FFUL &&
                head.ddpf.dwGBitMask == 0x0000FF00UL &&
                head.ddpf.dwBBitMask == 0x00FF0000UL) {
                if (head.ddpf.dwABitMask == 0xFF000000UL) {
                    temp.clr = "ABGR";
                    d3dfmt = D3DFMT_A8B8G8R8;
                }
                else {
                    temp.clr = "XBGR";
                    d3dfmt = D3DFMT_X8B8G8R8;
                }
                temp.wh[0] = 8;
                temp.wh[1] = 8;
                temp.wh[2] = 8;
                temp.wh[3] = 8;
            }
            else
            if (head.ddpf.dwRBitMask == 0x0000FFFFUL &&
                head.ddpf.dwGBitMask == 0xFFFF0000UL) {
                temp.clr = "GR";
                temp.wh[0] = 16;
                temp.wh[1] = 16;
                d3dfmt = D3DFMT_G16R16;
            }
            else
            if (head.ddpf.dwRBitMask == 0x3FF00000UL &&
                head.ddpf.dwGBitMask == 0x000FFC00UL &&
                head.ddpf.dwBBitMask == 0x000003FFUL &&
                head.ddpf.dwABitMask == 0xC0000000UL) {
                temp.clr = "ARGB";
                temp.wh[0] = 2;
                temp.wh[1] = 10;
                temp.wh[2] = 10;
                temp.wh[3] = 10;
                d3dfmt = D3DFMT_A2R10G10B10;
            }
            else
            if (head.ddpf.dwRBitMask == 0x000003FFUL &&
                head.ddpf.dwGBitMask == 0x000FFC00UL &&
                head.ddpf.dwBBitMask == 0x3FF00000UL &&
                head.ddpf.dwABitMask == 0xC0000000UL) {
                temp.clr = "ABGR";
                temp.wh[0] = 2;
                temp.wh[1] = 10;
                temp.wh[2] = 10;
                temp.wh[3] = 10;
                d3dfmt = D3DFMT_A2B10G10R10;
            }
            else {
                return (NULL);
            }
        }
        else
        if (head.ddpf.dwRGBBitCount == 24) {
            temp.bpp = 24;
            if (head.ddpf.dwRBitMask == 0xFF0000UL &&
                head.ddpf.dwGBitMask == 0x00FF00UL &&
                head.ddpf.dwBBitMask == 0x0000FFUL) {
                temp.clr = "RGB";
                temp.wh[0] = 8;
                temp.wh[1] = 8;
                temp.wh[2] = 8;
                d3dfmt = D3DFMT_R8G8B8;
            }
            else {
                return (NULL);
            }
        }
        else
        if (head.ddpf.dwRGBBitCount == 16) {
            temp.bpp = 16;
            if (head.ddpf.dwRBitMask == 0xF800UL &&
                head.ddpf.dwGBitMask == 0x07E0UL &&
                head.ddpf.dwBBitMask == 0x001FUL) {
                temp.clr = "RGB";
                temp.wh[0] = 5;
                temp.wh[1] = 6;
                temp.wh[2] = 5;
                d3dfmt = D3DFMT_R5G6B5;
            }
            else
            if (head.ddpf.dwRBitMask == 0x7C00UL &&
                head.ddpf.dwGBitMask == 0x03E0UL &&
                head.ddpf.dwBBitMask == 0x001FUL) {
                if (head.ddpf.dwABitMask == 0x8000UL) {
                    temp.clr = "ARGB";
                    d3dfmt = D3DFMT_A1R5G5B5;
                }
                else {
                    temp.clr = "XRGB";
                    d3dfmt = D3DFMT_X1R5G5B5;
                }
                temp.wh[0] = 1;
                temp.wh[1] = 5;
                temp.wh[2] = 5;
                temp.wh[3] = 5;
            }
            else
            if (head.ddpf.dwRBitMask == 0x0F00UL &&
                head.ddpf.dwGBitMask == 0x00F0UL &&
                head.ddpf.dwBBitMask == 0x000FUL) {
                if (head.ddpf.dwABitMask == 0xF000UL) {
                    temp.clr = "ARGB";
                    d3dfmt = D3DFMT_A4R4G4B4;
                }
                else {
                    temp.clr = "XRGB";
                    d3dfmt = D3DFMT_X4R4G4B4;
                }
                temp.wh[0] = 4;
                temp.wh[1] = 4;
                temp.wh[2] = 4;
                temp.wh[3] = 4;
            }
            else
            if (head.ddpf.dwRBitMask == 0x00E0UL &&
                head.ddpf.dwGBitMask == 0x001CUL &&
                head.ddpf.dwBBitMask == 0x0003UL &&
                head.ddpf.dwABitMask == 0xFF00UL) {
                temp.clr = "ARGB";
                temp.wh[0] = 8;
                temp.wh[1] = 3;
                temp.wh[2] = 3;
                temp.wh[3] = 2;
                d3dfmt = D3DFMT_A8R3G3B2;
            }
            else {
                return (NULL);
            }
        }
        else
        if (head.ddpf.dwRGBBitCount == 8) {
            temp.bpp = 8;
            if (head.ddpf.dwRBitMask == 0xE0UL &&
                head.ddpf.dwGBitMask == 0x1CUL &&
                head.ddpf.dwBBitMask == 0x03UL) {
                temp.clr = "RGB";
                temp.wh[1] = 3;
                temp.wh[2] = 3;
                temp.wh[3] = 2;
                d3dfmt = D3DFMT_R3G3B2;
            }
            else {
                return (NULL);
            }
        }
        else {
            return (NULL);
        }
    }
    else
    if (head.ddpf.dwFlags & DDPF_LUMINANCE)
    {
        /* 灰度位图 */
        temp.fmt = CR_PIC_GREY;
        if (head.ddpf.dwRGBBitCount == 8) {
            temp.bpp = 8;
            if (head.ddpf.dwRBitMask == 0xFFUL) {
                temp.clr = "I";
                temp.wh[0] = 8;
                d3dfmt = D3DFMT_L8;
            }
            else
            if (head.ddpf.dwRBitMask == 0x0FUL &&
                head.ddpf.dwABitMask == 0xF0UL) {
                temp.clr = "AI";
                temp.wh[0] = 4;
                temp.wh[1] = 4;
                d3dfmt = D3DFMT_A4L4;
            }
            else {
                return (NULL);
            }
        }
        else
        if (head.ddpf.dwRGBBitCount == 16) {
            temp.bpp = 16;
            if (head.ddpf.dwRBitMask == 0xFFFFUL) {
                temp.clr = "I";
                temp.wh[0] = 16;
                d3dfmt = D3DFMT_L16;
            }
            else
            if (head.ddpf.dwRBitMask == 0x00FFUL &&
                head.ddpf.dwABitMask == 0xFF00UL) {
                temp.clr = "AI";
                temp.wh[0] = 8;
                temp.wh[1] = 8;
                d3dfmt = D3DFMT_A8L8;
            }
            else {
                return (NULL);
            }
        }
        else {
            return (NULL);
        }
    }
    else
    if (head.ddpf.dwFlags & DDPF_ALPHA)
    {
        /* 透明通道位图 */
        if (head.ddpf.dwRGBBitCount != 8 ||
            head.ddpf.dwABitMask != 0xFFUL)
            return (NULL);
        temp.fmt = CR_PIC_GREY;
        temp.bpp = 8;
        temp.clr = "A";
        temp.wh[0] = 8;
        d3dfmt = D3DFMT_A8;
    }
    else
    {
        /* 未知格式位图 */
        return (NULL);
    }

    /* 读取图片数据 */
    imgdat = CR_VCALL(datin)->get(datin, &size, FALSE);
    if (imgdat == NULL)
        return (NULL);

    /* 转到32位色位图 */
    temp.pic = img_d3dx_to_32(NULL, 0, 0, imgdat, size, ww, hh,
                              NULL, d3dfmt, sizeof(int32u));
    mem_free(imgdat);
    if (temp.pic == NULL)
        return (NULL);

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "DirectDraw Surface (DDS)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
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
