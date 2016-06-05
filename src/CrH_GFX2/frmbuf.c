/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-04  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> CrHack FrameBuffer 图形绘制接口实现 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "pixels.h"
#include "strlib.h"
#include "gfx2/frmbuf.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

/*****************************************************************************/
/*                                 绘制接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    释放图形绘制接口
---------------------------------------
*/
static void_t
iGFX2_FB_release (
  __CR_IN__ iGFX2*  that
    )
{
    iGFX2_FB*   real;

    real = (iGFX2_FB*)that;
    mem_free(real->__back__.data);
    munmap(real->m_main, real->m_size);
    close(real->m_file);
    mem_free(real);
}

/*
---------------------------------------
    获取扩展的绘制接口
---------------------------------------
*/
static void_t*
iGFX2_FB_getMore (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iGFX2::FB") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    改变屏幕大小
---------------------------------------
*/
static bool_t
iGFX2_FB_reset (
  __CR_IN__ iGFX2*  that
    )
{
    /* 不支持 */
    CR_NOUSE(that);
    return (FALSE);
}

/*
---------------------------------------
    锁住后台缓冲
---------------------------------------
*/
static sIMAGE*
iGFX2_FB_lock (
  __CR_IN__ iGFX2*  that
    )
{
    return (&that->__back__);
}

/*
---------------------------------------
    解锁后台缓冲
---------------------------------------
*/
static void_t
iGFX2_FB_unlock (
  __CR_IN__ iGFX2*  that
    )
{
    CR_NOUSE(that);
}

/*
---------------------------------------
    显示后台缓冲
---------------------------------------
*/
static bool_t
iGFX2_FB_flip (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ bool_t  sync
    )
{
    iGFX2_FB*   real = (iGFX2_FB*)that;

    CR_NOUSE(sync);
    mem_cpy(real->m_main, real->__back__.data, real->m_size);
    return (TRUE);
}

/*
---------------------------------------
    清除后台缓冲
---------------------------------------
*/
#define _image_clear12  iGFX2_FB_clear12
#define _image_clear15  iGFX2_FB_clear15
#define _image_clear16  iGFX2_FB_clear16
#define _image_clear17  iGFX2_FB_clear17
#define _image_clear24  iGFX2_FB_clear24
#define _image_clear32  iGFX2_FB_clear32

#include "clear2d.inl"

/*
---------------------------------------
    设置后台调色板
---------------------------------------
*/
static bool_t
iGFX2_FB_setPal (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ uint_t  start,
  __CR_IN__ uint_t  count
    )
{
    /* 不支持 */
    CR_NOUSE(that);
    CR_NOUSE(start);
    CR_NOUSE(count);
    return (FALSE);
}

/* 接口虚函数表 */
static const iGFX2_vtbl _rom_ s_img12_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear12, iGFX2_FB_setPal,
};

static const iGFX2_vtbl _rom_ s_img15_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear15, iGFX2_FB_setPal,
};

static const iGFX2_vtbl _rom_ s_img16_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear16, iGFX2_FB_setPal,
};

static const iGFX2_vtbl _rom_ s_img17_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear17, iGFX2_FB_setPal,
};

static const iGFX2_vtbl _rom_ s_img24_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear24, iGFX2_FB_setPal,
};

static const iGFX2_vtbl _rom_ s_img32_vtbl =
{
    iGFX2_FB_release, iGFX2_FB_getMore,
    iGFX2_FB_reset, iGFX2_FB_lock, iGFX2_FB_unlock,
    iGFX2_FB_flip, iGFX2_FB_clear32, iGFX2_FB_setPal,
};

