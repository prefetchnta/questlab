
#include "QstLibs.h"
#if defined(_CR_SYS64_)
    #include <io.h>
#endif

#if defined(_CR_BUILD_DLL_)
/*
=======================================
    DLL 入口点
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
            misc_is_win64();
            misc_is_win11(NULL);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}

#endif  /* _CR_BUILD_DLL_ */

/* 公用查找结构 */
typedef struct
{
        /* 名称放在第一位 */
        const ansi_t*   name;

} sQST_FIND;

/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
unit_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sQST_FIND*  unit = (sQST_FIND*)obj;

    return (str_cmpA(unit->name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
unit_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*****************************************************************************/
/*                               内存共享文件                                */
/*****************************************************************************/

/*
=======================================
    创建内存共享文件
=======================================
*/
CR_API share_t STDCALL
share_file_open (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ ansi_t          strn[50],
  __CR_IN__ leng_t          size
    )
{
    int32u  crc32;
    leng_t  length;
    byte_t  hash[24];

    /* 大小为-1表示打开已有 */
    if (size == ((leng_t)-1))
        return ((share_t)OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name));

    /* 文件大小过滤 */
    if (size < CR_K2B(4))
        size = CR_K2B(4);
    else
    if (size >= ((leng_t)-1) / 8)
        return (NULL);

    /* 直接使用名称 */
    if (strn == NULL) {
        return ((share_t)CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
            PAGE_READWRITE, (DWORD)size_hi(size), (DWORD)size, name));
    }

    /* 使用哈希过的名称 */
    length = str_lenA(name);
    crc32 = hash_crc32i_total(name, length);
    *(int32u*)(&hash[0]) = DWORD_BE(crc32);
    *(int32u*)(&hash[4]) = DWORD_BE(timer_get32());
    hash_md5_total(&hash[8], name, length);
    hex2strA(strn, hash, sizeof(hash));
    return ((share_t)CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
        PAGE_READWRITE, (DWORD)size_hi(size), (DWORD)size, strn));
}

/*
=======================================
    关闭内存共享文件
=======================================
*/
CR_API void_t STDCALL
share_file_close (
  __CR_IN__ share_t sfile
    )
{
    CloseHandle((HANDLE)sfile);
}

/*
=======================================
    填充内存共享文件
=======================================
*/
CR_API bool_t STDCALL
share_file_fill (
  __CR_IN__ share_t         sfile,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    LPVOID  buff;
    HANDLE  fmap = (HANDLE)sfile;

    /* 将本地文件复制到共享内存 */
    buff = MapViewOfFile(fmap, FILE_MAP_WRITE, 0, 0, size);
    if (buff == NULL)
        return (FALSE);
    mem_cpy(buff, data, size);
    UnmapViewOfFile(buff);
    return (TRUE);
}

/*
=======================================
    映射内存共享文件
=======================================
*/
CR_API void_t* STDCALL
share_file_map (
  __CR_IN__ share_t sfile,
  __CR_IN__ leng_t  size
    )
{
    /* 共享内存映射到当前进程 */
    return (MapViewOfFile((HANDLE)sfile, FILE_MAP_READ | FILE_MAP_WRITE,
                                  0, 0, size));
}

/*
=======================================
    解除映射内存共享文件
=======================================
*/
CR_API bool_t STDCALL
share_file_unmap (
  __CR_IN__ share_t sfile
    )
{
    /* 解除映射共享内存到当前进程 */
    return (UnmapViewOfFile((HANDLE)sfile));
}

/*
=======================================
    获取内存共享文件
=======================================
*/
CR_API void_t* STDCALL
share_file_get (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ leng_t          size
    )
{
    LPVOID  buff;
    HANDLE  fmap;
    void_t* data;

    /* 将共享文件复制到本地内存 */
    fmap = OpenFileMappingA(FILE_MAP_READ, FALSE, name);
    if (fmap == NULL)
        return (NULL);
    buff = MapViewOfFile(fmap, FILE_MAP_READ, 0, 0, size);
    if (buff == NULL)
        goto _failure1;
    data = mem_malloc(size);
    if (data == NULL)
        goto _failure2;
    mem_cpy(data, buff, size);
    UnmapViewOfFile(buff);
    CloseHandle(fmap);
    return (data);

_failure2:
    UnmapViewOfFile(buff);
_failure1:
    CloseHandle(fmap);
    return (NULL);
}

/*
=======================================
    设置内存共享文件
=======================================
*/
CR_API bool_t STDCALL
share_file_set (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    LPVOID  buff;
    HANDLE  fmap;

    /* 将本地文件复制到共享内存 */
    fmap = OpenFileMappingA(FILE_MAP_WRITE, FALSE, name);
    if (fmap == NULL)
        return (FALSE);
    buff = MapViewOfFile(fmap, FILE_MAP_WRITE, 0, 0, size);
    if (buff == NULL) {
        CloseHandle(fmap);
        return (FALSE);
    }
    mem_cpy(buff, data, size);
    UnmapViewOfFile(buff);
    CloseHandle(fmap);
    return (TRUE);
}

/*****************************************************************************/
/*                               UDP 网络通讯                                */
/*****************************************************************************/

/*
=======================================
    UDP 网络库初始化
=======================================
*/
CR_API bool_t STDCALL
message_pipe_init (void_t)
{
    return (socket_init());
}

/*
=======================================
    创建消息接收端
=======================================
*/
CR_API socket_t STDCALL
message_recv_open (
  __CR_IN__ int16u  sport
    )
{
    socket_t    netw;

    netw = server_udp_open("127.0.0.1", sport);
    if (netw != NULL)
        socket_set_timeout(netw, -1, -1);
    return (netw);
}

