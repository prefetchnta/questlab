/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-04-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> Holidays 停车算费头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_HOLIDAYS_H__
#define __CR_HOLIDAYS_H__

#include "../rtclib.h"

/* Holidays 对象类型 */
typedef void_t*     holidays_t;

/* Holidays 参数结构 */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* Holidays 收费时段 */
CR_TYPEDEF struct
{
        byte_t  num;    /* 0表示按原来的算, 否则为跳转的编号 */
        byte_t  st[2];              /* 开始时间 */
        byte_t  et[2];              /* 结束时间 */
        byte_t  cost_buf[2];        /* 单位时间 */
        byte_t  value_buf[2];       /* 单位价格 */
        byte_t  max_value_buf[2];   /* 最大限额 */

} CR_PACKED sHOLI_SECS;

/* ------------------------------------ */
#define HOLI_MAX_NUM    5   /* 最大段数 */
#define HOLI_MAX_TYP    8   /* 最大规则 */
/* ------------------------------------ */
#define HOLI_TYPE_FS    0   /* 分时收费 */
#define HOLI_TYPE_JS    1   /* 计时收费 */
#define HOLI_TYPE_SC    2   /* 时长收费 */
#define HOLI_TYPE_AC    3   /* 按次收费 */
/* ------------------------------------ */
#define HOLI_TYPE_JT    4   /* 计天收费 */
#define HOLI_TYPE_TC    5   /* 天长收费 */
#define HOLI_TYPE_MAX   5   /* 最大取值 */
/* ------------------------------------ */
#define HOLI_FL_MASK  0x0F  /* 类型掩码 */
#define HOLI_FL_ZERO  0x80  /* 过零即天 */
#define HOLI_FL_CRSS  0x40  /* 跨段合并 */
#define HOLI_FL_STUP  0x20  /* 向上进位 */
#define HOLI_FL_FREE  0x10  /* 内部免费 */
/* ------------------------------------ */

/* Holidays 收费规则 */
CR_TYPEDEF struct
{
        int16u      okay;       /* 规则是否有效 */
        byte_t      numb;       /* 收费规则编号 */
        byte_t      type;       /* 收费类型 */
        byte_t      free_tm;    /* 免费时间 */
        byte_t      if_free;    /* 免费时间过后, 免费时间是否合算收费 */
        sHOLI_SECS  list[HOLI_MAX_NUM];     /* 时间段内详细收费规则 */
        byte_t      maxv[2];    /* 全天最大金额 */
        byte_t      full_cost;  /* 是否足时收费 */

} CR_PACKED sHOLI_RULE;

/* Holidays 节假日 */
CR_TYPEDEF struct
{
        sBCD_DT beg, end;               /* 节假日起止 */
        byte_t  rule[HOLI_MAX_TYP];     /* 收费规则编号 */

} CR_PACKED sHOLI_DAYS;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/* Holidays 规则段 */
typedef struct
{
        sDATETIME   date;           /* 规则开始时间 */
        /* ----------- */
        byte_t  rule, week, holi;   /* 三种规则的编码 */

} sHOLI_RNGS;

/* Holidays 清除类型 */
#define HOLI_CLEAR_WEEK     0x01    /* 清除周计划 */
#define HOLI_CLEAR_HDAY     0x02    /* 清除节假日 */
#define HOLI_CLEAR_RULE     0x03    /* 清除收费规则 */

/* Holidays 基础 API */
CR_API holidays_t   holidays_init (void_t);
CR_API void_t       holidays_kill (holidays_t hdays);
CR_API void_t       holidays_clear (holidays_t hdays, uint_t type);
CR_API sint_t       holidays_rule_set (holidays_t hdays,
                                       const sHOLI_RULE *rule);
CR_API uint_t       holidays_hday_set (holidays_t hdays,
                                       const sHOLI_DAYS *holi,
                                       uint_t count);
CR_API sint_t       holidays_week_set (holidays_t hdays,
                                       const byte_t *weeks,
                                       uint_t index);
CR_API sint_t       holidays_free_get (holidays_t hdays,
                                       const sDATETIME *date,
                                       byte_t rule_id,
                                       byte_t week_id,
                                       byte_t holi_id);
CR_API sint_t       holidays_money (holidays_t hdays,
                                    const sDATETIME *leave,
                                    const sDATETIME *enter,
                                    const sDATETIME *mbeg,
                                    const sDATETIME *mend, bool_t is_pre,
                                    byte_t rule_id, byte_t rule_wid,
                                    byte_t rule_hid, byte_t month_id,
                                    byte_t month_wid, byte_t month_hid);
CR_API sint_t       holidays_rests (holidays_t hdays,
                                    const sDATETIME *leave,
                                    const sDATETIME *enter,
                                    const sDATETIME *mbeg,
                                    const sDATETIME *mend, bool_t is_pre,
                                    byte_t rule_id, byte_t rule_wid,
                                    byte_t rule_hid, byte_t month_id,
                                    byte_t month_wid, byte_t month_hid);
CR_API sint_t       holidays_stepa (holidays_t hdays,
                                    const sDATETIME *leave,
                                    const sDATETIME *enter,
                                    const sDATETIME *mbeg,
                                    const sDATETIME *mend, bool_t is_pre,
                                    byte_t rule_id, byte_t rule_wid,
                                    byte_t rule_hid, byte_t month_id,
                                    byte_t month_wid, byte_t month_hid);
CR_API sint_t       holidays_free_get2 (holidays_t hdays,
                                        const sDATETIME *date,
                                        const sHOLI_RNGS *range,
                                        uint_t count, byte_t rule_id,
                                        byte_t week_id, byte_t holi_id);
CR_API sint_t       holidays_money2 (holidays_t hdays,
                                     const sDATETIME *leave,
                                     const sDATETIME *enter,
                                     const sHOLI_RNGS *range,
                                     uint_t count, bool_t is_pre,
                                     byte_t rule_id, byte_t week_id,
                                     byte_t holi_id);
CR_API sint_t       holidays_rests2 (holidays_t hdays,
                                     const sDATETIME *leave,
                                     const sDATETIME *enter,
                                     const sHOLI_RNGS *range,
                                     uint_t count, bool_t is_pre,
                                     byte_t rule_id, byte_t week_id,
                                     byte_t holi_id);
CR_API sint_t       holidays_stepa2 (holidays_t hdays,
                                     const sDATETIME *leave,
                                     const sDATETIME *enter,
                                     const sHOLI_RNGS *range,
                                     uint_t count, bool_t is_pre,
                                     byte_t rule_id, byte_t week_id,
                                     byte_t holi_id);

#endif  /* !__CR_HOLIDAYS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
