#-*- coding: utf-8 -*-
##################################
#   压缩包批量测试脚本
#
#   创建时间: 2022-07-18
##################################

import os, sys, time

okays = 0
count = 0
chd_cmd_str = 'chdman.exe verify -i "'
rar_cmd_str = 'Rar.exe t "'
zip_cmd_str = '7z.exe t "'
if len(sys.argv) >= 2:
    rar_cmd_str += '-p%s "' % sys.argv[1]
    zip_cmd_str += '-p%s "' % sys.argv[1]
if os.path.exists("__list__.txt"):
    os.remove("__list__.txt")
for dirpath, dirnames, filenames in os.walk("."):
    if len(filenames) == 0:
        continue
    for filename in filenames:
        ext = os.path.splitext(filename)[1].lower()
        if ext == ".chd":
            cmd_str = chd_cmd_str
        elif ext == ".rar":
            find_part = filename.lower()
            if find_part.count(".part") == 1:
                find_leng = len(find_part) - 5
                if find_part.rfind("1.rar") != find_leng:
                    continue
                find_leng -= 1
                if find_part[find_leng] != 't' and find_part[find_leng] != '0':
                    continue
            cmd_str = rar_cmd_str
        elif ext == ".001":
            find_part = filename.lower()
            if find_part.count(".7z.001") != 1 and find_part.count(".zip.001") != 1:
                continue
            cmd_str = zip_cmd_str
        elif ext == ".zip" or ext == ".7z":
            cmd_str = zip_cmd_str
        else:
            continue
        full_path = dirpath + "\\" + filename
        base = time.time()
        if os.system(cmd_str + full_path + '"') == 0:
            okays += 1
            full_path += " [OKAY] "
        else:
            full_path += " [FAIL] "
        delta = time.time() - base
        if delta < 10:
            full_path += str(int(delta * 1000)) + "(ms)"
        else:
            full_path += str(int(delta + 0.5)) + "(S)"
        print("######################################################################")
        print(full_path);
        print("######################################################################")
        fp = open("__list__.txt", "a")
        fp.write(full_path + "\n")
        fp.close()
        count += 1
print("\nTOTAL:", count, "OKAY:", okays, "FAIL:", count - okays)
