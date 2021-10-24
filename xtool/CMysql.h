#pragma once

#include <iostream>
#include <mysql.h>
#include <string>
#include <vector>
#include <map>
#include <list>


using namespace std;

#define CMYSQL _declspec(dllexport)

struct CMYSQL SSqlParamSign
{
	string strHost;
	string strUser;
	string strPassword;
	string strDbName;

	int iPort = -1;

	bool empty()
	{
		if ((strHost.empty()) || (strUser.empty()) || (strPassword.empty()) || (strDbName.empty()) || (-1 == iPort))
		{
			return true;
		}

		return false;
	}
};

class CMYSQL CMysql
{
public:
	CMysql();

	~CMysql();

	void close(); // 关闭服务器，关闭MYSQL

	void free(); // 释放结果集

	bool isPing(); // MYSQL是否处于在线状态

	bool init(SSqlParamSign& sParamTemp); // 初始化MYSQL，登陆MYSQL

	bool query(string strQuery); // 传入命令

	int getColumn(); // 获取当前表的所有列数

	string getQueryLine(string strFind); // 获取第一次找到包含串的行

	string getQueryLine(string strFind, int iColumn); // 根据所在列获取第一次找到包含串的行

	vector<string>getQueryAll(); // 获取所有行

	vector<string>getQueryAll(string strFind);// 获取所有包含串的行

	vector<string>getQueryAll(string strFind, int iColumn); // 根据所在列获取所有包含串的行

	MYSQL_ROW getFetchRow(); // 自定义获取结果集

public:
	SSqlParamSign m_SParamSign;

	string m_strSqlError; // 错误原因

	MYSQL_ROW SqlRow; // 自定义接收结果集

private:
	MYSQL SqlClass;

	MYSQL_RES* pSqlRes;
};