/*
=======================================
    创建消息发送端
=======================================
*/
CR_API socket_t STDCALL
message_send_open (
  __CR_IN__ int16u  dport
    )
{
    socket_t    netw;

    netw = client_udp_open("127.0.0.1", dport);
    if (netw != NULL)
        socket_set_timeout(netw, -1, -1);
    return (netw);
}

/*
=======================================
    关闭消息通道
=======================================
*/
CR_API void_t STDCALL
message_pipe_close (
  __CR_IN__ socket_t    mess
    )
{
    socket_close(mess);
}

/*
=======================================
    设置消息读取超时
=======================================
*/
CR_API void_t STDCALL
message_pipe_timeout (
  __CR_IN__ socket_t    mess,
  __CR_IN__ int32s      time
    )
{
    /* time 小于0表示永远 */
    socket_set_timeout(mess, -1, time);
}

/*
=======================================
    发送消息
=======================================
*/
CR_API bool_t STDCALL
message_send_buffer (
  __CR_IN__ socket_t        mess,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    if (socket_udp_send(mess, NULL, 0, data, size) != size)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    发送字符串
=======================================
*/
CR_API bool_t STDCALL
message_send_string (
  __CR_IN__ socket_t        mess,
  __CR_IN__ const ansi_t*   string
    )
{
    return (message_send_buffer(mess, string, (uint_t)str_lenA(string)));
}

/*
=======================================
    接收消息
=======================================
*/
CR_API uint_t STDCALL
message_recv_buffer (
  __CR_IN__ socket_t    mess,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{
    uint_t  back;

    back = socket_udp_recv(mess, data, size);
    return ((back == CR_U_ERROR) ? 0 : back);
}

/*
=======================================
    接收字符串
=======================================
*/
CR_API bool_t STDCALL
message_recv_string (
  __CR_IN__ socket_t    mess,
  __CR_OT__ ansi_t*     buff,
  __CR_IN__ uint_t      size
    )
{
    uint_t  back;

    if (size <= 1)
        return (FALSE);
    back = message_recv_buffer(mess, buff, size);
    if (back == 0)
        return (FALSE);
    if (back == size)
        back -= 1;
    buff[back] = 0;
    return (TRUE);
}

/*****************************************************************************/
/*                               TCP 网络通讯                                */
/*****************************************************************************/

/* 最大传输长度 */
#define QST_MAX_PACK           1024
#define QST_MAX_MESS    (QST_MAX_PACK - 2)

/*
=======================================
    TCP 网络发送字符串
=======================================
*/
CR_API bool_t STDCALL
netw_cmd_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   string
    )
{
    int16u  size;
    uint_t  back;
    byte_t  buf[QST_MAX_PACK];
    leng_t  leng = str_lenA(string);

    /* 封装发送数据 */
    if (leng > QST_MAX_MESS)
        return (FALSE);
    size = (int16u)leng;
    *(int16u*)buf = WORD_BE(size);
    mem_cpy(&buf[2], string, leng);

    /* 发送数据包 */
    leng += sizeof(int16u);
    back = socket_tcp_send(netw, buf, (uint_t)leng);
    return ((back != leng) ? FALSE : TRUE);
}

/*
=======================================
    TCP 网络接收字符串
=======================================
*/
CR_API ansi_t* STDCALL
netw_cmd_recv (
  __CR_IN__ socket_t    netw
    )
{
    int16u  size;
    uint_t  back;
    uint_t  leng;
    ansi_t* string;

    /* 先接收字符串长度 */
    back = socket_tcp_recv(netw, &size, 2);
    if (back != 2)
        return (NULL);
    leng = WORD_BE(size);
    string = str_allocA(leng + 1);
    if (string == NULL)
        return (NULL);

    /* 接收后面的字符串 */
    back = socket_tcp_recv(netw, string, leng);
    if (back != leng) {
        mem_free(string);
        return (NULL);
    }
    string[leng] = NIL;
    return (string);
}

/*
=======================================
    TCP 网络接收字符串 (服务端用)
=======================================
*/
CR_API ansi_t* STDCALL
netw_cmd_recv_ex (
  __CR_IN__ socket_t    netw
    )
{
    int16u  size;
    uint_t  back;
    uint_t  leng;
    ansi_t* string;

    /* 先接收字符串长度 */
    socket_set_timeout(netw, -1, -1);
    back = socket_tcp_recv(netw, &size, 2);
    if (back != 2)
        return (NULL);
    leng = WORD_BE(size);
    string = str_allocA(leng + 1);
    if (string == NULL)
        return (NULL);

    /* 接收后面的字符串 */
    socket_set_timeout(netw, -1, QST_TCP_TOUT);
    back = socket_tcp_recv(netw, string, leng);
    if (back != leng) {
        mem_free(string);
        return (NULL);
    }
    string[leng] = NIL;
    return (string);
}

/*
=======================================
    TCP 网络发送响应
=======================================
*/
CR_API bool_t STDCALL
netw_ack_send (
  __CR_IN__ socket_t    netw,
  __CR_IN__ bool_t      ack
    )
{
    uint_t  back;
    int32u  vals;

    if (ack)
        vals = mk_tag4("OKAY");
    else
        vals = mk_tag4("FAIL");
    back = socket_tcp_send(netw, &vals, 4);
    return ((back != 4) ? FALSE : TRUE);
}

/*
=======================================
    TCP 网络接收响应
=======================================
*/
CR_API bool_t STDCALL
netw_ack_recv (
  __CR_IN__ socket_t    netw,
  __CR_OT__ bool_t*     ack
    )
{
    uint_t  back;
    int32u  vals;

    back = socket_tcp_recv(netw, &vals, 4);
    if (back != 4)
        return (FALSE);
    if (vals == mk_tag4("OKAY"))
        *ack = TRUE;
    else
    if (vals == mk_tag4("FAIL"))
        *ack = FALSE;
    else
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    TCP 网络库初始化
=======================================
*/
CR_API bool_t STDCALL
netw_cli_init (void_t)
{
    return (socket_init());
}

/*
=======================================
    TCP 网络连接到服务器
=======================================
*/
CR_API socket_t STDCALL
netw_cli_open (
  __CR_IN__ const ansi_t*   name
    )
{
    sINIu*      ini;
    bool_t      ack;
    uint_t      temp;
    int16u      port;
    ansi_t*     addr;
    ansi_t*     text;
    socket_t    netw;

    /* 读取参数 */
    port = QST_DEF_PORT;
    addr = QST_DEF_ADDR;
    text = file_load_as_strA(QST_CFG_STARTUP);
    if (text != NULL) {
        ini = ini_parseU(text);
        mem_free(text);
        text = NULL;
        if (ini != NULL) {
            text = ini_key_stringU("serv::addr", ini);
            if (text != NULL)
                addr = text;
            temp = ini_key_intxU("serv::port", QST_DEF_PORT, ini);
            if (temp > 1024 && temp < 32768)
                port = (int16u)temp;
            ini_closeU(ini);
        }
    }

    /* 是否连接到集线器上 */
    if (mem_cmp(name, "Qst", 3) != 0)
        port += 1;

    /* 创建连接 */
    netw = client_tcp_open(addr, port, -1);
    TRY_FREE(text);
    if (netw == NULL)
        return (NULL);
    socket_set_timeout(netw, -1, QST_TCP_TOUT);

    /* 发送客户端名称 */
    if (!netw_cmd_send(netw, name))
        goto _failure;
    if (!netw_ack_recv(netw, &ack) || !ack)
        goto _failure;
    socket_set_timeout(netw, -1, -1);
    return (netw);

_failure:
    socket_close(netw);
    return (NULL);
}

/*
=======================================
    TCP 网络断开连接
=======================================
*/
CR_API void_t STDCALL
netw_cli_close (
  __CR_IN__ socket_t    netw
    )
{
    /* 确保数据发送完成 */
    socket_shutdown(netw, CR_NET_SHT_TX);
    socket_close(netw);
}

/*
=======================================
    TCP 网络关闭接收
=======================================
*/
CR_API void_t STDCALL
netw_rcv_shutdwn (
  __CR_IN__ socket_t    netw
    )
{
    socket_shutdown(netw, CR_NET_SHT_RX);
}

/*
=======================================
    TCP 网络设置接收超时 (ms)
=======================================
*/
CR_API void_t STDCALL
netw_rcv_timeout (
  __CR_IN__ socket_t    netw,
  __CR_IN__ int32s      time
    )
{
    /* time 小于0表示永远 */
    socket_set_timeout(netw, -1, time);
}

/*****************************************************************************/
/*                               操作命令函数                                */
/*****************************************************************************/

/*
=======================================
    是否为有效命令数据
=======================================
*/
CR_API bool_t STDCALL
cmd_type_okay (
  __CR_IN__ const ansi_t*   string
    )
{
    if (string[0] != '#' &&
        string[0] != '@' &&
        string[0] != '$')
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    命令数据提取
---------------------------------------
*/
static ansi_t*
cmd_data_get (
  __CR_IN__ ansi_t          type,
  __CR_IN__ const ansi_t*   string
    )
{
    if (string[0] != type)
        return (NULL);
    return (str_dupA(&string[1]));
}

/*
---------------------------------------
    命令数据合成
---------------------------------------
*/
static bool_t
cmd_data_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ ansi_t          type,
  __CR_IN__ const ansi_t*   string
    )
{
    bool_t  rett;
    leng_t  size;
    ansi_t* temp;

    size = str_lenA(string) + 1;
    temp = str_allocA(size + 1);
    if (temp == NULL)
        return (FALSE);
    temp[0] = type;
    mem_cpy(&temp[1], string, size);
    rett = netw_cmd_send(netw, temp);
    mem_free(temp);
    return (rett);
}

/*
=======================================
    INI 命令字符串提取
=======================================
*/
CR_API ansi_t* STDCALL
cmd_ini_get (
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_get('#', string));
}

/*
=======================================
    INI 命令字符串合成
=======================================
*/
CR_API bool_t STDCALL
cmd_ini_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_send(netw, '#', string));
}

