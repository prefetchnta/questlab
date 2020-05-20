/*
===============================================================================
*   潘多拉脚本
*               by prefetchnta
*
*   2007-12-25  修改风格
*   2008-09-05  移出版本信息
===============================================================================
*/

#ifndef _PANDORA_H_
#define _PANDORA_H_

#include "pndr_conf.h"

/*
===============================================================================
*   变量类型
===============================================================================
*/
#define PNDR_INT        0   /* 整数     */
#define PNDR_REAL       4   /* 实数     */
#define PNDR_FUNC       1   /* 脚本函数 */
#define PNDR_CALL_C     2   /* C   调用 */
#define PNDR_CALL_CPP   3   /* C++ 调用 */

/*
===============================================================================
*   变量结构
===============================================================================
*/
#pragma pack(push)
#pragma pack(1)

typedef struct  _pndr_var
{
        /* 变量数据 */
        union
        {
#ifdef  PNDR_SUPPORT_REAL
                real_t      real32;
#endif
                sint_t      int32s;
                uint_t      int32u;
                void*       pointer;
        };

        /* 变量信息 */
        uint_t      type;
        uint_t      param;
        uint_t      clear;
        uint_t      ret_type;
} pndr_var;

#pragma pack(pop)

/*
===============================================================================
*   表达式结构
===============================================================================
*/
typedef struct  _pndr_exp
{
        uint_t      op;
        pndr_var*   var;
} pndr_exp;

/*
===============================================================================
*   变量表结构
===============================================================================
*/
typedef struct  _pndr_table
{
        pndr_var                var;
        uint_t                  len;
        uint32                  hash1;
        uint32                  hash2;
        struct _pndr_table*     next;

#ifdef  PNDR_DEBUG
        char    name[PNDR_MAX_NAME + 1];
#endif
} pndr_table;

/*
===============================================================================
*   字符串常量结构
===============================================================================
*/
typedef struct  _pndr_string
{
        uint_t      len;
        uint_t      index;
        uint32      hash1;
        uint32      hash2;
        char*       string;
} pndr_string;

/*
===============================================================================
*   状态机结构
===============================================================================
*/
typedef struct  _pndr_state
{
        /* 关键字标志 */
        uint_t          flag_if;
        uint_t          flag_loop;
        uint_t          flag_time;

        /* 代码堆 */
        uint_t          exe_ptr;
        byte_t*         exe_heap;

        /* 字符串列表 */
        uint_t          str_num;
        uint_t          str_size;
        pndr_string*    str_list;

        /* 寄存器栈 */
        uint_t          reg_sp;
        uint_t          reg_size;
        pndr_var*       reg_stack;

        /* 表达式栈 */
        uint_t          exp_sp;
        uint_t          exp_size;
        pndr_exp*       exp_stack;

        /* 循环结尾栈 */
        uint_t          loop_sp;
        uint_t          loop_size;
        byte_t**        loop_stack;

        /* 全局变量堆 */
        pndr_table      global[PNDR_VAR_HEAP];

        /* 错误信息 */
        const char*     error;

        /* 局部状态机栈 */
        uint_t                  local_sp;
        uint_t                  local_size;
        struct _pndr_state*     local_state;

        /* 内存分配函数 */
        void*   (*pndr_alloc) (uint_t size);
        void    (*pndr_free ) (void *buffer);
} pndr_state;

/*
===============================================================================
*   API 函数
===============================================================================
*/
#ifdef  __cplusplus
extern "C"  {
#endif

/*************
    系统组
**************/
PNDR_API void   pndr_newState   (pndr_state *P, void *alloc, void *free);
PNDR_API void   pndr_delState   (pndr_state *P);
PNDR_API void   pndr_clearAll   (pndr_state *P);
PNDR_API void   pndr_regLibrary (pndr_state *P);

/*************
    运行组
**************/
PNDR_API       byte_t*  pndr_compile   (pndr_state *P,
                                        const char *src, uint_t *size);

PNDR_API const byte_t*  pndr_doScript  (pndr_state *P, const byte_t *src);
PNDR_API const byte_t*  pndr_doScriptF (pndr_state *P, const byte_t *src);

/*************
    变量组
**************/
PNDR_API pndr_var*  pndr_getVar (pndr_state *P, const char *name);
PNDR_API pndr_var*  pndr_newVar (pndr_state *P, const char *name);

/*************
    转换组
**************/
PNDR_API sint_t     pndr_toInt     (const pndr_var *var);
PNDR_API void*      pndr_toPointer (const pndr_var *var);

#ifdef  PNDR_SUPPORT_REAL
PNDR_API real_t     pndr_toReal    (const pndr_var *var);
#endif

#define pndr_toString(v)    (char*)pndr_toPointer(v)

/*************
    设置组
**************/
PNDR_API void   pndr_setInt     (pndr_state *P,
                                 const char *name, sint_t value);

#ifdef  PNDR_SUPPORT_REAL
PNDR_API void   pndr_setReal    (pndr_state *P,
                                 const char *name, real_t value);
#endif

PNDR_API void   pndr_setFunc    (pndr_state *P,
                                 const char *name, byte_t *func);

PNDR_API void   pndr_setPointer (pndr_state *P,
                                 const char *name, void *pointer);

#define pndr_setString(P,name,str)  pndr_setPointer(P, name, (char*)str)

PNDR_API void   pndr_setCallC   (pndr_state *P,
                                 const char *name, void *func,
                                    uint_t param, uint_t ret_type);

PNDR_API void   pndr_setCallStd (pndr_state *P,
                                 const char *name, void *func,
                                    uint_t param, uint_t ret_type);

PNDR_API void   pndr_setCallCpp (pndr_state *P,
                                 const char *name, void *func,
                                    uint_t param, uint_t ret_type);

PNDR_API void   pndr_changeCall (pndr_state *P,
                                 const char *name, uint_t param);

/*************
    库函数
**************/
PNDR_API uint_t     pndrL_doFile (pndr_state *P,
                                     const char *fileName, uint_t unsafe);

PNDR_API uint_t     pndrL_importBin (pndr_state *P, const char *str);
PNDR_API uint_t     pndrL_importXml (pndr_state *P, const char *fileName);

/*************
    类型组
**************/
#define pndr_isInt(v)       ((v)->type == PNDR_INT)
#define pndr_isReal(v)      ((v)->type == PNDR_REAL)
#define pndr_isFunc(v)      ((v)->type == PNDR_FUNC)
#define pndr_isCallC(v)     ((v)->type == PNDR_CALL_C)
#define pndr_isCallCpp(v)   ((v)->type == PNDR_CALL_CPP)

#ifdef  __cplusplus
}
#endif

#endif  /* _PANDORA_H_ */
