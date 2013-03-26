
#include "xOpenCV.h"

/* 外部库引用 */
#ifndef _CR_NO_PRAGMA_LIB_
    #pragma comment (lib, "opencv_calib3d244.lib")
    #pragma comment (lib, "opencv_contrib244.lib")
    #pragma comment (lib, "opencv_core244.lib")
    #pragma comment (lib, "opencv_features2d244.lib")
    #pragma comment (lib, "opencv_flann244.lib")
    #pragma comment (lib, "opencv_gpu244.lib")
    #pragma comment (lib, "opencv_haartraining_engine.lib")
    #pragma comment (lib, "opencv_highgui244.lib")
    #pragma comment (lib, "opencv_imgproc244.lib")
    #pragma comment (lib, "opencv_legacy244.lib")
    #pragma comment (lib, "opencv_ml244.lib")
    #pragma comment (lib, "opencv_nonfree244.lib")
    #pragma comment (lib, "opencv_objdetect244.lib")
    #pragma comment (lib, "opencv_photo244.lib")
    #pragma comment (lib, "opencv_stitching244.lib")
    #pragma comment (lib, "opencv_ts244.lib")
    #pragma comment (lib, "opencv_video244.lib")
    #pragma comment (lib, "opencv_videostab244.lib")
#endif

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