/*
=======================================
    XML 命令字符串提取
=======================================
*/
CR_API ansi_t* STDCALL
cmd_xml_get (
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_get('@', string));
}

/*
=======================================
    XML 命令字符串合成
=======================================
*/
CR_API bool_t STDCALL
cmd_xml_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_send(netw, '@', string));
}

/*
=======================================
    SHL 命令字符串提取
=======================================
*/
CR_API ansi_t* STDCALL
cmd_shl_get (
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_get('$', string));
}

/*
=======================================
    SHL 命令字符串合成
=======================================
*/
CR_API bool_t STDCALL
cmd_shl_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   string
    )
{
    return (cmd_data_send(netw, '$', string));
}

/*
=======================================
    SHL 命令参数提取
=======================================
*/
CR_API ansi_t** STDCALL
cmd_shl_split (
  __CR_IO__ ansi_t* string,
  __CR_OT__ uint_t* count
    )
{
    return (str_cmd_splitA(string, count));
}

/*
=======================================
    SHL 命令参数读取
=======================================
*/
CR_API ansi_t* STDCALL
cmd_shl_param (
  __CR_IN__ ansi_t**    param,
  __CR_IN__ uint_t      index
    )
{
    return (param[index]);
}

/* 命令行执行对象 */
typedef struct
{
        /* 执行单元查找表 */
        sCURBEAD    table;

} sQST_SHL;

/*
=======================================
    命令行执行对象释放
=======================================
*/
CR_API void_t STDCALL
cmd_exec_free (
  __CR_IN__ exec_t  objs
    )
{
    sQST_SHL*   real;

    real = (sQST_SHL*)objs;
    curbead_freeT(&real->table, sQST_CMD);
    mem_free((void_t*)objs);
}

