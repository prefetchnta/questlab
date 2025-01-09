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
        NCNN MobileNetSSD 相关代码
 ================================================
 */

static void
detect_mobilenetssd(ncnn::Net *mobilenetssd, const cv::Mat& img, bool is_v3,
                    std::vector<Object>& objects, const sNCNN_MobileNetSSD_Param *param)
{
    int width = img.cols;
    int height = img.rows;
    int target_size = param->target_size;
    int format = is_v3 ? ncnn::Mat::PIXEL_BGR2RGB : ncnn::Mat::PIXEL_BGR;

    ncnn::Mat in;
    int type = img.channels();

    if (type == 3)
    {
        in = ncnn::Mat::from_pixels_resize(img.data, format, width, height,
                                           target_size, target_size);
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
            return;
        in = ncnn::Mat::from_pixels_resize(bgr.data, format, width, height,
                                           target_size, target_size);
    }

    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = mobilenetssd->create_extractor();

    if (param->thread_num > 0)
        ex.set_num_threads(param->thread_num);
    ex.set_light_mode(!!param->light_mode);
    ex.input(param->input_layer, in);

    ncnn::Mat out;
    ex.extract(param->output_layer, out);

    if (is_v3)
    {
        for (int i = 0; i < out.h; i++)
        {
            const float* values = out.row(i);

            if (values[1] < param->prob_threshold)
                continue;

            Object object;
            object.label = (int)values[0];
            object.prob = values[1];

            // filter out cross-boundary
            float x1 = clamp(values[2] * target_size, 0.f, float(target_size - 1)) / target_size * width;
            float y1 = clamp(values[3] * target_size, 0.f, float(target_size - 1)) / target_size * height;
            float x2 = clamp(values[4] * target_size, 0.f, float(target_size - 1)) / target_size * width;
            float y2 = clamp(values[5] * target_size, 0.f, float(target_size - 1)) / target_size * height;

            object.rect.x = x1;
            object.rect.y = y1;
            object.rect.width = x2 - x1;
            object.rect.height = y2 - y1;

            objects.push_back(object);
        }
    }
    else
    {
        for (int i = 0; i < out.h; i++)
        {
            const float* values = out.row(i);

            if (values[1] < param->prob_threshold)
                continue;

            Object object;
            object.label = (int)values[0];
            object.prob = values[1];
            object.rect.x = values[2] * width;
            object.rect.y = values[3] * height;
            object.rect.width = values[4] * width - object.rect.x;
            object.rect.height = values[5] * height - object.rect.y;

            objects.push_back(object);
        }
    }
}
