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
 *             ##       CREATE: 2017-11-15
 *              #
 ================================================
        体彩 6+1 号码计算器
 ================================================
 */

#include "../QstLibs/QstLibs.h"

#include <conio.h>

uint_t  xx[7];
uint_t  n2[10][10];
uint_t  n3[10][10][10];
uint_t  n4[10][10][10][10];
uint_t  n5[10][10][10][10][10];
uint_t  n6[10][10][10][10][10][10];

/*
---------------------------------------
    号码统计器
---------------------------------------
*/
static sint_t
luck_cmp (
  __CR_IN__ const ansi_t*   name
    )
{
    sINIu*  ini;

    /* 读入文件 */
    name = file_load_as_strA(name);
    if (name == NULL)
        return (-1);
    ini = ini_parseU(name);
    mem_free(name);
    if (ini == NULL)
        return (-1);

    /* 解析数字 */
    mem_zero(n2, sizeof(n2));
    mem_zero(n3, sizeof(n3));
    mem_zero(n4, sizeof(n4));
    mem_zero(n5, sizeof(n5));
    mem_zero(n6, sizeof(n6));
    for (leng_t idx = 0; idx < ini->count; idx++) {
        name = ini->lines[idx];
        if (str_lenA(name) < 8)
            break;
        if (!is_digitA(name[0]) ||
            !is_digitA(name[1]) ||
            !is_digitA(name[2]) ||
            !is_digitA(name[3]) ||
            !is_digitA(name[4]) ||
            !is_digitA(name[5]) ||
            !is_digitA(name[7]) || name[6] != '+')
            break;
        xx[0] = name[0] & 0x0F;
        xx[1] = name[1] & 0x0F;
        xx[2] = name[2] & 0x0F;
        xx[3] = name[3] & 0x0F;
        xx[4] = name[4] & 0x0F;
        xx[5] = name[5] & 0x0F;
        xx[6] = name[7] & 0x0F;
        for (uint_t ii = 0; ii < 6; ii++)
            n2[xx[ii]][xx[ii+1]]++;
        for (uint_t ii = 0; ii < 5; ii++)
            n3[xx[ii]][xx[ii+1]][xx[ii+2]]++;
        for (uint_t ii = 0; ii < 4; ii++)
            n4[xx[ii]][xx[ii+1]][xx[ii+2]][xx[ii+3]]++;
        for (uint_t ii = 0; ii < 3; ii++)
            n5[xx[ii]][xx[ii+1]][xx[ii+2]][xx[ii+3]][xx[ii+4]]++;
        for (uint_t ii = 0; ii < 2; ii++)
            n6[xx[ii]][xx[ii+1]][xx[ii+2]][xx[ii+3]][xx[ii+4]][xx[ii+5]]++;
    }

    /* 输出结果 */
    for (uint_t aa = 0; aa < 10; aa++) {
        printf("[%u]\n", aa);
        for (uint_t bb = 0; bb < 10; bb++) {
            if (n2[aa][bb] == 0)
                continue;
            printf("  [%u]:", bb);
            printf("%u\n", n2[aa][bb]);
            for (uint_t cc = 0; cc < 10; cc++) {
                if (n3[aa][bb][cc] == 0)
                    continue;
                printf("    [%u]:", cc);
                printf("%u\n", n3[aa][bb][cc]);
                for (uint_t dd = 0; dd < 10; dd++) {
                    if (n4[aa][bb][cc][dd] == 0)
                        continue;
                    printf("      [%u]:", dd);
                    printf("%u\n", n4[aa][bb][cc][dd]);
                    for (uint_t ee = 0; ee < 10; ee++) {
                        if (n5[aa][bb][cc][dd][ee] == 0)
                            continue;
                        printf("        [%u]:", ee);
                        printf("%u\n", n5[aa][bb][cc][dd][ee]);
                        for (uint_t ff = 0; ff < 10; ff++) {
                            if (n6[aa][bb][cc][dd][ee][ff] == 0)
                                continue;
                            printf("          [%u]:", ff);
                            printf("%u\n", n6[aa][bb][cc][dd][ee][ff]);
                        }
                    }
                }
            }
        }
    }

    /* 释放内存 */
    ini_closeU(ini);
    return (0);
}

