#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 创建时间:2018/6/25
# 文件名:CDataBase.py
import sys
print(sys.version)

import mysql.connector

class CSqlForChat():
    def __init__(self):
        #1.链接数据库
        config={'host':'192.168.221.128',
                'user':'root',
                'password':'12345',
                'port':'3306',
                'database':'chat',
                'charset':'utf8'}
        print("正在链接数据库...")
        self.conn=mysql.connector.connect(**config)
        print("数据库链接成功！")
    def __del__(self):
        self.conn.close()
    def query(self,szSql,param=None):
        cursor=self.conn.cursor()
        try:
            cursor.execute(szSql,param)
            result=(cursor.fetchall(),cursor.rowcount)
            cursor.close()
            return result
        except:
            cursor.close()
            #发生错误时回滚
            self.conn.rollback()
            return None
    def insert(self,szSql,param=None):
        cursor=self.conn.cursor()
        try:
            cursor.execute(szSql,param)
            result=cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        except Exception as info:
            print(info)
            #发生错误时回滚
            cursor.close()
            self.conn.rollback()
            return None
