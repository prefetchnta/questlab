#-*- coding: utf-8 -*-
##################################
#   RAR 批量测试脚本
#
#   创建时间: 2022-07-18
##################################

import os, sys

okays = 0
count = 0
flist = "FAILED.TXT"
if len(sys.argv) < 2:
    zip_cmd_str = '7z.exe t "'
    rar_cmd_str = 'Rar.exe t "'
else:
    zip_cmd_str = '7z.exe t -p%s "' % sys.argv[1]
    rar_cmd_str = 'Rar.exe t -p%s "' % sys.argv[1]
if os.path.exists(flist):
    os.remove(flist)
os.environ["PATH"] = "C:\\Program Files\\7-Zip;" + os.environ["PATH"]
os.environ["PATH"] = "C:\\Program Files\\WinRAR;" + os.environ["PATH"]
for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        ext = os.path.splitext(filename)[1].lower()
        if ext == ".rar":
            find_part = filename.lower()
            if find_part.count(".part") == 1:
                find_leng = len(find_part) - 5
                if find_part.rfind("1.rar") != find_leng:
                    continue
                find_leng -= 1
                if find_part[find_leng] != 't' and find_part[find_leng] != '0':
                    continue
            cmd_str = rar_cmd_str
        elif ext == ".zip" or ext == ".7z":
            cmd_str = zip_cmd_str
        elif ext == ".001":
            find_part = filename.lower()
            if find_part.count(".7z.001") != 1 and find_part.count(".zip.001") != 1:
                continue
            cmd_str = zip_cmd_str
        else:
            continue
        full_path = dirpath + "\\" + filename
        if os.system(cmd_str + full_path + '"') == 0:
            okays += 1
        else:
            fp = open(flist, "a")
            fp.write(full_path + "\n")
            fp.close()
        count += 1
print("\nTOTAL:", count, "OKAY:", okays, "FAIL:", count - okays)
