
/* 关闭没用函数的警告 */
#if defined(_QUEST64_)
    #pragma warning (push)
    #pragma warning (disable: 4505)
#endif

/*****************************************************************************/
/*                               内存共享文件                                */
/*****************************************************************************/

/*
=======================================
    创建内存共享文件
=======================================
*/
static share_t STDCALL
share_file_open (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ ansi_t          strn[50],
  __CR_IN__ leng_t          size
    )
{
    int32u  crc32;
    leng_t  length;
    byte_t  hash[24];

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
static void_t STDCALL
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
static bool_t STDCALL
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
    获取内存共享文件
=======================================
*/
static void_t* STDCALL
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
static bool_t STDCALL
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
static bool_t STDCALL
message_pipe_init (void_t)
{
    return (socket_init());
}

/*
=======================================
    创建消息接收端
=======================================
*/
static socket_t STDCALL
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
static socket_t STDCALL
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
static void_t STDCALL
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
static void_t STDCALL
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
static bool_t STDCALL
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
static bool_t STDCALL
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
static uint_t STDCALL
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
static bool_t STDCALL
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
/*                               杂项功能函数                                */
/*****************************************************************************/

/*
=======================================
    检测程序是否运行
=======================================
*/
static bool_t STDCALL
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

/* 恢复警告设置 */
#if defined(_QUEST64_)
    #pragma warning (pop)
#endif
