#include "MyIni.h"

CMyIni::CMyIni()
{
	this->m_strError.clear();
	this->m_pFile = nullptr;
}

CMyIni::~CMyIni()
{
	if (this->m_pFile)
	{
		fclose(this->m_pFile);
	}
}

void CMyIni::close()
{
	if (this->m_pFile)
	{
		fclose(this->m_pFile);
	}

	this->m_pFile = nullptr;
	this->m_strDataPath.clear();
	this->m_strError.clear();
	this->m_strSaveIni.clear();
}

void CMyIni::setHead(string strIniHead)
{
	this->m_vctChildHeadText.clear();

	this->m_strSaveIni += "[" + strIniHead + "]";
	this->m_strSaveIni += "\n";
}

void CMyIni::setChildText(string strIniChildHead, string strText)
{
	string strTrand;

	strTrand += "\t";
	strTrand += strIniChildHead;
	strTrand += "=";
	strTrand += strText;

	this->m_strSaveIni += strTrand + "\n";
}

void CMyIni::setGroupChildText(string strIniChildHead, vector<string>vctText)
{
	string strTrand;

	int iCount = 0;

	strTrand += "\t";
	strTrand += strIniChildHead;
	strTrand += "=";

	for (vector<string>::iterator it = vctText.begin(); it != vctText.end(); it++)
	{
		if (iCount == vctText.size() - 1)
		{
			strTrand += *it;
		}
		else
		{
			strTrand += *it;
			strTrand += ",";
		}

		iCount++;
	}

	this->m_strSaveIni += strTrand + "\n";
}

void CMyIni::setAllGroupChild(vector<pair<string, string>>pvctHeadText)
{
	string strTrand;

	for (vector<pair<string, string>>::iterator it = pvctHeadText.begin(); it != pvctHeadText.end(); it++)
	{
		strTrand += "\t";
		strTrand += (*it).first;
		strTrand += "=";
		strTrand += (*it).second;
		strTrand += "\n";
	}

	this->m_strSaveIni += strTrand;
}

void CMyIni::setAllGroupChild(vector<pair<string, vector<string>>>pvctHeadText)
{
	string strTrand;

	for (vector<pair<string, vector<string>>>::iterator pit = pvctHeadText.begin(); pit != pvctHeadText.end(); pit++)
	{
		string strTemp;
		int iCount = 0;

		strTrand += "\t";
		strTrand += (*pit).first;
		strTrand += "=";

		for (vector<string>::iterator it = pit->second.begin(); it != pit->second.end(); it++)
		{
			if (iCount == pit->second.size() - 1)
			{
				strTemp += (*it);
			}
			else
			{
				strTemp += (*it);
				strTemp += ",";
			}

			iCount++;
		}

		strTrand += strTemp + "\n";
	}

	this->m_strSaveIni += strTrand;
}

void CMyIni::setAllGroupChild(map<string, string>mapHeadText)
{
	string strTrand;

	for (map<string, string>::iterator it = mapHeadText.begin(); it != mapHeadText.end(); it++)
	{
		strTrand += "\t";
		strTrand += (*it).first;
		strTrand += "=";
		strTrand += (*it).second;
		strTrand += "\n";
	}

	this->m_strSaveIni += strTrand;
}

void CMyIni::setAllGroupChild(map<string, vector<string>>mapHeadText)
{
	string strTrand;

	for (map<string, vector<string>>::iterator mapit = mapHeadText.begin(); mapit != mapHeadText.end(); mapit++)
	{
		string strTemp;
		int iCount = 0;

		strTrand += "\t";
		strTrand += (*mapit).first;
		strTrand += "=";

		for (vector<string>::iterator it = mapit->second.begin(); it != mapit->second.end(); it++)
		{
			if (iCount == mapit->second.size() - 1)
			{
				strTemp += (*it);
			}
			else
			{
				strTemp += (*it);
				strTemp += ",";
			}

			iCount++;
		}

		strTrand += strTemp + "\n";
	}

	this->m_strSaveIni += strTrand;
}

