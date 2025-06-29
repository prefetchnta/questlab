
#include "xOpenCV.h"
#include "../QstLibs/QstLibs.h"
#include "opencv2/opencv.hpp"
#include "libdecodeqr/decodeqr.h"
using namespace cv;
#pragma warning (disable: 4127)
#pragma warning (disable: 4505)
#pragma warning (disable: 4512)

#include "easypr.h"
using namespace easypr;

/*
---------------------------------------
    自适应二值化
---------------------------------------
*/
static bool_t
adaptive_binary (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  type, size;
    fp64_t  vmax, delta;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    vmax = xml_attr_fp64U("max", 255.0, param);
    delta = xml_attr_fp64U("delta", 10.0, param);
    type = xml_attr_intxU("type", CV_ADAPTIVE_THRESH_MEAN_C, param);
    size = xml_attr_intxU("size", 65, param);

    Mat outp, inpt = cvarrToMat(&draw, false);

    if (size < 3)
        size = 3;
    else if (size % 2 == 0)
        size += 1;
    if (type != CV_ADAPTIVE_THRESH_MEAN_C)
        type = ADAPTIVE_THRESH_GAUSSIAN_C;
    cvtColor(inpt, outp, CV_BGR2GRAY);
    adaptiveThreshold(outp, outp, vmax, type, THRESH_BINARY, size, delta);
    cvtColor(outp, inpt, CV_GRAY2BGRA);
    return (TRUE);
}

/*
---------------------------------------
    均值模糊处理
---------------------------------------
*/
static bool_t
box_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  ksize_x, ksize_y;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize_x = xml_attr_intxU("ksize_x", 9, param);
    ksize_y = xml_attr_intxU("ksize_y", 9, param);

    Mat inpt = cvarrToMat(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (ksize_x < 3)
        ksize_x = 3;
    else if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else if (ksize_y % 2 == 0)
        ksize_y += 1;
    blur(inpt, inpt, Size(ksize_x, ksize_y));
    return (TRUE);
}

/*
---------------------------------------
    高斯模糊处理
---------------------------------------
*/
static bool_t
gaussian_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    uint_t  ksize_x, ksize_y;
    fp64_t  sigma_x, sigma_y;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize_x = xml_attr_intxU("ksize_x", 9, param);
    ksize_y = xml_attr_intxU("ksize_y", 9, param);
    sigma_x = xml_attr_fp64U("sigma_x", 0.0, param);
    sigma_y = xml_attr_fp64U("sigma_y", 0.0, param);

    Mat inpt = cvarrToMat(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (ksize_x < 3)
        ksize_x = 3;
    else if (ksize_x % 2 == 0)
        ksize_x += 1;
    if (ksize_y < 3)
        ksize_y = 3;
    else if (ksize_y % 2 == 0)
        ksize_y += 1;
    GaussianBlur(inpt, inpt, Size(ksize_x, ksize_y), sigma_x, sigma_y);
    return (TRUE);
}

