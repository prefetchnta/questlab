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
 *             ##       CREATE: 2013-09-05
 *              #
 ================================================
        QuestLAB 全局资源文件加载路由
 ================================================
 */

#include "../QstLibs/QstLibs.h"

/* 从宿主传过来的参数 */
static resource_load_t  s_loader = NULL;

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            s_loader = NULL;
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                 接口实现                                  */
/*****************************************************************************/

/*
---------------------------------------
    设置外部文件加载接口
---------------------------------------
*/
static void_t
router_setup (
  __CR_IN__ resource_load_t loader
    )
{
    s_loader = loader;
}

/*
---------------------------------------
    通过路由加载外部文件
---------------------------------------
*/
static bool_t
router_load (
  __CR_OT__ sEX_FILE*       filex,
  __CR_IN__ const ansi_t*   type,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage
    )
{
    if (s_loader == NULL)
        return (FALSE);
    return (s_loader(filex, type, QST_STR_GLOBALS, name, cpage));
}

/*****************************************************************************/
/*                                 接口导出                                  */
/*****************************************************************************/

/* 外部文件路由接口表 */
static const sRES_ROUTER
s_res_router = { router_setup, router_load };

/*
=======================================
    获取外部文件路由接口表
=======================================
*/
CR_API const sRES_ROUTER*
res_router_get (void_t)
{
    return (&s_res_router);
}