void CMyIni::save(string strDataPath /*= ""*/)
{
	if (strDataPath.empty())
	{
		strDataPath = this->m_strDataPath;
	}

	WriteTxt(strDataPath, this->m_strSaveIni, false);

	close();

	return;
}

void CMyIni::WriteTxt(string strDataPath, string strText, bool bIsJudgeExist /* = true */)
{
	FILE* pFile = nullptr;

	int iFileLength = 0;

	if ((strDataPath.empty()) || (strText.empty()))
	{
		this->m_strError = "Cbinary index text is empty!";
		return;
	}

	if (bIsJudgeExist)
	{
		if (-1 == _access(strDataPath.c_str(), 0))
		{
			_mkdir(strDataPath.c_str());
		}

		strDataPath += "\\Ini.txt";
	}

	pFile = fopen(strDataPath.c_str(), "a+");
	if (!pFile)
	{
		this->m_strError = "Cbinary index text path open failed!";
		return;
	}

	fwrite(strText.c_str(), strText.size(), sizeof(char), pFile);

	if (pFile)
	{
		fclose(pFile);
	}

	return;
}

string CMyIni::TrimSpaceTab(string strTrim)
{
	string strTemp;
	strTemp = strTrim;

	while (string::npos != strTemp.find(" "))
	{
		strTemp.replace(strTemp.find(" "), 1, "");
	}

	while (string::npos != strTemp.find("\t"))
	{
		strTemp.replace(strTemp.find("\t"), 1, "");
	}

	while (string::npos != strTemp.find("\n"))
	{
		strTemp.replace(strTemp.find("\n"), 1, "");
	}

	while (string::npos != strTemp.find("\\n\\"))
	{
		strTemp.replace(strTemp.find("\\n\\"), 5, "");
	}

	return strTemp;
}

std::vector<std::string> CMyIni::SplitComma(string strTrim)
{
	vector<string>vctRes;

	if ((strTrim.empty()) || (string::npos == strTrim.find(",")))
	{
		return vctRes;
	}

	if (string::npos != strTrim.find("="))
	{
		strTrim.erase(strTrim.begin(), strTrim.begin() + strTrim.find("=") + 1);
	}

	while (string::npos != strTrim.find(","))
	{
		string strTemp;

		strTemp = strTrim.substr(0, strTrim.find(",") + 1);

		while (string::npos != strTemp.find(","))
		{
			strTemp.replace(strTemp.find(","), 1, "");
		}

		if (!strTemp.empty())
		{
			vctRes.push_back(strTemp);
		}

		strTrim.erase(strTrim.begin(), strTrim.begin() + strTrim.find(",") + 1);
	}

	if (!strTrim.empty())
	{
		vctRes.push_back(strTrim);
	}

	return vctRes;
}

bool CMyIni::IsError()
{
	if (!this->m_strError.empty())
	{
		return true;
	}

	return false;
}

bool CMyIni::open(string strDataPath)
{
	if (this->m_pFile)
	{
		close();
	}

	if (strDataPath.empty())
	{
		this->m_strError = "ini file path is empty!";
		return false;
	}

	this->m_pFile = fopen(strDataPath.c_str(), "r+");
	if (!this->m_pFile)
	{
		this->m_strError = "file open failed!";
		return false;
	}

	return true;
}

bool CMyIni::open(string strDataPath, string strCbinaryIndex)
{
	if (this->m_pFile)
	{
		close();
	}

	if ((strDataPath.empty()) || (strCbinaryIndex.empty()))
	{
		this->m_strError = "file is empty!";
		return false;
	}

	string strSaveText;
	string strSavePath;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsIndexFind = false;

	this->m_pFile = fopen(strDataPath.c_str(), "r+");
	if (!this->m_pFile)
	{
		this->m_strError = "file open failed!";
		return false;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	fseek(this->m_pFile, 0L, SEEK_SET);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (bIsIndexFind)
		{
			if (string::npos != strTemp.find(",0x"))
			{
				break;
			}

			strTemp = TrimSpaceTab(strTemp);
			strSaveText += strTemp;
			strSaveText += "\n";
		}

		if (string::npos != strTemp.find(strCbinaryIndex))
		{
			bIsIndexFind = true;
		}
	}

	if (strSaveText.empty())
	{
		this->m_strError = "Cbinary index find failed!";
		return false;
	}

	strSavePath = CBINARYWRITEINI;

	WriteTxt(CBINARYWRITEINI, strSaveText);

	strSavePath += "\\Ini.txt";

	if (!open(strSavePath))
	{
		close();
		this->m_strError = "Cbinary Write open failed!";
		return false;
	}

	return true;
}