/*
---------------------------------------
    中值模糊处理
---------------------------------------
*/
static bool_t
median_blur (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    uint_t      size;
    sIMAGE*     dest;
    IplImage    draw;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    size = xml_attr_intxU("ksize", 5, param);

    Mat inpt = cvarrToMat(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (size < 3)
        size = 3;
    else if (size % 2 == 0)
        size += 1;
    medianBlur(inpt, inpt, size);
    return (TRUE);
}

/*
---------------------------------------
    Hough 变换圆搜索
---------------------------------------
*/
static bool_t
hough_circles (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    fp64_t  dp, min_dist;
    fp64_t  param1, param2;
    uint_t  ww, min_radius;
    uint_t  hh, max_radius;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ww = dest->position.ww;
    hh = dest->position.hh;
    if (ww > hh) ww = hh;
    dp = xml_attr_fp64U("dp", 2.0, param);
    min_dist = xml_attr_fp64U("min_dist", ww / 4, param);
    param1 = xml_attr_fp64U("param1", 200.0, param);
    param2 = xml_attr_fp64U("param2", 100.0, param);
    min_radius = xml_attr_intxU("min_radius", 0, param);
    max_radius = xml_attr_intxU("max_radius", ww / 2, param);

    size_t  idx, count;
    std::vector<Vec3f>  result;
    Mat outp, inpt = cvarrToMat(&draw, false);

    /* 变换需要8位单通道的图片 */
    cvtColor(inpt, outp, CV_BGR2GRAY);
    HoughCircles(outp, result, CV_HOUGH_GRADIENT, dp, min_dist,
                 param1, param2, min_radius, max_radius);
    count = result.size();
    for (idx = 0; idx < count; idx++)
    {
        Point   center(cvRound(result[idx][0]),
                       cvRound(result[idx][1]));
        sint_t  radius=cvRound(result[idx][2]);

        circle(inpt, center,      3, Scalar(0, 255, 0, 255), -1, 8, 0);
        circle(inpt, center, radius, Scalar(0, 0, 255, 255),  3, 8, 0);
    }
    return (TRUE);
}

/*
---------------------------------------
    Hough 变换线搜索
---------------------------------------
*/
static bool_t
hough_lines (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    fp64_t  rho, the;
    uint_t  ksize, thres;
    fp64_t  param1, param2;

    CR_NOUSE(netw);
    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    ksize = xml_attr_intxU("ksize", 3, param);
    param1 = xml_attr_fp64U("param1", 50.0, param);
    param2 = xml_attr_fp64U("param2", 200.0, param);
    rho = xml_attr_fp64U("rho", 1.0, param);
    the = xml_attr_fp64U("theta", CV_PI / 180, param);

    size_t  idx, count;
    Mat outp, inpt = cvarrToMat(&draw, false);

    /* 两种搜索直线的方法 */
    Canny(inpt, outp, param1, param2, ksize);
    if (str_cmpA(param->name, "opencv_hough_lines") == 0)
    {
        fp64_t  srn, stn;
        std::vector<Vec2f>  lines;

        srn = xml_attr_fp64U("srn", 0.0, param);
        stn = xml_attr_fp64U("stn", 0.0, param);
        thres = xml_attr_intxU("thres", 100, param);
        HoughLines(outp, lines, rho, the, thres, srn, stn);
        count = lines.size();
        for (idx = 0; idx < count; idx++)
        {
            fp64_t  aa, bb;
            fp64_t  x0, y0;

            rho = lines[idx][0];
            the = lines[idx][1];
            aa = cos(the);
            bb = sin(the);
            x0 = aa * rho;
            y0 = bb * rho;

            Point   pt1(cvRound(x0 + 1000 * (-bb)),
                        cvRound(y0 + 1000 * ( aa)));
            Point   pt2(cvRound(x0 - 1000 * (-bb)),
                        cvRound(y0 - 1000 * ( aa)));

            line(inpt, pt1, pt2, Scalar(0, 0, 255, 255), 3, 8);
        }
    }
    else
    {
        fp64_t  min_len, max_gap;
        std::vector<Vec4i>  lines;

        thres = xml_attr_intxU("thres", 80, param);
        min_len = xml_attr_fp64U("min_len", 30.0, param);
        max_gap = xml_attr_fp64U("max_gap", 10.0, param);
        HoughLinesP(outp, lines, rho, the, thres, min_len, max_gap);
        count = lines.size();
        for (idx = 0; idx < count; idx++) {
            line(inpt, Point(lines[idx][0], lines[idx][1]),
                       Point(lines[idx][2], lines[idx][3]),
                       Scalar(0, 0, 255, 255), 3, 8);
        }
    }
    return (TRUE);
}

/*
---------------------------------------
    QrCode 查找识别
---------------------------------------
*/
static bool_t
qrcode_decode (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;
    /* ----------- */
    sint_t  size, delta;

    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);
    size  = xml_attr_intxU("size", DEFAULT_ADAPTIVE_TH_SIZE, param);
    delta = xml_attr_intxU("delta", DEFAULT_ADAPTIVE_TH_DELTA, param);

    int16u          state = 0;
    QrDecoderHandle qrcode = qr_decoder_open();

    /* 两种方式二值化图片并识别二维码 */
    qr_decoder_set_image_buffer(qrcode, &draw);
    if (size <= 0) {
        state = qr_decoder_decode(qrcode, size, delta);
    }
    else {
        if (size < 3)
            size = 3;
        for (size |= 1; size >= 3; size -= 2) {
            state = qr_decoder_decode(qrcode, size, delta);
            if (state & QR_IMAGEREADER_DECODED)
                break;
        }
    }

    ansi_t*         str;
    ansi_t*         tmp;
    ansi_t*         hex;
    CvPoint         point;
    QrCodeHeader    header;

    /* 无法识别直接返回 */
    if (!qr_decoder_get_header(qrcode, &header))
        goto _func_out;

    /* 获取得到的文本字符串 */
    if (netw != NULL) {
        size = header.byte_size;
        tmp = str_allocA(size + 1);
        if (tmp == NULL)
            goto _func_out;
        qr_decoder_get_body(qrcode, (byte_t*)tmp, size);
        tmp[size] = NIL;
        hex = misc_str2hex(tmp);
        if (hex == NULL) {
            str = str_fmtA("|libdecodeqr| QRCode: %s", tmp);
        }
        else {
            str = str_fmtA("|libdecodeqr| QRCode: %s (%s)", tmp, hex);
            mem_free(hex);
        }
        mem_free(tmp);
        if (str != NULL) {
            netw_cmd_send((socket_t)netw, str);
            mem_free(str);
        }
    }

    CvBox2D*    boxes;
    CvPoint*    vertexes;

    /* 绘制定位到的二维码 */
    vertexes = qr_decoder_get_coderegion_vertexes(qrcode);
    point = vertexes[3];
    for (uint_t idx = 0; idx < 4; idx++) {
        cvLine(&draw, point, vertexes[idx], cvScalar(0, 255, 0, 255), 3, 8);
        point = vertexes[idx];
    }
    boxes = qr_decoder_get_finderpattern_boxes(qrcode);
    for (uint_t idx = 0; idx < 3; idx++) {
        CvSize  sz = cvSize((int)boxes[idx].size.width / 2,
                            (int)boxes[idx].size.height / 2);
        cvEllipse(&draw, cvPointFrom32f(boxes[idx].center), sz,
                  boxes[idx].angle, 0, 360, cvScalar(0, 255, 0, 255), 2, 8);
    }
_func_out:
    qr_decoder_close(qrcode);
    return (TRUE);
}

