#-*- coding: utf-8 -*-
##################################
#   DLL 兼容性测试脚本
#
#   创建时间: 2018-01-04
##################################

import os
from ctypes import *

okays = 0
count = 0
sbin = WinDLL("KERNEL32.dll")
sbin.LoadLibraryA.restype = c_void_p
sbin.LoadLibraryA.argtypes = [c_char_p]
ext_list = [".dll", ".bpl", ".pyd"]
for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        ext = os.path.splitext(filename)[1].lower()
        if ext not in ext_list:
            continue
        full_path = dirpath + "/" + filename
        leng = 70 - len(full_path)
        sdll = sbin.LoadLibraryA(full_path.encode(encoding="gb2312"))
        if sdll == None:
            print(full_path, "-" * leng, "[FAIL]")
        else:
            okays += 1
            print(full_path, "#" * leng, "[OKAY]")
        count += 1
print("TOTAL:", count, "OKAY:", okays, "FAIL:", count - okays)
input()
