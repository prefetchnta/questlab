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
#if defined(min)
    #undef  min
#endif
#if defined(max)
    #undef  max
#endif
#if defined(_CR_CC_MSC_)
    #pragma warning (disable: 4251)
    #pragma warning (disable: 4273)
#endif
#include "ncnn/net.h"

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

static inline float
intersection_area(const Object& a, const Object& b)
{
    cv::Rect_<float> inter = a.rect & b.rect;
    return inter.area();
}

static void
qsort_descent_inplace(std::vector<Object>& faceobjects, int left, int right)
{
    int i = left;
    int j = right;
    float p = faceobjects[(left + right) / 2].prob;

    while (i <= j)
    {
        while (faceobjects[i].prob > p)
            i++;

        while (faceobjects[j].prob < p)
            j--;

        if (i <= j)
        {
            // swap
            std::swap(faceobjects[i], faceobjects[j]);

            i++;
            j--;
        }
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            if (left < j) qsort_descent_inplace(faceobjects, left, j);
        }
        #pragma omp section
        {
            if (i < right) qsort_descent_inplace(faceobjects, i, right);
        }
    }
}

static void
qsort_descent_inplace(std::vector<Object>& faceobjects)
{
    if (faceobjects.empty())
        return;

    qsort_descent_inplace(faceobjects, 0, (int)(faceobjects.size() - 1));
}

static void
nms_sorted_bboxes(const std::vector<Object>& faceobjects,
                  std::vector<int>& picked, float nms_threshold,
                  bool agnostic = false)
{
    picked.clear();

    const int n = (int)faceobjects.size();

    std::vector<float> areas(n);
    for (int i = 0; i < n; i++)
    {
        areas[i] = faceobjects[i].rect.area();
    }

    for (int i = 0; i < n; i++)
    {
        const Object& a = faceobjects[i];

        int keep = 1;
        for (int j = 0; j < (int)picked.size(); j++)
        {
            const Object& b = faceobjects[picked[j]];

            if (!agnostic && a.label != b.label)
                continue;

            // intersection over union
            float inter_area = intersection_area(a, b);
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            // float IoU = inter_area / union_area
            if (inter_area / union_area > nms_threshold)
                keep = 0;
        }

        if (keep)
            picked.push_back(i);
    }
}

/*****************************************************************************/
/*                                 nanodet                                   */
/*****************************************************************************/

static void
generate_proposals_nanodet(const ncnn::Mat& cls_pred, const ncnn::Mat& dis_pred,
                           int stride, const ncnn::Mat& in_pad, float prob_threshold,
                           std::vector<Object>& objects)
{
    const int num_grid = cls_pred.h;

    int num_grid_x;
    int num_grid_y;
    if (in_pad.w > in_pad.h)
    {
        num_grid_x = in_pad.w / stride;
        num_grid_y = num_grid / num_grid_x;
    }
    else
    {
        num_grid_y = in_pad.h / stride;
        num_grid_x = num_grid / num_grid_y;
    }

    const int num_class = cls_pred.w;
    const int reg_max_1 = dis_pred.w / 4;

    for (int i = 0; i < num_grid_y; i++)
    {
        for (int j = 0; j < num_grid_x; j++)
        {
            const int idx = i * num_grid_x + j;

            const float* scores = cls_pred.row(idx);

            // find label with max score
            int label = -1;
            float score = -FLT_MAX;
            for (int k = 0; k < num_class; k++)
            {
                if (scores[k] > score)
                {
                    label = k;
                    score = scores[k];
                }
            }

            if (score >= prob_threshold)
            {
                ncnn::Mat bbox_pred(reg_max_1, 4, (void*)dis_pred.row(idx));
                {
                    ncnn::Layer* softmax = ncnn::create_layer("Softmax");

                    ncnn::ParamDict pd;
                    pd.set(0, 1); // axis
                    pd.set(1, 1);
                    softmax->load_param(pd);

                    ncnn::Option opt;
                    opt.num_threads = 1;
                    opt.use_packing_layout = false;

                    softmax->create_pipeline(opt);

                    softmax->forward_inplace(bbox_pred, opt);

                    softmax->destroy_pipeline(opt);

                    delete softmax;
                }

                float pred_ltrb[4];
                for (int k = 0; k < 4; k++)
                {
                    float dis = 0.f;
                    const float* dis_after_sm = bbox_pred.row(k);
                    for (int l = 0; l < reg_max_1; l++)
                    {
                        dis += l * dis_after_sm[l];
                    }

                    pred_ltrb[k] = dis * stride;
                }

                float pb_cx = (j + 0.5f) * stride;
                float pb_cy = (i + 0.5f) * stride;

                float x0 = pb_cx - pred_ltrb[0];
                float y0 = pb_cy - pred_ltrb[1];
                float x1 = pb_cx + pred_ltrb[2];
                float y1 = pb_cy + pred_ltrb[3];

                Object obj;
                obj.rect.x = x0;
                obj.rect.y = y0;
                obj.rect.width = x1 - x0;
                obj.rect.height = y1 - y0;
                obj.label = label;
                obj.prob = score;

                objects.push_back(obj);
            }
        }
    }
}

