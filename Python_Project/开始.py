#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 创建时间:2018/6/24
# 文件名:CserverStart.py
import sys
print(sys.version)

from CServer  import *
from CDataBase import *

HOST = '127.0.0.1'#服务器ip
PORT = 1234

def main():
    #启动服务器等待客户端连接
    server = CServerSocket(HOST,PORT)
    server.MyAccept()
    #下面的代码是和数据库交互用的
    sql = CSqlForChat()
    while True:
        print("1.查询 2.添加")
        index = int(input())
        if index == 1:
            result = sql.query(input("请输入查询语句:"))
            print(result[0])
        elif index == 2:
            result = sql.insert(input("请输入插入语句:"))
            if result[1] == 1:
                print("添加成功")
            pass


if __name__ == '__main__':
    main()