#-*- coding: utf-8 -*-
##################################
#   HTTP 批量自动下载脚本
#
#   创建时间: 2022-04-20
##################################

import ctypes, os, sys, time

# 参数判断
if len(sys.argv) < 2:
    print("Usage: httpdown.py <list> [dir] [proxy]")
    exit()

# 加载 URL 列表
try:
    fp = open(sys.argv[1], "r")
    lines = fp.readlines()
    fp.close()
except:
    print("invalid list file: " + sys.argv[1])
    exit()

# 抽出参数
root_dir = ""
root_path = None
if len(sys.argv) > 2:
    root_dir = sys.argv[2]
    root_path = root_dir
proxy_srv = ""
if len(sys.argv) > 3:
    proxy_srv = sys.argv[3]

# 循环处理 URL 项
idx = 0
cnt = len(lines)
delays = 0
while idx < cnt:
    urls = lines[idx].strip()
    if len(urls) > 7:
        # 判断磁盘是否已满
        free_bytes = ctypes.c_ulonglong(0)
        retc = ctypes.windll.kernel32.GetDiskFreeSpaceExW(ctypes.c_wchar_p(root_path), None, None, ctypes.pointer(free_bytes))
        if retc == 0:
            print("GetDiskFreeSpaceExW() failure")
            break
        disk_free = free_bytes.value / 1024 / 1024 / 1024
        print("\n============ DISK FREE: %.2f GB ============\n" % disk_free)
        if disk_free < 16:
            print("<<<<<<<<<<<< DISK FULL !!!!!!! >>>>>>>>>>>>")
            break
        # 拼装成下载命令行
        cmd_str = "aria2c "
        dir_len = len(root_dir)
        if dir_len != 0:
            cmd_str += "--dir="
            cmd_str += root_dir
            cmd_str += " "
            if root_dir[dir_len - 1] != "\\":
                root_dir += "\\"
        if len(proxy_srv) != 0:
            cmd_str += "--all-proxy="
            cmd_str += proxy_srv
            cmd_str += " "
        cmd_str += "\""
        cmd_str += urls
        cmd_str += "\""
        os.system(cmd_str)
        print("\n%s\n" % cmd_str)
        # 判断文件是否下载完毕
        spath = urls.split("/")
        fname = root_dir + spath[len(spath) - 1]
        sfile = fname + ".aria2"
        if (os.path.exists(sfile)) or (not os.path.exists(fname)):
            time.sleep(10 + delays)
            delays += 5
            continue
    idx += 1
    delays = 0
