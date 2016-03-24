/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-05-21  */
/*     #######          ###    ###      [GFX3]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 摄像机控制函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx3.h"

#ifndef _CR_NO_STDC_
    #if defined(_CR_OS_ANDROID_) && (ANDROID_API <= 19)
        #if defined(__pure2)
            #undef  __pure2
        #endif
    #endif
    #include <math.h>
#endif

/*
=======================================
    刷新摄像机角度参数
=======================================
*/
CR_API void_t
camera_refresh (
  __CR_IO__ sCAMERA*    cam
    )
{
    fp32_t  dx = cam->eye->x - cam->lookat->x;
    fp32_t  dy = cam->eye->y - cam->lookat->y;
    fp32_t  dz = cam->eye->z - cam->lookat->z;
    fp32_t  nn = FSQRT(dx * dx + dz * dz);

    cam->distance = FSQRT(dy * dy + nn * nn);
    if (nn < CR_ABIT32) {
        if (dy >= 0.0f)
            cam->phi =  CR_PI / 2.0f;
        else
            cam->phi = -CR_PI / 2.0f;
        cam->theta = 0.0f;
    }
    else {
        cam->phi   = FATAN(dy / nn);
        cam->theta = FASIN(dz / nn);
        if (dz < 0.0f) {
            if (cam->phi >= 0.0f)
                cam->phi =  CR_PI - cam->phi;
            else
                cam->phi = -CR_PI - cam->phi;
        }
        if (dx < 0.0f) {
            if (cam->theta >= 0.0f)
                cam->theta =  CR_PI - cam->theta;
            else
                cam->theta = -CR_PI - cam->theta;
        }
    }
}

/*
=======================================
    初始化摄像机
=======================================
*/
CR_API void_t
camera_init (
  __CR_OT__ sCAMERA*    cam,
  __CR_IN__ vec4d_t*    eye,
  __CR_IN__ vec4d_t*    lookat
    )
{
    cam->eye = eye;
    cam->lookat = lookat;
    camera_refresh(cam);
}

/*
=======================================
    获取行走方向矢量2D
=======================================
*/
CR_API void_t
camera_walkdir2 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_OT__ vec3d_t*        vdir
    )
{
    vdir->x = -FCOS(cam->theta);
    vdir->z = -FSIN(cam->theta);
}

/*
=======================================
    获取行走方向矢量3D
=======================================
*/
CR_API void_t
camera_walkdir3 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_OT__ vec3d_t*        vdir
    )
{
    vdir->x = -FCOS(cam->theta);
    vdir->y = -FSIN(cam->phi);
    vdir->z = -FSIN(cam->theta);
}

/*
=======================================
    眼球位置向左移动
=======================================
*/
CR_API void_t
camera_eye_a (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x += step * FSIN(cam->theta);
    cam->eye->z -= step * FCOS(cam->theta);
}

/*
=======================================
    眼球位置向右移动
=======================================
*/
CR_API void_t
camera_eye_d (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x -= step * FSIN(cam->theta);
    cam->eye->z += step * FCOS(cam->theta);
}

/*
=======================================
    眼球位置向前移动2D
=======================================
*/
CR_API void_t
camera_eye_w2 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x -= step * FCOS(cam->theta);
    cam->eye->z -= step * FSIN(cam->theta);
}

/*
=======================================
    眼球位置向后移动2D
=======================================
*/
CR_API void_t
camera_eye_s2 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x += step * FCOS(cam->theta);
    cam->eye->z += step * FSIN(cam->theta);
}

/*
=======================================
    眼球位置向前移动3D
=======================================
*/
CR_API void_t
camera_eye_w3 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x -= step * FCOS(cam->theta);
    cam->eye->y -= step * FSIN(cam->phi);
    cam->eye->z -= step * FSIN(cam->theta);
}

/*
=======================================
    眼球位置向后移动3D
=======================================
*/
CR_API void_t
camera_eye_s3 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->eye->x += step * FCOS(cam->theta);
    cam->eye->y += step * FSIN(cam->phi);
    cam->eye->z += step * FSIN(cam->theta);
}

/*
=======================================
    视点位置向左移动
=======================================
*/
CR_API void_t
camera_lookat_a (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x += step * FSIN(cam->theta);
    cam->lookat->z -= step * FCOS(cam->theta);
}

/*
=======================================
    视点位置向右移动
=======================================
*/
CR_API void_t
camera_lookat_d (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x -= step * FSIN(cam->theta);
    cam->lookat->z += step * FCOS(cam->theta);
}

/*
=======================================
    视点位置向前移动2D
=======================================
*/
CR_API void_t
camera_lookat_w2 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x -= step * FCOS(cam->theta);
    cam->lookat->z -= step * FSIN(cam->theta);
}

/*
=======================================
    视点位置向后移动2D
=======================================
*/
CR_API void_t
camera_lookat_s2 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x += step * FCOS(cam->theta);
    cam->lookat->z += step * FSIN(cam->theta);
}

/*
=======================================
    视点位置向前移动3D
=======================================
*/
CR_API void_t
camera_lookat_w3 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x -= step * FCOS(cam->theta);
    cam->lookat->y -= step * FSIN(cam->phi);
    cam->lookat->z -= step * FSIN(cam->theta);
}

/*
=======================================
    视点位置向后移动3D
=======================================
*/
CR_API void_t
camera_lookat_s3 (
  __CR_IN__ const sCAMERA*  cam,
  __CR_IN__ fp32_t          step
    )
{
    cam->lookat->x += step * FCOS(cam->theta);
    cam->lookat->y += step * FSIN(cam->phi);
    cam->lookat->z += step * FSIN(cam->theta);
}

/*
=======================================
    按 FPS 方式刷新摄像机
=======================================
*/
CR_API void_t
camera_update_fps (
  __CR_IO__ sCAMERA*    cam
    )
{
    fp32_t  sin_t = FSIN(cam->theta);
    fp32_t  cos_t = FCOS(cam->theta);
    fp32_t  sin_p = FSIN(cam->phi) * cam->distance;
    fp32_t  cos_p = FCOS(cam->phi) * cam->distance;

    if (cos_p < 0.0f) cos_p = -cos_p;
    cam->lookat->x = cam->eye->x - cos_p * cos_t;
    cam->lookat->y = cam->eye->y - sin_p;
    cam->lookat->z = cam->eye->z - cos_p * sin_t;
}

/*
=======================================
    按 TPS 方式刷新摄像机
=======================================
*/
CR_API void_t
camera_update_tps (
  __CR_IO__ sCAMERA*    cam
    )
{
    fp32_t  sin_t = FSIN(cam->theta);
    fp32_t  cos_t = FCOS(cam->theta);
    fp32_t  sin_p = FSIN(cam->phi) * cam->distance;
    fp32_t  cos_p = FCOS(cam->phi) * cam->distance;

    if (cos_p < 0.0f) cos_p = -cos_p;
    cam->eye->x = cam->lookat->x + cos_p * cos_t;
    cam->eye->y = cam->lookat->y + sin_p;
    cam->eye->z = cam->lookat->z + cos_p * sin_t;
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
