/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-04-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 2D 表面清除函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#if defined(_image_clear01)
/*
---------------------------------------
    清除缓冲 (IDX1)
---------------------------------------
*/
static bool_t
_image_clear01 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    byte_t  val = 0;
    int32u* pal = that->__back__.pal;

    if (color == pal[1])
        val = 0xFF;
    mem_set(that->__back__.data, val,
            that->__back__.size);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear01 */

#if defined(_image_clear02)
/*
---------------------------------------
    清除缓冲 (IDX2)
---------------------------------------
*/
static bool_t
_image_clear02 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    uint_t  idx = 0;
    byte_t  val = 0;
    int32u* pal = that->__back__.pal;

    for (; idx < 4; idx++) {
        if (color == pal[idx]) {
            idx = (idx << 2) | idx;
            idx = (idx << 4) | idx;
            val = (byte_t)idx;
            break;
        }
    }
    mem_set(that->__back__.data, val,
            that->__back__.size);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear02 */

#if defined(_image_clear04)
/*
---------------------------------------
    清除缓冲 (IDX4)
---------------------------------------
*/
static bool_t
_image_clear04 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    uint_t  idx = 0;
    byte_t  val = 0;
    int32u* pal = that->__back__.pal;

    for (; idx < 16; idx++) {
        if (color == pal[idx]) {
            idx = (idx << 4) | idx;
            val = (byte_t)idx;
            break;
        }
    }
    mem_set(that->__back__.data, val,
            that->__back__.size);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear04 */

#if defined(_image_clear08)
/*
---------------------------------------
    清除缓冲 (IDX8)
---------------------------------------
*/
static bool_t
_image_clear08 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    uint_t  idx = 0;
    byte_t  val = 0;
    int32u* pal = that->__back__.pal;

    for (; idx < 256; idx++) {
        if (color == pal[idx]) {
            val = (byte_t)idx;
            break;
        }
    }
    mem_set(that->__back__.data, val,
            that->__back__.size);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear08 */

#if defined(_image_clear12)
/*
---------------------------------------
    清除缓冲 (4444)
---------------------------------------
*/
static bool_t
_image_clear12 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    int16u  val;

    pixel32to4444(&val, color);
    mem_setw(that->__back__.data, val,
             that->__back__.size / 2);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear12 */

#if defined(_image_clear15)
/*
---------------------------------------
    清除缓冲 (X555)
---------------------------------------
*/
static bool_t
_image_clear15 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    int16u  val;

    pixel32toX555(&val, color);
    mem_setw(that->__back__.data, val,
             that->__back__.size / 2);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear15 */

#if defined(_image_clear16)
/*
---------------------------------------
    清除缓冲 (565)
---------------------------------------
*/
static bool_t
_image_clear16 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    int16u  val;

    pixel32to565(&val, color);
    mem_setw(that->__back__.data, val,
             that->__back__.size / 2);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear16 */

#if defined(_image_clear17)
/*
---------------------------------------
    清除缓冲 (1555)
---------------------------------------
*/
static bool_t
_image_clear17 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    int16u  val;

    pixel32to1555(&val, color);
    mem_setw(that->__back__.data, val,
             that->__back__.size / 2);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear17 */

#if defined(_image_clear24)
/*
---------------------------------------
    清除缓冲 (888)
---------------------------------------
*/
static bool_t
_image_clear24 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    uchar*  data = that->__back__.data;
    leng_t  nbpl = that->__back__.bpl;
    uint_t  wtmp = that->__back__.position.ww;
    uint_t  htmp = that->__back__.position.hh;

    for (; htmp != 0; htmp--) {
        mem_sett(data, (byte_t*)(&color), wtmp);
        data += nbpl;
    }
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear24 */

#if defined(_image_clear32)
/*
---------------------------------------
    清除缓冲 (8888)
---------------------------------------
*/
static bool_t
_image_clear32 (
  __CR_IN__ iGFX2*  that,
  __CR_IN__ cl32_t  color,
  __CR_IN__ int32u  param
    )
{
    mem_setd(that->__back__.data, color,
             that->__back__.size / 4);
    CR_NOUSE(param);
    return (TRUE);
}
#endif  /* _image_clear32 */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
