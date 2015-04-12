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
 *             ##       CREATE: 2015-02-28
 *              #
 ================================================
        文本 OBJ 模型转二进制小工具
 ================================================
 */

#include "../QstLibs/QstLibs.h"
#pragma comment (lib, "CrH_GFX3.lib")

/*
=======================================
    主程序
=======================================
*/
int main (int argc, char *argv[])
{
    FILE *fp;
    int32u dw;
    sWAVEFRONT mesh;
    ansi_t *obj, *mtl, *mtl1;

    /* 建立 CrHack 系统 */
    if (!set_app_type(CR_APP_CUI))
        return (QST_ERROR);

    /* 参数解析 <是否合并网格> <输入文件> <输出文件> <是否输出纹理> */
    if ((argc < 5) ||
        (strcmp(argv[1], "0") != 0 && strcmp(argv[1], "1") != 0) ||
        (strcmp(argv[4], "0") != 0 && strcmp(argv[4], "1") != 0)) {
        printf("obj2bobj <0/1> <input.obj> <output.bobj> <0/1>\n");
        return (QST_ERROR);
    }

    /* 读入模型文件 */
    obj = file_load_as_strA(argv[2]);
    if (obj == NULL) {
        printf("missing input file: %s\n", argv[2]);
        return (QST_ERROR);
    }

    /* 读入材质文件 */
    mtl1 = str_dupA(argv[2]);
    if (mtl1 == NULL) {
        printf("out of memory\n");
        return (QST_ERROR);
    }
    filext_changeA(mtl1, argv[2], ".mtl");
    mtl = file_load_as_strA(mtl1);
    if (mtl == NULL) {
        printf("missing input file: %s\n", mtl1);
        return (QST_ERROR);
    }
    if (memcmp(mtl, BOM_UTF8, 3) == 0)
        mtl1 = mtl + 3;
    else
        mtl1 = mtl;

    /* 解析模型和材质字符串 */
    if (!wfront_obj_load(&mesh, obj, FALSE, FALSE)) {
        printf("invalid obj format\n");
        return (QST_ERROR);
    }
    if (!wfront_mtl_load(&mesh, mtl1)) {
        printf("invalid mtl format\n");
        return (QST_ERROR);
    }

    /* 合并同材质网格 */
    if (((argv[1])[0]) & 1) {
        if (!wfront_obj_combine(&mesh)) {
            printf("combine model failed\n");
            return (QST_ERROR);
        }
    }

    /* 保存成二进制格式 */
    fp = fopen(argv[3], "wb");
    if (fp == NULL) {
        printf("save file failed\n");
        return (QST_ERROR);
    }

    /* 文件标志 */
    if (fwrite("BOBJ", 1, 4, fp) != 4)
        goto _failure;

    /* 材质文件 */
    if (mesh.mtl != NULL) {
        dw = strlen(mesh.mtl) + 1;
        if (dw <= 1 || dw > 255)
            dw = 0;
    }
    else {
        dw = 0;
    }
    if (fwrite(&dw, 1, 1, fp) != 1)
        goto _failure;
    if (dw != 0) {
        if (fwrite(mesh.mtl, 1, dw, fp) != dw)
            goto _failure;
    }

    /* 空间坐标 */
    if (fwrite(&mesh.n_v, 1, 4, fp) != 4)
        goto _failure;
    dw = mesh.n_v * sizeof(vec3d_t);
    if (fwrite(mesh.p_v, 1, dw, fp) != dw)
        goto _failure;

    /* 贴图坐标 */
    if (fwrite(&mesh.n_vt, 1, 4, fp) != 4)
        goto _failure;
    if (mesh.n_vt != 0) {
        dw = mesh.n_vt * sizeof(vec3d_t);
        if (fwrite(mesh.p_vt, 1, dw, fp) != dw)
            goto _failure;
    }

    /* 法线向量 */
    if (fwrite(&mesh.n_vn, 1, 4, fp) != 4)
        goto _failure;
    if (mesh.n_vn != 0) {
        dw = mesh.n_vn * sizeof(vec3d_t);
        if (fwrite(mesh.p_vn, 1, dw, fp) != dw)
            goto _failure;
    }

    /* 顶点索引 */
    if (fwrite(&mesh.n_f, 1, 4, fp) != 4)
        goto _failure;
    dw = mesh.n_f * sizeof(sWAVEFRONT_F);
    if (fwrite(mesh.p_f, 1, dw, fp) != dw)
        goto _failure;

    /* 材质列表 */
    if (fwrite(&mesh.n_m, 1, 4, fp) != 4)
        goto _failure;
    for (leng_t idx = 0; idx < mesh.n_m; idx++)
    {
        /* 材质标志 */
        if (fwrite(&mesh.p_m[idx].flags, 1, 4, fp) != 4)
            goto _failure;

        /* 材质名称 */
        dw = strlen(mesh.p_m[idx].name) + 1;
        if (dw <= 1 || dw > 255)
            goto _failure;
        if (fwrite(&dw, 1, 1, fp) != 1)
            goto _failure;
        if (fwrite(mesh.p_m[idx].name, 1, dw, fp) != dw)
            goto _failure;

        /* 一堆浮点数 */
        dw = (4 + 5 * 3) * 4;
        if (fwrite(&mesh.p_m[idx].d, 1, dw, fp) != dw)
            goto _failure;

        /* 一堆整数 */
        dw = 3 * 4;
        if (fwrite(&mesh.p_m[idx].illum, 1, dw, fp) != dw)
            goto _failure;

        ansi_t **lst;

        /* 一堆字符串 */
        lst = &mesh.p_m[idx].map_ka;
        for (leng_t kk = 0; kk < 6; kk++) {
            if ((lst[kk] != NULL) &&
                ((((argv[4])[0]) & 1) || file_existA(lst[kk]))) {
                dw = strlen(lst[kk]) + 1;
                if (dw <= 1 || dw > 255)
                    dw = 0;
            }
            else {
                dw = 0;
            }
            if (fwrite(&dw, 1, 1, fp) != 1)
                goto _failure;
            if (dw != 0) {
                if (fwrite(lst[kk], 1, dw, fp) != dw)
                    goto _failure;
            }
        }
    }

    /* 模型列表 */
    if (fwrite(&mesh.n_g, 1, 4, fp) != 4)
        goto _failure;
    for (leng_t idx = 0; idx < mesh.n_g; idx++)
    {
        /* 模型名称 */
        dw = strlen(mesh.p_g[idx].name) + 1;
        if (dw <= 1 || dw > 255)
            goto _failure;
        if (fwrite(&dw, 1, 1, fp) != 1)
            goto _failure;
        if (fwrite(mesh.p_g[idx].name, 1, dw, fp) != dw)
            goto _failure;

        /* 材质名称 */
        if (mesh.p_g[idx].mtl != NULL) {
            dw = strlen(mesh.p_g[idx].mtl) + 1;
            if (dw <= 1 || dw > 255)
                dw = 0;
        }
        else {
            dw = 0;
        }
        if (fwrite(&dw, 1, 1, fp) != 1)
            goto _failure;
        if (dw != 0) {
            if (fwrite(mesh.p_g[idx].mtl, 1, dw, fp) != dw)
                goto _failure;
        }

        /* 模型顶点相关的参数 */
        if (fwrite(&mesh.p_g[idx].beg, 1, 4, fp) != 4)
            goto _failure;
        if (fwrite(&mesh.p_g[idx].end, 1, 4, fp) != 4)
            goto _failure;
        if (fwrite(&mesh.p_g[idx].num, 1, 4, fp) != 4)
            goto _failure;
        if (fwrite(&mesh.p_g[idx].attr, 1, 4, fp) != 4)
            goto _failure;
    }

    /* 保存完成 */
    fclose(fp);
    return (QST_OKAY);

_failure:
    printf("write file failed\n");
    fclose(fp);
    file_deleteA(argv[3]);
    return (QST_ERROR);
}
