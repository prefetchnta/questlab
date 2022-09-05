#-*- coding: utf-8 -*-
##################################
#   PYD 库迁移脚本
#
#   创建时间: 2022-03-15
##################################

import os, sys

# 参数判断
if len(sys.argv) != 4:
    print("Usage: pyd2pyd.py <path> <src ver> <dst ver>")
    exit()
cnts = 0
root = sys.argv[1]
sver = ".cp" + sys.argv[2]
dver = ".cp" + sys.argv[3]

# 更改指定版本号文件到目标版本号（SVN）
for dirpath, dirnames, filenames in os.walk(root):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        if (os.path.splitext(filename))[1].lower() != ".pyd":
            continue
        idx = filename.find(sver)
        if idx <= 0:
            continue
        spath = dirpath + "/" + filename
        print(spath)
        dpath = dirpath + "/" + filename.replace(sver, dver, 1)
        cmdstr = "svn -q mv %s %s" % (spath, dpath)
        os.system(cmdstr)
        cnts += 1
print(cnts)
