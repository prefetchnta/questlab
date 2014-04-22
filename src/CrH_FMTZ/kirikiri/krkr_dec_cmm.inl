/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-09-22  */
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
/*  >>>>>>>>>>>>>>>> CrHack KiriKiri2 文件数据解密库 (常用) <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
---------------------------------------
    "auto_xor8" 自识别8位异或
---------------------------------------
*/
static void_t
dec_auto_xor8 (
  __CR_IO__ void_t*                 data,
  __CR_IN__ leng_t                  size,
  __CR_IN__ const sPAK_XP3_FILE*    info
    )
{
    byte_t          key, *ptr;
    const ansi_t*   file = info->base.name;
    const ansi_t*   fext = file + str_lenA(file);

    /* 定位到扩展名 */
    while (fext != file) {
        if (*fext == CR_AC('.'))
            break;
        fext--;
    }
    ptr = (byte_t*)data;

    /* 根据文件类型反算出密钥 */
    if (str_cmpIA(fext, ".png") == 0) {
        key = ptr[0] ^ 0x89;
    }
    else
    if (str_cmpIA(fext, ".jpg") == 0) {
        key = ptr[0] ^ 0xFF;
    }
    else
    if (str_cmpIA(fext, ".bmp") == 0) {
        key = ptr[0] ^ 'B';
    }
    else
    if (str_cmpIA(fext, ".wav") == 0) {
        key = ptr[0] ^ 'R';
    }
    else
    if (str_cmpIA(fext, ".mp3") == 0) {
        key = ptr[0] ^ 'I';
        if ((ptr[1] ^ key) != 'D')
            key = ptr[0] ^ 0xFF;
    }
    else
    if (str_cmpIA(fext, ".ogg") == 0) {
        key = ptr[0] ^ 'O';
    }
    else
    if (str_cmpIA(fext, ".mpg") == 0) {
        key = ptr[0];
    }
    else
    if (str_cmpIA(fext, ".ks") == 0 ||
        str_cmpIA(fext, ".tjs") == 0) {
        key = ptr[0] ^ 0xFF;
        if ((ptr[1] ^ key) != 0xFE)
            return;
    }
    else {
        return;
    }

    /* 解密数据 */
    for (; size != 0; size--)
        *ptr++ ^= key;
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
