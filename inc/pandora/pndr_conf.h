/*
===============================================================================
*   配置文件
*
*   2007-12-25  修改风格
===============================================================================
*/

#ifndef _PNDR_CONF_H_
#define _PNDR_CONF_H_

/*
===============================================================================
*   平台相关
===============================================================================
*/
/*
    使用 WIN32 API

#define PNDR_WIN32
*/

/*
    调试状态
    保存变量名以便检查冲突情况

#define PNDR_DEBUG
*/

/*
    支持浮点数运算

#define PNDR_SUPPORT_REAL
*/

/*
    两个编译器的设置

#define PNDR_INLINE     static
#define PNDR_FASTCALL
*/
#define PNDR_INLINE     __inline
#define PNDR_FASTCALL   __fastcall

/*
===============================================================================
*   是否编译成 DLL
===============================================================================
*/
#ifdef  PNDR_WIN32
    #ifdef  PNDR_DLL
        #define PNDR_API    __declspec(dllexport)
    #else
        #define PNDR_API
    #endif
#else
    #define PNDR_API
#endif

/*
===============================================================================
*   类型定义
===============================================================================
*/
typedef unsigned char   byte_t;
typedef unsigned short  word_t;
typedef   signed int    sint_t;
typedef unsigned int    uint_t;
typedef unsigned long   uint32;
#ifdef  PNDR_SUPPORT_REAL
typedef float           real_t;
#endif

/*
===============================================================================
*   状态机初始状态
===============================================================================
*/
/*
    寄存器栈初始大小
*/
#define PNDR_REG_STACK      64

/*
    表达式栈初始大小
*/
#define PNDR_EXP_STACK      64

/*
    循环结尾栈初始大小
*/
#define PNDR_LOOP_STACK     32

/*
    局部堆初始个数
*/
#define PNDR_LOCAL_HEAP     32

/*
    字符串常量个数
*/
#define PNDR_STR_LIST       512

/*
    代码堆大小
*/
#define PNDR_EXE_HEAP       4096

/*
    变量名最多字符数
*/
#define PNDR_MAX_NAME       32

/*
    调用的最大参数个数
*/
#define PNDR_MAX_PARAM      16

/*
    全局变量堆大小
    可选大小见下面的列表
*/
#define PNDR_VAR_HEAP       673

/*
17
37
79
163
331
673
1361
2729
5471
10949
21911
43853
87719
175447
350899
701819
1403641
2807303
5614657
11229331
22458671
44917381
89834777
179669557
359339171
718678369
1437356741
2147483647
*/

#endif  /* _PNDR_CONF_H_ */
