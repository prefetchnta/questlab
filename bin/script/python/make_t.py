#-*- coding: utf-8 -*-
##################################
#   MakeT 计算脚本
#
#   创建时间: 2020-07-28
##################################

M1 = float(input("money hi: "))
C1 = float(input("value hi: "))
F1 = M1 / C1
print("count hi: %f" % F1)
M2 = float(input("money lo: "))
C2 = float(input("value lo: "))
if M2 <= 0 and C2 <= 0:
    exit()
if M2 <= 0:
    C3 = float(input("value mm: "))
    M2 = (C1 - C3) * F1 * C2 / (C3 - C2)
    print("money lo: %f" % M2)
elif C2 <= 0:
    C3 = float(input("value mm: "))
    C2 = C3 * M2 / (C1 * F1 - C3 * F1 + M2)
    print("value lo: %f" % C2)
else:
    F2 = M2 / C2
    print("count lo: %f" % F2)
    C3 = (C1 * F1 + C2 * F2) / (F1 + F2)
    print("value mm: %f" % C3)
