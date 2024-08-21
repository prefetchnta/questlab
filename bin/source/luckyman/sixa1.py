#-*- coding: utf-8 -*-
##################################
#   体彩 6+1 数据分析脚本
#
#   创建时间: 2021-10-20
##################################

import json, math, sys, time

#######################################
#   统计同号类型
#######################################
def group_type(numb):
    ilst = (0, 1, 2, 3, 4, 5, 7)
    cnts = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    for idx in ilst:
        cnts[int(numb[idx])] += 1
    sum2 = 0
    sum3 = 0
    for cnt in cnts:
        if cnt == 2:
            sum2 += 1
        elif cnt == 3:
            sum3 += 1
    return (sum2, sum3)

#######################################
#   统计相似度分类
#######################################
def simxy_type(vals, step, maxn):
    idx = 0
    cmpn = step
    while idx < maxn:
        if vals < cmpn:
            return idx
        idx += 1
        cmpn += step
    return idx

#######################################
#   计算号码平均概率
#######################################
def count_make(table, numb):
    freq = 0
    ilst = (0, 1, 2, 3, 4, 5, 7)
    for idx in ilst:
        freq += table[numb[idx]]
    return freq / 7

#######################################
#   频率数据分析
#######################################
def analyse0(table, item):
    # 频率计算
    numb = item["number"]
    ilst = (0, 1, 2, 3, 4, 5, 7)
    for idx in ilst:
        table[numb[idx]] += 1
    grp = group_type(numb)
    if grp[0] == 1 and grp[1] == 0:
        table["A02"] += 1
    elif grp[0] == 0 and grp[1] == 1:
        table["A03"] += 1
    elif grp[0] == 2 and grp[1] == 0:
        table["A22"] += 1
    elif grp[0] == 1 and grp[1] == 1:
        table["A23"] += 1

#######################################
#   号码数据分析
#######################################
def analyse1(table, item):
    # 和值计算
    numb = item["number"]
    sum  = int(numb[0])
    sum += int(numb[1])
    sum += int(numb[2])
    sum += int(numb[3])
    sum += int(numb[4])
    sum += int(numb[5])
    sum += int(numb[7])
    key  = str(sum)
    if key not in table:
        table[key] = 1
    else:
        table[key] += 1

#######################################
#   连号数据分析
#######################################
def analyse2(table, item, count):
    # 连号统计
    numb = item["number"]
    idx = 0
    max = 6 - count
    while idx <= max:
        key = str(idx) + numb[idx : idx + count]
        if key not in table:
            table[key] = 1
        else:
            table[key] += 1
        idx += 1
    # 特殊号码
    key = str(idx) + numb[idx : idx + count - 1] + numb[7:8]
    if key not in table:
        table[key] = 1
    else:
        table[key] += 1

#######################################
#   号码相似度分析
#######################################
def analyse3(item1, item2):
    numb1 = item1["number"]
    numb2 = item2["number"]
    ilst = (0, 1, 2, 3, 4, 5, 7)
    ex = 0
    ey = 0
    exy = 0
    dlt = 0
    tot = 0
    dv1 = 0
    dv2 = 0
    for idx in ilst:
        x = int(numb1[idx])
        y = int(numb2[idx])
        ex += x
        ey += y
        xx = x * x
        yy = y * y
        xy = x * y
        dlt += (x - y) * (x - y)
        tot += yy
        exy += xy
        dv1 += xx
        dv2 += yy
    # 欧氏相似度
    sim1 = 1 - math.sqrt(float(dlt) / tot)
    # 余弦相似度
    sim2 = exy / (math.sqrt(dv1 * dv2))
    # 皮尔逊相似度
    ax = float(ex) / 7
    ay = float(ey) / 7
    axy = float(exy) / 7
    axy -= ax * ay
    # 方差计算
    dv1 = 0
    dv2 = 0
    for idx in ilst:
        x = int(numb1[idx]) - ax
        y = int(numb2[idx]) - ay
        dv1 += x * x
        dv2 += y * y
    sdx = float(dv1) / 7
    sdy = float(dv2) / 7
    sim3 = axy / math.sqrt(sdx * sdy)
    return (sim1, sim2, sim3)

