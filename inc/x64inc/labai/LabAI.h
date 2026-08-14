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
 *             ##       CREATE: 2018-03-05
 *              #
 ================================================
        LabAI 总头文件
 ================================================
 */

#ifndef __AI_LAB_H__
#define __AI_LAB_H__

#include "crhack.h"

/*****************/
/* NCNN GPU 相关 */
/*****************/
typedef struct
{
    uint_t  gpu_type;   /* 0 = discrete gpu
                           1 = integrated gpu
                           2 = virtual gpu
                           3 = cpu */
    int32u  vers_api;
    int32u  vers_drv;
    int32u  vendor_id;
    int32u  device_id;
    int32u  driver_id;
    cstr_t  device_name;
    cstr_t  driver_name;

} sNCNN_GPU_INFO;

CR_API uint_t   labai_ncnn_gpu_count (void_t);
CR_API uint_t   labai_ncnn_gpu_default (void_t);
CR_API bool_t   labai_ncnn_gpu_info (sNCNN_GPU_INFO *gpu, sint_t idx);

/****************/
/* 子模块头文件 */
/****************/
#include "LabIMG.h"

#endif  /* !__AI_LAB_H__ */
