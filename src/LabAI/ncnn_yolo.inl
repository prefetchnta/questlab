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

/*******************/
/* YOLOv2 - YOLOv4 */
/*******************/
static void
detect_yolo_2_3_4(ncnn::Net *yolo, const cv::Mat& img, std::vector<Object>& objects,
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
            cv::cvtColor(img, bgr, cv::COLOR_BGRA2BGR);
        else
            return;
        in = ncnn::Mat::from_pixels_resize(bgr.data, format, width, height,
                                           target_size, target_size);
    }

    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = yolo->create_extractor();

    ex.input(param->input_layer, in);

    ncnn::Mat out;
    ex.extract(param->output_layer8, out);

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

/*******************/
/* YOLOv5 / YOLOv7 */
/*******************/
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
generate_proposals_yolo_5_7(const ncnn::Mat& anchors,
                            int stride,
                            const ncnn::Mat& in_pad,
                            const ncnn::Mat& feat_blob,
                            float prob_threshold,
                            std::vector<Object>& objects)
{
    const int num_grid = feat_blob.h;

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

    const int num_class = feat_blob.w - 5;

    const int num_anchors = anchors.w / 2;

    for (int q = 0; q < num_anchors; q++)
    {
        const float anchor_w = anchors[q * 2];
        const float anchor_h = anchors[q * 2 + 1];

        const ncnn::Mat feat = feat_blob.channel(q);

        for (int i = 0; i < num_grid_y; i++)
        {
            for (int j = 0; j < num_grid_x; j++)
            {
                const float* featptr = feat.row(i * num_grid_x + j);
                float box_confidence = sigmoid(featptr[4]);
                if (box_confidence >= prob_threshold)
                {
                    // find class index with max class score
                    int class_index = 0;
                    float class_score = -FLT_MAX;
                    for (int k = 0; k < num_class; k++)
                    {
                        float score = featptr[5 + k];
                        if (score > class_score)
                        {
                            class_index = k;
                            class_score = score;
                        }
                    }
                    float confidence = box_confidence * sigmoid(class_score);
                    if (confidence >= prob_threshold)
                    {
                        float dx = sigmoid(featptr[0]);
                        float dy = sigmoid(featptr[1]);
                        float dw = sigmoid(featptr[2]);
                        float dh = sigmoid(featptr[3]);

                        float pb_cx = (dx * 2.f - 0.5f + j) * stride;
                        float pb_cy = (dy * 2.f - 0.5f + i) * stride;

                        float pb_w = (float)pow(dw * 2.f, 2) * anchor_w;
                        float pb_h = (float)pow(dh * 2.f, 2) * anchor_h;

                        float x0 = pb_cx - pb_w * 0.5f;
                        float y0 = pb_cy - pb_h * 0.5f;
                        float x1 = pb_cx + pb_w * 0.5f;
                        float y1 = pb_cy + pb_h * 0.5f;

                        Object obj;
                        obj.rect.x = x0;
                        obj.rect.y = y0;
                        obj.rect.width = x1 - x0;
                        obj.rect.height = y1 - y0;
                        obj.label = class_index;
                        obj.prob = confidence;

                        objects.push_back(obj);
                    }
                }
            }
        }
    }
}

static void
generate_proposals_yolo_5_7_pnnx(const ncnn::Mat& anchors,
                                 int stride,
                                 const ncnn::Mat& feat_blob,
                                 float prob_threshold,
                                 std::vector<Object>& objects)
{
    const int num_grid_x = feat_blob.w;
    const int num_grid_y = feat_blob.h;

    const int num_anchors = anchors.w / 2;

    const int num_class = feat_blob.c / num_anchors - 5;

    const int feat_offset = num_class + 5;

    for (int q = 0; q < num_anchors; q++)
    {
        const float anchor_w = anchors[q * 2];
        const float anchor_h = anchors[q * 2 + 1];

        for (int i = 0; i < num_grid_y; i++)
        {
            for (int j = 0; j < num_grid_x; j++)
            {
                // find class index with max class score
                int class_index = 0;
                float class_score = -FLT_MAX;
                for (int k = 0; k < num_class; k++)
                {
                    float score = feat_blob.channel(q * feat_offset + 5 + k).row(i)[j];
                    if (score > class_score)
                    {
                        class_index = k;
                        class_score = score;
                    }
                }

                float box_score = feat_blob.channel(q * feat_offset + 4).row(i)[j];

                float confidence = sigmoid(box_score) * sigmoid(class_score);

                if (confidence >= prob_threshold)
                {
                    float dx = sigmoid(feat_blob.channel(q * feat_offset + 0).row(i)[j]);
                    float dy = sigmoid(feat_blob.channel(q * feat_offset + 1).row(i)[j]);
                    float dw = sigmoid(feat_blob.channel(q * feat_offset + 2).row(i)[j]);
                    float dh = sigmoid(feat_blob.channel(q * feat_offset + 3).row(i)[j]);

                    float pb_cx = (dx * 2.f - 0.5f + j) * stride;
                    float pb_cy = (dy * 2.f - 0.5f + i) * stride;

                    float pb_w = (float)pow(dw * 2.f, 2) * anchor_w;
                    float pb_h = (float)pow(dh * 2.f, 2) * anchor_h;

                    float x0 = pb_cx - pb_w * 0.5f;
                    float y0 = pb_cy - pb_h * 0.5f;
                    float x1 = pb_cx + pb_w * 0.5f;
                    float y1 = pb_cy + pb_h * 0.5f;

                    Object obj;
                    obj.rect.x = x0;
                    obj.rect.y = y0;
                    obj.rect.width = x1 - x0;
                    obj.rect.height = y1 - y0;
                    obj.label = class_index;
                    obj.prob = confidence;

                    objects.push_back(obj);
                }
            }
        }
    }
}

