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
 *             ##       CREATE: 2024-12-17
 *              #
 ================================================
        ImageLAB NCNN 目标识别函数库
 ================================================
 */

#include "imglab_int.hpp"
#include "ncnn_common.inl"
#include "ncnn_nanodet.inl"
#include "ncnn_mobilenetssd.inl"
#include "ncnn_yolo.inl"

/*****************************************************************************/
/*                                 NanoDet                                   */
/*****************************************************************************/

/*
=======================================
    创建 NanoDet 识别器
=======================================
*/
CR_API nanodet_ncnn_t
imglab_ncnn_nanodet_new (
  __CR_IN__ sint_t  vk_gpu
    )
{
    ncnn::Net*  nndt;

    nndt = new(std::nothrow) ncnn::Net ();
    if (nndt == NULL)
        return (NULL);
    if (vk_gpu >= 0)
        nndt->set_vulkan_device(vk_gpu);
    return ((nanodet_ncnn_t)nndt);
}

/*
=======================================
    释放 NanoDet 识别器
=======================================
*/
CR_API void_t
imglab_ncnn_nanodet_del (
  __CR_IN__ nanodet_ncnn_t  nnet
    )
{
    ncnn::Net*  nndt;

    nndt = (ncnn::Net*)nnet;
    delete nndt;
}

/*
=======================================
    加载 NanoDet 识别器模型
=======================================
*/
CR_API bool_t
imglab_ncnn_nanodet_load (
  __CR_IN__ nanodet_ncnn_t  nnet,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ bool_t          bin_param,
  __CR_IN__ bool_t          use_vulkan,
  __CR_IN__ bool_t          use_bf16
    )
{
    ansi_t*     file;
    ncnn::Net*  nndt;

    file = str_fmtA("%s.param", name);
    if (file == NULL)
        return (FALSE);
    nndt = (ncnn::Net*)nnet;
    nndt->opt.use_vulkan_compute = !!use_vulkan;
    nndt->opt.use_bf16_storage = !!use_bf16;
    if (bin_param) {
        if (nndt->load_param_bin(file))
            goto _failure;
    }
    else {
        if (nndt->load_param(file))
            goto _failure;
    }
    sprintf(file, "%s.bin", name);
    if (nndt->load_model(file))
        goto _failure;
    mem_free(file);
    return (TRUE);

_failure:
    mem_free(file);
    return (FALSE);
}

/*
=======================================
    执行 NanoDet 的识别
=======================================
*/
CR_API sRECT_OBJECT*
imglab_ncnn_nanodet_doit (
  __CR_IN__ nanodet_ncnn_t              nnet,
  __CR_IN__ ximage_t                    mat,
  __CR_IN__ const sNCNN_NanoDetParam*   param,
  __CR_OT__ size_t*                     count
    )
{
    std::vector<Object> results;
    /* ---------------------- */
    cv::Mat*    mm = (cv::Mat*)mat;
    ncnn::Net*  nndt = (ncnn::Net*)nnet;

    detect_nanodet(nndt, *mm, results, param);

    sRECT_OBJECT*   rett;
    /* --------------- */
    size_t num = results.size();

    if (count != NULL)
        *count = num;
    if (num == 0) return (NULL);
    rett = mem_talloc(num, sRECT_OBJECT);
    if (rett != NULL) {
        for (size_t idx = 0; idx < num; idx++) {
            rect_set_wh(&rett[idx].rect, (sint_t)results[idx].rect.x,
                                         (sint_t)results[idx].rect.y,
                                         (sint_t)results[idx].rect.width,
                                         (sint_t)results[idx].rect.height);
            rett[idx].type = results[idx].label;
            rett[idx].prob = results[idx].prob;
        }
    }
    return (rett);
}