static int
detect_nanodet(ncnn::Net *nanodet, const cv::Mat& img,
               std::vector<Object>& objects, const sNCNN_NanoDetParam *param)
{
    int width = img.cols;
    int height = img.rows;

    // pad to multiple of 32
    int w = width;
    int h = height;
    float scale = 1.f;
    if (w > h)
    {
        scale = (float)param->target_size / w;
        w = param->target_size;
        h = (int)(h * scale);
    }
    else
    {
        scale = (float)param->target_size / h;
        h = param->target_size;
        w = (int)(w * scale);
    }

    ncnn::Mat in;
    int type = img.channels();

    if (type == 3)
    {
        in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_BGR,
                                           width, height, w, h);
    }
    else
    {
        cv::Mat bgr;

        if (type == 1)
            cv::cvtColor(img, bgr, cv::COLOR_GRAY2BGR);
        else
        if (type == 4)
            cv::cvtColor(img, bgr, cv::COLOR_RGBA2BGR);
        else
            return 0;
        in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR,
                                           width, height, w, h);
    }

    // pad to target_size rectangle
    int wpad = (w + 31) / 32 * 32 - w;
    int hpad = (h + 31) / 32 * 32 - h;
    ncnn::Mat in_pad;
    ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2,
                           wpad - wpad / 2, ncnn::BORDER_CONSTANT, 0.f);

    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in_pad.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = nanodet->create_extractor();

    if (param->thread_num > 0)
        ex.set_num_threads(param->thread_num);
    ex.set_light_mode(!!param->light_mode);
    ex.input(param->input_layer, in_pad);

    std::vector<Object> proposals;

    // stride 8
    {
        ncnn::Mat cls_pred;
        ncnn::Mat dis_pred;
        ex.extract(param->cls_pred8, cls_pred);
        ex.extract(param->dis_pred8, dis_pred);

        std::vector<Object> objects8;
        generate_proposals_nanodet(cls_pred, dis_pred, 8, in_pad,
                            param->prob_threshold, objects8);

        proposals.insert(proposals.end(), objects8.begin(), objects8.end());
    }

    // stride 16
    {
        ncnn::Mat cls_pred;
        ncnn::Mat dis_pred;
        ex.extract(param->cls_pred16, cls_pred);
        ex.extract(param->dis_pred16, dis_pred);

        std::vector<Object> objects16;
        generate_proposals_nanodet(cls_pred, dis_pred, 16, in_pad,
                            param->prob_threshold, objects16);

        proposals.insert(proposals.end(), objects16.begin(), objects16.end());
    }

    // stride 32
    {
        ncnn::Mat cls_pred;
        ncnn::Mat dis_pred;
        ex.extract(param->cls_pred32, cls_pred);
        ex.extract(param->dis_pred32, dis_pred);

        std::vector<Object> objects32;
        generate_proposals_nanodet(cls_pred, dis_pred, 32, in_pad,
                            param->prob_threshold, objects32);

        proposals.insert(proposals.end(), objects32.begin(), objects32.end());
    }

    // sort all proposals by score from highest to lowest
    qsort_descent_inplace(proposals);

    // apply nms with nms_threshold
    std::vector<int> picked;
    nms_sorted_bboxes(proposals, picked, param->nms_threshold);

    int count = (int)picked.size();

    objects.resize(count);
    for (int i = 0; i < count; i++)
    {
        objects[i] = proposals[picked[i]];

        // adjust offset to original unpadded
        float x0 = (objects[i].rect.x - (wpad / 2)) / scale;
        float y0 = (objects[i].rect.y - (hpad / 2)) / scale;
        float x1 = (objects[i].rect.x + objects[i].rect.width - (wpad / 2)) / scale;
        float y1 = (objects[i].rect.y + objects[i].rect.height - (hpad / 2)) / scale;

        // clip
        x0 = std::max(std::min(x0, (float)(width - 1)), 0.f);
        y0 = std::max(std::min(y0, (float)(height - 1)), 0.f);
        x1 = std::max(std::min(x1, (float)(width - 1)), 0.f);
        y1 = std::max(std::min(y1, (float)(height - 1)), 0.f);

        objects[i].rect.x = x0;
        objects[i].rect.y = y0;
        objects[i].rect.width = x1 - x0;
        objects[i].rect.height = y1 - y0;
    }

    return 0;
}

/*
=======================================
    创建 nanodet 识别器
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
    释放 nanodet 识别器
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
    加载 nanodet 识别器模型
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
    执行 nanodet 的识别
=======================================
*/
CR_API sIMGLAB_OBJECT*
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

    sIMGLAB_OBJECT* rett;
    /* --------------- */
    size_t num = results.size();

    if (count != NULL)
        *count = num;
    if (num == 0) return (NULL);
    rett = mem_talloc(num, sIMGLAB_OBJECT);
    if (rett != NULL) {
        for (size_t idx = 0; idx < num; idx++) {
            rect_set_wh(&rett[idx].rect, (sint_t)results[idx].rect.x,
                                         (sint_t)results[idx].rect.y,
                                         (sint_t)results[idx].rect.width,
                                         (sint_t)results[idx].rect.height);
            rett[idx].type = results[idx].label;
            rett[idx].prop = results[idx].prob;
        }
    }
    return (rett);
}
