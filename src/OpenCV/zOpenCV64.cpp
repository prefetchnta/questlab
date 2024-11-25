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
 *             ##       CREATE: 2021-05-20
 *              #
 ================================================
    【64】基于 OpenCV 的 QV2D z 扩展服务
 ================================================
 */

#define _QUEST64_
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "opencv2/opencv.hpp"
#include "../QstLibs/QstLibs.h"
#include "../CrHackSet/xCrHackSet.h"

/* 一些公用宏 */
#define EXE_XNAME   "zOpenCV64"

/* 外部库引用 */
#pragma comment (lib, "opencv_world4100.lib")

/* 日志函数 */
#include "../logit.inl"

/* 桥接函数 */
#include "../bridge.inl"

/* 工作变量 */
static int32u   s_size;
static share_t  s_smem;

/* 简化代码 */
#define IFSBOCV64   "Image file supported by OpenCV64"
#define VFSBOCV64   "Video file supported by OpenCV64"
#define CFCBOCV64   "Image captured from camera by OpenCV64"

#define OCV_REMOTE_CAMERA       ":computer/x64camera"
#define opencv_return_false()   message_send_buffer(mess, "", 1)

/*
---------------------------------------
    返回图片数据
---------------------------------------
*/
static void_t
opencv_send_image (
  __CR_IN__ socket_t        mess,
  __CR_IN__ const cv::Mat&  image
    )
{
    int32u          nbpl;
    byte_t*         data;
    sPIC_RMT_INFO   info;

    /* 填充图片结构体 */
    struct_zero(&info, sPIC_RMT_INFO);
    info.bpp = 24;
    info.fmt = CR_PIC_ARGB;
    mem_cpy(info.clr, "BGR", 3);
    mem_set(info.wnh,  0x08, 3);
    info.img_ww = (int32u)image.cols;
    info.img_hh = (int32u)image.rows;
    info.dat_sz  = info.img_ww;
    info.dat_sz *= info.img_hh;
    info.dat_sz *= sizeof(int32u);

    /* 填充图片数据 */
    if (s_size < info.dat_sz) {
        if (s_smem != NULL)
            share_file_close(s_smem);
        s_smem = share_file_open(CRH_RMT_IMG_DATA, NULL, info.dat_sz);
        if (s_smem == NULL) {
            opencv_return_false();
            s_size = 0;
            return;
        }
        s_size = info.dat_sz;
    }
    data = (byte_t*)share_file_map(s_smem, info.dat_sz);
    if (data == NULL) {
        opencv_return_false();
        share_file_close(s_smem);
        s_smem = NULL;
        s_size = 0;
        return;
    }
    nbpl = info.img_ww * sizeof(int32u);
    for (int32u yy = 0; yy < info.img_hh; yy++) {
        mem_cpy(data, image.ptr(yy), nbpl);
        data += nbpl;
    }
    share_file_unmap(s_smem);

    /* 发送图片信息 */
    message_send_buffer(mess, &info, sizeof(info));
}

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI
WinMain (
  __CR_IN__ HINSTANCE   curt_app,
  __CR_IN__ HINSTANCE   prev_app,
  __CR_IN__ LPSTR       cmd_line,
  __CR_IN__ int         cmd_show
    )
{
    uint_t      back;
    leng_t      leng;
    int32u      crrt;
    void_t*     data;
    socket_t    mess;

    CR_NOUSE(curt_app);
    CR_NOUSE(prev_app);
    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);

    /* 只允许一个例程 */
    if (misc_is_running(EXE_XNAME))
        return (QST_ERROR);

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_GUI))
        return (QST_ERROR);
    message_pipe_init();
    s_size = 0;
    s_smem = NULL;

    /* 创建工作端口 */
    mess = message_recv_open(CRH_REMOTE_IMG_PORT);
    if (mess == NULL)
        return (QST_ERROR);
    message_pipe_timeout(mess, QST_TCP_TOUT);

    ansi_t name[260];
    cv::Mat* single;
    sPIC_RMT_HEAD imghdr;
    cv::VideoCapture* frames;
    std::vector<cv::Mat> images;

    crrt = 0;
    single = NULL;
    frames = NULL;
    images.clear();
    struct_zero(&imghdr, sPIC_RMT_HEAD);

    /* 开始工作 */
    for (;;)
    {
        /* 接收数据包 */
        back = message_recv_buffer(mess, name, sizeof(name));
        if (back == sizeof(name))
        {
            /* 释放上次 */
            images.clear();
            SAFE_PDEL(single);
            SAFE_PDEL(frames);
            struct_zero(&imghdr, sPIC_RMT_HEAD);
            crrt = 0;

            /* 处理文件加载 */
            leng = str_lenA(":" CRH_LCL_IMG_FILE);
            if (chr_cmpA(name, ":" CRH_LCL_IMG_FILE, leng) == 0)
            {
                /* 内存文件 */
                leng = str2intA(&name[leng]);
                data = share_file_get(CRH_LCL_IMG_FILE, leng);
                if (data == NULL) {
                    opencv_return_false();
                    continue;
                }

                cv::Mat buf = cv::Mat(1, (int)leng, CV_8UC1, data);
                cv::Mat img = cv::imdecode(buf, cv::IMREAD_COLOR);

                mem_free(data);

                /* 是否加载成功 */
                if (img.data == NULL) {
                    opencv_return_false();
                    continue;
                }

                single = new cv::Mat (img.rows, img.cols, CV_8UC4);

                /* 保存返回的对象 */
                if (single == NULL) {
                    opencv_return_false();
                    continue;
                }
                cv::cvtColor(img, *single, cv::COLOR_BGR2BGRA);
                str_cpyA(imghdr.info, IFSBOCV64);
                imghdr.count = 1;
            }
            else
            {
                leng = str_lenA(OCV_REMOTE_CAMERA);
                if (chr_cmpA(name, OCV_REMOTE_CAMERA, leng) == 0)
                {
                    /* 打开摄像头 */
                    logit("camera open");
                    leng = str2intxA(&name[leng]);
                    frames = new cv::VideoCapture ((int)leng, cv::CAP_DSHOW);
                    if (frames == NULL) {
                        opencv_return_false();
                        continue;
                    }
                    if (!frames->isOpened()) {
                        opencv_return_false();
                        delete frames;
                        frames = NULL;
                        logit("camera failure");
                        continue;
                    }
                    logit("camera opened");
                    frames->set(cv::CAP_PROP_CONVERT_RGB, 1.0);
                    str_cpyA(imghdr.info, CFCBOCV64);
                    imghdr.count = 256;
                    logit("camera ready");

                    cv::Mat tmp;

                    /* 需要提前抓一下图？ */
                    frames->grab();
                    frames->retrieve(tmp);
                    logit("camera success");
                }
                else
                {
                    /* 尝试打开文件 */
                    if (cv::imreadmulti(name, images, cv::IMREAD_COLOR))
                    {
                        /* 获取图片帧数 */
                        str_cpyA(imghdr.info, IFSBOCV64);
                        imghdr.count = (int32u)images.size();
                    }
                    else
                    {
                        /* 尝试打开视频 */
                        frames = new cv::VideoCapture (name);
                        if (frames == NULL) {
                            opencv_return_false();
                            continue;
                        }
                        if (!frames->isOpened()) {
                            opencv_return_false();
                            delete frames;
                            frames = NULL;
                            continue;
                        }

                        double  idx;

                        /* 获取视频帧数 */
                        idx = frames->get(cv::CAP_PROP_FRAME_COUNT);
                        if (idx < 1) {
                            opencv_return_false();
                            delete frames;
                            frames = NULL;
                            continue;
                        }
                        frames->set(cv::CAP_PROP_POS_FRAMES, idx);
                        idx = frames->get(cv::CAP_PROP_POS_FRAMES);
                        frames->set(cv::CAP_PROP_POS_FRAMES, 0.0);
                        frames->set(cv::CAP_PROP_CONVERT_RGB, 1.0);
                        if (idx > 0x7FFFFFFF)
                            imghdr.count = 0x7FFFFFFF;
                        else
                            imghdr.count = (int32u)idx;
                        str_cpyA(imghdr.info, VFSBOCV64);
                    }
                }
            }

            /* 帧数为0表示打开失败 */
            if (imghdr.count == 0) {
                opencv_return_false();
            }
            else {
                message_send_buffer(mess, &imghdr, sizeof(imghdr));
            }
        }
        else
        if (back == sizeof(int32u))
        {
            int32u  index;

            /* 处理图片返回 */
            if (single != NULL)
            {
                /* 内存图片 */
                opencv_send_image(mess, *single);
            }
            else
            if (images.size() != 0)
            {
                /* 磁盘图片 */
                index = *(int32u*)name;
                leng = images.size();
                if (index > (int32u)leng)
                    index = (int32u)leng;

                cv::Mat *pic = &images[index];
                cv::Mat cvt = cv::Mat(pic->rows, pic->cols, CV_8UC4);

                cv::cvtColor(*pic, cvt, cv::COLOR_BGR2BGRA);
                opencv_send_image(mess, cvt);
            }
            else
            if (frames != NULL)
            {
                int32u  nows;

                /* 视频设备 */
                index = *(int32u*)name;
                if (imghdr.info[0] == 'V') {
                    if (index > imghdr.count)
                        index = imghdr.count;
                    if (index != crrt) {
                        frames->set(cv::CAP_PROP_POS_FRAMES, (double)index);
                        nows = (int32u)frames->get(cv::CAP_PROP_POS_FRAMES);
                        for (; nows < index; nows++) frames->grab();
                        crrt = (int32u)frames->get(cv::CAP_PROP_POS_FRAMES);
                    }
                }
                else {
                    logit("camera grab enter");
                }

                cv::Mat tmp;

                /* 抓取图片并返回 */
                if (frames->grab() && frames->retrieve(tmp))
                {
                    cv::Mat cvt = cv::Mat(tmp.rows, tmp.cols, CV_8UC4);

                    cv::cvtColor(tmp, cvt, cv::COLOR_BGR2BGRA);
                    opencv_send_image(mess, cvt);

                    /* 视频文件当前帧加一 */
                    if (imghdr.info[0] == 'V')
                        crrt += 1;
                }
                else
                {
                    /* 抓取失败 */
                    opencv_return_false();
                    logit("video decode error");
                }

                if (imghdr.info[0] != 'V')
                    logit("camera grab leave");
            }
            else
            {
                /* 返回出错 */
                opencv_return_false();
            }
        }
        else
        if (back == sizeof(int16u))
        {
            /* 退出工作线程 */
            if (*(int16u*)name == 0x55AA)
                break;
        }
        else
        {
            /* 返回出错 */
            opencv_return_false();
        }
    }

    /* 释放资源 */
    images.clear();
    TRY_PDEL(single);
    TRY_PDEL(frames);
    if (s_smem != NULL)
        share_file_close(s_smem);
    message_pipe_close(mess);
    return (QST_OKAY);
}
