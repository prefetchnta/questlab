/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2011-06-21  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 第三方库函数重定向头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_L3RD_H__
#define __CR_L3RD_H__ 0x105D37EDUL

#ifndef _CR_NO_PORT_

#include "applib.h"
#include "memlib.h"
#include "strlib.h"

/* LIBC 重定向 */
#undef  abort
#undef  free
#undef  malloc
#undef  calloc
/* ---- */
#define abort       quit_now
#define free        mem_free_port
#define malloc      mem_malloc
#define calloc      mem_calloc
/* ---- */

#undef  memset
#undef  memcpy
#undef  memcmp
#undef  memmove
/* ---- */
#define memset      mem_set
#define memcpy      mem_cpy
#define memcmp      mem_cmp
#define memmove     mem_mov
/* ---- */

#undef  isalnum
#undef  isalpha
#undef  iscntrl
#undef  isdigit
#undef  isgraph
#undef  islower
#undef  isprint
#undef  ispunct
#undef  isspace
#undef  isupper
#undef  isxdigit
/* ---- */
#define isalnum     is_alnumA
#define isalpha     is_alphaA
#define iscntrl     is_cntrlA
#define isdigit     is_digitA
#define isgraph     is_graphA
#define islower     is_lowerA
#define isprint     is_printA
#define ispunct     is_punctA
#define isspace     is_spaceA
#define isupper     is_upperA
#define isxdigit    is_xnumbA
/* ---- */

#undef  iswalnum
#undef  iswalpha
#undef  iswcntrl
#undef  iswdigit
#undef  iswgraph
#undef  iswlower
#undef  iswprint
#undef  iswpunct
#undef  iswspace
#undef  iswupper
#undef  iswxdigit
/* ---- */
#define iswalnum    is_alnumW
#define iswalpha    is_alphaW
#define iswcntrl    is_cntrlW
#define iswdigit    is_digitW
#define iswgraph    is_graphW
#define iswlower    is_lowerW
#define iswprint    is_printW
#define iswpunct    is_punctW
#define iswspace    is_spaceW
#define iswupper    is_upperW
#define iswxdigit   is_xnumbW
/* ---- */

#undef  strlen
#undef  wcslen
#undef  strcmp
#undef  wcscmp
#undef  strcpy
#undef  wcscpy
#undef  strcat
#undef  wcscat
#undef  strstr
#undef  wcsstr
#undef  strchr
#undef  wcschr
#undef  strupr
#undef  wcsupr
#undef  strlwr
#undef  wcslwr
#undef  strnlen
#undef  wcsnlen
#undef  strncpy
#undef  wcsncpy
#undef  strncmp
#undef  wcsncmp
/* ---- */
#define strlen      str_lenA
#define wcslen      str_lenW
#define strcmp      str_cmpA
#define wcscmp      str_cmpW
#define strcpy      str_cpyA
#define wcscpy      str_cpyW
#define strcat      str_catA
#define wcscat      str_catW
#define strstr      str_strA
#define wcsstr      str_strW
#define strchr      str_chrA
#define wcschr      str_chrW
#define strupr      str_uprA
#define wcsupr      str_uprW
#define strlwr      str_lwrA
#define wcslwr      str_lwrW
#define strnlen     str_lenNA
#define wcsnlen     str_lenNW
#define strncpy     str_cpyNA
#define wcsncpy     str_cpyNW
#define strncmp     str_cmpNA
#define wcsncmp     str_cmpNW
/* ---- */

#endif  /* !_CR_NO_PORT_ */

#endif  /* !__CR_L3RD_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
