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
 *             ##       CREATE: 2025-01-09
 *              #
 ================================================
        NCNN NanoDet 相关代码
 ================================================
 */

static void
generate_proposals_nanodet(const ncnn::Mat& cls_pred,
                           const ncnn::Mat& dis_pred,
                           int stride,
                           const ncnn::Mat& in_pad,
                           float prob_threshold,
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

static void
detect_nanodet(ncnn::Net *nanodet,
               const cv::Mat& img,
               std::vector<Object>& objects,
               const sNCNN_NanoDetParam *param)
{
    int width = img.cols;
    int height = img.rows;

    // pad to target_size rectangle
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
            cv::cvtColor(img, bgr, cv::COLOR_BGRA2BGR);
        else
            return;
        in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR,
                                           width, height, w, h);
    }

    // pad to multiple of 32
    int wpad = (w + 31) / 32 * 32 - w;
    int hpad = (h + 31) / 32 * 32 - h;
    ncnn::Mat in_pad;
    ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2,
                           wpad - wpad / 2, ncnn::BORDER_CONSTANT, 0.f);
    wpad /= 2;  hpad /= 2;
    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in_pad.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = nanodet->create_extractor();

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
        float x0 = (objects[i].rect.x - wpad) / scale;
        float y0 = (objects[i].rect.y - hpad) / scale;
        float x1 = (objects[i].rect.x + objects[i].rect.width - wpad) / scale;
        float y1 = (objects[i].rect.y + objects[i].rect.height - hpad) / scale;

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
}

static void
generate_proposals_nanodet_plus(const ncnn::Mat& pred,
                                int stride,
                                int num_class,
                                bool_t have_sigmoid,
                                float prob_threshold,
                                std::vector<Object>& objects)
{
    int num_grid_x = pred.w;
    int num_grid_y = pred.h;

    const int reg_max_1 = (pred.c - num_class) / 4;

    for (int i = 0; i < num_grid_y; i++)
    {
        for (int j = 0; j < num_grid_x; j++)
        {
            // find label with max score
            int label = -1;
            float score = -FLT_MAX;
            for (int k = 0; k < num_class; k++)
            {
                float s = pred.channel(k).row(i)[j];
                if (s > score)
                {
                    label = k;
                    score = s;
                }
            }

            if (!have_sigmoid)
                score = sigmoid(score);

            if (score >= prob_threshold)
            {
                ncnn::Mat bbox_pred(reg_max_1, 4);
                for (int k = 0; k < reg_max_1 * 4; k++)
                {
                    bbox_pred[k] = pred.channel(num_class + k).row(i)[j];
                }
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

                float pb_cx = (float)(j * stride);
                float pb_cy = (float)(i * stride);

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

static void
detect_nanodet_plus(ncnn::Net *nanodet,
                    const cv::Mat& img,
                    std::vector<Object>& objects,
                    const sNCNN_NanoDetPlusParam *param)
{
    int width = img.cols;
    int height = img.rows;

    // pad to target_size rectangle
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
            cv::cvtColor(img, bgr, cv::COLOR_BGRA2BGR);
        else
            return;
        in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR,
                                           width, height, w, h);
    }

    // pad to multiple of 32
    int wpad = (w + 31) / 32 * 32 - w;
    int hpad = (h + 31) / 32 * 32 - h;
    ncnn::Mat in_pad;
    ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2,
                           wpad - wpad / 2, ncnn::BORDER_CONSTANT, 0.f);
    wpad /= 2;  hpad /= 2;
    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in_pad.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = nanodet->create_extractor();

    ex.set_light_mode(!!param->light_mode);
    ex.input(param->input_layer, in_pad);

    std::vector<Object> proposals;

    // stride 8
    {
        ncnn::Mat pred;
        ex.extract(param->pred8, pred);

        std::vector<Object> objects8;
        generate_proposals_nanodet_plus(pred, 8, param->num_class,
                        param->have_sigmoid, param->prob_threshold, objects8);

        proposals.insert(proposals.end(), objects8.begin(), objects8.end());
    }

    // stride 16
    {
        ncnn::Mat pred;
        ex.extract(param->pred16, pred);

        std::vector<Object> objects16;
        generate_proposals_nanodet_plus(pred, 16, param->num_class,
                        param->have_sigmoid, param->prob_threshold, objects16);

        proposals.insert(proposals.end(), objects16.begin(), objects16.end());
    }

    // stride 32
    {
        ncnn::Mat pred;
        ex.extract(param->pred32, pred);

        std::vector<Object> objects32;
        generate_proposals_nanodet_plus(pred, 32, param->num_class,
                        param->have_sigmoid, param->prob_threshold, objects32);

        proposals.insert(proposals.end(), objects32.begin(), objects32.end());
    }

    // stride 64
    {
        ncnn::Mat pred;
        ex.extract(param->pred64, pred);

        std::vector<Object> objects64;
        generate_proposals_nanodet_plus(pred, 64, param->num_class,
                        param->have_sigmoid, param->prob_threshold, objects64);

        proposals.insert(proposals.end(), objects64.begin(), objects64.end());
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
        float x0 = (objects[i].rect.x - wpad) / scale;
        float y0 = (objects[i].rect.y - hpad) / scale;
        float x1 = (objects[i].rect.x + objects[i].rect.width - wpad) / scale;
        float y1 = (objects[i].rect.y + objects[i].rect.height - hpad) / scale;

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
}