/*
---------------------------------------
    中文车牌查找识别
---------------------------------------
*/
static bool_t
carplate_find (
  __CR_IN__ void_t*     netw,
  __CR_IO__ void_t*     image,
  __CR_IN__ sXNODEu*    param
    )
{
    sIMAGE*     dest;
    IplImage    draw;

    dest = (sIMAGE*)image;
    if (dest->fmt != CR_ARGB8888)
        return (TRUE);
    if (!ilab_img2ipl_set(&draw, dest))
        return (TRUE);

    CPlateRecognize pr;
    uint_t count, type, result;
    std::vector<CPlate> plate_list;

    pr.LoadSVM("source/easypr/model/svm_hist.xml");
    pr.LoadANN("source/easypr/model/ann.xml");
    pr.LoadChineseANN("source/easypr/model/ann_chinese.xml");
    pr.LoadGrayChANN("source/easypr/model/annCh.xml");
    pr.LoadChineseMapping("source/easypr/model/province_mapping");

    count = xml_attr_intxU("count", 1, param);
    type = xml_attr_intxU("type", PR_DETECT_CMSER, param);

    pr.setDebug(false);
    pr.setLifemode(true);
    pr.setResultShow(false);
    pr.setMaxPlates(count);
    pr.setDetectType(type);

    Mat dst = cvarrToMat(&draw, true);
    Mat src = cvarrToMat(&draw, false);

    /* 防止程序爆掉 */
    try {
        result = pr.plateRecognize(dst, plate_list);
    }
    catch (...) {
        return (TRUE);
    }

    if (result == 0)
    {
        size_t  num;
        ansi_t* str;
        Point2f pnts[4];

        num = plate_list.size();
        for (size_t idx = 0; idx < num; idx++)
        {
            CPlate      plate = plate_list.at(idx);
            Color       rclrs = plate.getPlateColor();
            RotatedRect rrect = plate.getPlatePos();
            std::string rtext = plate.getPlateStr();

            rrect.points(pnts);
            for (int ii = 0; ii < 4; ii++) {
                line(src, pnts[ii], pnts[(ii + 1) % 4],
                     cvScalar(0, 255, 0, 255), 2, 8);
            }
            if (netw != NULL) {
                str = str_fmtA("|EasyPR| %s (%u)", rtext.c_str(), (int)rclrs);
                if (str != NULL) {
                    netw_cmd_send((socket_t)netw, str);
                    mem_free(str);
                }
            }
        }
    }
    return (TRUE);
}

/*
=======================================
    滤镜接口导出表
=======================================
*/
CR_API const sXC_PORT   qst_v2d_filter[] =
{
    { "opencv_auto_binary", adaptive_binary },
    { "opencv_box_blur", box_blur },
    { "opencv_gauss_blur", gaussian_blur },
    { "opencv_median_blur", median_blur },
    { "opencv_hough_circles", hough_circles },
    { "opencv_hough_lines", hough_lines },
    { "opencv_hough_linesp", hough_lines },
    { "opencv_qrcode_decode", qrcode_decode },
    { "opencv_carplate_find", carplate_find },
    { NULL, NULL },
};
