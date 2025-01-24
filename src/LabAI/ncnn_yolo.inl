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
 *             ##       CREATE: 2025-01-22
 *              #
 ================================================
        NCNN YOLO 相关代码
 ================================================
 */

class YoloV5Focus : public ncnn::Layer
{
public:
    YoloV5Focus()
    {
        one_blob_only = true;
    }

    virtual int forward(const ncnn::Mat& bottom_blob, ncnn::Mat& top_blob, const ncnn::Option& opt) const
    {
        int w = bottom_blob.w;
        int h = bottom_blob.h;
        int channels = bottom_blob.c;

        int outw = w / 2;
        int outh = h / 2;
        int outc = channels * 4;

        top_blob.create(outw, outh, outc, 4u, 1, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int p = 0; p < outc; p++)
        {
            const float* ptr = bottom_blob.channel(p % channels).row((p / channels) % 2) + ((p / channels) / 2);
            float* outptr = top_blob.channel(p);

            for (int i = 0; i < outh; i++)
            {
                for (int j = 0; j < outw; j++)
                {
                    *outptr = *ptr;

                    outptr += 1;
                    ptr += 2;
                }

                ptr += w;
            }
        }

        return 0;
    }
};

DEFINE_LAYER_CREATOR(YoloV5Focus)

static void
detect_yolo(ncnn::Net *yolo, const cv::Mat& img, std::vector<Object>& objects,
            const sNCNN_YOLO_Param *param)
{
    int width = img.cols;
    int height = img.rows;
    int target_size = param->target_size;
    int format = (param->yolo_version == 4) ? ncnn::Mat::PIXEL_BGR2RGB : ncnn::Mat::PIXEL_BGR;

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

    if (param->yolo_version == 2) {
        if (param->norm_vals[0] >= 0)
            in.substract_mean_normalize(0, param->norm_vals);
        if (param->mean_vals[0] >= 0)
            in.substract_mean_normalize(param->mean_vals, 0);
    }
    else {
        if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
            in.substract_mean_normalize(param->mean_vals, param->norm_vals);
    }

    ncnn::Extractor ex = yolo->create_extractor();

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