bool CMyIni::setWriteIni(string strDataPath)
{
	if (this->m_pFile)
	{
		close();
	}

	if (strDataPath.empty())
	{
		return false;
	}

	this->m_strDataPath = strDataPath;
	this->m_strSaveIni.clear();
	this->m_vctChildHeadText.clear();

	return true;
}

int CMyIni::getInt(string strIniHead, string strIniChild)
{
	int iRes = 0;
	string strRes;

	if ((strIniHead.empty()) || (strIniChild.empty()))
	{
		return iRes;
	}

	strRes = getText(strIniHead, strIniChild);
	if (!strRes.empty())
	{
		iRes = atoi(strRes.c_str());
	}

	return iRes;
}

double CMyIni::getDouble(string strIniHead, string strIniChild)
{
	double dRes = 0;

	string strRes;

	if ((strIniHead.empty()) || (strIniChild.empty()))
	{
		return dRes;
	}

	strRes = getText(strIniHead, strIniChild);

	if (!strRes.empty())
	{
		dRes = atof(strRes.c_str());
	}

	return dRes;
}

std::string CMyIni::getText(string strIniHead, string strIniChild)
{
	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return strText;
	}

	if ((strIniHead.empty()) || (strIniChild.empty()))
	{
		this->m_strError = "ini head or ini child is empty!";
		return strText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return strText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return strText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find(strIniChild))
		{
			strTemp = TrimSpaceTab(strTemp);

			if (string::npos != strTemp.find("="))
			{
				int iPosTemp = 0;
				iPosTemp = strTemp.find("=");

				strText = strTemp.substr(iPosTemp + 1, strTemp.size() - iPosTemp + 1);
				break;
			}
			else
			{
				this->m_strError = "ini chlid not find =!";
				return strText;
			}
		}
	}

	if (strText.empty())
	{
		this->m_strError = "ini find child error!";
		return strText;
	}

	return strText;
}

vector<string>CMyIni::getGroupText(string strIniHead, string strIniChild)
{
	vector<string>vctText;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return vctText;
	}

	if ((strIniHead.empty()) || (strIniChild.empty()))
	{
		this->m_strError = "ini head or ini child is empty!";
		return vctText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return vctText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return vctText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find(strIniChild))
		{
			strTemp = TrimSpaceTab(strTemp);

			if (string::npos != strTemp.find("="))
			{
				int iPosTemp = 0;
				iPosTemp = strTemp.find("=");

				strText = strTemp.substr(iPosTemp + 1, strTemp.size() - iPosTemp + 1);

				if (string::npos != strText.find(","))
				{
					vctText = SplitComma(strText);
					break;
				}
				else
				{
					this->m_strError = "ini chlid not find ,";
					return vctText;
				}

				break;
			}
			else
			{
				this->m_strError = "ini chlid not find =!";
				return vctText;
			}
		}
	}

	return vctText;
}

std::vector<pair<string, string>> CMyIni::getSecionsVct(string strIniHead)
{
	vector<pair<string, string>>pvctText;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return pvctText;
	}

	if (strIniHead.empty())
	{
		this->m_strError = "ini head or ini child is empty!";
		return pvctText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return pvctText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				strTemp = TrimSpaceTab(strTemp);
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return pvctText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find("="))
		{
			string strHead;
			string strText;

			pair<string, string>pairTemp;

			strHead = strTemp.substr(0, strTemp.find("="));
			strText = strTemp.substr(strTemp.find("=") + 1, strTemp.size() - strTemp.find("=") + 1);

			pairTemp.first = strHead;
			pairTemp.second = strText;

			pvctText.push_back(pairTemp);
		}
	}

	return pvctText;
}

