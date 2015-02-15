/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-12-02  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 连接索引生成函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3.h"
#include "memlib.h"

/*
=======================================
    生成连接索引
=======================================
*/
CR_API uint_t
gen_interlock (
  __CR_OT__ int16u**    pibuf,
  __CR_OT__ leng_t*     inum,
  __CR_IO__ uint_t*     level,
  __CR_IN__ uint_t      tile_size
    )
{
    int16u* ti;
    uint_t  step, point, nibuf;
    uint_t  sz, xx, zz, ii, lv;

    /* 参数过滤 */
    if ((tile_size < 2) || (tile_size > 256))
        return (0);

    /* 计算最多的等级 */
    lv = 1;
    point = tile_size - 1;
    if (point % 2 == 0) {
        while (point % 2 == 0) {
            point /= 2;
            lv++;
        }
        if (level != NULL) {
            if (*level > lv || *level == 0)
                *level = lv;
            else
                lv = *level;
        }
    }
    else {
        if (level != NULL)
            *level = lv;
    }

    /* 计算索引缓冲的个数 */
    nibuf = (lv - 1) * 9 + 1;
    if (pibuf == NULL || inum == NULL)
        return (nibuf);
    point = 0;

    /* full detail */
    step = 1;
    for (ii = 0; ii < lv; ii++) {
        inum[point] = ((tile_size - 1) / step) *
                      ((tile_size - 1) / step) * 6;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        for (zz = 0; zz < tile_size - 1; zz += step)
        for (xx = 0; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
        step *= 2;
    }

    if (lv == 1)
        return (nibuf);

    /* joint up */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step) * 6 +
                      ((tile_size - 1) / step) * 9;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        sz = tile_size - step - 1;
        for (xx = 0; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(sz * tile_size + xx);
            ti[1] = (int16u)((sz + step) * tile_size + xx);
            ti[2] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[3] = (int16u)(sz * tile_size + xx);
            ti[4] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[5] = (int16u)((sz + step) * tile_size + xx + step);
            ti[6] = (int16u)(sz * tile_size + xx);
            ti[7] = (int16u)((sz + step) * tile_size + xx + step);
            ti[8] = (int16u)(sz * tile_size + xx + step);
            ti += 9;
        }
        for (zz = 0; zz < tile_size - step - 1; zz += step)
        for (xx = 0; xx < tile_size - 1;        xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint down */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step) * 6 +
                      ((tile_size - 1) / step) * 9;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        for (xx = 0; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(xx);
            ti[1] = (int16u)(step * tile_size + xx);
            ti[2] = (int16u)(step * tile_size + xx + step);
            ti[3] = (int16u)(xx);
            ti[4] = (int16u)(step * tile_size + xx + step);
            ti[5] = (int16u)(xx + step / 2);
            ti[6] = (int16u)(xx + step / 2);
            ti[7] = (int16u)(step * tile_size + xx + step);
            ti[8] = (int16u)(xx + step);
            ti += 9;
        }
        for (zz = step; zz < tile_size - 1; zz += step)
        for (xx = 0;    xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint left */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step) * 6 +
                      ((tile_size - 1) / step) * 9;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        sz = tile_size - step - 1;
        for (zz = 0; zz < tile_size - 1; zz += step) {
            ti[0] = (int16u)(zz * tile_size + sz);
            ti[1] = (int16u)((zz + step) * tile_size + sz);
            ti[2] = (int16u)((zz + step) * tile_size + sz + step);
            ti[3] = (int16u)(zz * tile_size + sz);
            ti[4] = (int16u)((zz + step) * tile_size + sz + step);
            ti[5] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[6] = (int16u)(zz * tile_size + sz);
            ti[7] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[8] = (int16u)(zz * tile_size + sz + step);
            ti += 9;
        }
        for (zz = 0; zz < tile_size - 1;        zz += step)
        for (xx = 0; xx < tile_size - step - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint right */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step) * 6 +
                      ((tile_size - 1) / step) * 9;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        for (zz = 0; zz < tile_size - 1; zz += step) {
            ti[0] = (int16u)((zz + step) * tile_size + step);
            ti[1] = (int16u)((zz + step / 2) * tile_size);
            ti[2] = (int16u)((zz + step) * tile_size);
            ti[3] = (int16u)((zz + step) * tile_size + step);
            ti[4] = (int16u)(zz * tile_size);
            ti[5] = (int16u)((zz + step / 2) * tile_size);
            ti[6] = (int16u)((zz + step) * tile_size + step);
            ti[7] = (int16u)(zz * tile_size + step);
            ti[8] = (int16u)(zz * tile_size);
            ti += 9;
        }
        for (zz = 0;    zz < tile_size - 1; zz += step)
        for (xx = step; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint up left */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step - 1) * 6 +
                      ((tile_size - 1) / step - 1) * 2 * 9 + 12;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        sz = tile_size - step - 1;
        for (xx = 0; xx < tile_size - step - 1; xx += step) {
            ti[0] = (int16u)(sz * tile_size + xx);
            ti[1] = (int16u)((sz + step) * tile_size + xx);
            ti[2] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[3] = (int16u)(sz * tile_size + xx);
            ti[4] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[5] = (int16u)((sz + step) * tile_size + xx + step);
            ti[6] = (int16u)(sz * tile_size + xx);
            ti[7] = (int16u)((sz + step) * tile_size + xx + step);
            ti[8] = (int16u)(sz * tile_size + xx + step);
            ti += 9;
        }
        for (zz = 0; zz < tile_size - step - 1; zz += step) {
            ti[0] = (int16u)(zz * tile_size + sz);
            ti[1] = (int16u)((zz + step) * tile_size + sz);
            ti[2] = (int16u)((zz + step) * tile_size + sz + step);
            ti[3] = (int16u)(zz * tile_size + sz);
            ti[4] = (int16u)((zz + step) * tile_size + sz + step);
            ti[5] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[6] = (int16u)(zz * tile_size + sz);
            ti[7] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[8] = (int16u)(zz * tile_size + sz + step);
            ti += 9;
        }
        ti[0]  = (int16u)(sz * tile_size + sz);
        ti[1]  = (int16u)((sz + step) * tile_size + sz);
        ti[2]  = (int16u)((sz + step) * tile_size + sz + step / 2);
        ti[3]  = (int16u)(sz * tile_size + sz);
        ti[4]  = (int16u)((sz + step) * tile_size + sz + step / 2);
        ti[5]  = (int16u)((sz + step) * tile_size + sz + step);
        ti[6]  = (int16u)(sz * tile_size + sz);
        ti[7]  = (int16u)((sz + step) * tile_size + sz + step);
        ti[8]  = (int16u)((sz + step / 2) * tile_size + sz + step);
        ti[9]  = (int16u)(sz * tile_size + sz);
        ti[10] = (int16u)((sz + step / 2) * tile_size + sz + step);
        ti[11] = (int16u)(sz * tile_size + sz + step);
        ti += 12;
        for (zz = 0; zz < tile_size - step - 1; zz += step)
        for (xx = 0; xx < tile_size - step - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint up right */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step - 1) * 6 +
                      ((tile_size - 1) / step - 1) * 2 * 9 + 12;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        sz = tile_size - step - 1;
        for (xx = step; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(sz * tile_size + xx);
            ti[1] = (int16u)((sz + step) * tile_size + xx);
            ti[2] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[3] = (int16u)(sz * tile_size + xx);
            ti[4] = (int16u)((sz + step) * tile_size + xx + step / 2);
            ti[5] = (int16u)((sz + step) * tile_size + xx + step);
            ti[6] = (int16u)(sz * tile_size + xx);
            ti[7] = (int16u)((sz + step) * tile_size + xx + step);
            ti[8] = (int16u)(sz * tile_size + xx + step);
            ti += 9;
        }
        for (zz = 0; zz < tile_size - step - 1; zz += step) {
            ti[0] = (int16u)((zz + step) * tile_size + step);
            ti[1] = (int16u)((zz + step / 2) * tile_size);
            ti[2] = (int16u)((zz + step) * tile_size);
            ti[3] = (int16u)((zz + step) * tile_size + step);
            ti[4] = (int16u)(zz * tile_size);
            ti[5] = (int16u)((zz + step / 2) * tile_size);
            ti[6] = (int16u)((zz + step) * tile_size + step);
            ti[7] = (int16u)(zz * tile_size + step);
            ti[8] = (int16u)(zz * tile_size);
            ti += 9;
        }
        ti[0]  = (int16u)(sz * tile_size + step);
        ti[1]  = (int16u)(sz * tile_size);
        ti[2]  = (int16u)((sz + step / 2) * tile_size);
        ti[3]  = (int16u)(sz * tile_size + step);
        ti[4]  = (int16u)((sz + step / 2) * tile_size);
        ti[5]  = (int16u)((sz + step) * tile_size);
        ti[6]  = (int16u)(sz * tile_size + step);
        ti[7]  = (int16u)((sz + step) * tile_size);
        ti[8]  = (int16u)((sz + step) * tile_size + step / 2);
        ti[9]  = (int16u)(sz * tile_size + step);
        ti[10] = (int16u)((sz + step) * tile_size + step / 2);
        ti[11] = (int16u)((sz + step) * tile_size + step);
        ti += 12;
        for (zz = 0;    zz < tile_size - step - 1; zz += step)
        for (xx = step; xx < tile_size - 1;        xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint down left */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step - 1) * 6 +
                      ((tile_size - 1) / step - 1) * 2 * 9 + 12;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        sz = tile_size - step - 1;
        for (xx = 0; xx < tile_size - step - 1; xx += step) {
            ti[0] = (int16u)(xx);
            ti[1] = (int16u)(step * tile_size + xx);
            ti[2] = (int16u)(step * tile_size + xx + step);
            ti[3] = (int16u)(xx);
            ti[4] = (int16u)(step * tile_size + xx + step);
            ti[5] = (int16u)(xx + step / 2);
            ti[6] = (int16u)(xx + step / 2);
            ti[7] = (int16u)(step * tile_size + xx + step);
            ti[8] = (int16u)(xx + step);
            ti += 9;
        }
        for (zz = step; zz < tile_size - 1; zz += step) {
            ti[0] = (int16u)(zz * tile_size + sz);
            ti[1] = (int16u)((zz + step) * tile_size + sz);
            ti[2] = (int16u)((zz + step) * tile_size + sz + step);
            ti[3] = (int16u)(zz * tile_size + sz);
            ti[4] = (int16u)((zz + step) * tile_size + sz + step);
            ti[5] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[6] = (int16u)(zz * tile_size + sz);
            ti[7] = (int16u)((zz + step / 2) * tile_size + sz + step);
            ti[8] = (int16u)(zz * tile_size + sz + step);
            ti += 9;
        }
        ti[0]  = (int16u)(step * tile_size + sz);
        ti[1]  = (int16u)(sz + step / 2);
        ti[2]  = (int16u)(sz);
        ti[3]  = (int16u)(step * tile_size + sz);
        ti[4]  = (int16u)(sz + step);
        ti[5]  = (int16u)(sz + step / 2);
        ti[6]  = (int16u)(step * tile_size + sz);
        ti[7]  = (int16u)(step / 2 * tile_size + step + sz);
        ti[8]  = (int16u)(sz + step);
        ti[9]  = (int16u)(step * tile_size + sz);
        ti[10] = (int16u)(step * tile_size + step + sz);
        ti[11] = (int16u)(step / 2 * tile_size + step + sz);
        ti += 12;
        for (zz = step; zz < tile_size - 1;        zz += step)
        for (xx = 0;    xx < tile_size - step - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }

    /* joint down right */
    step = 1;
    for (ii = 1; ii < lv; ii++) {
        step *= 2;
        inum[point] = ((tile_size - 1) / step - 1) *
                      ((tile_size - 1) / step - 1) * 6 +
                      ((tile_size - 1) / step - 1) * 2 * 9 + 12;
        pibuf[point] = mem_talloc(inum[point], int16u);
        if (pibuf[point] == NULL)
            goto _failure;
        ti = pibuf[point];
        for (xx = step; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(xx);
            ti[1] = (int16u)(step * tile_size + xx);
            ti[2] = (int16u)(step * tile_size + xx + step);
            ti[3] = (int16u)(xx);
            ti[4] = (int16u)(step * tile_size + xx + step);
            ti[5] = (int16u)(xx + step / 2);
            ti[6] = (int16u)(xx + step / 2);
            ti[7] = (int16u)(step * tile_size + xx + step);
            ti[8] = (int16u)(xx + step);
            ti += 9;
        }
        for (zz = step; zz < tile_size - 1; zz += step) {
            ti[0] = (int16u)((zz + step) * tile_size + step);
            ti[1] = (int16u)((zz + step / 2) * tile_size);
            ti[2] = (int16u)((zz + step) * tile_size);
            ti[3] = (int16u)((zz + step) * tile_size + step);
            ti[4] = (int16u)(zz * tile_size);
            ti[5] = (int16u)((zz + step / 2) * tile_size);
            ti[6] = (int16u)((zz + step) * tile_size + step);
            ti[7] = (int16u)(zz * tile_size + step);
            ti[8] = (int16u)(zz * tile_size);
            ti += 9;
        }
        ti[0]  = (int16u)(step * tile_size + step);
        ti[1]  = (int16u)(step);
        ti[2]  = (int16u)(step / 2);
        ti[3]  = (int16u)(step * tile_size + step);
        ti[4]  = (int16u)(step / 2);
        ti[5]  = (int16u)(0);
        ti[6]  = (int16u)(step * tile_size + step);
        ti[7]  = (int16u)(0);
        ti[8]  = (int16u)(step / 2 * tile_size);
        ti[9]  = (int16u)(step * tile_size + step);
        ti[10] = (int16u)(step / 2 * tile_size);
        ti[11] = (int16u)(step * tile_size);
        ti += 12;
        for (zz = step; zz < tile_size - 1; zz += step)
        for (xx = step; xx < tile_size - 1; xx += step) {
            ti[0] = (int16u)(zz * tile_size + xx);
            ti[1] = (int16u)((zz + step) * tile_size + xx);
            ti[2] = (int16u)((zz + step) * tile_size + xx + step);
            ti[3] = (int16u)(zz * tile_size + xx);
            ti[4] = (int16u)((zz + step) * tile_size + xx + step);
            ti[5] = (int16u)(zz * tile_size + xx + step);
            ti += 6;
        }
        point++;
    }
    return (nibuf);

_failure:
    for (ii = 0; ii < point; ii++)
        mem_free(pibuf[ii]);
    return (0);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
