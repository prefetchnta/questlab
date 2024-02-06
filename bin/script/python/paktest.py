#-*- coding: utf-8 -*-
##################################
#   压缩包批量测试脚本
#
#   创建时间: 2022-07-18
##################################

import os, sys, time

okays = 0
count = 0
flist = "FLIST.TXT"
NSCB_LIST = "__NS__.TXT"
NSCB_ROOT = "D:\\PRJ\\GamePlay\\HACK-NS\\SAK\\bin"
if len(sys.argv) < 2:
    zip_cmd_str = '7z.exe t "'
    rar_cmd_str = 'Rar.exe t "'
else:
    zip_cmd_str = '7z.exe t -p%s "' % sys.argv[1]
    rar_cmd_str = 'Rar.exe t -p%s "' % sys.argv[1]
chd_cmd_str = 'chdman.exe verify -i "'
sak_cmd_str = 'squirrel.exe -v . -vt lv3 -tfile ' + NSCB_LIST
if os.path.exists(flist):
    os.remove(flist)
os.environ["PATH"] = "C:\\Program Files\\7-Zip;" + os.environ["PATH"]
os.environ["PATH"] = "C:\\Program Files\\WinRAR;" + os.environ["PATH"]
os.environ["PATH"] = "D:\\PRJ\\GamePlay\\MAME;" + os.environ["PATH"]
os.environ["PATH"] = NSCB_ROOT + ";" + os.environ["PATH"]
if not os.path.exists("DB"):
    os.system("mklink /J DB " + NSCB_ROOT + "\\DB")
if not os.path.exists("keys.txt"):
    os.system("copy " + NSCB_ROOT + "\\prod.keys keys.txt")
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
        elif ext == ".chd":
            cmd_str = chd_cmd_str
        elif ext == ".nsp" or ext == ".nsx" or ext == ".nsz" or ext == ".xci" or ext == ".xcz" or ext == ".nca":
            full_path = dirpath + "\\" + filename
            fp = open(NSCB_LIST, "wb")
            fp.write(full_path.lower().encode("utf-8"))
            fp.close()
            os.system(sak_cmd_str)
            continue
        else:
            continue
        full_path = dirpath + "\\" + filename
        base = time.time()
        if os.system(cmd_str + full_path + '"') == 0:
            okays += 1
            full_path += " [OKAY] "
        else:
            full_path += " [FAIL] "
        delta = int(time.time() - base + 0.5)
        full_path += str(delta) + "S"
        print(full_path);
        fp = open(flist, "a")
        fp.write(full_path + "\n")
        fp.close()
        count += 1
if os.path.exists(NSCB_LIST):
    os.remove(NSCB_LIST)
if os.path.exists("keys.txt"):
    os.remove("keys.txt")
if os.path.exists("DB"):
    os.rmdir("DB")
print("\nTOTAL:", count, "OKAY:", okays, "FAIL:", count - okays)
