#define  _CRT_SECURE_NO_WARNINGS

#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <list>
#include <direct.h>
#include <io.h>

using namespace std;

#define CBINARYWRITEINI "F:\\WriteIni"

class CMyIni
{
public:
	CMyIni();

	~CMyIni();

	bool IsError();

	bool open(string strDataPath);

	bool open(string strDataPath, string strCbinaryIndex);

	bool setWriteIni(string strDataPath);

	void close();

	void setHead(string strIniHead);

	void setChildText(string strIniChildHead, string strText);

	void setGroupChildText(string strIniChildHead, vector<string>vctText);

	void setAllGroupChild(vector<pair<string, string>>pvctHeadText);

	void setAllGroupChild(vector<pair<string, vector<string>>>pvctHeadText);

	void setAllGroupChild(map<string, string>mapHeadText);

	void setAllGroupChild(map<string, vector<string>>mapHeadText);

	void save(string strDataPath = "");

	int getInt(string strIniHead, string strIniChild);

	double getDouble(string strIniHead, string strIniChild);

	string getText(string strIniHead, string strIniChild);

	vector<string>getGroupText(string strIniHead, string strIniChild);

	vector<pair<string, string>>getSecionsVct(string strIniHead);

	vector<pair<string, vector<string>>>getGroupSecionsVct(string strIniHead);

	map<string, string>getSecionsMap(string strIniHead);

	map<string, vector<string>>getGroupSecionsMap(string strIniHead);

private:
	void WriteTxt(string strDataPath, string strText, bool bIsJudgeExist = true);

	string TrimSpaceTab(string strTrim);

	vector<string>SplitComma(string strTrim);

private:
	FILE* m_pFile;

	string m_strDataPath;

	vector<string>m_vctChildHeadText;

public:
	string m_strError;
	string m_strSaveIni;
};



