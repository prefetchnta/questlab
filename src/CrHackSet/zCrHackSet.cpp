
#include "xCrHackSet.h"
#include "fmtz/fmtint.h"
#include "../QstLibs/QstLibs.h"

/* 不支持多线程同时操作 */
static sPIC_RMT_HEAD    s_head;
static sPIC_RMT_INFO    s_info;

/*
---------------------------------------
    读取一帧图片
---------------------------------------
*/
static sFMT_PIC*
remote_image_get (
  __CR_IN__ socket_t    mess,
  __CR_IN__ int32u      index
    )
{
    uint_t      leng;
    void_t*     data;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 发送帧号 */
    if (!message_send_buffer(mess, &index, sizeof(index)))
        return (NULL);

    /* 接收图片信息 */
    leng = message_recv_buffer(mess, &s_info, sizeof(s_info));
    if (leng != sizeof(s_info))
        return (NULL);

    /* 填充图片信息 */
    temp.pic = image_new(0, 0, s_info.img_ww, s_info.img_hh,
                         CR_ARGB8888, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);
    if (temp.pic->size != s_info.dat_sz)
        goto _failure;
    temp.bpp = s_info.bpp;
    temp.fmt = s_info.fmt;
    temp.clr = s_info.clr;
    mem_cpy(temp.wh, s_info.wnh, sizeof(s_info.wnh));

    /* 获取图片数据 */
    data = share_file_get(CRH_RMT_IMG_DATA, temp.pic->size);
    if (data == NULL)
        goto _failure;

    /* 替换原有数据 */
    mem_free(temp.pic->data);
    temp.pic->data = (byte_t*)data;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        mem_free(rett);
        goto _failure;
    }
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = s_head.info;
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
}

/*****************************************************************************/
/*                             远程图片获取接口                              */
/*****************************************************************************/

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iPICTURE    pics;

        /* 个性部分 */
        socket_t    m_mess;

} iPIC_CRH;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPIC_CRH_release (
  __CR_IN__ iPICTURE*   that
    )
{
    iPIC_CRH*   real;

    real = (iPIC_CRH*)that;
    message_pipe_close(real->m_mess);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPIC_CRH_getMore (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ port_t      iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iPICTURE::CRH_REMOTE") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    获取图片帧
---------------------------------------
*/
static sFMT_PIC*
iPIC_CRH_get (
  __CR_IN__ iPICTURE*   that,
  __CR_IN__ int32u      index
    )
{
    iPIC_CRH*   real;

    /* 帧号过滤 */
    if (index >= that->__count__)
        return (NULL);

    /* 获取一帧远程图像 */
    real = (iPIC_CRH*)that;
    return (remote_image_get(real->m_mess, index));
}

/* 接口虚函数表 */
static const iPICTURE_vtbl _rom_ s_pics_vtbl =
{
    iPIC_CRH_release, iPIC_CRH_getMore, iPIC_CRH_get,
};

/*
=======================================
    远程图片读取
=======================================
*/
CR_API sFMTZ*
load_remote_pic (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    leng_t  leng;
    ansi_t* path;
    ansi_t  name[260];
    /* ------------ */
    share_t     smem;
    socket_t    mess;
    sFMT_PIC*   ret1;
    sFMT_PRT*   ret2;
    iPIC_CRH*   port;

    smem = NULL;

    /* 文件名转换成固定长度数组 */
    switch (param->type)
    {
        case CR_LDR_ANSI:
            leng = str_lenA(param->name.ansi);
            if (leng >= sizeof(name))
                return (NULL);
            mem_zero(name, sizeof(name));
            mem_cpy(name, param->name.ansi, leng);
            break;

        case CR_LDR_WIDE:
            path = utf16_to_local(CR_LOCAL, param->name.wide);
            if (path == NULL)
                return (NULL);
            leng = str_lenA(path);
            if (leng >= sizeof(name)) {
                mem_free(path);
                return (NULL);
            }
            mem_zero(name, sizeof(name));
            mem_cpy(name, path, leng);
            mem_free(path);
            break;

        case CR_LDR_BUFF:
            smem = share_file_open(CRH_LCL_IMG_FILE, NULL, param->buff.size);
            if (smem == NULL)
                return (NULL);
            if (!share_file_fill(smem, param->buff.data, param->buff.size))
                goto _failure1;
            str_cpyA(name, ":" CRH_LCL_IMG_FILE);
            break;

        default:
            return (NULL);
    }

    /* 连接到远程图片端口 */
    mess = message_send_open(CRH_REMOTE_IMG_PORT);
    if (mess == NULL)
        goto _failure1;
    message_pipe_timeout(mess, QST_TCP_TOUT * 5);

    /* 发送文件名, 接收文件头 */
    if (!message_send_buffer(mess, name, sizeof(name)))
        goto _failure2;
    leng = message_recv_buffer(mess, &s_head, sizeof(s_head));
    if (leng != sizeof(s_head) || s_head.count < 1)
        goto _failure2;

    /* 根据帧数判断返回接口类型 */
    if (s_head.count == 1)
    {
        /* 单帧图片 */
        ret1 = remote_image_get(mess, 0);
        if (ret1 == NULL)
            goto _failure2;
        message_pipe_close(mess);
        if (smem != NULL)
            share_file_close(smem);
        return ((sFMTZ*)ret1);
    }

    /* 生成多帧图片接口对象 */
    port = struct_new(iPIC_CRH);
    if (port == NULL)
        goto _failure2;
    if (smem != NULL)
        share_file_close(smem);
    port->m_mess = mess;
    port->pics.__count__ = s_head.count;
    port->pics.__vptr__ = &s_pics_vtbl;

    /* 返回读取的文件数据 */
    ret2 = struct_new(sFMT_PRT);
    if (ret2 == NULL) {
        iPIC_CRH_release((iPICTURE*)port);
        return (NULL);
    }
    CR_NOUSE(datin);
    ret2->type = CR_FMTZ_PRT;
    ret2->port = (iPORT*)port;
    ret2->more = "iPICTURE";
    ret2->infor = s_head.info;
    return ((sFMTZ*)ret2);

_failure2:
    message_pipe_close(mess);
_failure1:
    if (smem != NULL)
        share_file_close(smem);
    return (NULL);
}

/*****************************************************************************/
/*                                 引擎接口                                  */
/*****************************************************************************/

/*
---------------------------------------
    文件格式加载接口
---------------------------------------
*/
static sFMTZ*
engine_remote_load (
  __CR_IN__ sENGINE*    engine,
  __CR_IO__ sLOADER*    loader
    )
{
    /* 过滤加载类型 */
    if (!(engine->mask & CR_FMTZ_MASK_PIC))
        return (NULL);
    return (load_remote_pic(NULL, loader));
}

/*
=======================================
    获取引擎插件接口
=======================================
*/
CR_API sENGINE*
engine_remote (void_t)
{
    sENGINE*    engine;

    engine = engine_init(NULL, NULL, NULL, NULL);
    if (engine == NULL)
        return (NULL);
    engine->fmtz_load = engine_remote_load;
    engine->info = "Remote FMTz Engine (Done by CrHackOS)";
    return (engine);
}

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    获取引擎插件接口 (同名)
=======================================
*/
CR_API sENGINE*
engine_get (void_t)
{
    return (engine_remote());
}
#endif  /* _CR_BUILD_DLL_ */
