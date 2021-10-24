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

	string GetAllFileText(string strFilePath)  // ��ȡȫ�����ݵ��ַ����У�Ч�ʸ�
	{
		FILE* pFile = fopen(strFilePath.c_str(), "r");
		string strText;
		int iCount = 0;

		if (pFile)
		{
			char* chText;  //Ҫ��ȡ���ַ���  
			int iLength = 0;   //��ȡ�ĳ���  

			fseek(pFile, 0, SEEK_END);  //�Ƶ�β��  
			iLength = ftell(pFile);        //��ȡ����  
			rewind(pFile);             //�ع�ԭλ  

			//����buf�ռ�  
			chText = (char*)malloc(sizeof(char) * iLength + 1);
			if (!chText)
			{
				return NULL;
			}

			//��ȡ�ļ�  
			//��ȡ����buf����λ��С�����ȣ��ļ�ָ��  
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
	void GetLineFileText(string strFileData, string strIndex, vector<string>& vctStrText)  // ���ж�ȡ
	{
		FILE* pFile = NULL;

		char* chLine = NULL;

		int iFileLen = 0;
		pFile = fopen(strFileData.c_str(), "r");
		if (NULL == pFile)
		{
			return;
		}

		fseek(pFile, 0L, SEEK_END); // �ƶ�ָ�����ļ�β��
		iFileLen = ftell(pFile); // �����ļ�����
		chLine = (char*)malloc(iFileLen + 1); // ��ȡ�ڴ�ռ� ����???
		memset(chLine, 0, iFileLen + 1); // ��ʼ���ڴ�ռ�
		fseek(pFile, 0L, SEEK_SET); // �ƶ�ָ�����ļ�ͷ��

		while (NULL != fgets(chLine, iFileLen, pFile)) // ���ж�ȡ
		{
			string strTemp;
			strTemp += chLine;

			if (string::npos != strTemp.find(strIndex))
			{
				vctStrText.push_back(strTemp);
			}
		}

		// �ͷ�
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
	bool IsCallId(string strText, string& strCallName, string& strCall)  // �Ƿ���Call ?
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
				strCallName = strTemp;   // ȡCall 

				strTemp = strText;
				iGetTextPos = 0;
				iSignPos = 0;

				iGetTextPos = strTemp.find(strgetTextNodeId);
				strTemp = strTemp.substr(iGetTextPos + strgetTextNodeId.size(), strTemp.size() - 1);
				iSignPos = strTemp.find("(");

				char chTemp = strTemp.at(iSignPos + 2);
				if (0 != isdigit(strTemp.at(iSignPos + 2)))  // ȡID
				{
					iSignPos = strTemp.find('"');
					strTemp.erase(strTemp.begin(), strTemp.begin() + iSignPos + 1);
					iSignPos = strTemp.find('"');
					strCall = strTemp.substr(0, iSignPos);
					return true;
				}
			}

			if (string::npos != strText.find(strGetCalllower))  // �ܲ����ҵ�Call_
			{
				int iGetTextPos = 0;
				int iSignPos = 0;
				string strTemp;
				strTemp = strText;

				iGetTextPos = strTemp.find(strGetCalllower);
				strTemp.erase(strTemp.begin(), strTemp.begin() + iGetTextPos);
				iSignPos = strTemp.find('"');
				strTemp.erase(strTemp.begin() + iSignPos, strTemp.end());
				strCallName = strTemp;   // ȡCall 

				strTemp = strText;
				iGetTextPos = 0;
				iSignPos = 0;

				iGetTextPos = strTemp.find(strgetTextNodeId);
				strTemp = strTemp.substr(iGetTextPos + strgetTextNodeId.size(), strTemp.size() - 1);
				iSignPos = strTemp.find("(");

				char chTemp = strTemp.at(iSignPos + 2);
				if (0 != isdigit(strTemp.at(iSignPos + 2)))  // ȡID  �Ƿ�������
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
	bool IsStepId(string strText, string& strStepName, string& strStep)  // �Ƿ���Step ?
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
			if (0 != isdigit(strTemp.at(iSignPos + 2)))  // ȡID
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
	set<pair<string, string>>GetAllJavaID(string strJavaPath) // JAVA�ļ��н�ȡȥ�غ��JAVA�ı�ID
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
		while (NULL != fgets(chLine, iFileLen, pFile)) // ���ж�ȡ
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
		while (NULL != fgets(chLine, iFileLen, pFile)) // ���ж�ȡ
		{
			string strStepName;
			string strTemp;
			strTemp += chLine;

			if (IsStepId(strTemp, strStepName, strStep))
			{
				setStepJavaID.insert(make_pair(strStepName, strStep));
			}

			strStepName = GetStepOrSelectName(strTemp);  // ��ȡStep ���� Select��

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
	string GetIDString(string strText, set<pair<string, string>>setJavaID, vector<pair<string, vector<string>>>pvctStep)  // DIDB�н�ȡJAVA�ı�ID�Ĵ�
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
				string strFileID = "F:\\��������\\JAVA�����ı�ID���Ĳ�ѯ\\����\\didb_GFS-v.zh_CN.TAB";

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
		unsigned int nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
		unsigned char chr = *str;
		bool bAllAscii = true;
		for (unsigned int i = 0; str[i] != '\0'; ++i)
		{
			chr = *(str + i);
			//�ж��Ƿ�ASCII����,�������,˵���п�����UTF8,ASCII��7λ����,���λ���Ϊ0,0xxxxxxx
			if (nBytes == 0 && (chr & 0x80) != 0)
			{
				bAllAscii = false;
			}
			if (nBytes == 0)
			{
				//�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
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
				//���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
				if ((chr & 0xC0) != 0x80)
				{
					return false;
				}
				//����Ϊ��Ϊֹ
				nBytes--;
			}
		}
		//Υ��UTF8�������
		if (nBytes != 0)
		{
			return false;
		}
		if (bAllAscii)
		{ //���ȫ������ASCII, Ҳ��UTF8
			return true;
		}
		return true;
	}
	int GetREQPos(string& strText, string strREQ)
	{
		string strTemp = strText;
		string strExtra = strREQ;
		string strTrand;
		strExtra = TrimSpace(strExtra); // ȥ�ո�
		strExtra = strExtra.substr(strExtra.size() - 8, 8); // 31 B8 00 00

		int iREQPos = 0;
		int iREQPosStart = 0;
		int iREQPosEnd = 0;
		int iCount = 0;
		//int iValue = 0;

		for (int i = 1; i < strExtra.size() / 2 + 1; i++) // �ӿո�
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

		while (string::npos != strTemp.find(strTrand)) // �滻��*
		{
			iREQPos = strTemp.find(strTrand);
			strTemp.at(iREQPos) = '*';
		}

		iREQPosEnd = strTemp.find("REQ", iREQPos);  // ��һ��REQ

		//strTime = strTemp.substr(iREQPosEnd, strTemp.find("\n", iREQPosEnd) - iREQPosEnd);

		strTemp.erase(strTemp.begin() + iREQPosStart, strTemp.begin() + iREQPosEnd);  // ɾ������������REQ��RES
		strText = strTemp;

		//iValue = strText.find(strTime);

		return iREQPosStart;  // ����REQ��ԭʼλ��
	}
	string HandleJumpFileName(string strText, string strJumpName) // DIDB�н�ȡ��ת�ļ���
	{
		string strKey;

		strKey = strJumpName + "       " + "BESCHREIBUNG";  // �����������
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
	vector<string>GetJumpJavaName(string strDiDbPath)  // ��ȡJAVA��ת�ļ���
	{
		vector<string>vctJumpName;

		if (!setCallID.empty())
		{
			string strText;

			strText = GetAllFileText(strDiDbPath);

			for (set<pair<string, string>>::iterator it = setCallID.begin(); it != setCallID.end(); it++)  // �����������
			{
				string strTemp = HandleJumpFileName(strText, it->second);  // ��ȡ��ת�ļ���

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
					strTrand = strTemp.substr(0, iClassPos);  // һ��Step �� Select�Ĵ���

					while (string::npos != strTrand.find(it->second)) // ȡ�ı�ID
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
					while (string::npos != strTemp.find(it->second)) // ȡ�ı�ID
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
		string strFile = "F:\\��������\\JAVA�����ı�ID���Ĳ�ѯ\\����\\didb_GFS-v.zh_CN.TAB";
		//string strFile = "D:\\Platform\\maker\\GS_VW\\worktools\\LibraryFile\\Debug\\DB\\V\\TRANSLATEDTEXT\\didb_GFS-v.zh_CN.TAB";
		//string strFileID = "F:\\��������\\java - ;��2003\\SYS94_1T_____1_0103_11_Grundeinstellung_00011.java";
		string strFileID = "F:\\��������\\java - �µ�Q2-2017\\sys_4h_xxxx_1_0413_21_GrundeinstellungLWR_ASAM_00021.java";

		GetStepOrSelectJavaID(strFileID, psetstr, vctstrr);
		//strText = GetAllFileText(strFileID);
		pvctStep = GetStepOrSelectTextIndex(strText, psetstr, vctstrr);

		psetstr = GetAllJavaID(strFileID);
		strText = GetAllFileText(strFile);
		strIDString = GetIDString(strText, psetstr, pvctStep);


		vctstrr = GetJumpJavaName(strFile);
		strText = GetAllFileText(strFile);


		string strData = "D:\\ƽ̨\\cardata\\������������\\;��2003.txt";
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

			//binSendDate = CBinary("\x2E\xF1\x99", 3) + binYear + binMonth + binDay;  // 2EF199����������ʱ��



			time_t mmtime;
			time(&mmtime);
			tm* ptime = NULL;
			ptime = gmtime(&mmtime);
			int iyear = 1900 + ptime->tm_year;

			return 0;
		}
	}


}


