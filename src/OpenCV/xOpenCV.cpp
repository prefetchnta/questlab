
#include "xOpenCV.h"
#include "opencv2/opencv.hpp"
using namespace cv;

/* 外部库引用 */
#pragma comment (lib, "QstLibs.lib")
#pragma comment (lib, "libeasypr.lib")
#pragma comment (lib, "opencv_world300.lib")

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    插件 DLL 入口点
=======================================
*/
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
{
    static bool_t xcom = FALSE;

    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            xcom = com_init();
            break;

        case DLL_PROCESS_DETACH:
            if (xcom)
                com_kill();
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/*****************************************************************************/
/*                                结构体互换                                 */
/*****************************************************************************/

/*
=======================================
    释放 IplImage 图片
=======================================
*/
CR_API void_t
ilab_ipl_del (
  __CR_IN__ const ipls_t*   ipl
    )
{
    IplImage*   arr;

    arr = (IplImage*)ipl;
    cvReleaseImage(&arr);
}

/*
=======================================
    释放 sIMAGE 图片
=======================================
*/
CR_API void_t
ilab_img_del (
  __CR_IN__ const sIMAGE*   img
    )
{
    image_del(img);
}

/*
=======================================
    由 IplImage 设置 sIMAGE
=======================================
*/
CR_API bool_t
ilab_ipl2img_set (
  __CR_OT__ sIMAGE*         img,
  __CR_IN__ const ipls_t*   ipl
    )
{
    uint_t      fmt;
    uint_t      aln;
    bool_t      gdi;
    IplImage*   arr;

    /* 结构头检查 */
    arr = (IplImage*)ipl;
    if (arr->ID != 0 ||
        arr->nSize != sizeof(IplImage))
        return (FALSE);

    /* 只支持8位通道深度 */
    if (arr->depth != IPL_DEPTH_8S &&
        arr->depth != IPL_DEPTH_8U)
        return (FALSE);

    /* 像素通道格式过滤 */
    if (arr->dataOrder != IPL_DATA_ORDER_PIXEL)
        return (FALSE);

    /* 计算选择像素格式 */
    switch (arr->nChannels)
    {
        default: return (FALSE);
        case 1: fmt = CR_INDEX8; break;
        case 3: fmt = CR_ARGB888; break;
        case 4: fmt = CR_ARGB8888; break;
    }

    /* 是否上下颠倒 */
    if (arr->origin == IPL_ORIGIN_TL)
        gdi = FALSE;
    else
    if (arr->origin == IPL_ORIGIN_BL)
        gdi = TRUE;
    else
        return (FALSE);

    /* 对齐参数的过滤 */
    if (arr->align != IPL_ALIGN_4BYTES &&
        arr->align != IPL_ALIGN_8BYTES &&
        arr->align != IPL_ALIGN_16BYTES &&
        arr->align != IPL_ALIGN_32BYTES)
        return (FALSE);

    /* 不能相信它的对齐值 */
    if (arr->widthStep % 8 == 0)
        aln = 8;
    else
    if (arr->widthStep % 4 == 0)
        aln = 4;
    else
        aln = 0;

    /* 设置目标图片结构 */
    if (image_set(img, arr->imageData, arr->imageSize, 0, 0,
                  arr->width, arr->height, fmt, gdi, aln) == NULL)
        return (FALSE);

    /* 校验一下计算后的图片参数 */
    if (img->bpl  != (leng_t)arr->widthStep ||
        img->size != (leng_t)arr->imageSize)
        return (FALSE);

    /* 灰度图需要填充调色板 */
    if (fmt == CR_INDEX8)
        pal_set_gray8(img->pal, 256);
    return (TRUE);
}

/*
=======================================
    IplImage 复制到 sIMAGE
=======================================
*/
CR_API sIMAGE*
ilab_ipl2img_dup (
  __CR_IN__ const ipls_t*   ipl
    )
{
    sIMAGE  temp;

    /* 先填充到一个结构然后克隆 */
    if (!ilab_ipl2img_set(&temp, ipl))
        return (NULL);
    return (image_dup(&temp));
}

/*
=======================================
    由 sIMAGE 设置 IplImage
=======================================
*/
CR_API bool_t
ilab_img2ipl_set (
  __CR_OT__ ipls_t*         ipl,
  __CR_IN__ const sIMAGE*   img
    )
{
    sint_t      chn;
    sint_t      org;
    CvSize      rct;
    IplImage*   arr;

    /* OpenCV 只支持这两种对齐 */
    arr = (IplImage*)ipl;
    if (img->align != 4 && img->align != 8)
        return (FALSE);

    /* 计算图片通道数量 */
    switch (img->fmt)
    {
        default: return (FALSE);
        case CR_INDEX8: chn = 1; break;
        case CR_ARGB888: chn = 3; break;
        case CR_ARGB8888: chn = 4; break;
    }

    /* 是否上下颠倒 */
    if (img->gdi)
        org = IPL_ORIGIN_BL;
    else
        org = IPL_ORIGIN_TL;

    /* 填充 IplImage 结构头 */
    struct_zero(arr, IplImage);
    rct.width  = (int)img->position.ww;
    rct.height = (int)img->position.hh;
    cvInitImageHeader(arr, rct, IPL_DEPTH_8U, chn,
                      org, (int)img->align);

    /* 校验一下计算后的图片参数 */
    cvSetData((CvArr*)arr, img->data, (int)img->bpl);
    if (img->bpl  != (leng_t)arr->widthStep ||
        img->size != (leng_t)arr->imageSize)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    sIMAGE 复制到 IplImage
=======================================
*/
CR_API ipls_t*
ilab_img2ipl_dup (
  __CR_IN__ const sIMAGE*   img
    )
{
    IplImage    temp;

    /* 先填充到一个结构然后克隆 */
    if (!ilab_img2ipl_set(&temp, img))
        return (NULL);
    return (cvCloneImage(&temp));
}

/*****************************************************************************/
/*                                摄像头操作                                 */
/*****************************************************************************/

/*
=======================================
    获取摄像头对象
=======================================
*/
CR_API camera_t
ilab_camera_new (
  __CR_IN__ uint_t  id
    )
{
    CvCapture*  cap;

    cap = cvCaptureFromCAM((int)id);
    if (cap != NULL)
        cvSetCaptureProperty(cap, CV_CAP_PROP_CONVERT_RGB, 1.0);
    return ((camera_t)cap);
}

/*
=======================================
    获取摄像头对象 (通过名字)
=======================================
*/
CR_API camera_t
ilab_camera_new2 (
  __CR_IN__ const ansi_t*   name
    )
{
    uint_t          id, num;
    const ansi_t*   clist[8];

    num = dshow_cam_list(clist, 8);
    if (num == 0)
        return (NULL);
    for (id = 0; id < num; id++) {
        if (clist[id] == NULL)
            continue;
        if (strcmp(clist[id], name) == 0)
            break;
    }
    for (uint_t jj = 0; jj < num; jj++)
        TRY_FREE(clist[jj]);
    if (id >= num)
        return (NULL);
    return (ilab_camera_new(id));
}

/*
=======================================
    释放摄像头对象
=======================================
*/
CR_API void_t
ilab_camera_del (
  __CR_IN__ camera_t    cam
    )
{
    CvCapture*  cap;

    cap = (CvCapture*)cam;
    cvReleaseCapture(&cap);
}

/*
=======================================
    获取一帧图像
=======================================
*/
CR_API ipls_t*
ilab_camera_get (
  __CR_IN__ camera_t    cam
    )
{
    IplImage*   arr;

    arr = cvQueryFrame((CvCapture*)cam);
    if (arr == NULL)
        return (NULL);
    return (cvCloneImage(arr));
}

/*****************************************************************************/
/*                               图片文件加载                                */
/*****************************************************************************/

/*
=======================================
    从文件加载图片A
=======================================
*/
CR_API ipls_t*
ilab_load_fileA (
  __CR_IN__ const ansi_t*   name
    )
{
    /* 加载到彩色图片 */
    return (cvLoadImage(name, CV_LOAD_IMAGE_COLOR));
}

/*
=======================================
    从文件加载图片W
=======================================
*/
CR_API ipls_t*
ilab_load_fileW (
  __CR_IN__ const wide_t*   name
    )
{
    ipls_t* img;
    ansi_t* tmp;

    tmp = utf16_to_local(CR_LOCAL, name);
    if (tmp == NULL)
        return (NULL);
    img = cvLoadImage(tmp, CV_LOAD_IMAGE_COLOR);
    mem_free(tmp);
    return (img);
}

/*
=======================================
    从内存加载图片
=======================================
*/
CR_API ipls_t*
ilab_load_mem (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CvMat   mat;
    uint_t  cvt;

    if (cut_size(&cvt, size))
        return (NULL);
    mat = cvMat(1, (int)cvt, CV_8UC1, (void_t*)data);
    return (cvDecodeImage(&mat, CV_LOAD_IMAGE_COLOR));
}

/*****************************************************************************/
/*                               视频文件加载                                */
/*****************************************************************************/

/*
=======================================
    获取视频文件对象A
=======================================
*/
CR_API xvideo_t
ilab_video_newA (
  __CR_IN__ const ansi_t*   name
    )
{
    CvCapture*  cap;

    cap = cvCaptureFromFile(name);
    if (cap != NULL)
        cvSetCaptureProperty(cap, CV_CAP_PROP_CONVERT_RGB, 1.0);
    return ((xvideo_t)cap);
}

/*
=======================================
    获取视频文件对象W
=======================================
*/
CR_API xvideo_t
ilab_video_newW (
  __CR_IN__ const wide_t*   name
    )
{
    ansi_t*     tmp;
    CvCapture*  cap;

    tmp = utf16_to_local(CR_LOCAL, name);
    if (tmp == NULL)
        return (NULL);
    cap = cvCaptureFromFile(tmp);
    mem_free(tmp);
    if (cap != NULL)
        cvSetCaptureProperty(cap, CV_CAP_PROP_CONVERT_RGB, 1.0);
    return ((xvideo_t)cap);
}

/*
=======================================
    释放视频文件对象
=======================================
*/
CR_API void_t
ilab_video_del (
  __CR_IN__ xvideo_t    avi
    )
{
    CvCapture*  cap;

    cap = (CvCapture*)avi;
    cvReleaseCapture(&cap);
}

/*
=======================================
    获取一帧图像
=======================================
*/
CR_API ipls_t*
ilab_video_get (
  __CR_IN__ xvideo_t    avi
    )
{
    IplImage*   arr;

    arr = cvQueryFrame((CvCapture*)avi);
    if (arr == NULL)
        return (NULL);
    return (cvCloneImage(arr));
}

/*
=======================================
    返回视频的帧数 (为0表示无法获取)
=======================================
*/
CR_API int64u
ilab_video_count (
  __CR_IN__ xvideo_t    avi
    )
{
    int64s      bak;
    int64s      tmp;
    CvCapture*  cap;

    /* 获取实际的帧数 */
    cap = (CvCapture*)avi;
    tmp = (int64s)cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT);
    if (tmp == 0) return (0);
    bak = (int64s)cvGetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES);
    cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, (double)tmp);
    tmp = (int64s)cvGetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES);
    cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, (double)bak);
    return ((int64u)tmp);
}

