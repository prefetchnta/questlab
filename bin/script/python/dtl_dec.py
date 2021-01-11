#-*- coding: utf-8 -*-
##################################
#   驱动管家数据库解密脚本
#
#   创建时间: 2018-10-11
##################################

fp = open("Driver.db", "rb")
bin = fp.read()
fp.close()
tbl = (0x5A, 0x95, 0x55, 0x2D, 0x04, 0x44, 0x96, 0x22, 0x51, 0x47, 0x3A, 0x29, 0x51, 0x29, 0xE5, 0x2C)
idx = 0
tsz = len(tbl)
fsz = len(bin)
dec = bytearray(fsz)
while idx < fsz:
    dec[idx] = bin[idx] ^ tbl[idx % tsz]
    idx += 1
fp = open("Driver1.db", "wb")
fp.write(dec)
fp.close()
