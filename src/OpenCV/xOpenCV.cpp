
#include "xOpenCV.h"
#include "opencv2/opencv.hpp"
using namespace cv;

/* 外部库引用 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "opencv_calib3d247.lib")
    #pragma comment (lib, "opencv_contrib247.lib")
    #pragma comment (lib, "opencv_core247.lib")
    #pragma comment (lib, "opencv_features2d247.lib")
    #pragma comment (lib, "opencv_flann247.lib")
    #pragma comment (lib, "opencv_gpu247.lib")
    #pragma comment (lib, "opencv_highgui247.lib")
    #pragma comment (lib, "opencv_imgproc247.lib")
    #pragma comment (lib, "opencv_legacy247.lib")
    #pragma comment (lib, "opencv_ml247.lib")
    #pragma comment (lib, "opencv_nonfree247.lib")
    #pragma comment (lib, "opencv_objdetect247.lib")
    #pragma comment (lib, "opencv_ocl247.lib")
    #pragma comment (lib, "opencv_photo247.lib")
    #pragma comment (lib, "opencv_stitching247.lib")
    #pragma comment (lib, "opencv_superres247.lib")
    #pragma comment (lib, "opencv_ts247.lib")
    #pragma comment (lib, "opencv_video247.lib")
    #pragma comment (lib, "opencv_videostab247.lib")
#endif

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
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
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
    OpenCV 获取摄像头对象
=======================================
*/
CR_API camera_t
ilab_camera_new (
  __CR_IN__ uint_t  id
    )
{
    return ((camera_t)cvCaptureFromCAM((int)id));
}

/*
=======================================
    OpenCV 释放摄像头对象
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
    OpenCV 获取一帧图像
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
