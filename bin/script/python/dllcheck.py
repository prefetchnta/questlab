#-*- coding: utf-8 -*-
##################################
#   DLL 兼容性测试脚本
#
#   创建时间: 2018-01-04
##################################

import os

dirs = {}
dlls = []
okays = 0
count = 0
max_len = 0
ext_list = [".dll", ".bpl", ".pyd"]
nop_list = []
for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    if dirpath.find(".\\x64bin") >= 0 or \
       dirpath.find(".\\source\\ffmpeg") >= 0:
        continue
    for filename in filenames:
        pth, fnm = os.path.split(filename)
        if fnm.lower() in nop_list:
            continue
        ext = os.path.splitext(filename)[1].lower()
        if ext not in ext_list:
            continue
        if dirpath not in dirs:
            dirs[dirpath] = True
            os.environ["PATH"] = os.environ["PATH"] + ";" + dirpath
        full_path = dirpath + "/" + filename
        leng = len(full_path)
        if max_len < leng:
            max_len = leng
        dlls.append(full_path)
path_base = os.environ["PATH"]
for full_path in dlls:
    leng = len(full_path)
    if leng >= max_len:
        leng = 1
    else:
        leng = max_len - leng + 1
    os.environ["PATH"] = os.path.dirname(full_path) + ";" + path_base
    cmd_str = "sbinload.exe %u %s" % (leng, full_path)
    if os.system(cmd_str) == 0:
        okays += 1
    count += 1
print("TOTAL:", count, "OKAY:", okays, "FAIL:", count - okays)
input()
