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
 *             ##       CREATE: 2025-08-21
 *              #
 ================================================
        ImageLAB NCNN 基础功能函数库
 ================================================
 */

#define NCNN_STATIC_DEFINE
#include "ncnn/gpu.h"
#include "imglab.h"

/*
=======================================
    NCNN 返回 GPU 个数
=======================================
*/
CR_API uint_t
imglab_ncnn_gpu_count (void_t)
{
    return (ncnn::get_gpu_count());
}

/*
=======================================
    NCNN 返回默认 GPU 索引
=======================================
*/
CR_API uint_t
imglab_ncnn_gpu_default (void_t)
{
    return (ncnn::get_default_gpu_index());
}

/*
=======================================
    NCNN 返回指定 GPU 信息
=======================================
*/
CR_API bool_t
imglab_ncnn_gpu_info (
  __CR_OT__ sNCNN_GPU_INFO* gpu,
  __CR_IN__ sint_t          idx
    )
{
    if (idx < 0)
        idx = ncnn::get_default_gpu_index();
    if (idx >= ncnn::get_gpu_count())
        return (FALSE);

    const ncnn::GpuInfo&    info = ncnn::get_gpu_info(idx);

    gpu->gpu_type = info.type();
    gpu->vers_api = info.api_version();
    gpu->vers_drv = info.driver_version();
    gpu->vendor_id = info.vendor_id();
    gpu->device_id = info.device_id();
    gpu->driver_id = info.driver_id();
    gpu->device_name = info.device_name();
    gpu->driver_name = info.driver_name();
    return (TRUE);
}
