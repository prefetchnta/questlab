#-*- coding: utf-8 -*-
##################################
#   压缩包批量更名脚本
#
#   创建时间: 2022-08-02
##################################

import os

for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        ext = os.path.splitext(filename)[1].lower()
        if ext == ".rar" or ext == ".zip" or ext == ".7z":
            full_path = dirpath + "\\" + filename
            base = os.path.splitext(full_path)[0]
            fp = open(full_path, "rb")
            hdr = fp.read(4)
            fp.close()
            if hdr == b"\x52\x61\x72\x21":
                if ext != ".rar":
                    os.rename(full_path, base + ".rar")
                    print("[%s] is RAR" % full_path)
            elif hdr == b"\x50\x4B\x03\x04":
                if ext != ".zip":
                    os.rename(full_path, base + ".zip")
                    print("[%s] is ZIP" % full_path)
            elif hdr == b"\x37\x7A\xBC\xAF":
                if ext != ".7z":
                    os.rename(full_path, base + ".7z")
                    print("[%s] is 7-ZIP" % full_path)
