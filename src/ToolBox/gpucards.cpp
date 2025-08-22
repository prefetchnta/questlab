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
 *             ##       CREATE: 2025-08-22
 *              #
 ================================================
        GPU 显卡设备枚举小工具
 ================================================
 */

#include "LabAI.h"
#include <stdio.h>
#pragma comment (lib, "LabAI.lib")

/*
=======================================
    主程序
=======================================
*/
int main ()
{
    printf("==========================================\n");

    uint_t  cnts = imglab_ncnn_gpu_count();
    uint_t  defs = imglab_ncnn_gpu_default();

    printf("==========================================\n");
    printf("GPU count %u, default index %u\n", cnts, defs);
    printf("==========================================\n");
    for (uint_t idx = 0; idx < cnts; idx++)
    {
        sNCNN_GPU_INFO  gpu;

        imglab_ncnn_gpu_info(&gpu, idx);
        printf("%u: %s", idx, gpu.device_name);
        switch (gpu.gpu_type)
        {
            default:
                printf(" [unknown]\n");
                break;

            case 0:
                printf(" [discrete gpu]\n");
                break;

            case 1:
                printf(" [integrated gpu]\n");
                break;

            case 2:
                printf(" [virtual gpu]\n");
                break;

            case 3:
                printf(" [cpu]\n");
                break;
        }
        printf("\tapi_vers    : %08X\n", gpu.vers_api);
        printf("\tdrv_vers    : %08X\n", gpu.vers_drv);
        printf("\tvendor_id   : %08X\n", gpu.vendor_id);
        printf("\tdevice_id   : %08X\n", gpu.device_id);
        printf("\tdriver_id   : %u\n", gpu.driver_id);
        printf("\tdriver_name : %s\n", gpu.driver_name);
        if (idx != cnts - 1)
            printf("------------------------------------------\n");
    }
    printf("==========================================\n");
    return (0);
}