/*
=======================================
    命令行执行对象建立
=======================================
*/
CR_API exec_t STDCALL
cmd_exec_init (
  __CR_IN__ const sQST_CMD* list,
  __CR_IN__ leng_t          count
    )
{
    leng_t      idx;
    sQST_SHL*   rett;

    /* 建立结构 */
    rett = struct_new(sQST_SHL);
    if (rett == NULL)
        return (NULL);

    /* 初始化表 */
    if (!curbead_initT(&rett->table, sQST_CMD, 0))
        goto _failure;
    rett->table.find = unit_find;
    rett->table.comp = unit_comp;

    /* 插入表格 */
    for (idx = 0; idx < count; idx++) {
        if (curbead_insertT(&rett->table, sQST_CMD,
                list[idx].name, &list[idx]) == NULL) {
            curbead_freeT(&rett->table, sQST_CMD);
            goto _failure;
        }
    }
    return ((exec_t)rett);

_failure:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    命令行执行对象添加
=======================================
*/
CR_API bool_t STDCALL
cmd_exec_addn (
  __CR_IN__ exec_t          objs,
  __CR_IN__ const sQST_CMD* list
    )
{
    leng_t      idx;
    leng_t      cnts;
    sQST_SHL*   real;

    /* 统计个数 */
    cnts = 0;
    while (list[cnts].name != NULL &&
           list[cnts].exec != NULL)
        cnts += 1;

    /* 插入表格 */
    real = (sQST_SHL*)objs;
    for (idx = 0; idx < cnts; idx++) {
        if (curbead_insertT(&real->table, sQST_CMD,
                list[idx].name, &list[idx]) == NULL)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    命令行执行对象执行
=======================================
*/
CR_API bool_t STDCALL
cmd_exec_main (
  __CR_IN__ exec_t          objs,
  __CR_IN__ void_t*         param,
  __CR_IN__ const ansi_t*   cmdz
    )
{
    bool_t      rett;
    uint_t      argc;
    ansi_t*     nstr;
    ansi_t**    argv;
    sQST_CMD*   unit;
    sQST_SHL*   real;

    /* 获取命令行 */
    nstr = cmd_shl_get(cmdz);
    if (nstr == NULL)
        return (FALSE);
    argv = cmd_shl_split(nstr, &argc);
    if (argv == NULL) {
        mem_free(nstr);
        return (FALSE);
    }

    /* 查找执行命令 */
    real = (sQST_SHL*)objs;
    unit = curbead_findT(&real->table, sQST_CMD, argv[0]);
    if (unit != NULL)
        rett = unit->exec(param, argc, argv);
    else
        rett = FALSE;
    mem_free(argv);
    mem_free(nstr);
    return (rett);
}

/*****************************************************************************/
/*                               杂项功能函数                                */
/*****************************************************************************/

/*
=======================================
    分配内存包装
=======================================
*/
CR_API void_t* STDCALL
misc_mem_malloc (
  __CR_IN__ uint_t  size
    )
{
    return (mem_malloc(size));
}

/*
=======================================
    分配内存包装
=======================================
*/
CR_API void_t* STDCALL
misc_mem_calloc (
  __CR_IN__ uint_t  num,
  __CR_IN__ uint_t  size
    )
{
    return (mem_calloc(num, size));
}

/*
=======================================
    重分配内存包装
=======================================
*/
CR_API void_t* STDCALL
misc_mem_realloc (
  __CR_IN__ void_t* ptr,
  __CR_IN__ uint_t  new_size
    )
{
    return (mem_realloc(ptr, new_size));
}

/*
=======================================
    释放内存包装
=======================================
*/
CR_API void_t STDCALL
misc_mem_free (
  __CR_IN__ const void_t*   data
    )
{
    mem_free(data);
}

/*
=======================================
    判断目录是否存在
=======================================
*/
CR_API bool_t STDCALL
misc_dir_exist (
  __CR_IN__ const ansi_t*   path
    )
{
    bool_t              rett;
    HANDLE              find;
    WIN32_FIND_DATAA    wfda;

    find = FindFirstFileA(path, &wfda);
    if (find == INVALID_HANDLE_VALUE)
        return (FALSE);
    if (wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        rett = TRUE;
    else
        rett = FALSE;
    FindClose(find);
    return (rett);
}

/* 空命令参数时使用 */
static ansi_t*  s_temp;

/*
=======================================
    返回命令行参数
=======================================
*/
CR_API ansi_t** STDCALL
misc_get_param (
  __CR_IN__ const ansi_t*   cmdz,
  __CR_IN__ uint_t*         count
    )
{
    ansi_t*     copy;
    ansi_t**    argv;

    /* 整个程序只调用一次
       返回的结果无需/不能释放 */
    copy = str_dupA(cmdz);
    if (copy == NULL) {
        s_temp = "";
        if (count != NULL)
            *count = 0;
        return (&s_temp);
    }
    argv = cmd_shl_split(copy, count);
    if (argv == NULL) {
        s_temp = "";
        if (count != NULL)
            *count = 0;
        return (&s_temp);
    }
    return (argv);
}

/*
=======================================
    判断是否是64位系统
=======================================
*/
CR_API bool_t STDCALL
misc_is_win64 (void_t)
{
    static sint_t   is_x64 = -1;

    if (is_x64 < 0)
    {
        ansi_t  tmp[MAX_PATH];

        if (GetEnvironmentVariableA("ProgramFiles(x86)",
                            tmp, sizeof(tmp)) == 0) {
            is_x64 = FALSE;
        }
        else {
            is_x64 = TRUE;
        }
    }
    return ((bool_t)is_x64);
}

/* RtlGetNtVersionNumbers() 函数类型 */
typedef void (WINAPI *gver_t) (int32u*, int32u*, int32u*);

/*
=======================================
    判断是否是 Windows 11 系统
=======================================
*/
CR_API bool_t STDCALL
misc_is_win11 (
  __CR_OT__ int32u* vers
    )
{
    static sint_t   is_w11 = -1;
    static int32u   back[3] = { 0, 0, 0 };

    if (is_w11 < 0)
    {
        sbin_t  sbin;
        gver_t  gver;

        if (vers != NULL) {
            vers[0] = 0;
            vers[1] = 0;
            vers[2] = 0;
        }
        sbin = sbin_loadA("ntdll.dll");
        if (sbin == NULL)
            return (FALSE);
        gver = sbin_exportT(sbin, "RtlGetNtVersionNumbers", gver_t);
        if (gver == NULL) {
            sbin_unload(sbin);
            return (FALSE);
        }
        gver(&back[0], &back[1], &back[2]);
        back[2] &= 0xFFFF;
        sbin_unload(sbin);
        if (back[0] < 10) {
            is_w11 = FALSE;
        }
        else {
            is_w11 = ((back[2] >= 22000) ? TRUE : FALSE);
        }
    }

    if (vers != NULL) {
        vers[0] = back[0];
        vers[1] = back[1];
        vers[2] = back[2];
    }
    return ((bool_t)is_w11);
}

/*
=======================================
    判断是否是控制台程序
=======================================
*/
CR_API sint_t STDCALL
misc_is_console (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ int32u*         x64bin
    )
{
    FILE*   fp;
    int32u  pos;
    byte_t  dat[4];
    bool_t  is_x64;
    bool_t  is_cui;

    /* 文件打开 */
    fp = fopen(name, "rb");
    if (fp == NULL)
        return (-1);

    /* 检查 MZ 标志 */
    if (fread(dat, 1, 2, fp) != 2)
        goto _failure;
    if (dat[0] != 0x4D || dat[1] != 0x5A)
        goto _failure;

    /* 读取 PE 头偏移 */
    if (fseek(fp, 0x3C, SEEK_SET) != 0)
        goto _failure;
    if (fread(&pos, 1, 4, fp) != 4)
        goto _failure;
    if (fseek(fp, pos, SEEK_SET) != 0)
        goto _failure;

    /* 检查 PE 标志 */
    if (fread(dat, 1, 4, fp) != 4)
        goto _failure;
    if (dat[0] != 0x50 || dat[1] != 0x45 ||
        dat[2] != 0x00 || dat[3] != 0x00)
        goto _failure;

    /* 检查机器类型 */
    if (fread(dat, 1, 2, fp) != 2)
        goto _failure;
    if (*(int16u*)dat == 0x014C)
        is_x64 = FALSE;
    else
    if (*(int16u*)dat == 0x8664)
        is_x64 = TRUE;
    else
        goto _failure;

    /* 检查 PE 可选头标志 */
    if (fseek(fp, 0x12, SEEK_CUR) != 0)
        goto _failure;
    if (fread(dat, 1, 2, fp) != 2)
        goto _failure;
    if (*(int16u*)dat == 0x10B) {
        if (is_x64)
            goto _failure;
    }
    else
    if (*(int16u*)dat == 0x20B) {
        if (!is_x64)
            goto _failure;
    }
    else {
        goto _failure;
    }

    /* 读取子系统标志 */
    if (fseek(fp, 0x42, SEEK_CUR) != 0)
        goto _failure;
    if (fread(dat, 1, 2, fp) != 2)
        goto _failure;
    if (*(int16u*)dat == 3)
        is_cui = TRUE;
    else
        is_cui = FALSE;
    if (x64bin != NULL)
        *x64bin = is_x64 ? TRUE : FALSE;
    fclose(fp);
    return (is_cui ? 1 : 0);

_failure:
    fclose(fp);
    return (-1);
}

/*
=======================================
    判断是否是终端启动
=======================================
*/
CR_API bool_t STDCALL
misc_is_terminal (void_t)
{
    HKEY    hkey;
    BYTE    data[39];
    DWORD   size, type;

    /* 没有键值使用系统默认的配置 */
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Console\\%%Startup",
                      0, KEY_READ, &hkey) != ERROR_SUCCESS)
        goto _default1;
    size = 0;
    type = REG_NONE;
    if (RegQueryValueExA(hkey, "DelegationConsole", NULL, &type,
                         NULL, &size) != ERROR_SUCCESS)
        goto _default2;
    if (type != REG_SZ || size != sizeof(data))
        goto _default2;
    if (RegQueryValueExA(hkey, "DelegationConsole", NULL, &type,
                         data, &size) != ERROR_SUCCESS)
        goto _default2;
    if (mem_cmp(data, "{B23D10C0-E52E-411E-9D5B-C09FDF709C7D}",
                            sizeof(data)) != 0)
        goto _default2;
    RegCloseKey(hkey);
    return (FALSE);

_default2:
    RegCloseKey(hkey);
_default1:
    return (misc_is_win11(NULL));
}

/*
=======================================
    检测程序是否运行 (主进程启动调一次)
=======================================
*/
CR_API bool_t STDCALL
misc_is_running (
  __CR_IN__ const ansi_t*   name
    )
{
    HANDLE  mutex;

    mutex = CreateMutexA(NULL, FALSE, name);
    if (mutex != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    检测程序是否运行 (其他进程可多次调)
=======================================
*/
CR_API bool_t STDCALL
misc_check_running (
  __CR_IN__ const ansi_t*   name
    )
{
    HANDLE  mutex;
    bool_t  retc = FALSE;

    mutex = CreateMutexA(NULL, FALSE, name);
    if (mutex != NULL) {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
            retc = TRUE;
        CloseHandle(mutex);
    }
    return (retc);
}

/*
=======================================
    生成 UUID 字符串
=======================================
*/
CR_API void_t STDCALL
misc_gen_uuid (
  __CR_OT__ ansi_t  uuid[33]
    )
{
    sprintf(uuid, "%08X%08X%016I64X", GetCurrentProcessId(),
                                      GetCurrentThreadId(),
                                      __rdtsc());
}

/*
=======================================
    调用外部进程
=======================================
*/
CR_API bool_t STDCALL
misc_call_exe (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ bool_t          wait,
  __CR_IN__ bool_t          hide
    )
{
    FILE*               fp;
    DWORD               cf;
    sint_t              ty;
    int32u              x6;
    ansi_t              bk;
    ansi_t*             tt;
    ansi_t*             pt;
    const ansi_t*       jp;
    STARTUPINFOA        si;
    PROCESS_INFORMATION pi;

    /* 去除头尾空白 */
    tt = str_dupA(name);
    if (tt == NULL)
        return (FALSE);
    str_trimA(tt);

    /* 拆分出执行文件 */
    for (bk = 0, pt = tt; *pt != 0; pt++) {
        if (is_spaceA(*pt)) {
            bk = *pt;
            *pt = 0;
            break;
        }
    }

    /* 判断是否是可执行文件 */
    if (filext_checkA(tt, ".exe"))
    {
        /* 判断是否是控制台程序 */
        ty = misc_is_console(tt, &x6);
    }
    else
    {
        /* 非可执行文件 */
        ty = -1;
        x6 = FALSE;
    }
    *pt = bk;
    pt = NULL;

    /* 二进制可执行文件 */
    if (ty >= 0)
    {
        ansi_t  uuid[33];

        /* Windows 11 下控制台默认放到终端里运行了，导致一些
           控制台程序窗口位置大小控制失败，所以这里要区分对待 */
        if (!wait && ty > 0 && misc_is_terminal())
        {
            leng_t  len;
            ansi_t* hex;

            /* 生成执行批处理文件 */
            misc_gen_uuid(uuid);
            pt = str_fmtA("call_win11_%s.bat", uuid);
            if (pt == NULL)
                goto _failure;
            fp = fopen(pt, "w");
            if (fp == NULL) {
                mem_free(pt);
                goto _failure;
            }

            /* 转换成16进制串防传参出错 */
            len = str_lenA(tt);
            hex = str_allocA(len * 2 + 1);
            if (hex == NULL) {
                fclose(fp);
                file_deleteA(pt);
                mem_free(pt);
                goto _failure;
            }
            hex2strA(hex, tt, len);
            mem_free(tt);
            tt = hex;

#if defined(_CR_SYS64_)
            if (_access(".\\jump_cui.exe", 0) == 0)
                jp = "jump_cui.exe";
            else
#endif
                jp = "x64bin\\jump_cui.exe";

            /* Windows 11 以后没有32位的系统 */
            if (hide)
                fprintf(fp, "@echo off\nstart %s 1%s\n", jp, tt);
            else
                fprintf(fp, "@echo off\nstart %s 0%s\n", jp, tt);
            fclose(fp);
            mem_free(tt);
            tt = pt;
            wait = TRUE;
            hide = TRUE;
        }
#if defined(_CR_SYS32_)
        else if (x6)        /* 64位的二进制可执行文件不能等待进程结束再返回 */
        {
            /* 32位系统下跳过执行 */
            if (!misc_is_win64())
                goto _failure;

            /* 生成执行批处理文件 */
            misc_gen_uuid(uuid);
            pt = str_fmtA("call_x64_%s.bat", uuid);
#else
        else if (!x6)       /* 32位的二进制可执行文件不能等待进程结束再返回 */
        {
            /* 生成执行批处理文件 */
            misc_gen_uuid(uuid);
            pt = str_fmtA("call_x86_%s.bat", uuid);
#endif
            if (pt == NULL)
                goto _failure;
            fp = fopen(pt, "w");
            if (fp == NULL) {
                mem_free(pt);
                goto _failure;
            }
            fprintf(fp, "@echo off\nstart %s\n", tt);
            fclose(fp);
            mem_free(tt);
            tt = pt;
            wait = TRUE;
            hide = TRUE;
        }
    }

    /* 执行命令 */
    mem_zero(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFOA);
    cf = hide ? CREATE_NO_WINDOW : CREATE_NEW_CONSOLE;
    if (!CreateProcessA(NULL, tt, NULL, NULL, FALSE, cf,
                        NULL, NULL, &si, &pi))
        goto _failure;
    if (wait)
        WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    if (pt != NULL)
        file_deleteA(pt);
    mem_free(tt);
    return (TRUE);

_failure:
    mem_free(tt);
    return (FALSE);
}

/*
=======================================
    将窗口提到最前面
=======================================
*/
CR_API void_t STDCALL
misc_bring2top (
  __CR_IN__ hwnd_t  hwnd,
  __CR_IN__ hwnd_t  parent
    )
{
    LONG    style;

    /* 恢复窗口 */
    if (parent == NULL)
        parent = hwnd;
    if (IsIconic(parent))
        ShowWindow(parent, SW_RESTORE);
    style = GetWindowLong(hwnd, GWL_EXSTYLE);

    /* 强拉到最前面 */
    while (!SetWindowPos(hwnd, HWND_TOPMOST,
        0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE))
            thread_sleep(1);
    if (style & WS_EX_TOPMOST)
        return;
    thread_sleep(1);
    while (!SetWindowPos(hwnd, HWND_NOTOPMOST,
        0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE))
            thread_sleep(1);
}

/*
=======================================
    设置 CUI 程序窗口位置
=======================================
*/
CR_API bool_t STDCALL
misc_cui_setwin (
  __CR_IN__ hwnd_t  hwnd,
  __CR_IN__ hcui_t  hcui,
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y,
  __CR_IN__ uint_t  w,
  __CR_IN__ uint_t  h
    )
{
    RECT                rect;
    COORD               wwhh;
    CONSOLE_FONT_INFO   info;

    /* 先尝试设置位置 */
    if (!SetWindowPos(hwnd, HWND_TOP, (int)x, (int)y,
                 (int)w, (int)h, SWP_SHOWWINDOW))
        return (FALSE);

    /* 获取输出字体宽高 */
    if (!GetCurrentConsoleFont(hcui, FALSE, &info))
        return (FALSE);
    wwhh = GetConsoleFontSize(hcui, info.nFont);
    if (wwhh.X == 0 || wwhh.Y == 0)
        return (FALSE);

    /* 如果窗口高度超出则向小的靠拢 */
    if (!GetWindowRect(hwnd, &rect))
        return (FALSE);
    rect.right -= rect.left;
    rect.bottom -= rect.top;
    if (h == (uint_t)rect.bottom)
        return (TRUE);
    w = (uint_t)rect.right;
    if (h < (uint_t)rect.bottom)
        h = rect.bottom - wwhh.Y;
    return (SetWindowPos(hwnd, HWND_TOP, (int)x, (int)y,
                 (int)w, (int)h, SWP_SHOWWINDOW));
}

/*
---------------------------------------
    合成窗口配置路径
---------------------------------------
*/
static ansi_t*
misc_desk_mk_path (
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t  size;
    ansi_t* full;

    /* 限定在这个目录下 */
    size  = str_lenA(name) + 1;
    size += str_lenA(QST_PATH_WINDOW);
    full = str_allocA(size);
    if (full == NULL)
        return (NULL);
    str_cpyA(full, QST_PATH_WINDOW);
    return (str_catA(full, name));
}

/*
=======================================
    保存窗口位置配置
=======================================
*/
CR_API bool_t STDCALL
misc_desk_save (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ sint_t          left,
  __CR_IN__ sint_t          top,
  __CR_IN__ uint_t          width,
  __CR_IN__ uint_t          height
    )
{
    FILE*   fp;
    ansi_t* full;

    /* 过滤参数 */
    if (((sint_t)width <= 0) ||
        ((sint_t)height <= 0))
        return (FALSE);

    /* 打开配置文件 */
    full = misc_desk_mk_path(name);
    if (full == NULL)
        return (FALSE);
    fp = fopen(full, "w");
    mem_free(full);
    if (fp == NULL)
        return (FALSE);

    /* 写入配置文件 */
    fprintf(fp, "win::left = %d\n", left);
    fprintf(fp, "win::top = %d\n", top);
    fprintf(fp, "win::width = %u\n", width);
    fprintf(fp, "win::height = %u\n", height);
    fclose(fp);
    return (TRUE);
}

/*
=======================================
    加载窗口位置配置
=======================================
*/
CR_API bool_t STDCALL
misc_desk_load (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ sint_t*         left,
  __CR_OT__ sint_t*         top,
  __CR_OT__ uint_t*         width,
  __CR_OT__ uint_t*         height
    )
{
    sINIu*  ini;
    ansi_t* text;
    ansi_t* full;
    sint_t  x1, y1;
    uint_t  ww, hh;

    /* 打开配置文件 */
    full = misc_desk_mk_path(name);
    if (full == NULL)
        return (FALSE);
    text = file_load_as_strA(full);
    mem_free(full);
    if (text == NULL)
        return (FALSE);

    /* 解析配置文件 */
    ini = ini_parseU(text);
    mem_free(text);
    if (ini == NULL)
        return (FALSE);
    x1 = ini_key_intxU("win::left", 0, ini);
    if (!ini->found)
        goto _failure;
    y1 = ini_key_intxU("win::top", 0, ini);
    if (!ini->found)
        goto _failure;
    ww = ini_key_intxU("win::width", 0, ini);
    if (!ini->found)
        goto _failure;
    hh = ini_key_intxU("win::height", 0, ini);
    if (!ini->found)
        goto _failure;
    ini_closeU(ini);

    /* 过滤参数 */
    if (((sint_t)ww <= 0) ||
        ((sint_t)hh <= 0))
        return (FALSE);

    /* 返回结果 */
    if (left   != NULL) *left   = x1;
    if (top    != NULL) *top    = y1;
    if (width  != NULL) *width  = ww;
    if (height != NULL) *height = hh;
    return (TRUE);

_failure:
    ini_closeU(ini);
    return (FALSE);
}

/*
=======================================
    初始化窗口位置配置
=======================================
*/
CR_API void_t STDCALL
misc_desk_init (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ sint_t*         left,
  __CR_OT__ sint_t*         top,
  __CR_OT__ uint_t*         width,
  __CR_OT__ uint_t*         height,
  __CR_IN__ uint_t          def_w,
  __CR_IN__ uint_t          def_h
    )
{
    RECT    full;
    sint_t  x1, y1;
    uint_t  ww, hh;
    uint_t  sw, sh;

    /* 加载窗口位置 */
    if (misc_desk_load(name, left, top, width, height))
        return;

    /* 失败, 使用默认 */
    ww = def_w; hh = def_h;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &full, 0))
    {
        /* 取桌面大小 */
        sw = full.right - full.left;
        sh = full.bottom - full.top;
    }
    else
    {
        /* 失败, 取屏幕大小 */
        sw = GetSystemMetrics(SM_CXSCREEN);
        sh = GetSystemMetrics(SM_CYSCREEN);
    }

    /* 窗口居中放置 */
    if (sw < ww) ww = sw;
    if (sh < hh) hh = sh;
    x1 = (sint_t)((sw - ww) / 2);
    y1 = (sint_t)((sh - hh) / 2);
    misc_desk_save(name, x1, y1, ww, hh);

    /* 返回结果 */
    if (left   != NULL) *left   = x1;
    if (top    != NULL) *top    = y1;
    if (width  != NULL) *width  = ww;
    if (height != NULL) *height = hh;
}

/*
=======================================
    异步调用一个函数
=======================================
*/
CR_API void_t STDCALL
misc_async_call (
  __CR_IN__ mt_main_t   func,
  __CR_IO__ sQST_CTX*   param
    )
{
    thrd_t  thrd;

    param->copyed = FALSE;
    thrd = thread_new(0, func, param, FALSE, CR_PRRT_NRM, NULL);
    if (thrd != NULL) {
        thread_del(thrd);
        while (!param->copyed)
            thread_sleep(1);
    }
}

/*
=======================================
    异步上下文复制完毕
=======================================
*/
CR_API void_t STDCALL
misc_async_okay (
  __CR_IO__ sQST_CTX*   param
    )
{
    atom_inc(&param->copyed);
}

/*****************************************************************************/
/*                               QstGraph API                                */
/*****************************************************************************/

/*
=======================================
    QstGraph 客户端初始化
=======================================
*/
CR_API socket_t STDCALL
QstGraph_Init (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ uint_t          port
    )
{
    if (port <= 1024 || port >= 32768)
        return (NULL);
    return (client_udp_open(addr, (int16u)port));
}

/*
=======================================
    QstGraph 客户端释放
=======================================
*/
CR_API void_t STDCALL
QstGraph_Free (
  __CR_IN__ socket_t    netw
    )
{
    socket_close(netw);
}

/*
=======================================
    QstGraph 图表清除
=======================================
*/
CR_API void_t STDCALL
QstGraph_Clear (
  __CR_IN__ socket_t    netw
    )
{
    byte_t  cmd[2] = { 0x00, 0x00 };

    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置图表横轴
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetAxisBottom (
  __CR_IN__ socket_t    netw,
  __CR_IN__ uint_t      max
    )
{
    byte_t  cmd[2 + 4];

    cmd[0] = 0x00;
    cmd[1] = 0x01;
    mem_cpy(&cmd[2], &max, sizeof(max));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置图表横轴
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetAxisBottom2 (
  __CR_IN__ socket_t    netw,
  __CR_IN__ fp32_t      min,
  __CR_IN__ fp32_t      max
    )
{
    byte_t  cmd[2 + 4 + 4];

    cmd[0] = 0x00;
    cmd[1] = 0x02;
    mem_cpy(&cmd[2], &min, sizeof(min));
    mem_cpy(&cmd[6], &max, sizeof(max));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置图表竖轴
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetAxisLeft (
  __CR_IN__ socket_t    netw,
  __CR_IN__ fp32_t      min,
  __CR_IN__ fp32_t      max
    )
{
    byte_t  cmd[2 + 4 + 4];

    cmd[0] = 0x00;
    cmd[1] = 0x03;
    mem_cpy(&cmd[2], &min, sizeof(min));
    mem_cpy(&cmd[6], &max, sizeof(max));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置曲线颜色
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetLineColor (
  __CR_IN__ socket_t    netw,
  __CR_IN__ uint_t      r,
  __CR_IN__ uint_t      g,
  __CR_IN__ uint_t      b
    )
{
    byte_t  cmd[2 + 4];

    cmd[0] = 0x00;
    cmd[1] = 0x04;
    cmd[2] = (byte_t)b;
    cmd[3] = (byte_t)g;
    cmd[4] = (byte_t)r;
    cmd[5] = 0x00;
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (Char)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetChar (
  __CR_IN__ socket_t    netw,
  __CR_IN__ ansi_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 1];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x00;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (UChar)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUChar (
  __CR_IN__ socket_t    netw,
  __CR_IN__ byte_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 1];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x01;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (Short)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetShort (
  __CR_IN__ socket_t    netw,
  __CR_IN__ int16s      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 2];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x02;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (UShort)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUShort (
  __CR_IN__ socket_t    netw,
  __CR_IN__ int16u      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 2];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x03;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (Int)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetInt (
  __CR_IN__ socket_t    netw,
  __CR_IN__ sint_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 4];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x04;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (UInt)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUInt (
  __CR_IN__ socket_t    netw,
  __CR_IN__ uint_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 4];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x05;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (Float)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetFloat (
  __CR_IN__ socket_t    netw,
  __CR_IN__ fp32_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 4];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x06;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一个数值 (Double)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetDouble (
  __CR_IN__ socket_t    netw,
  __CR_IN__ fp64_t      value,
  __CR_IN__ bool_t      move
    )
{
    byte_t  cmd[2 + 1 + 8];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    cmd[2] = 0x07;
    if (!move) cmd[2] |= 0x80;
    mem_cpy(&cmd[3], &value, sizeof(value));
    socket_udp_send(netw, NULL, 0, cmd, sizeof(cmd));
}

/*
=======================================
    QstGraph 设置一组数值 (Char)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetCharList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(ansi_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (UChar)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUCharList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const byte_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x01;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(byte_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (Short)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetShortList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const int16s*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x02;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(int16s);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (UShort)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUShortList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const int16u*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x03;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(int16u);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (Int)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetIntList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const sint_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x04;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(sint_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (UInt)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetUIntList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const uint_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x05;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(uint_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (Float)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetFloatList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const fp32_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x06;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(fp32_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}

/*
=======================================
    QstGraph 设置一组数值 (Double)
=======================================
*/
CR_API void_t STDCALL
QstGraph_SetDoubleList (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const fp64_t*   list,
  __CR_IN__ uint_t          count,
  __CR_IN__ bool_t          move
    )
{
    byte_t  cmd[1500];

    if (count > 128)
        count = 128;
    cmd[0] = 0x01;
    cmd[1] = 0x01;
    cmd[2] = 0x07;
    if (!move) cmd[2] |= 0x80;
    cmd[3] = (byte_t)count;
    count *= sizeof(fp64_t);
    mem_cpy(&cmd[4], list, count);
    count += sizeof(int32u);
    socket_udp_send(netw, NULL, 0, cmd, count);
}