std::vector<pair<string, vector<string>>> CMyIni::getGroupSecionsVct(string strIniHead)
{
	vector<pair<string, vector<string>>>pmvctText;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return pmvctText;
	}

	if (strIniHead.empty())
	{
		this->m_strError = "ini head or ini child is empty!";
		return pmvctText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return pmvctText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				strTemp = TrimSpaceTab(strTemp);
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return pmvctText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find("="))
		{
			string strHead;

			vector<string>vctTemp;

			strHead = strTemp.substr(0, strTemp.find("="));

			vctTemp = SplitComma(strTemp);

			pmvctText.push_back(make_pair(strHead, vctTemp));
		}
	}

	return pmvctText;
}

std::map<std::string, std::string> CMyIni::getSecionsMap(string strIniHead)
{
	map<string, string>mapText;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return mapText;
	}

	if (strIniHead.empty())
	{
		this->m_strError = "ini head or ini child is empty!";
		return mapText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return mapText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				strTemp = TrimSpaceTab(strTemp);
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return mapText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find("="))
		{
			string strHead;
			string strText;

			strHead = strTemp.substr(0, strTemp.find("="));
			strText = strTemp.substr(strTemp.find("=") + 1, strTemp.size() - strTemp.find("=") + 1);

			mapText.insert(make_pair(strHead, strText));
		}
	}

	return mapText;
}

std::map<std::string, std::vector<std::string>> CMyIni::getGroupSecionsMap(string strIniHead)
{
	map<string, vector<string>>mapvctText;

	int iFileLength = 0;

	char* chLine = nullptr;

	bool bIsFindHeadFinish = false;
	bool bIsFindTextFinish = false;

	string strText;

	vector<string>vctFindText;

	if (!this->m_pFile)
	{
		this->m_strError = "ini file open failed!";
		return mapvctText;
	}

	if (strIniHead.empty())
	{
		this->m_strError = "ini head or ini child is empty!";
		return mapvctText;
	}

	fseek(this->m_pFile, 0L, SEEK_END);
	iFileLength = ftell(this->m_pFile);
	chLine = (char*)malloc(iFileLength + 1);
	memset(chLine, 0L, iFileLength + 1);
	fseek(this->m_pFile, 0L, SEEK_SET);

	while (nullptr != fgets(chLine, iFileLength, this->m_pFile))
	{
		string strTemp;
		strTemp += chLine;

		if (!bIsFindHeadFinish)
		{
			if (string::npos != strTemp.find(strIniHead))
			{
				if ((string::npos == strTemp.find("[")) || (string::npos == strTemp.find("]")))
				{
					this->m_strError = "ini head error!";
					return mapvctText;
				}

				bIsFindHeadFinish = true;
			}
		}
		else
		{
			if ((string::npos != strTemp.find("[")) && (string::npos != strTemp.find("]")))
			{
				bIsFindHeadFinish = true;
				break;
			}

			if (!bIsFindTextFinish)
			{
				strTemp = TrimSpaceTab(strTemp);
				vctFindText.push_back(strTemp);
			}
		}
	}

	if (vctFindText.empty())
	{
		this->m_strError = "ini child error!";
		return mapvctText;
	}

	for (vector<string>::iterator it = vctFindText.begin(); it != vctFindText.end(); it++)
	{
		string strTemp;
		strTemp = *it;

		if (string::npos != strTemp.find("="))
		{
			string strHead;

			vector<string>vctTemp;

			strHead = strTemp.substr(0, strTemp.find("="));

			vctTemp = SplitComma(strTemp);

			mapvctText.insert(make_pair(strHead, vctTemp));
		}
	}

	return mapvctText;
}






























