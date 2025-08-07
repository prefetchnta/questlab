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
 *             ##       CREATE: 2025-07-29
 *              #
 ================================================
        ImageLAB HyperLPR 车牌识别函数库
 ================================================
 */

#include "imglab_int.hpp"

/*****************************************************************************/
/*                                HyperLPRv1                                 */
/*****************************************************************************/

#include "hyperlpr/v1/include/Pipeline.h"

/*
=======================================
    创建 HyperLPRv1 车牌识别器
=======================================
*/
CR_API hyperlpr1_t
imglab_hyperlpr1_new (
  __CR_IN__ const ansi_t*   path
    )
{
    pr1::PipelinePR*    xlpr;

    try {
        std::string root = path;
        std::string detector_filename = root + "/cascade.xml";
        std::string finemapping_prototxt = root + "/HorizonalFinemapping.prototxt";
        std::string finemapping_caffemodel = root + "/HorizonalFinemapping.caffemodel";
        std::string segmentation_prototxt = root + "/Segmentation.prototxt";
        std::string segmentation_caffemodel = root + "/Segmentation.caffemodel";
        std::string charRecognization_proto = root + "/CharacterRecognization.prototxt";
        std::string charRecognization_caffemodel = root + "/CharacterRecognization.caffemodel";
        std::string segmentationfree_proto = root + "/SegmenationFree-Inception.prototxt";
        std::string segmentationfree_caffemodel = root + "/SegmenationFree-Inception.caffemodel";

        xlpr = new pr1::PipelinePR (detector_filename,
                                    finemapping_prototxt, finemapping_caffemodel,
                                    segmentation_prototxt, segmentation_caffemodel,
                                    charRecognization_proto, charRecognization_caffemodel,
                                    segmentationfree_proto, segmentationfree_caffemodel);
    }
    catch (...) {
        return (NULL);
    }
    return ((hyperlpr1_t)xlpr);
}

/*
=======================================
    释放 HyperLPRv1 车牌识别器
=======================================
*/
CR_API void_t
imglab_hyperlpr1_del (
  __CR_IN__ hyperlpr1_t hlpr
    )
{
    pr1::PipelinePR*    xlpr;

    xlpr = (pr1::PipelinePR*)hlpr;
    delete xlpr;
}

/*
=======================================
    执行 HyperLPRv1 车牌识别
=======================================
*/
CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr1_doit (
  __CR_IN__ hyperlpr1_t hlpr,
  __CR_IN__ ximage_t    mat,
  __CR_IN__ uint_t      method,
  __CR_IN__ fp32_t      gate,
  __CR_OT__ size_t*     count
    )
{
    sint_t              type;
    pr1::PipelinePR*    xlpr;

    if (count != NULL)
        *count = 0;
    xlpr = (pr1::PipelinePR*)hlpr;

    cv::Mat*    mm = (cv::Mat*)mat;
    /* ------------------------- */
    std::vector<pr1::PlateInfo> res;

    type = mm->channels();
    if (type == 3)
    {
        res = xlpr->RunPiplineAsImage(*mm, method);
    }
    else
    {
        cv::Mat bgr;

        if (type == 4)
            cv::cvtColor(*mm, bgr, cv::COLOR_BGRA2BGR);
        else
        if (type == 1)
            cv::cvtColor(*mm, bgr, cv::COLOR_GRAY2BGR);
        else
            return (NULL);
        res = xlpr->RunPiplineAsImage(bgr, method);
    }

    size_t  nreal, num = res.size();

    /* 返回识别结果 */
    if (num == 0)
        return (NULL);
    nreal = 0;
    for (size_t idx = 0; idx < num; idx++) {
        if (res[idx].confidence > gate)
            nreal++;
    }

    sRECT_OBJECT_DESC*  back;

    if (count != NULL)
        *count = nreal;
    back = mem_talloc(nreal, sRECT_OBJECT_DESC);
    if (back == NULL)
        return (NULL);
    nreal = 0;
    for (size_t idx = 0; idx < num; idx++) {
        if (res[idx].confidence > gate)
        {
            cv::Rect    region = res[idx].getPlateRect();

            rect_set_wh(&back[nreal].rect, region.x, region.y,
                             region.width, region.height);
            back[nreal].type = 0;   /* 不支持车牌类型 */
            back[nreal].prob = res[idx].confidence;
            str_cpyA(back[nreal++].desc, res[idx].getPlateName().c_str());
        }
    }
    return (back);
}

