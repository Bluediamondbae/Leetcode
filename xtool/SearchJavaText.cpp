#define _CRT_SECURE_NO_DEPRECATE
#pragma once
#include <iostream>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <vector>
#include <set>
#include <fstream>
#include <stack>
#include <queue>
#include<malloc.h>
//#include <hash_map>
#include <io.h>
#include <map>
#include <sstream>
#include <atltime.h>
#include <algorithm>
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include<thread>
#include <iomanip>

using namespace std;
using namespace pugi;

namespace SearchJavaText
{
	set<pair<string, string>>setCallID;

	string TrimSpace(string& str)
	{
		int iNdex = 0;
		if (!str.empty())
		{
			while ((iNdex = str.find(' ', iNdex)) != string::npos)
			{
				str.erase(iNdex, 1);
			}
		}
		return str;
	}

	string GetAllFileText(string strFilePath)  // 读取全部内容到字符串中，效率高
	{
		FILE* pFile = fopen(strFilePath.c_str(), "r");
		string strText;
		int iCount = 0;

		if (pFile)
		{
			char* chText;  //要获取的字符串  
			int iLength = 0;   //获取的长度  

			fseek(pFile, 0, SEEK_END);  //移到尾部  
			iLength = ftell(pFile);        //提取长度  
			rewind(pFile);             //回归原位  

			//分配buf空间  
			chText = (char*)malloc(sizeof(char) * iLength + 1);
			if (!chText)
			{
				return NULL;
			}

			//读取文件  
			//读取进的buf，单位大小，长度，文件指针  
			iLength = fread(chText, sizeof(char), iLength, pFile);
			chText[iLength] = '\0';

			if (iLength != strlen(chText))
			{
				return NULL;
			}

			strText = chText;

			fclose(pFile);
			free(chText);

			return strText;
		}

		return NULL;
	}
	void GetLineFileText(string strFileData, string strIndex, vector<string>& vctStrText)  // 逐行读取
	{
		FILE* pFile = NULL;

		char* chLine = NULL;

		int iFileLen = 0;
		pFile = fopen(strFileData.c_str(), "r");
		if (NULL == pFile)
		{
			return;
		}

		fseek(pFile, 0L, SEEK_END); // 移动指针至文件尾部
		iFileLen = ftell(pFile); // 计算文件长度
		chLine = (char*)malloc(iFileLen + 1); // 获取内存空间 长度???
		memset(chLine, 0, iFileLen + 1); // 初始化内存空间
		fseek(pFile, 0L, SEEK_SET); // 移动指针至文件头部

		while (NULL != fgets(chLine, iFileLen, pFile)) // 逐行读取
		{
			string strTemp;
			strTemp += chLine;

			if (string::npos != strTemp.find(strIndex))
			{
				vctStrText.push_back(strTemp);
			}
		}

		// 释放
		if (NULL != pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		if (chLine)
		{
			free(chLine);
		}
	}
	bool IsCallId(string strText, string& strCallName, string& strCall)  // 是否是Call ?
	{
		string strGetCall = "Call_";
		string strGetCalllower = "call_";
		string strGetText = "localIVaudasTestingFactory.createTestNodeId";
		string strgetTextNodeId = "createTestNodeId";

		if (string::npos != strText.find(strGetText))
		{
			if (string::npos != strText.find(strGetCall))
			{
				int iGetTextPos = 0;
				int iSignPos = 0;
				string strTemp;
				strTemp = strText;

				iGetTextPos = strTemp.find(strGetCall);
				strTemp.erase(strTemp.begin(), strTemp.begin() + iGetTextPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());
				strCallName = strTemp;   // 取Call 

				strTemp = strText;
				iGetTextPos = 0;
				iSignPos = 0;

				iGetTextPos = strTemp.find(strgetTextNodeId);
				strTemp = strTemp.substr(iGetTextPos + strgetTextNodeId.size(), strTemp.size() - 1);
				iSignPos = strTemp.find("(");

				char chTemp = strTemp.at(iSignPos + 2);
				if (0 != isdigit(strTemp.at(iSignPos + 2)))  // 取ID
				{
					iSignPos = strTemp.find('"');
					strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
					iSignPos = strTemp.find('"');
					strCall = strTemp.substr(0, iSignPos);
					return true;
				}
			}

			if (string::npos != strText.find(strGetCalllower))  // 能不能找到Call_
			{
				int iGetTextPos = 0;
				int iSignPos = 0;
				string strTemp;
				strTemp = strText;

				iGetTextPos = strTemp.find(strGetCalllower);
				strTemp.erase(strTemp.begin(), strTemp.begin() + iGetTextPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());
				strCallName = strTemp;   // 取Call 

				strTemp = strText;
				iGetTextPos = 0;
				iSignPos = 0;

				iGetTextPos = strTemp.find(strgetTextNodeId);
				strTemp = strTemp.substr(iGetTextPos + strgetTextNodeId.size(), strTemp.size() - 1);
				iSignPos = strTemp.find("(");

				char chTemp = strTemp.at(iSignPos + 2);
				if (0 != isdigit(strTemp.at(iSignPos + 2)))  // 取ID  是否是数字
				{
					iSignPos = strTemp.find('"');
					strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
					iSignPos = strTemp.find('"');
					strCall = strTemp.substr(0, iSignPos);
					return true;
				}
			}
		}

		return false;
	}
	bool IsStepId(string strText, string& strStepName, string& strStep)  // 是否是Step ?
	{
		string strGetText = "getGfsXmlStringHandler().getTextIdForId";
		string strgetTextIdForId = "getTextIdForId";
		if (string::npos != strText.find(strGetText))
		{
			int iGetTextPos;
			int iSignPos;
			string strTemp;
			strTemp = strText;
			iGetTextPos = strTemp.find(strgetTextIdForId);
			strTemp = strTemp.substr(iGetTextPos + strgetTextIdForId.size(), strTemp.size() - 1);
			iSignPos = strTemp.find("(");

			char chTemp = strTemp.at(iSignPos + 2);  // "allgsystemtexte", "JA", || "08004521800472c7"
			if (0 != isdigit(strTemp.at(iSignPos + 2)))  // 取ID
			{
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
				iSignPos = strTemp.find('"');
				strStep = strTemp.substr(0, iSignPos);

				strStepName = "step";
				return true;
			}
		}

		return false;
	}
	set<pair<string, string>>GetAllJavaID(string strJavaPath) // JAVA文件中截取去重后的JAVA文本ID
	{
		//set<string>setJavaID;
		set<pair<string, string>>setJavaID;
		string strStepName;
		string strCallName;
		string strStep;
		string strCall;

		FILE* pFile = NULL;

		char* chLine = NULL;

		int iFileLen = 0;
		pFile = fopen(strJavaPath.c_str(), "r");
		if (NULL == pFile)
		{
			return setJavaID;
		}

		fseek(pFile, 0L, SEEK_END);
		iFileLen = ftell(pFile);
		chLine = (char*)malloc(iFileLen + 1);
		memset(chLine, 0, iFileLen + 1);
		fseek(pFile, 0L, SEEK_SET);

		int iTemp = 0;
		while (NULL != fgets(chLine, iFileLen, pFile)) // 逐行读取
		{
			string strTemp;
			strTemp += chLine;

			if (IsStepId(strTemp, strStepName, strStep))
			{
				//setJavaID.insert(strStep);
				setJavaID.insert(make_pair(strStepName, strStep));
			}
			if (IsCallId(strTemp, strCallName, strCall))
			{
				setCallID.insert(make_pair(strCallName, strCall));
				//setJavaID.insert(strCall);
				setJavaID.insert(make_pair(strCallName, strCall));
			}
		}

		if (NULL != pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		if (chLine)
		{
			free(chLine);
		}

		return setJavaID;
	}
	string GetStepOrSelectName(string strLineText)
	{
		string strValue;
		string strTemp;
		strTemp = strLineText;

		if (string::npos != strTemp.find("getTestmodulePossibleNodes().put"))
		{
			if ((string::npos != strTemp.find("Select_")))
			{
				int iSelectPos = 0;
				int iSignPos = 0;

				iSelectPos = strTemp.find("Select_");
				strTemp.erase(strTemp.begin(), strTemp.begin() + iSelectPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());

				return strTemp;
			}

			if ((string::npos != strTemp.find("select_")))
			{
				int iSelectPos = 0;
				int iSignPos = 0;

				iSelectPos = strTemp.find("select_");
				strTemp.erase(strTemp.begin(), strTemp.begin() + iSelectPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());

				return strTemp;
			}

			if ((string::npos != strTemp.find("Step_")))
			{
				int iStepPos = 0;
				int iSignPos = 0;

				iStepPos = strTemp.find("Step_");
				strTemp.erase(strTemp.begin(), strTemp.begin() + iStepPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());

				return strTemp;
			}

			if ((string::npos != strTemp.find("step_")))
			{
				int iStepPos = 0;
				int iSignPos = 0;

				iStepPos = strTemp.find("step_");
				strTemp.erase(strTemp.begin(), strTemp.begin() + iStepPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());

				return strTemp;
			}
		}

		return strValue;
	}
	void GetStepOrSelectJavaID(string strJavaPath, set<pair<string, string>>& setStepJavaID, vector<string>& vctStep)
	{
		string strStepIdFord;
		string strStep;

		FILE* pFile = NULL;

		char* chLine = NULL;

		int iFileLen = 0;
		pFile = fopen(strJavaPath.c_str(), "r");
		if (NULL == pFile)
		{
			return;
		}

		fseek(pFile, 0L, SEEK_END);
		iFileLen = ftell(pFile);
		chLine = (char*)malloc(iFileLen + 1);
		memset(chLine, 0, iFileLen + 1);
		fseek(pFile, 0L, SEEK_SET);

		int iTemp = 0;
		while (NULL != fgets(chLine, iFileLen, pFile)) // 逐行读取
		{
			string strStepName;
			string strTemp;
			strTemp += chLine;

			if (IsStepId(strTemp, strStepName, strStep))
			{
				setStepJavaID.insert(make_pair(strStepName, strStep));
			}

			strStepName = GetStepOrSelectName(strTemp);  // 获取Step 或者 Select名

			if (!strStepName.empty())
			{
				//unmapStepID[strStepName].push_back("");
				if (vctStep.end() == find(vctStep.begin(), vctStep.end(), strStepName))
				{
					vctStep.push_back(strStepName);
				}
			}
		}

		if (NULL != pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		if (chLine)
		{
			free(chLine);
		}

		return;
	}
	string GetIDString(string strText, set<pair<string, string>>setJavaID, vector<pair<string, vector<string>>>pvctStep)  // DIDB中截取JAVA文本ID的串
	{
		//list<string>listIDString;
		string strValue;

		for (set<pair<string, string>>::iterator it = setJavaID.begin(); it != setJavaID.end(); it++)
		{
			string strTrand;
			string strTemp;
			strTemp = strText;

			if ((string::npos != it->first.find("Call_")) || (string::npos != it->first.find("call_")))
			{
				if (string::npos != strTemp.find(it->second))
				{
					string strTran = strTemp;
					int iJavaIdCout = 0;
					while (string::npos != strTran.find(it->second))
					{
						iJavaIdCout++;
						int iPos = strTran.find(it->second);
						strTran.erase(strTran.begin(), strTran.begin() + iPos + it->second.size());
					}

					while (0 != iJavaIdCout)
					{
						int iTemp = 0;
						int iPosTemp = 0;
						string strIDText;
						string strTran;
						iPosTemp = strTemp.find(it->second);

						if (-1 == iPosTemp)
						{
							break;
						}
						strTemp.erase(strTemp.begin(), strTemp.begin() + iPosTemp);
						iTemp = strTemp.find('\n');
						strTran = strTemp.substr(0, iTemp);

						strTemp.erase(0, iTemp + 1);
						strIDText += strTran;
						strTrand += strIDText;
						//listIDString.push_back(strIDText);
						iJavaIdCout--;
					}

					strTrand = "----------------" + it->first + "----------------" + "\r" + strTrand + "\n";
					strValue.append("\n" + strTrand);
				}
			}
			else
			{
				strTemp = strText;
				vector<string>vctStepText;
				string strFileID = "F:\\引导开发\\JAVA内容文本ID中文查询\\大众\\didb_GFS-v.zh_CN.TAB";

				int iTextIDPos = 0;
				int iIndexPos = 0;
				iTextIDPos = strTemp.find(it->second);
				strTemp.erase(strTemp.begin(), strTemp.begin() + iTextIDPos);

				while (string::npos != strTemp.find(it->second))
				{
					string strTrand;
					int iTextPos = strTemp.find(it->second);
					int iSignPos = strTemp.find("\n", iTextPos);
					strTrand = strTemp.substr(iTextPos, iSignPos);
					vctStepText.push_back(strTrand);
					strTemp.erase(iTextPos, iSignPos);
				}

				for (int i = 0; i < pvctStep.size(); i++)
				{
					int iCount = 0;
					for (vector<string>::iterator itp = pvctStep[i].second.begin(); itp != pvctStep[i].second.end(); itp++)
					{
						for (int j = 0; j < vctStepText.size(); j++)
						{
							if (string::npos != vctStepText[j].find(*itp))
							{
								string strTemp = vctStepText[j];
								iCount++;
								if (1 == iCount)
								{
									strTemp = "----------------" + pvctStep[i].first + "----------------" + "\r" + strTemp;
									strTemp = "\n" + strTemp;
								}
								strValue.append(strTemp);
							}
						}
					}
				}
			}
		}

		return strValue;
	}
	string Utf8ToGBK(const char* src_str)  // UTF8 -> GBK
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
		wchar_t* wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char* szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		string strTemp(szGBK);
		if (wszGBK) delete[] wszGBK;
		if (szGBK) delete[] szGBK;
		return strTemp;
	}
	bool isTextUTF8(const char* str)
	{
		unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		unsigned char chr = *str;
		bool bAllAscii = true;
		for (unsigned int i = 0; str[i] != '\0'; ++i)
		{
			chr = *(str + i);
			//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
			if (nBytes == 0 && (chr & 0x80) != 0)
			{
				bAllAscii = false;
			}
			if (nBytes == 0)
			{
				//如果不是ASCII码,应该是多字节符,计算字节数
				if (chr >= 0x80)
				{
					if (chr >= 0xFC && chr <= 0xFD)
					{
						nBytes = 6;
					}
					else if (chr >= 0xF8)
					{
						nBytes = 5;
					}
					else if (chr >= 0xF0)
					{
						nBytes = 4;
					}
					else if (chr >= 0xE0)
					{
						nBytes = 3;
					}
					else if (chr >= 0xC0)
					{
						nBytes = 2;
					}
					else
					{
						return false;
					}
					nBytes--;
				}
			}
			else
			{
				//多字节符的非首字节,应为 10xxxxxx
				if ((chr & 0xC0) != 0x80)
				{
					return false;
				}
				//减到为零为止
				nBytes--;
			}
		}
		//违返UTF8编码规则
		if (nBytes != 0)
		{
			return false;
		}
		if (bAllAscii)
		{ //如果全部都是ASCII, 也是UTF8
			return true;
		}
		return true;
	}
	int GetREQPos(string& strText, string strREQ)
	{
		string strTemp = strText;
		string strExtra = strREQ;
		string strTrand;
		strExtra = TrimSpace(strExtra); // 去空格
		strExtra = strExtra.substr(strExtra.size() - 8, 8); // 31 B8 00 00

		int iREQPos = 0;
		int iREQPosStart = 0;
		int iREQPosEnd = 0;
		int iCount = 0;
		//int iValue = 0;

		for (int i = 1; i < strExtra.size() / 2 + 1; i++) // 加空格
		{
			string strRope = strExtra.substr(iCount, 2);
			if (6 == iCount)
			{
				strTrand += strRope;
			}
			else
			{
				strTrand += strRope + " ";
			}
			iCount = i * 2;
		}

		iREQPosStart = strTemp.find(strREQ);
		if (-1 == iREQPosStart)
		{
			return -1;
		}

		while (string::npos != strTemp.find(strTrand)) // 替换成*
		{
			iREQPos = strTemp.find(strTrand);
			strTemp.at(iREQPos) = '*';
		}

		iREQPosEnd = strTemp.find("REQ", iREQPos);  // 下一个REQ

		//strTime = strTemp.substr(iREQPosEnd, strTemp.find("\n", iREQPosEnd) - iREQPosEnd);

		strTemp.erase(strTemp.begin() + iREQPosStart, strTemp.begin() + iREQPosEnd);  // 删除符合条件的REQ和RES
		strText = strTemp;

		//iValue = strText.find(strTime);

		return iREQPosStart;  // 返回REQ的原始位置
	}
	string HandleJumpFileName(string strText, string strJumpName) // DIDB中截取跳转文件名
	{
		string strKey;

		strKey = strJumpName + "       " + "BESCHREIBUNG";  // 三种索引情况
		if ((string::npos != strText.find(strKey)))
		{
			string strTemp = strText;
			int iPos = strTemp.find(strKey);

			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos);
			iPos = strTemp.find("\n");
			strTemp.erase(strTemp.begin() + iPos, strTemp.end());

			iPos = strTemp.find(strKey);
			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos + strKey.size());

			strTemp = TrimSpace(strTemp);

			return strTemp;
		}

		strKey.clear();
		strKey = "testerDescription" + strJumpName;
		if (string::npos != strText.find(strKey))
		{
			string strTemp = strText;
			int iPos = strTemp.find(strKey);

			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos);
			iPos = strTemp.find("\n");
			strTemp.erase(strTemp.begin() + iPos, strTemp.end());

			iPos = strTemp.find(strKey);
			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos + strKey.size());

