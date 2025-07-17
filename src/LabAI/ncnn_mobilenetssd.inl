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
detect_mobilenetssd(ncnn::Net *mobilenetssd, const cv::Mat& img,
                    std::vector<Object>& objects, const sNCNN_MobileNetSSD_Param *param)
{
    int width = img.cols;
    int height = img.rows;
    int target_size = param->target_size;
    int format = (param->net_version == 3) ? ncnn::Mat::PIXEL_BGR2RGB : ncnn::Mat::PIXEL_BGR;

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
            cv::cvtColor(img, bgr, cv::COLOR_BGRA2BGR);
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