/*
=======================================
    生成 FB 图形绘制接口
=======================================
*/
CR_API iGFX2_FB*
create_fb_canvas (
  __CR_IN__ const ansi_t*   fb
    )
{
    iGFX2_FB*   rett;

    /* 创建对象 */
    rett = struct_new(iGFX2_FB);
    if (rett == NULL)
        return (NULL);
    struct_zero(&rett->__back__, sIMAGE);

    /* 初始化 FB */
    if (fb == NULL)
        fb = "/dev/fb0";
    rett->m_file = open(fb, O_RDWR);
    if (rett->m_file < 0)
        return (NULL);
    if (ioctl(rett->m_file, FBIOGET_FSCREENINFO, &rett->m_finfo) < 0)
        goto _failure;
    if (ioctl(rett->m_file, FBIOGET_VSCREENINFO, &rett->m_vinfo) < 0)
        goto _failure;
    if (rett->m_vinfo.bits_per_pixel <= 8 ||
        rett->m_vinfo.red.msb_right || rett->m_vinfo.green.msb_right ||
        rett->m_vinfo.blue.msb_right)
        goto _failure;

    /* 填充参数结构 */
    rett->__back__.bpl = rett->m_finfo.line_length;
    if (rett->__back__.bpl % 16 == 0)
        rett->__back__.align = 16;
    else
    if (rett->__back__.bpl % 8 == 0)
        rett->__back__.align = 8;
    else
    if (rett->__back__.bpl % 4 == 0)
        rett->__back__.align = 4;
    rect_set_wh(&rett->__back__.clip_win, 0, 0, rett->m_vinfo.xres,
                 rett->m_vinfo.yres);
    struct_cpy(&rett->__back__.position, &rett->__back__.clip_win, sRECT);
    rett->__back__.size  = rett->__back__.bpl;
    rett->__back__.size *= rett->m_vinfo.yres;

    /* 识别颜色格式 */
    if (rett->m_vinfo.bits_per_pixel == 16) {
        if (rett->m_vinfo.red.length == 5 &&
            rett->m_vinfo.green.length == 6 &&
            rett->m_vinfo.blue.length == 5 &&
            rett->m_vinfo.red.offset == 11 &&
            rett->m_vinfo.green.offset == 5 &&
            rett->m_vinfo.blue.offset == 0) {
            rett->__back__.fmt = CR_ARGB565;
            rett->__vptr__ = &s_img16_vtbl;
        }
        else
        if (rett->m_vinfo.red.length == 5 &&
            rett->m_vinfo.green.length == 5 &&
            rett->m_vinfo.blue.length == 5 &&
            rett->m_vinfo.red.offset == 10 &&
            rett->m_vinfo.green.offset == 5 &&
            rett->m_vinfo.blue.offset == 0) {
            if (rett->m_vinfo.transp.length == 0) {
                rett->__back__.fmt = CR_ARGBX555;
                rett->__vptr__ = &s_img15_vtbl;
            }
            else {
                rett->__back__.fmt = CR_ARGB1555;
                rett->__vptr__ = &s_img17_vtbl;
            }
        }
        else
        if (rett->m_vinfo.red.length == 4 &&
            rett->m_vinfo.green.length == 4 &&
            rett->m_vinfo.blue.length == 4 &&
            rett->m_vinfo.red.offset == 8 &&
            rett->m_vinfo.green.offset == 4 &&
            rett->m_vinfo.blue.offset == 0) {
            rett->__back__.fmt = CR_ARGB4444;
            rett->__vptr__ = &s_img12_vtbl;
        }
        else {
            goto _failure;
        }
        rett->__back__.bpc = 2;
    }
    else
    if (rett->m_vinfo.bits_per_pixel == 32) {
        rett->__back__.bpc = 4;
        rett->__back__.fmt = CR_ARGB8888;
        rett->__vptr__ = &s_img32_vtbl;
    }
    else
    if (rett->m_vinfo.bits_per_pixel == 24) {
        rett->__back__.bpc = 3;
        rett->__back__.fmt = CR_ARGB888;
        rett->__vptr__ = &s_img24_vtbl;
    }
    else {
        goto _failure;
    }
    rett->__back__.data = (byte_t*)mem_malloc(rett->__back__.size + 16);
    if (rett->__back__.data == NULL)
        goto _failure;
    mem_zero(rett->__back__.data, rett->__back__.size);

    /* 映射前台缓冲 */
    rett->m_size = (uint_t)rett->__back__.size;
    rett->m_main = mmap(NULL, rett->m_size, PROT_READ | PROT_WRITE,
                        MAP_SHARED, rett->m_file, 0);
    if (rett->m_main == MAP_FAILED) {
        mem_free(rett->__back__.data);
        goto _failure;
    }
    mem_zero(rett->m_main, rett->m_size);
    return (rett);

_failure:
    close(rett->m_file);
    return (NULL);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取绘图插件接口 (同名)
=======================================
*/
CR_API iGFX2*
create_canvas (
  __CR_IN__ void_t*         handle,
  __CR_IN__ uint_t          scn_cw,
  __CR_IN__ uint_t          scn_ch,
  __CR_IN__ uint_t          scn_fmt,
  __CR_IN__ bool_t          full,
  __CR_IN__ const int32u*   param,
  __CR_IN__ uint_t          count
    )
{
    CR_NOUSE(full); CR_NOUSE(param);
    CR_NOUSE(count); CR_NOUSE(scn_cw);
    CR_NOUSE(scn_ch); CR_NOUSE(scn_fmt);
    return ((iGFX2*)create_fb_canvas((ansi_t*)handle));
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
