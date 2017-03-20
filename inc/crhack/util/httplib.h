/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>>> HTTPLIB 头文件 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_HTTPLIB_H__
#define __CR_HTTPLIB_H__

#include "../defs.h"

/* HTTP 命令 */
#define HTTPLIB_GET     0
#define HTTPLIB_HEAD    1
#define HTTPLIB_PUT     2
#define HTTPLIB_POST    3
#define HTTPLIB_TRACE   4
#define HTTPLIB_OPTIONS 5
#define HTTPLIB_DELETE  6

/* HTTPLIB 连接对象 */
typedef void_t*     chttp_t;

/* HTTPLIB 接收回调 */
typedef void_t  (*chttp_recv_t) (void_t*, const void_t*, uint_t, uint_t);

/* HTTPLIB API 函数 */
CR_API chttp_t  chttp_open (const ansi_t *host, int16u port,
                            uint_t vers, int32s timeout);
CR_API void_t   chttp_close (chttp_t conn);
CR_API void_t   chttp_timeout (chttp_t conn, int32s wr_time,
                               int32s rd_time);
CR_API void_t   chttp_disconn (chttp_t conn);
CR_API void_t   chttp_restart (chttp_t conn);
CR_API void_t   chttp_safe_size (chttp_t conn, sint_t size);
CR_API bool_t   chttp_callback (chttp_t conn, chttp_recv_t func,
                                void_t *param, uint_t cache);
CR_API bool_t   chttp_head_add (chttp_t conn, const ansi_t *lines);
CR_API bool_t   chttp_head_fin (chttp_t conn);
CR_API void_t   chttp_head_rst (chttp_t conn);
CR_API bool_t   chttp_req_str (chttp_t conn, uint_t method,
                               const ansi_t *path, const ansi_t *body);
CR_API bool_t   chttp_req_bin (chttp_t conn, uint_t method,
                               const ansi_t *path, const void_t *data,
                               uint_t size);
CR_API ansi_t*  chttp_res_hdr (chttp_t conn);
CR_API ansi_t*  chttp_res_str (chttp_t conn);
CR_API void_t*  chttp_res_bin (chttp_t conn, uint_t *size);
CR_API uint_t   chttp_response (chttp_t conn, uint_t *vers, ansi_t **text);
CR_API uint_t   chttp_hdr_num (chttp_t conn);
CR_API ansi_t** chttp_hdr_lst (chttp_t conn);
CR_API bool_t   chttp_hdr_check (const ansi_t *line, const ansi_t *key,
                                 const ansi_t *value);
CR_API ansi_t*  chttp_hdr_value (const ansi_t *line, const ansi_t *key);
CR_API ansi_t*  chttp_hdr_find (chttp_t conn, const ansi_t *key);

#endif  /* !__CR_HTTPLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
