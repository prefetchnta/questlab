#-*- coding: utf-8 -*-
##################################
#   彩票中奖测试脚本
#
#   创建时间: 2018-09-12
##################################

import sys

#######################################
#   体彩 6+1 有效判断
#######################################
def check_sixa1(num):
    if len(num) < 6 + 1 + 1:
        return False
    if num[0].isdigit() and \
       num[1].isdigit() and \
       num[2].isdigit() and \
       num[3].isdigit() and \
       num[4].isdigit() and \
       num[5].isdigit() and \
       num[7].isdigit() and num[6] == "+":
        return True
    return False

#######################################
#   体彩 6+1 中奖判断
#######################################
def lucky_sixa1(zhong, ur_num):
    if zhong == ur_num:
        print(ur_num + ": <T HIT !!!!!!!!!!!!>")
        return True
    if zhong[:6] == ur_num[:6]:
        print(ur_num + ": <1 HIT !!!!!!!!>")
        return True
    if zhong[:5] == ur_num[:5] or zhong[1:6] == ur_num[1:6]:
        print(ur_num + ": <2 HIT !!!!>")
        return True
    money = ("300", "20", "5")
    for idx in range(0, 3):
        leng = 4 - idx
        cnts = 3 + idx
        for beg in range(0, cnts):
            sub1 = zhong[beg:beg+leng]
            sub2 = ur_num[beg:beg+leng]
            if sub1 == sub2:
                print(ur_num + ": <" + money[idx] + " YUAN ^_^>")
                return True
    print(ur_num + ":")
    return False

#######################################
#   双色球有效判断
#######################################
def check_dball(num):
    if len(num) < 6 * 2 + 1 + 2:
        return False
    numbs = []
    if num[ 0].isdigit() and num[ 1].isdigit() and \
       num[ 2].isdigit() and num[ 3].isdigit() and \
       num[ 4].isdigit() and num[ 5].isdigit() and \
       num[ 6].isdigit() and num[ 7].isdigit() and \
       num[ 8].isdigit() and num[ 9].isdigit() and \
       num[10].isdigit() and num[11].isdigit() and \
       num[13].isdigit() and num[14].isdigit() and num[12] == "+":
        for idx in range(0, 12, 2):
            ball = int(num[idx:idx+2])
            if ball < 1 or ball > 33 or ball in numbs:
                return False
            numbs.append(ball)
        ball = int(num[13:15])
        if ball < 1 or ball > 16:
            return False
        return True
    return False

#######################################
#   双色球中奖判断
#######################################
def lucky_dball(zhong, ur_num):
    ll = 0
    rr = 0
    zhong_last = 0
    zhong_list = []
    ur_num_last = 0
    ur_num_list = []
    for idx in range(0, 12, 2):
        ball1 = int(zhong[idx:idx+2])
        ball2 = int(ur_num[idx:idx+2])
        zhong_list.append(ball1)
        ur_num_list.append(ball2)
    zhong_last = int(zhong[13:15])
    ur_num_last = int(ur_num[13:15])
    ii = 0
    while ii < len(ur_num_list):
        jj = 0
        while jj < len(zhong_list):
            if ur_num_list[ii] == zhong_list[jj]:
                ll += 1
                zhong_list[jj] = 0
                break
            jj += 1
        ii += 1
    if ur_num_last == zhong_last:
        rr += 1
    if (ll == 6 and rr == 1):
        print(ur_num + ": <1 HIT !!!!!!!!>")
        return True
    if (ll == 6):
        print(ur_num + ": <2 HIT !!!!>")
        return True
    if (ll == 5 and rr == 1):
        print(ur_num + ": <3000 YUAN ^_^>")
        return True
    if (ll == 5) or (ll == 4 and rr == 1):
        print(ur_num + ": <200 YUAN ^_^>")
        return True
    if (ll == 4) or (ll == 3 and rr == 1):
        print(ur_num + ": <10 YUAN ^_^>")
        return True
    if (rr == 1) or (ll == 1 and rr == 1) or (ll == 2 and rr == 1):
        print(ur_num + ": <5 YUAN ^_^>")
        return True
    print(ur_num + ":")
    return False

# 参数判断
if len(sys.argv) != 4:
    print("Usage: luck_chk.py <type> <number> <ur numbers>")
    exit()
luck_type = (sys.argv[1]).strip()
luck_numb = (sys.argv[2]).strip()
luck_file = (sys.argv[3]).strip()
try:
    fp = open(luck_file, "r")
    lines = fp.readlines()
    fp.close()
except:
    print("invalid number file: " + luck_file)
    exit()

# 根据类型判断
if luck_type == "sixa1":
    check_it = check_sixa1
    lucky_it = lucky_sixa1
elif luck_type == "dball":
    check_it = check_dball
    lucky_it = lucky_dball
else:
    print("invalid type: " + luck_type)
    exit()

# 统一判断中奖
if not check_it(luck_numb):
    print("invalid number: " + luck_numb)
    exit()
for numb in lines:
    numb = numb.strip()
    if not check_it(numb):
        print("invalid ur number: " + numb)
    else:
        lucky_it(luck_numb, numb)
