#-*- coding: utf-8 -*-
##################################
#   电池放电容量计算脚本
#
#   创建时间: 2019-02-14
##################################

import json, sys

# 参数判断
if len(sys.argv) != 2:
    print("Usage: battery.py <ohm>")
    exit()
ohm = float(sys.argv[1])

# 电压列表
try:
    fp = open("voltage.json", "r")
    text = fp.read()
    fp.close()
    vlst = json.loads(text)
    num = len(vlst)
except:
    print("invalid voltage.json")
    exit()

# 计算容量
if num <= 1:
    print("invalid voltage.json")
    exit()
idx = 0
num -= 1
power = 0.0
while idx < num:
    total = (vlst[idx + 1])["vols"] + (vlst[idx])["vols"]
    power += (total * (vlst[idx + 1])["time"])
    idx += 1
ohm *= 7.2
print("capacity: %.2f mAh" % (power / ohm))
