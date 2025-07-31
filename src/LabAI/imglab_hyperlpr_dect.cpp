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

#include "hyperlpr/v3/hyper_lpr_sdk.h"
#include "imglab_int.hpp"

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
  __CR_IN__ size_t*     count
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