			strTemp = TrimSpace(strTemp);

			return strTemp;
		}

		strKey.clear();
		strKey = "displayName" + strJumpName;
		if (string::npos != strText.find(strKey))
		{
			string strTemp = strText;
			int iPos = strTemp.find(strKey);

			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos);
			iPos = strTemp.find("\n");
			strTemp.erase(strTemp.begin() + iPos, strTemp.end());

			iPos = strTemp.find(strKey);
			strTemp.erase(strTemp.begin(), strTemp.begin() + iPos + strKey.size());

			strTemp = TrimSpace(strTemp);

			return strTemp;
		}

		return "";
	}
	vector<string>GetJumpJavaName(string strDiDbPath)  // 获取JAVA跳转文件名
	{
		vector<string>vctJumpName;

		if (!setCallID.empty())
		{
			string strText;

			strText = GetAllFileText(strDiDbPath);

			for (set<pair<string, string>>::iterator it = setCallID.begin(); it != setCallID.end(); it++)  // 两种索引情况
			{
				string strTemp = HandleJumpFileName(strText, it->second);  // 截取跳转文件名

				vctJumpName.push_back(it->first + "	" + strTemp);
			}
		}
		else
		{
			return vctJumpName;
		}

		return vctJumpName;
	}
	void ReDuplicate(vector<string>& vctStr)
	{
		sort(vctStr.begin(), vctStr.end());
		vctStr.erase(unique(vctStr.begin(), vctStr.end()), vctStr.end());
	}
	vector<pair<string, vector<string>>>GetStepOrSelectTextIndex(string strText, set<pair<string, string>>setStep, vector<string>vctStep)
	{
		unordered_map<string, vector<string>>unmapStep;
		vector<pair<string, vector<string>>>pvctStep;

		string strTemp = strText;

		for (set<pair<string, string>>::iterator it = setStep.begin(); it != setStep.end(); it++)
		{
			for (int i = 0; i < vctStep.size(); i++)
			{
				string strTrand;
				string strTextIndex;
				int iClassPos = 0;
				int iTextIndexPos = 0;
				int iSignPos = 0;
				iClassPos = strTemp.find("public " + vctStep[i] + "()");
				strTemp.erase(strTemp.begin(), strTemp.begin() + iClassPos);

				if (string::npos != strTemp.find("public class "))
				{
					iClassPos = strTemp.find("public class ");
					strTrand = strTemp.substr(0, iClassPos);  // 一个Step 或 Select的代码

					while (string::npos != strTrand.find(it->second)) // 取文本ID
					{
						iTextIndexPos = strTrand.find(it->second);
						iSignPos = strTrand.find(',', iTextIndexPos);
						strTrand.erase(strTrand.begin(), strTrand.begin() + iSignPos + 1);
						iSignPos = strTrand.find('"');
						strTrand.erase(strTrand.begin(), strTrand.begin() + iSignPos + 1);
						strTextIndex = strTrand.substr(0, strTrand.find('"', iSignPos));

						unmapStep[vctStep[i]].push_back(strTextIndex);
					}
				}
				else
				{
					while (string::npos != strTemp.find(it->second)) // 取文本ID
					{
						iTextIndexPos = strTemp.find(it->second);
						iSignPos = strTemp.find(',', iTextIndexPos);
						strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
						iSignPos = strTemp.find('"');
						strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
						strTextIndex = strTemp.substr(0, strTemp.find('"', iSignPos));

						unmapStep[vctStep[i]].push_back(strTextIndex);
					}
				}
			}
		}

		for (int i = 0; i < vctStep.size(); i++)
		{
			vector<string>vctTemp;
			vctTemp = unmapStep[vctStep[i]];
			pvctStep.push_back(make_pair(vctStep[i], vctTemp));
		}

		return pvctStep;
	}


	int mainFindTextJava()
	{

		unordered_map<string, vector<string>>unmapstep;

		vector<pair<string, vector<string>>>pvctStep;

		set<string>setstrr;
		set<pair<string, string>>psetstr;
		unordered_set<string>unsetStep;
		vector<string>vctstrr;

		string strText;
		string strIDString;
		string strLine;
		string strr;
		string strFile = "F:\\引导开发\\JAVA内容文本ID中文查询\\大众\\didb_GFS-v.zh_CN.TAB";
		//string strFile = "D:\\Platform\\maker\\GS_VW\\worktools\\LibraryFile\\Debug\\DB\\V\\TRANSLATEDTEXT\\didb_GFS-v.zh_CN.TAB";
		//string strFileID = "F:\\引导开发\\java - 途观2003\\SYS94_1T_____1_0103_11_Grundeinstellung_00011.java";
		string strFileID = "F:\\引导开发\\java - 奥迪Q2-2017\\sys_4h_xxxx_1_0413_21_GrundeinstellungLWR_ASAM_00021.java";

		GetStepOrSelectJavaID(strFileID, psetstr, vctstrr);
		//strText = GetAllFileText(strFileID);
		pvctStep = GetStepOrSelectTextIndex(strText, psetstr, vctstrr);

		psetstr = GetAllJavaID(strFileID);
		strText = GetAllFileText(strFile);
		strIDString = GetIDString(strText, psetstr, pvctStep);


		vctstrr = GetJumpJavaName(strFile);
		strText = GetAllFileText(strFile);


		string strData = "D:\\平台\\cardata\\引导功能数据\\途观2003.txt";
		string strreq = "REQ=05 07 60 10 00 02 1A 9B ";
		strText = GetAllFileText(strData);
		int itemp = GetREQPos(strText, strreq);

		GetLineFileText(strData, "", vctstrr);

		psetstr = GetAllJavaID(strFileID);
		vctstrr = GetJumpJavaName(strFile);

		strText = GetAllFileText(strFile);
		strIDString = GetIDString(strText, psetstr, pvctStep);
		if (isTextUTF8(strIDString.c_str()))
		{
			strIDString = Utf8ToGBK(strIDString.c_str());



			CTime ctime;
			ctime = CTime::GetCurrentTime();
			int iYear = ctime.GetYear();
			int iMonth = ctime.GetMonth();
			int iDay = ctime.GetDay();

			//binYear = HexString2Binary(strYear);
			//binMonth = HexString2Binary(strMonth);
			//binDay = HexString2Binary(strDay);

			//binSendDate = CBinary("\x2E\xF1\x99", 3) + binYear + binMonth + binDay;  // 2EF199加入年月日时间



			time_t mmtime;
			time(&mmtime);
			tm* ptime = NULL;
			ptime = gmtime(&mmtime);
			int iyear = 1900 + ptime->tm_year;

			return 0;
		}
	}


}