/*
---------------------------------------
    号码过滤器
---------------------------------------
*/
static bool_t
luck_flt (
  __CR_IN__ int32u  luck,
  __CR_IN__ uint_t  smin,
  __CR_IN__ uint_t  smax,
  __CR_OT__ uint_t* osum
    )
{
    uint_t  cnts[10], sum;
    uint_t  idx, sum2, sum3;

    /* 统计各个数字的个数 */
    mem_zero(cnts, sizeof(cnts));
    for (sum = 0, idx = 0; idx < 6+1; idx++) {
        sum2 = luck % 10;
        cnts[sum2] += 1;
        sum += sum2;
        luck /= 10;
    }

    /* 数值的和值过滤 */
    if (sum < smin || sum > smax)
        return (FALSE);

    /* 统计两个或三个数字重复的个数 */
    sum2 = sum3 = 0;
    for (idx = 0; idx < 10; idx++) {
        if (cnts[idx] == 2)
            sum2 += 1;
        else
        if (cnts[idx] == 3)
            sum3 += 1;
    }

    /* 根据数据分析得：
       A02 > A22 >> A03 > A23 */
    if (!((sum3 == 0 && sum2 == 1) ||
          (sum3 == 0 && sum2 == 2) ||
          (sum3 == 1 && sum2 == 0) ||
          (sum3 == 1 && sum2 == 1)))
        return (FALSE);

    if (osum != NULL)
        *osum = sum;
    return (TRUE);
}

/*
---------------------------------------
    号码混合器
---------------------------------------
*/
static int32u
luck_gen (
  __CR_IN__ uint_t  man,
  __CR_IN__ int32u  sys,
  __CR_IN__ int64u  cpu
    )
{
    int32u  luck, mask;

    luck = hash_adler32_total(&man, sizeof(man));
    mask  = hash_crc16c_total(&sys, sizeof(sys));
    mask <<= 16;
    mask |= hash_crc16i_total(&cpu, sizeof(cpu));
    luck ^= mask;
    luck ^= hash_crc32a_total(&man, sizeof(man));
    luck ^= hash_crc32c_total(&sys, sizeof(sys));
    luck ^= hash_crc32i_total(&cpu, sizeof(cpu));
    cpu = hash_crc64e_total(&luck, sizeof(luck));
    mask ^= hash_crc32h_total(&cpu, sizeof(cpu));
    return ((luck ^ mask) % 9999999);
}

/*
=======================================
    命令行主函数
=======================================
*/
int main (int argc, char *argv[])
{
    int32u  mix;
    int32u  sys;
    int64u  cpu;
    uint_t  sum;
    sint_t  idx;
    sint_t  cnt = 1;
    sint_t  min = 0;
    sint_t  max = 63;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 */
    if (argc > 1)
    {
        /* 参数1为要出的号码数量 */
        cnt = str2intA(argv[1]);
        if (argc > 2)
        {
            /* 参数2为和值下限 */
            min = str2intA(argv[2]);
            if (argc > 3)
            {
                /* 参数3为和值上限 */
                max = str2intA(argv[3]);
            }
        }
    }

    /* 数字统计功能 */
    if (cnt < 1)
        return (luck_cmp(argv[2]));

    /* 参数范围过滤 */
    if      (min <  0) min = 0;
    else if (min > 63) min = 63;
    if      (max <  0) max = 0;
    else if (max > 63) max = 63;
    for (idx = 0; idx < cnt; idx++) {
        for (mix = 10; ; mix /= 88) {
            if (mix >= 10000)
                mix /= 8;
            thread_sleep(mix);
            sys = timer_get32();
            cpu = timer_get64();
            mix = luck_gen(idx + 1, sys, cpu);
            if (mix > 100000 && luck_flt(mix, min, max, &sum))
                break;
        }
        printf("%06u+%u=%u\n", mix / 10, mix % 10, sum);
    }
    return (QST_OKAY);
}
