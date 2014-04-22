/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-31  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack D3DFMT 定义头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_D3DFMT_H__
#define __CR_D3DFMT_H__

#include "defs.h"

#define D3DFMT_UNKNOWN          0
#define D3DFMT_R8G8B8           20
#define D3DFMT_A8R8G8B8         21
#define D3DFMT_X8R8G8B8         22
#define D3DFMT_R5G6B5           23
#define D3DFMT_X1R5G5B5         24
#define D3DFMT_A1R5G5B5         25
#define D3DFMT_A4R4G4B4         26
#define D3DFMT_R3G3B2           27
#define D3DFMT_A8               28
#define D3DFMT_A8R3G3B2         29
#define D3DFMT_X4R4G4B4         30
#define D3DFMT_A2B10G10R10      31
#define D3DFMT_A8B8G8R8         32
#define D3DFMT_X8B8G8R8         33
#define D3DFMT_G16R16           34
#define D3DFMT_A2R10G10B10      35
#define D3DFMT_A16B16G16R16     36
#define D3DFMT_A8P8             40
#define D3DFMT_P8               41
#define D3DFMT_L8               50
#define D3DFMT_L16              81
#define D3DFMT_A8L8             51
#define D3DFMT_A4L4             52

#if defined(_CR_ORDER_LE_)
    #define D3DFMT_DXT1     0x31545844UL
    #define D3DFMT_DXT2     0x32545844UL
    #define D3DFMT_DXT3     0x33545844UL
    #define D3DFMT_DXT4     0x34545844UL
    #define D3DFMT_DXT5     0x35545844UL
#else   /* (_CR_ORDER_BE_) */
    #define D3DFMT_DXT1     0x44585431UL    /* 'DXT1' */
    #define D3DFMT_DXT2     0x44585432UL    /* 'DXT2' */
    #define D3DFMT_DXT3     0x44585433UL    /* 'DXT3' */
    #define D3DFMT_DXT4     0x44585434UL    /* 'DXT4' */
    #define D3DFMT_DXT5     0x44585435UL    /* 'DXT5' */
#endif

#endif  /* !__CR_D3DFMT_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