/*
=======================================
    执行 NanoDetPlus 的识别
=======================================
*/
CR_API sRECT_OBJECT*
imglab_ncnn_nanodet_plus_doit (
  __CR_IN__ nanodet_ncnn_t                  nnet,
  __CR_IN__ ximage_t                        mat,
  __CR_IN__ const sNCNN_NanoDetPlusParam*   param,
  __CR_OT__ size_t*                         count
    )
{
    std::vector<Object> results;
    /* ---------------------- */
    cv::Mat*    mm = (cv::Mat*)mat;
    ncnn::Net*  nndt = (ncnn::Net*)nnet;

    detect_nanodet_plus(nndt, *mm, results, param);

    sRECT_OBJECT*   rett;
    /* --------------- */
    size_t num = results.size();

    if (count != NULL)
        *count = num;
    if (num == 0) return (NULL);
    rett = mem_talloc(num, sRECT_OBJECT);
    if (rett != NULL) {
        for (size_t idx = 0; idx < num; idx++) {
            rect_set_wh(&rett[idx].rect, (sint_t)results[idx].rect.x,
                                         (sint_t)results[idx].rect.y,
                                         (sint_t)results[idx].rect.width,
                                         (sint_t)results[idx].rect.height);
            rett[idx].type = results[idx].label;
            rett[idx].prob = results[idx].prob;
        }
    }
    return (rett);
}

/*****************************************************************************/
/*                               MobileNetSSD                                */
/*****************************************************************************/

/*
=======================================
    创建 MobileNetSSD 识别器
=======================================
*/
CR_API mbntssd_ncnn_t
imglab_ncnn_mbntssd_new (
  __CR_IN__ sint_t  vk_gpu
    )
{
    ncnn::Net*  nndt;

    nndt = new(std::nothrow) ncnn::Net ();
    if (nndt == NULL)
        return (NULL);
    if (vk_gpu >= 0)
        nndt->set_vulkan_device(vk_gpu);
    return ((mbntssd_ncnn_t)nndt);
}

/*
=======================================
    释放 MobileNetSSD 识别器
=======================================
*/
CR_API void_t
imglab_ncnn_mbntssd_del (
  __CR_IN__ mbntssd_ncnn_t  nnet
    )
{
    ncnn::Net*  nndt;

    nndt = (ncnn::Net*)nnet;
    delete nndt;
}

/*
=======================================
    加载 MobileNetSSD 识别器模型
=======================================
*/
CR_API bool_t
imglab_ncnn_mbntssd_load (
  __CR_IN__ mbntssd_ncnn_t  nnet,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   silence,
  __CR_IN__ bool_t          bin_param,
  __CR_IN__ bool_t          use_vulkan,
  __CR_IN__ bool_t          use_bf16
    )
{
    ansi_t*     file;
    ncnn::Net*  nndt;

    file = str_fmtA("%s.param", name);
    if (file == NULL)
        return (FALSE);
    nndt = (ncnn::Net*)nnet;
    nndt->opt.use_vulkan_compute = !!use_vulkan;
    nndt->opt.use_bf16_storage = !!use_bf16;
    nndt->register_custom_layer(silence, Noop_layer_creator);
    if (bin_param) {
        if (nndt->load_param_bin(file))
            goto _failure;
    }
    else {
        if (nndt->load_param(file))
            goto _failure;
    }
    sprintf(file, "%s.bin", name);
    if (nndt->load_model(file))
        goto _failure;
    mem_free(file);
    return (TRUE);

_failure:
    mem_free(file);
    return (FALSE);
}

/*
=======================================
    执行 MobileNetSSD 的识别
=======================================
*/
CR_API sRECT_OBJECT*
imglab_ncnn_mbntssd_doit (
  __CR_IN__ mbntssd_ncnn_t                  nnet,
  __CR_IN__ ximage_t                        mat,
  __CR_IN__ const sNCNN_MobileNetSSD_Param* param,
  __CR_OT__ size_t*                         count
    )
{
    if (param->net_version < 1 || param->net_version > 3)
        return (NULL);

    std::vector<Object> results;
    /* ---------------------- */
    cv::Mat*    mm = (cv::Mat*)mat;
    ncnn::Net*  nndt = (ncnn::Net*)nnet;

    detect_mobilenetssd(nndt, *mm, results, param);

    sRECT_OBJECT*   rett;
    /* --------------- */
    size_t num = results.size();

    if (count != NULL)
        *count = num;
    if (num == 0) return (NULL);
    rett = mem_talloc(num, sRECT_OBJECT);
    if (rett != NULL) {
        for (size_t idx = 0; idx < num; idx++) {
            rect_set_wh(&rett[idx].rect, (sint_t)results[idx].rect.x,
                                         (sint_t)results[idx].rect.y,
                                         (sint_t)results[idx].rect.width,
                                         (sint_t)results[idx].rect.height);
            rett[idx].type = results[idx].label;
            rett[idx].prob = results[idx].prob;
        }
    }
    return (rett);
}

