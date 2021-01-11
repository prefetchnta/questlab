#-*- coding: utf-8 -*-
##################################
#   模拟器 ROM 打包脚本
#
#   创建时间: 2020-01-23
##################################

import os

count = 0
ext_list = [".a26", ".a78", ".lnx", ".col", ".rom", ".nes", ".fds", ".unf",
            ".smc", ".sfc", ".bin", ".smd", ".md",  ".mdx", ".gen", ".32x",
            ".gba", ".gbc", ".gb",  ".gg",  ".sms", ".sg",  ".int", ".itv",
            ".ngc", ".z64", ".n64", ".v64", ".nds", ".chd", ".pce", ".bs",
            ".fig", ".mgd", ".swc", ".gam", ".vec", ".vb"]
for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        ext = os.path.splitext(filename)[1].lower()
        if ext not in ext_list:
            continue
        full_path = '"' + dirpath + "\\" + filename + '"'
        fzip_path = '"' + dirpath + "\\" + os.path.splitext(filename)[0] + '.zip"'
        os.system("@zip -9 -j " + fzip_path + " " + full_path)
        os.system("@del /Q " + full_path)
        count += 1
print("TOTAL:", count)
input()
