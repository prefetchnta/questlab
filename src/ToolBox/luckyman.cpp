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
#pragma comment (lib, "CrH_MATH.lib")

/*
---------------------------------------
    号码2向量 (字符串)
---------------------------------------
*/
static bool_t
luck_vec (
  __CR_OT__ fpxx_t*         vec,
  __CR_IN__ const ansi_t*   numb
    )
{
    uint_t  idx;

    if (str_lenA(numb) < 8)
        return (FALSE);
    for (idx = 0; idx < 6; idx++) {
        if (!is_digitA(*numb))
            return (FALSE);
        vec[idx] = (fpxx_t)((int)(0x0F & *numb++));
    }
    numb++;
    if (!is_digitA(*numb))
        return (FALSE);
    vec[idx] = (fpxx_t)((int)(0x0F & *numb));
    return (TRUE);
}

/*
---------------------------------------
    号码2向量 (整数值)
---------------------------------------
*/
static void_t
luck_vec (
  __CR_OT__ fpxx_t* vec,
  __CR_IN__ int32u  numb
    )
{
    ansi_t  str[10];

    sprintf(str, "%06u+%u", numb / 10, numb % 10);
    luck_vec(vec, str);
}

/*
---------------------------------------
    加载历史中奖号码
---------------------------------------
*/
static fpxx_t*
luck_his (
  __CR_IN__ const ansi_t*   file,
  __CR_OT__ uint_t*         count
    )
{
    sINIu*  ini;
    fpxx_t* dat;

    /* 读入文件 */
    file = file_load_as_strA(file);
    if (file == NULL)
        return (NULL);
    ini = ini_parseU(file);
    mem_free(file);
    if (ini == NULL)
        return (NULL);
    dat = mem_talloc(ini->count * 7, fpxx_t);
    if (dat == NULL) {
        ini_closeU(ini);
        return (NULL);
    }

    uint_t  num = 0;

    /* 解析 JSON 行 */
    for (leng_t idx = 0; idx < ini->count; idx++) {
        file = str_strA(ini->lines[idx], "\", \"number\":\"");
        if (file == NULL)
            continue;
        if (!luck_vec(&dat[num * 7], file + 13)) {
            mem_free(dat);
            ini_closeU(ini);
            return (NULL);
        }
        num++;
    }

    /* 释放返回 */
    *count = num;
    ini_closeU(ini);
    return (dat);
}

/*
---------------------------------------
    计算号码相似度
---------------------------------------
*/
static void_t
luck_sim (
  __CR_OT__ fpxx_t*         simx,
  __CR_IN__ int32u          numb,
  __CR_IN__ const fpxx_t*   vect,
  __CR_IN__ uint_t          count
    )
{
    fpxx_t  dvec[7];

    luck_vec(dvec, numb);
    simx[1] = simx[2] = simx[3] = 0;

    /* 计算与所有历史中奖号码的平均相似度 */
    for (uint_t idx = 0; idx < count; idx++)
    {
        const fpxx_t*   svec = &vect[idx * 7];

        simx[1] += sim_euclidean(svec, dvec, 7);
        simx[2] += sim_cosine(svec, dvec, 7);
        simx[3] += sim_pearson(svec, dvec, 7);
    }
    simx[1] /= count;
    simx[2] /= count;
    simx[3] /= count;

    /* 合成一个综合相似度 */
    simx[0] = (simx[1] * 2 + simx[2] + simx[3] * 15) / 18;
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

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    fpxx_t  sim[4];
    sint_t  cnt = 1;
    sint_t  min = 0;
    sint_t  max = 63;
    uint_t  num = 0;
    fpxx_t* vec = NULL;

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
                if (argc > 4)
                {
                    /* 参数4为历史号码文件 */
                    vec = luck_his(argv[4], &num);
                }
            }
        }
    }

    /* 参数范围过滤 */
    if      (min <  0) min = 0;
    else if (min > 63) min = 63;
    if      (max <  0) max = 0;
    else if (max > 63) max = 63;
    rand_seed(timer_get32());
    for (idx = 0; idx < cnt; idx++) {
        for (mix = 188; ; mix /= 88) {
            if (mix >= 8888)
                mix %= 8888;
            thread_sleep(mix);
            sys = GetTickCount();
            cpu = __rdtsc();
            mix = luck_gen(idx + rand_get(), sys, cpu);
            if (mix > 100000 && luck_flt(mix, min, max, &sum))
                break;
        }
        printf("%06u+%u=%u", mix / 10, mix % 10, sum);
        if (vec != NULL && num != 0) {
            luck_sim(sim, mix, vec, num);
            printf(" [%.15f, %.15f, %.15f, %.15f]",
                    sim[0], sim[1], sim[2], sim[3]);
        }
        printf("\n");
    }
    TRY_FREE(vec);
    return (QST_OKAY);
}
