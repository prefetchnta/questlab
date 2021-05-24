#-*- coding: utf-8 -*-
##################################
#   X64 插件服务退出脚本
#
#   创建时间: 2021-05-24
##################################

import socket

netw = socket.socket(type=socket.SOCK_DGRAM)
netw.sendto(b"\xAA\x55", ("127.0.0.1", 12345))
netw.sendto(b"\xAA\x55", ("127.0.0.1", 12350))
