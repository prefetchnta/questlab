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
    【64】基于 OpenCV 的 QV2D fz 扩展服务
 ================================================
 */

#define _QUEST64_
#include "opencv2/opencv.hpp"
#include "../QstLibs/QstLibs.h"
#include "../CrHackSet/xCrHackSet.h"
#include "../../../Asylum/asylum.hpp"

/* 一些公用宏 */
#define EXE_XNAME   "xOpenCV64"

/* 外部库引用 */
#pragma comment (lib, "opencv_core452.lib")
#pragma comment (lib, "opencv_imgcodecs452.lib")
#pragma comment (lib, "opencv_imgproc452.lib")
#pragma comment (lib, "opencv_videoio452.lib")

/* 桥接函数引入 */
#include "../bridge.inl"

/* 工作线程 */
static int32u           s_size[2];
static bool_t           s_quit[2];
static share_t          s_smem[2];
static asy::asy_thrd_t  s_thrd[2];
static void_t*  STDCALL opencv_loader (void_t*);
static void_t*  STDCALL opencv_filter (void_t*);

/* 简化代码 */
#define IFSBOCV64   "Image file supported by OpenCV64"
#define VFSBOCV64   "Video file supported by OpenCV64"
#define CFCBOCV64   "Image captured from camera by OpenCV64"

#define OCV_REMOTE_CAMERA       ":computer/remote_cam"
#define opencv_return_false()   message_send_buffer(mess, "", 1)

/*****************************************************************************/
/*                                 公共部分                                  */
/*****************************************************************************/

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
    s_size[0] = s_size[1] = 0;
    s_smem[0] = s_smem[1] = NULL;
    s_thrd[0] = s_thrd[1] = NULL;
    s_quit[0] = s_quit[1] = FALSE;

    /* 创建工作线程 */
    mess = message_recv_open(CRH_REMOTE_IMG_PORT);
    if (mess == NULL)
        return (QST_ERROR);
    message_pipe_timeout(mess, QST_TCP_TOUT);
    s_thrd[0] = asy::thread_run(opencv_loader, mess);
    if (s_thrd[0] == NULL) {
        message_pipe_close(mess);
        return (QST_ERROR);
    }
    mess = message_recv_open(CRH_REMOTE_FLT_PORT);
    if (mess == NULL) {
        s_quit[0] = TRUE;
        asy::thread_end(s_thrd[0]);
        return (QST_ERROR);
    }
    message_pipe_timeout(mess, QST_TCP_TOUT);
    s_thrd[1] = asy::thread_run(opencv_filter, mess);
    if (s_thrd[1] == NULL) {
        message_pipe_close(mess);
        s_quit[0] = TRUE;
        asy::thread_end(s_thrd[0]);
        return (QST_ERROR);
    }

    /* 等待线程结束 */
    asy::thread_end(s_thrd[1]);
    asy::thread_end(s_thrd[0]);
    return (QST_OKAY);
}

/*****************************************************************************/
/*                                 图片部分                                  */
/*****************************************************************************/

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
    if (s_size[0] < info.dat_sz) {
        if (s_smem[0] != NULL)
            share_file_close(s_smem[0]);
        s_smem[0] = share_file_open(CRH_RMT_IMG_DATA, NULL, info.dat_sz);
        if (s_smem[0] == NULL) {
            opencv_return_false();
            s_size[0] = 0;
            return;
        }
        s_size[0] = info.dat_sz;
    }
    data = (byte_t*)share_file_map(s_smem[0], info.dat_sz);
    if (data == NULL) {
        opencv_return_false();
        share_file_close(s_smem[0]);
        s_smem[0] = NULL;
        s_size[0] = 0;
        return;
    }
    nbpl = info.img_ww * sizeof(int32u);
    for (int32u yy = 0; yy < info.img_hh; yy++) {
        mem_cpy(data, image.ptr(yy), nbpl);
        data += nbpl;
    }
    share_file_unmap(s_smem[0]);

    /* 发送图片信息 */
    message_send_buffer(mess, &info, sizeof(info));
}

/*
---------------------------------------
    图片解析工作函数
---------------------------------------
*/
static void_t* STDCALL
opencv_loader (
  __CR_IN__ void_t* arg
    )
{
    uint_t      back;
    int32u      crrt;
    leng_t      leng;
    void_t*     data;
    ansi_t      name[260];
    socket_t    mess = (socket_t)arg;
    /* --------------------------- */
    cv::Mat*                single;
    sPIC_RMT_HEAD           imghdr;
    cv::VideoCapture*       frames;
    std::vector<cv::Mat>    images;

    crrt = 0;
    single = NULL;
    frames = NULL;
    images.clear();
    struct_zero(&imghdr, sPIC_RMT_HEAD);

    /* 开始工作 */
    while (!s_quit[0])
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
                    leng = str2intxA(&name[leng]);
                    frames = new cv::VideoCapture ((int)leng);
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
                    frames->set(cv::CAP_PROP_CONVERT_RGB, 1.0);
                    str_cpyA(imghdr.info, CFCBOCV64);
                    imghdr.count = 256;
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
                }
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
                s_quit[0] = TRUE;
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
    if (s_smem[0] != NULL)
        share_file_close(s_smem[0]);
    message_pipe_close(mess);
    return (NULL);
}

/*****************************************************************************/
/*                                 滤镜部分                                  */
/*****************************************************************************/

/*
---------------------------------------
    滤镜实现工作函数
---------------------------------------
*/
static void_t* STDCALL
opencv_filter (
  __CR_IN__ void_t* arg
    )
{
    socket_t    mess = (socket_t)arg;
#if 0
    while (!s_quit[1]) {
        asy::thread_yield(1);
    }
#endif
    /* 释放资源 */
    if (s_smem[1] != NULL)
        share_file_close(s_smem[1]);
    message_pipe_close(mess);
    return (NULL);
}
