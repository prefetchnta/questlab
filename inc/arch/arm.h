/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-12-04  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: 2012-04-02  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ARM 架构系统头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ARM_H__
#define __CR_ARM_H__ 0x531A9B72UL

#include "defs.h"

/* 过滤编译器 */
#if !defined(_CR_AR_ARM_) || \
    !defined(_CR_AR_THUMB_)
    #error "arm.h: invalid AR TYPE define!"
#endif

/*
=======================================
    打开 IRQ 中断响应
=======================================
*/
inline void_t
irqa_enabled (void_t)
{
#if     defined(_CR_CC_ICCARM_)
    #if (_CR_ARM_V32_ == 0)
        __enable_interrupt();
    #else
        __enable_irq();
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            bic     r0, r0, #0x80
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsie   i
        }
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_THUMB_) && \
        (_CR_ARM_V16_ >= 3)
        __asm {
            cpsie   i
        }
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    关闭 IRQ 中断响应
=======================================
*/
inline void_t
irqa_disable (void_t)
{
#if     defined(_CR_CC_ICCARM_)
    #if (_CR_ARM_V32_ == 0)
        __disable_interrupt();
    #else
        __disable_irq();
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            orr     r0, r0, #0x80
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsid   i
        }
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_THUMB_) && \
        (_CR_ARM_V16_ >= 3)
        __asm {
            cpsid   i
        }
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    打开 FIQ 中断响应
=======================================
*/
inline void_t
fiqa_enabled (void_t)
{
#if     defined(_CR_CC_ICCARM_)
    #if (_CR_ARM_V32_ != 0)
        __enable_fiq();
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            bic     r0, r0, #0x40
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsie   f
        }
    #endif
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    关闭 FIQ 中断响应
=======================================
*/
inline void_t
fiqa_disable (void_t)
{
#if     defined(_CR_CC_ICCARM_)
    #if (_CR_ARM_V32_ != 0)
        __disable_fiq();
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            orr     r0, r0, #0x40
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsid   f
        }
    #endif
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    整个系统的中断打开 (必需)
=======================================
*/
inline void_t
irqx_enabled (void_t)
{
#if     defined(_CR_CC_ICCARM_)

        __enable_interrupt();

#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            bic     r0, r0, #0xC0
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsie   if
        }
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_THUMB_) && \
        (_CR_ARM_V16_ >= 3)
        __asm {
            cpsie   i
        }
#else
    #error "arm.h: AR & CP TYPE not supported yet!"
#endif  /* AR & CP TYPE predefines */
}

/*
=======================================
    整个系统的中断关闭 (必需)
=======================================
*/
inline void_t
irqx_disable (void_t)
{
#if     defined(_CR_CC_ICCARM_)

        __disable_interrupt();

#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_ARM_)
    #if (_CR_ARM_V32_ <= 5)
        int32u  r0;

        __asm {
            mrs     r0, cpsr
            orr     r0, r0, #0xC0
            msr     cpsr_c, r0
        }
    #else
        __asm {
            cpsid   if
        }
    #endif
#elif   defined(_CR_ASM_INTL_) && defined(_CR_AR_THUMB_) && \
        (_CR_ARM_V16_ >= 3)
        __asm {
            cpsid   i
        }
#else
    #error "arm.h: AR & CP TYPE not supported yet!"
#endif  /* AR & CP TYPE predefines */
}

#endif  /* !__CR_ARM_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