/*****************************************************************************/
/*                                HyperLPRv2                                 */
/*****************************************************************************/

#include "hyperlpr/v2/include/Pipeline.h"

/*
=======================================
    创建 HyperLPRv2 车牌识别器
=======================================
*/
CR_API hyperlpr2_t
imglab_hyperlpr2_new (
  __CR_IN__ const ansi_t*   path
    )
{
    pr2::PipelinePR*    xlpr;

    try {
        std::string root = path;
        std::string detect_prototxt = root + "/mininet_ssd_v1.prototxt";
        std::string detect_caffemodel = root + "/mininet_ssd_v1.caffemodel";
        std::string finetune_prototxt = root + "/refinenet.prototxt";
        std::string finetune_caffemodel = root + "/refinenet.caffemodel";
        std::string platerec_prototxt = root + "/SegmenationFree-Inception.prototxt";
        std::string platerec_caffemodel = root + "/SegmenationFree-Inception.caffemodel";
        std::string dbstring = root + "/cascade_double.xml";

        xlpr = new pr2::PipelinePR (detect_prototxt, detect_caffemodel,
                                    finetune_prototxt, finetune_caffemodel,
                                    platerec_prototxt, platerec_caffemodel,
                                    dbstring);
    }
    catch (...) {
        return (NULL);
    }
    return ((hyperlpr2_t)xlpr);
}

/*
=======================================
    释放 HyperLPRv2 车牌识别器
=======================================
*/
CR_API void_t
imglab_hyperlpr2_del (
  __CR_IN__ hyperlpr2_t hlpr
    )
{
    pr2::PipelinePR*    xlpr;

    xlpr = (pr2::PipelinePR*)hlpr;
    delete xlpr;
}

/*
=======================================
    执行 HyperLPRv2 车牌识别
=======================================
*/
CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr2_doit (
  __CR_IN__ hyperlpr2_t hlpr,
  __CR_IN__ ximage_t    mat,
  __CR_IN__ bool_t      is_db,
  __CR_IN__ fp32_t      gate,
  __CR_OT__ size_t*     count
    )
{
    sint_t              type;
    pr2::PipelinePR*    xlpr;

    if (count != NULL)
        *count = 0;
    xlpr = (pr2::PipelinePR*)hlpr;

    cv::Mat*    mm = (cv::Mat*)mat;
    /* ------------------------- */
    std::vector<pr2::PlateInfo> res;

    type = mm->channels();
    if (type == 3)
    {
        res = xlpr->RunPiplineAsImage(*mm, is_db);
    }
    else
    {
        cv::Mat bgr;

        if (type == 4)
            cv::cvtColor(*mm, bgr, cv::COLOR_BGRA2BGR);
        else
        if (type == 1)
            cv::cvtColor(*mm, bgr, cv::COLOR_GRAY2BGR);
        else
            return (NULL);
        res = xlpr->RunPiplineAsImage(bgr, is_db);
    }

    size_t  nreal, num = res.size();

    /* 返回识别结果 */
    if (num == 0)
        return (NULL);
    nreal = 0;
    for (size_t idx = 0; idx < num; idx++) {
        if (res[idx].confidence > gate)
            nreal++;
    }

    sRECT_OBJECT_DESC*  back;

    if (count != NULL)
        *count = nreal;
    back = mem_talloc(nreal, sRECT_OBJECT_DESC);
    if (back == NULL)
        return (NULL);
    nreal = 0;
    for (size_t idx = 0; idx < num; idx++) {
        if (res[idx].confidence > gate)
        {
            cv::Rect    region = res[idx].getPlateRect();

            rect_set_wh(&back[nreal].rect, region.x, region.y,
                             region.width, region.height);
            back[nreal].type = 0;   /* 不支持车牌类型 */
            back[nreal].prob = res[idx].confidence;
            str_cpyA(back[nreal++].desc, res[idx].getPlateName().c_str());
        }
    }
    return (back);
}