/*
=======================================
    视频定位到指定帧
=======================================
*/
CR_API int64u
ilab_video_seek (
  __CR_IN__ xvideo_t    avi,
  __CR_IN__ int64u      frame
    )
{
    int64u      tmp;
    CvCapture*  cap;

    cap = (CvCapture*)avi;
    cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, (double)frame);
    tmp = (int64s)cvGetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES);
    for (; tmp < frame; tmp++) cvQueryFrame(cap);
    tmp = (int64s)cvGetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES);
    return ((int64u)tmp);
}

/*****************************************************************************/
/*                                图片过滤器                                 */
/*****************************************************************************/

/*
=======================================
    均值模糊处理
=======================================
*/
CR_API bool_t
image_ocv_blur_box (
  __CR_IO__ sIMAGE* image,
  __CR_IN__ uint_t  ksize_x,
  __CR_IN__ uint_t  ksize_y
    )
{
    IplImage    draw;

    if (!ilab_img2ipl_set(&draw, image))
        return (FALSE);

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
=======================================
    高斯模糊处理
=======================================
*/
CR_API bool_t
image_ocv_blur_gauss (
  __CR_IO__ sIMAGE* image,
  __CR_IN__ uint_t  ksize_x,
  __CR_IN__ uint_t  ksize_y,
  __CR_IN__ fp64_t  sigma_x,
  __CR_IN__ fp64_t  sigma_y
    )
{
    IplImage    draw;

    if (!ilab_img2ipl_set(&draw, image))
        return (FALSE);

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
=======================================
    中值模糊处理
=======================================
*/
CR_API bool_t
image_ocv_blur_median (
  __CR_IO__ sIMAGE* image,
  __CR_IN__ uint_t  ksize
    )
{
    IplImage    draw;

    if (!ilab_img2ipl_set(&draw, image))
        return (FALSE);

    Mat inpt = cvarrToMat(&draw, false);

    /* 卷积核大小必须为奇数 */
    if (ksize < 3)
        ksize = 3;
    else if (ksize % 2 == 0)
        ksize += 1;
    medianBlur(inpt, inpt, ksize);
    return (TRUE);
}