#######################################
#   主函数
#######################################
if __name__ == "__main__":

    # 加载数量文件
    prf = time.time()
    fp = open("sixa1.json", "r")
    text = fp.read()
    fp.close()
    list = json.loads(text)
    total = len(list)
    if len(sys.argv) > 1:
        used = int(sys.argv[1])
        if used > total or used <= 1:
            used = total
        list = list[:used]
    else:
        used = total
    print("number used: " + str(used))

    ############################# 和值分析 #############################

    # 逐行数据解析（全量）
    table1 = {}
    for item in list:
        analyse1(table1, item)
    table0 = sorted(table1.items(), key=lambda d: d[1], reverse=True)
    print("\n" + str(table0) + "\n")

    # 逐行数据解析（击中）
    table2 = {}
    for item in list:
        if item["hit"] != 0:
            analyse1(table2, item)
    table0 = sorted(table2.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    # 计算击中概率（击中数 / 全量数）
    table3 = {}
    for name in table1.keys():
        if name in table2:
            table3[name] = float(table2[name]) / table1[name]
    table0 = sorted(table3.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    ############################# 频率分析 #############################
    print("#############################################################\n")

    # 单数频率计算（全量）
    count1 = { "0":0, "1":0, "2":0, "3":0, "4":0, "5":0, "6":0, "7":0, "8":0, "9":0, "A02":0, "A03":0, "A22":0, "A23":0 }
    for item in list:
        analyse0(count1, item)
    table0 = sorted(count1.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    # 计算号码出现的整体概率
    sumx = count1["0"] + count1["1"] + count1["2"] + count1["3"] + count1["4"] + count1["5"] + count1["6"] + count1["7"] + count1["8"] + count1["9"]
    count1["0"] = float(count1["0"]) / sumx
    count1["1"] = float(count1["1"]) / sumx
    count1["2"] = float(count1["2"]) / sumx
    count1["3"] = float(count1["3"]) / sumx
    count1["4"] = float(count1["4"]) / sumx
    count1["5"] = float(count1["5"]) / sumx
    count1["6"] = float(count1["6"]) / sumx
    count1["7"] = float(count1["7"]) / sumx
    count1["8"] = float(count1["8"]) / sumx
    count1["9"] = float(count1["9"]) / sumx
    sumx = count1["A02"] + count1["A03"] + count1["A22"] + count1["A23"]
    count1["A02"] = float(count1["A02"]) / sumx
    count1["A03"] = float(count1["A03"]) / sumx
    count1["A22"] = float(count1["A22"]) / sumx
    count1["A23"] = float(count1["A23"]) / sumx
    table0 = sorted(count1.items(), key=lambda d: d[0], reverse=False)
    print(str(table0) + "\n")

    ############################# 连号分析 #############################
    print("#############################################################\n")

    # 全量2连号码统计
    count2 = {}
    for item in list:
        analyse2(count2, item, 2)
    table0 = sorted(count2.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    # 全量3连号码统计
    count3 = {}
    for item in list:
        analyse2(count3, item, 3)
    table0 = sorted(count3.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    # 全量4连号码统计
    count4 = {}
    for item in list:
        analyse2(count4, item, 4)
    table0 = sorted(count4.items(), key=lambda d: d[1], reverse=True)
    print(str(table0) + "\n")

    ############################ 自相似分析 ############################
    print("#############################################################\n")

    # 计算中奖号码自己跟自己的相似度
    idx1 = 0
    stot = len(list)
    scnt = stot - 1
    count0 = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
    stype = len(count0)
    sstep = 0.1 / stype
    while idx1 < stot:
        idx2 = 0
        simx = [0, 0, 0, 0]
        while idx2 < stot:
            if idx2 != idx1:
                simy = analyse3(list[idx1], list[idx2])
                simx[1] += simy[0]
                simx[2] += simy[1]
                simx[3] += simy[2]
            idx2 += 1
        simx[1] /= scnt
        simx[2] /= scnt
        simx[3] /= scnt
        simx[0] = (simx[1] * 2 + simx[2] + simx[3] * 15) / 18
        count0[simxy_type(simx[0], sstep, stype - 1)] += 1
        idx1 += 1
    print("step: " + str(sstep) + "\n")
    sumx = 0
    idx1 = 0
    while idx1 < stype:
        print("[%f, %f): %u" % (sumx, sumx + sstep, count0[idx1]))
        sumx += sstep
        idx1 += 1
    # 转换成概率
    sumx = 0
    idx1 = 0
    while idx1 < stype:
        sumx += count0[idx1]
        idx1 += 1
    idx1 = 0
    while idx1 < stype:
        count0[idx1] = float(count0[idx1]) / sumx
        idx1 += 1
    print("-------------------------------------")
    sumx = 0
    idx1 = 0
    while idx1 < stype:
        print("[%f, %f): %.15f" % (sumx, sumx + sstep, count0[idx1]))
        sumx += sstep
        idx1 += 1
    print("")

    ############################# 号码评分 #############################
    print("#############################################################\n")

    # 有号码文件则做评分
    try:
        fp = open("sixa1.txt", "r")
    except:
        exit(0)
    # 读取号码列表并评分
    table4 = {}
    table5 = {}
    table6 = {}
    while True:
        line = fp.readline()
        if not line:
            break
        # 抽取和值
        number = line[0:11]
        memo = line[11:].strip()
        sum = number[9:11]
        # 和值评分 = 全量次数
        if sum in table1:
            value = table1[sum]
        else:
            value = 0
        # 连号评分 = (2连次数 + 3连次数 * 4 + 4连次数 * 60)
        clist = ("0" + number[0:2], "1" + number[1:3], "2" + number[2:4], "3" + number[3:5], "4" + number[4:6], "5" + number[5:6] + number[7:8])
        cnt2 = 0
        for cnt in clist:
            if cnt in count2:
                cnt2 += count2[cnt]
        clist = ("0" + number[0:3], "1" + number[1:4], "2" + number[2:5], "3" + number[3:6], "4" + number[4:6] + number[7:8])
        cnt3 = 0
        for cnt in clist:
            if cnt in count3:
                cnt3 += count3[cnt]
        clist = ("0" + number[0:4], "1" + number[1:5], "2" + number[2:6], "3" + number[3:6] + number[7:8])
        cnt4 = 0
        for cnt in clist:
            if cnt in count4:
                cnt4 += count4[cnt]
        # 最终评分 = 和值评分 + 连号评分
        value += cnt2 + cnt3 * 4 + cnt4 * 60
        sims = json.loads(memo)
        val1 = count0[simxy_type(sims[0], sstep, stype - 1)]
        val2 = count_make(count1, number)
        # 备注号码类型
        grp = group_type(number)
        if grp[0] == 1 and grp[1] == 0:
            tags = " A02"
        elif grp[0] == 0 and grp[1] == 1:
            tags = " A03"
        elif grp[0] == 2 and grp[1] == 0:
            tags = " A22"
        elif grp[0] == 1 and grp[1] == 1:
            tags = " A23"
        else:
            tags = " XXX"
        table4[number] = value
        table5[number] = tags
        table6[number] = memo + " <" + str(val1) + ", " + str(val2) + ">"
    fp.close()
    table0 = sorted(table4.items(), key=lambda d: d[1], reverse=True)
    for item in table0:
        print(str(item) + table5[item[0]] + " " + table6[item[0]])
    print(time.time() - prf)