static void
detect_yolo_5_7(ncnn::Net *yolo, const cv::Mat& img, std::vector<Object>& objects,
                const sNCNN_YOLO_Param *param)
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
        in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_BGR2RGB,
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
        in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR2RGB,
                                           width, height, w, h);
    }

    int max_stride = 32;
    bool is_pnnx = false;

    if (param->yolo_version == 500 || param->yolo_version == 700)
        is_pnnx = true;

    // letterbox pad to multiple of MAX_STRIDE
    int wpad = (w + max_stride - 1) / max_stride * max_stride - w;
    int hpad = (h + max_stride - 1) / max_stride * max_stride - h;
    ncnn::Mat in_pad;
    ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2,
                           wpad - wpad / 2, ncnn::BORDER_CONSTANT, 114.f);
    wpad /= 2;  hpad /= 2;
    if (param->mean_vals[0] >= 0 && param->norm_vals[0] >= 0)
        in_pad.substract_mean_normalize(param->mean_vals, param->norm_vals);

    ncnn::Extractor ex = yolo->create_extractor();

    ex.input(param->input_layer, in_pad);

    std::vector<Object> proposals;

    // stride 8
    {
        ncnn::Mat out;
        ex.extract(param->output_layer8, out);

        ncnn::Mat anchors(6);
        anchors[0] = param->anchors8[0];
        anchors[1] = param->anchors8[1];
        anchors[2] = param->anchors8[2];
        anchors[3] = param->anchors8[3];
        anchors[4] = param->anchors8[4];
        anchors[5] = param->anchors8[5];

        std::vector<Object> objects8;
        if (!is_pnnx)
            generate_proposals_yolo_5_7(anchors, 8, in_pad, out, param->prob_threshold, objects8);
        else
            generate_proposals_yolo_5_7_pnnx(anchors, 8, out, param->prob_threshold, objects8);

        proposals.insert(proposals.end(), objects8.begin(), objects8.end());
    }

    // stride 16
    {
        ncnn::Mat out;
        ex.extract(param->output_layer16, out);

        ncnn::Mat anchors(6);
        anchors[0] = param->anchors16[0];
        anchors[1] = param->anchors16[1];
        anchors[2] = param->anchors16[2];
        anchors[3] = param->anchors16[3];
        anchors[4] = param->anchors16[4];
        anchors[5] = param->anchors16[5];

        std::vector<Object> objects16;
        if (!is_pnnx)
            generate_proposals_yolo_5_7(anchors, 16, in_pad, out, param->prob_threshold, objects16);
        else
            generate_proposals_yolo_5_7_pnnx(anchors, 16, out, param->prob_threshold, objects16);

        proposals.insert(proposals.end(), objects16.begin(), objects16.end());
    }

    // stride 32
    {
        ncnn::Mat out;
        ex.extract(param->output_layer32, out);

        ncnn::Mat anchors(6);
        anchors[0] = param->anchors32[0];
        anchors[1] = param->anchors32[1];
        anchors[2] = param->anchors32[2];
        anchors[3] = param->anchors32[3];
        anchors[4] = param->anchors32[4];
        anchors[5] = param->anchors32[5];

        std::vector<Object> objects32;
        if (!is_pnnx)
            generate_proposals_yolo_5_7(anchors, 32, in_pad, out, param->prob_threshold, objects32);
        else
            generate_proposals_yolo_5_7_pnnx(anchors, 32, out, param->prob_threshold, objects32);

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
