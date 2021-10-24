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

	void close(); // �رշ��������ر�MYSQL

	void free(); // �ͷŽ����

	bool isPing(); // MYSQL�Ƿ�������״̬

	bool init(SSqlParamSign& sParamTemp); // ��ʼ��MYSQL����½MYSQL

	bool query(string strQuery); // ��������

	int getColumn(); // ��ȡ��ǰ�����������

	string getQueryLine(string strFind); // ��ȡ��һ���ҵ�����������

	string getQueryLine(string strFind, int iColumn); // ���������л�ȡ��һ���ҵ�����������

	vector<string>getQueryAll(); // ��ȡ������

	vector<string>getQueryAll(string strFind);// ��ȡ���а���������

	vector<string>getQueryAll(string strFind, int iColumn); // ���������л�ȡ���а���������

	MYSQL_ROW getFetchRow(); // �Զ����ȡ�����

public:
	SSqlParamSign m_SParamSign;

	string m_strSqlError; // ����ԭ��

	MYSQL_ROW SqlRow; // �Զ�����ս����

private:
	MYSQL SqlClass;

	MYSQL_RES* pSqlRes;
};