/*****************************************************************************/
/*                                HyperLPRv3                                 */
/*****************************************************************************/

#include "hyperlpr/v3/hyper_lpr_sdk.h"

/*
=======================================
    创建 HyperLPRv3 车牌识别器
=======================================
*/
CR_API hyperlpr3_t
imglab_hyperlpr3_new (
  __CR_IN__ const ansi_t*       path,
  __CR_IN__ const sHLPR3_Param* param
    )
{
    P_HLPR_Context              xlpr;
    HLPR_ContextConfiguration   parm;

    parm.models_path = (char*)path;
    parm.max_num = param->max_num;
    parm.threads = param->threads;
    parm.use_half = !!param->use_half;
    parm.box_conf_threshold = param->box_conf_threshold;
    parm.nms_threshold = param->nms_threshold;
    parm.rec_confidence_threshold = param->rec_confidence_threshold;
    parm.det_level = (param->det_level == 0) ? DETECT_LEVEL_LOW : DETECT_LEVEL_HIGH;
    try {
        xlpr = HLPR_CreateContext(&parm);
        if (HLPR_ContextQueryStatus(xlpr) != 0) {
            HLPR_ReleaseContext(xlpr);
            return (NULL);
        }
    }
    catch (...) {
        return (NULL);
    }
    return ((hyperlpr3_t)xlpr);
}

/*
=======================================
    释放 HyperLPRv3 车牌识别器
=======================================
*/
CR_API void_t
imglab_hyperlpr3_del (
  __CR_IN__ hyperlpr3_t hlpr
    )
{
    HLPR_ReleaseContext((P_HLPR_Context)hlpr);
}

/*
=======================================
    执行 HyperLPRv3 车牌识别
=======================================
*/
CR_API sRECT_OBJECT_DESC*
imglab_hyperlpr3_doit (
  __CR_IN__ hyperlpr3_t hlpr,
  __CR_IN__ ximage_t    mat,
  __CR_OT__ size_t*     count
    )
{
    HLPR_ImageData  image;
    /* ---------------- */
    cv::Mat bgr, *mm = (cv::Mat*)mat;

    if (count != NULL)
        *count = 0;
    image.data = mm->data;
    image.width = mm->cols;
    image.height = mm->rows;
    image.rotation = CAMERA_ROTATION_0;

    sint_t  type = mm->channels();

    if (type == 3) {
        image.format = STREAM_BGR;
    }
    else
    if (type == 4) {
        image.format = STREAM_BGRA;
    }
    else
    if (type == 1) {
        cv::cvtColor(*mm, bgr, cv::COLOR_GRAY2BGR);
        image.data = bgr.data;
    }
    else {
        return (NULL);
    }

    P_HLPR_DataBuffer   buffer;

    try {
        buffer = HLPR_CreateDataBuffer(&image);
    }
    catch (...) {
        return (NULL);
    }

    sRECT_OBJECT_DESC*      back;
    HLPR_PlateResultList    list;

    if (HLPR_ContextUpdateStream((P_HLPR_Context)hlpr, buffer, &list) == 0)
    {
        size_t  num = list.plate_size;

        /* 返回识别结果 */
        if (count != NULL)
            *count = num;
        if (num != 0) {
            back = mem_talloc(num, sRECT_OBJECT_DESC);
            if (back != NULL) {
                for (size_t idx = 0; idx < num; idx++) {
                    rect_set_xy(&back[idx].rect, (int)list.plates[idx].x1,
                                                 (int)list.plates[idx].y1,
                                                 (int)list.plates[idx].x2,
                                                 (int)list.plates[idx].y2);
                    back[idx].type = (int)list.plates[idx].type + 1;
                    back[idx].prob = list.plates[idx].text_confidence;
                    mem_cpy(back[idx].desc, list.plates[idx].code, 128);
                }
            }
        }
        else {
            back = NULL;
        }
    }
    else {
        back = NULL;
    }
    HLPR_ReleaseDataBuffer(buffer);
    return (back);
}