/*****************************************************************************/
/*                                   YOLO                                    */
/*****************************************************************************/

/*
=======================================
    创建 YOLO 识别器
=======================================
*/
CR_API yolo_ncnn_t
imglab_ncnn_yolo_new (
  __CR_IN__ sint_t  vk_gpu
    )
{
    ncnn::Net*  nndt;

    nndt = new(std::nothrow) ncnn::Net ();
    if (nndt == NULL)
        return (NULL);
    if (vk_gpu >= 0)
        nndt->set_vulkan_device(vk_gpu);
    return ((yolo_ncnn_t)nndt);
}

/*
=======================================
    释放 YOLO 识别器
=======================================
*/
CR_API void_t
imglab_ncnn_yolo_del (
  __CR_IN__ yolo_ncnn_t nnet
    )
{
    ncnn::Net*  nndt;

    nndt = (ncnn::Net*)nnet;
    delete nndt;
}

/*
=======================================
    加载 YOLO 识别器模型
=======================================
*/
CR_API bool_t
imglab_ncnn_yolo_load (
  __CR_IN__ yolo_ncnn_t     nnet,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   v5focus,
  __CR_IN__ bool_t          bin_param,
  __CR_IN__ bool_t          use_vulkan,
  __CR_IN__ bool_t          use_bf16
    )
{
    ansi_t*     file;
    ncnn::Net*  nndt;

    file = str_fmtA("%s.param", name);
    if (file == NULL)
        return (FALSE);
    nndt = (ncnn::Net*)nnet;
    nndt->opt.use_vulkan_compute = !!use_vulkan;
    nndt->opt.use_bf16_storage = !!use_bf16;
    nndt->register_custom_layer(v5focus, YoloV5Focus_layer_creator);
    if (bin_param) {
        if (nndt->load_param_bin(file))
            goto _failure;
    }
    else {
        if (nndt->load_param(file))
            goto _failure;
    }
    sprintf(file, "%s.bin", name);
    if (nndt->load_model(file))
        goto _failure;
    mem_free(file);
    return (TRUE);

_failure:
    mem_free(file);
    return (FALSE);
}

/*
=======================================
    执行 YOLO 的识别
=======================================
*/
CR_API sRECT_OBJECT*
imglab_ncnn_yolo_doit (
  __CR_IN__ yolo_ncnn_t             nnet,
  __CR_IN__ ximage_t                mat,
  __CR_IN__ const sNCNN_YOLO_Param* param,
  __CR_OT__ size_t*                 count
    )
{
    std::vector<Object> results;
    /* ---------------------- */
    cv::Mat*    mm = (cv::Mat*)mat;
    ncnn::Net*  nndt = (ncnn::Net*)nnet;

    if (param->yolo_version >= 2 && param->yolo_version <= 4) {
        detect_yolo_2_3_4(nndt, *mm, results, param);
    }
    else
    if (param->yolo_version == 5 || param->yolo_version == 500 ||
        param->yolo_version == 7 || param->yolo_version == 700 ||
        param->yolo_version == 560 || param->yolo_version == 562) {
        detect_yolo_5_7(nndt, *mm, results, param);
    }
    else {
        return (NULL);
    }

    sRECT_OBJECT*   rett;
    /* --------------- */
    size_t num = results.size();

    if (count != NULL)
        *count = num;
    if (num == 0) return (NULL);
    rett = mem_talloc(num, sRECT_OBJECT);
    if (rett != NULL) {
        for (size_t idx = 0; idx < num; idx++) {
            rect_set_wh(&rett[idx].rect, (sint_t)results[idx].rect.x,
                                         (sint_t)results[idx].rect.y,
                                         (sint_t)results[idx].rect.width,
                                         (sint_t)results[idx].rect.height);
            rett[idx].type = results[idx].label;
            rett[idx].prob = results[idx].prob;
        }
    }
    return (rett);
}
