/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2013-11-13
 *              #
 ================================================
        QuestLAB 哈希计算器
 ================================================
 */

#ifndef __QL_QSTHASH_H__
#define __QL_QSTHASH_H__

#include "../QstLibs/QstLibs.h"

/* 公用定义宏 */
#ifndef EXE_XNAME
    #define EXE_XNAME   "QstHash"
    #define WIN_TITLE   "QstHash"
    #define WIN_CLASS   "QstHashCLSS"
    #define WIN_ICONF   "QstHash.ini"
    #define WIN_XCONF   "QstHash.xml"
#endif

/* 工作上下文结构 */
typedef struct
{
        /* 应用程序参数 */
        bool_t      quit;   /* 是否退出 */
        void_t*     form;   /* 窗口对象 */
        socket_t    netw;   /* 网络连接 */

} sQstHash;

/* 哈希校验执行单元 */
typedef struct
{
        /* 哈希名称串 */
        const ansi_t*   name;

        /* 是否支持分段计算 */
        bool_t  is_support_section;

        /* 获取哈希对象 */
        void_t* (*hash_init) (void_t);

        /* 更新哈希计算 */
        void_t  (*hash_update) (void_t *ctx,
                            const void_t *data, uint_t size);
        /* 结束哈希计算 */
        ansi_t* (*hash_finish) (void_t *ctx);

} sQHSH_UNIT;

#endif  /* !__QL_QSTHASH_H__ */
