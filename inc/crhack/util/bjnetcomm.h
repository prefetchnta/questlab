/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-04-05  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> BlowJobs 通讯协议头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BJNETCOMM_H__
#define __CR_BJNETCOMM_H__

#include "../defs.h"

/* BJNetComm 对象类型 */
typedef void_t*     bjnetcomm_t;

/* BJNetComm 基础 API */
CR_API bjnetcomm_t  bjnetcomm_init (void_t);
CR_API void_t       bjnetcomm_kill (bjnetcomm_t bjnet);
CR_API void_t       bjnetcomm_reset (bjnetcomm_t bjnet);
CR_API void_t       bjnetcomm_size_max (bjnetcomm_t bjnet,
                                        int32u size_kb);
CR_API void_t       bjnetcomm_update_token (bjnetcomm_t bjnet,
                                            const ansi_t *token);
CR_API ansi_t*      bjnetcomm_enc_bytes (bjnetcomm_t bjnet, uint_t type,
                                         const void_t *data, uint_t size,
                                         const ansi_t *user CR_DEFAULT(NULL));
CR_API ansi_t*      bjnetcomm_enc_string (bjnetcomm_t bjnet, uint_t type,
                                          const ansi_t *string,
                                          const ansi_t *user CR_DEFAULT(NULL));
CR_API void_t*      bjnetcomm_dec_bytes2 (bjnetcomm_t bjnet,
                                          uint_t tag_type,
                                          const ansi_t *tag_data,
                                          const ansi_t *tag_sign,
                                          uint_t *size);
CR_API void_t*      bjnetcomm_dec_bytes (bjnetcomm_t bjnet,
                                         const ansi_t *json,
                                         uint_t *size);
CR_API ansi_t*      bjnetcomm_dec_string (bjnetcomm_t bjnet,
                                          const ansi_t *json);

#endif  /* !__CR_BJNETCOMM_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
