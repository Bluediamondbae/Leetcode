#define _CRT_SECURE_NO_DEPRECATE
#include "xtool.h"

//#include <curl/curl.h>

//using namespace boost;

#include "CMysql.h"
#include "MyIni.h"


void WriteXml(string strHeadNode, string strChildNode, string strAttriNode, string strAttriName, string strAttrival, string strvalNodeName, string valNodeval)
{
	xml_document xDoc;
	xml_node xHead = xDoc.prepend_child(node_declaration);

	xHead.append_attribute("MyVersion").set_value("100.0");

	xml_node xHeadNode = xDoc.append_child(strHeadNode.c_str());

	xml_node xChildNode = xHeadNode.append_child(strChildNode.c_str());

	xml_node xAttriNode = xChildNode.append_child(strAttriNode.c_str());
	xAttriNode.append_attribute(strAttriName.c_str()).set_value(strAttrival.c_str());

	xml_node xvalNode = xAttriNode.append_child(strvalNodeName.c_str());
	xvalNode.append_attribute(valNodeval.c_str()).set_value("11111");


	xDoc.save_file("F:\\MyXml.xml");
}



void PrintTxt(set<string>& setStr)
{
	ofstream ofs;
	ofs.open("E:\\OutXml.txt", ios::out);

	if (setStr.empty())
	{
		ofs << "No val!" << endl;
	}
	else
	{
		ofs << "All Node Name:" << endl;
		ofs << "读取的子节点数量为：" << iChildNode << "个" << endl;
		ofs << "读取的属性节点数量为：" << iAttriNode << "个" << endl;
		ofs << endl;

		for (set<string>::iterator it = setStr.begin(); it != setStr.end(); it++)
		{
			ofs << *it << endl;
		}
	}

	ofs.close();
}

vector<int> GroupList(initializer_list<int>InitListArray)
{
	vector<int>vctInt;

	for (initializer_list<int>::iterator it = InitListArray.begin(); it != InitListArray.end(); it++)
	{
		vctInt.push_back(*it);
	}

	return vctInt;
}

void GetFilesName(string path, vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFilesName(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void GetFilesNameJava(string path, vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFilesNameJava(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				string strName = fileinfo.name;

				if (string::npos != strName.find(".java"))
				{
					int iNamePos = strName.find(".java");
					strName.erase(iNamePos, 5);
				}

				files.push_back(strName);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


void ReadXml(string strXmlPath)
{
	xml_document xDoc;

	xml_parse_result res = xDoc.load_file(strXmlPath.c_str());
	if (res.status != pugi::status_ok)
	{
		return;
	}

	xml_node xHead = xDoc.child("ODX").first_child();


	for (xml_node xChildNode = xHead.first_child(); xChildNode; xChildNode = xChildNode.next_sibling())
	{
		string strChildNode = xChildNode.name();
		if (!strChildNode.empty())
		{
			iChildNode++;
		}

		for (xml_node xAttriNode = xChildNode.first_child(); xAttriNode; xAttriNode = xAttriNode.next_sibling())
		{
			string strAttriName = xAttriNode.name();

			if (!strAttriName.empty())
			{
				iAttriNode++;
			}

			string strAtrrival = xAttriNode.child_value();
			setStr.insert(strAttriName);
		}
	}

	PrintTxt(setStr);
}


void ReadFolderXml(string strFolderPath)
{
	xml_document xDoc;

	GetFilesName(strFolderPath, vctFolderStr);

	for (int i = 0; i < vctFolderStr.size(); i++)
	{
		if (!xDoc.load_file(vctFolderStr[i].c_str()))
		{
			return;
		}

		xml_node xHead = xDoc.child("ODX").first_child();

		for (xml_node xChildNode = xHead.first_child(); xChildNode; xChildNode = xChildNode.next_sibling())
		{
			string strChildNode = xChildNode.name();
			if (!strChildNode.empty())
			{
				iChildNode++;
			}

			for (xml_node xAttriNode = xChildNode.first_child(); xAttriNode; xAttriNode = xAttriNode.next_sibling())
			{
				string strAttriName = xAttriNode.name();

				if (!strAttriName.empty())
				{
					iAttriNode++;
				}

				string strAtrrival = xAttriNode.child_value();
				setStr.insert(strAttriName);
			}
		}
	}

	vctFolderStr.clear();
	PrintTxt(setStr);
}


void StringToHex1(char* str, unsigned char* strhex)
{
	uint8_t i, cnt = 0;
	char* p = str;             //直针p初始化为指向str
	uint8_t len = strlen(str); //获取字符串中的字符个数

	while (*p != '\0') {        //结束符判断
		for (i = 0; i < len; i++)  //循环判断当前字符是数字还是小写字符还是大写字母
		{
			if ((*p >= '0') && (*p <= '9')) //当前字符为数字0~9时
				strhex[cnt] = *p - '0' + 0x30;//转为十六进制

			if ((*p >= 'A') && (*p <= 'Z')) //当前字符为大写字母A~Z时
				strhex[cnt] = *p - 'A' + 0x41;//转为十六进制

			if ((*p >= 'a') && (*p <= 'z')) //当前字符为小写字母a~z时
				strhex[cnt] = *p - 'a' + 0x61;  //转为十六进制

			p++;    //指向下一个字符
			cnt++;
		}
	}
}


string StringDecimalToHex(string str10)
{
	string strHex;
	int iNum = 0;
	char cBuf[100] = { 0 };

	if (str10.find(".") != string::npos)  // 有小数点的值，直接显示十进制
	{
		strHex = str10;
		return strHex;
	}

	if (str10.find("-") != string::npos)  // 处理负整数不限制位数
	{
		iNum = atoi(str10.c_str());

		sprintf(cBuf, "%X", iNum);
		strHex += cBuf;

		return strHex;
	}

	// 正整数时，为4位
	iNum = atoi(str10.c_str());

	if (INT_MAX == iNum)  // 如果大于int的最大值，显示FFFFFFFF
	{
		strHex = "FFFFFFFF";
		return strHex;
	}

	sprintf(cBuf, "%04X", iNum);
	strHex += cBuf;

	return strHex;
}


vector<string> split(const string& s, const string& delim)
{
	std::vector<string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}


string CutFileName(string str, string strStart, string strEnd)
{
	int iStart = 0;
	int iEnd = 0;

	iStart = str.find(strStart);
	str.erase(0, iStart + 1);

	iEnd = str.find(strEnd);
	str.erase(iEnd, (str.size() - iEnd));

	return str;
}

int HexCharToInt(char strC)
{
	if (isxdigit(strC))
	{
		string strUp = "ABCDEF";
		string strLow = "abcdef";

		int i = 0;
		for (i = 0; i < 6; i++)
		{
			if (strC == strUp[i] || strC == strLow[i])
			{
				break;
			}
		}

		if (i >= 0 && i <= 5)
		{
			return 10 + i;
		}
		else
		{
			stringstream stream;
			stream << strC;
			string strBuf = stream.str();
			return atoi(strBuf.c_str());
		}
	}
	return 0;
}

int HexStringToInt(string strTemp)
{
	/* 有效性检查 */
	if ((strTemp[0] == '0') && ((strTemp[1] == 'x') || (strTemp[1] == 'X')))
	{
		strTemp = strTemp.substr(2, strTemp.length() - 2);
	}

	int iVal = 0;
	int i = 0;
	int iLen = strTemp.length();
	while (i < iLen)
	{
		int iC = HexCharToInt(strTemp[i]);
		int iPow = pow(16, (iLen - 1 - i));
		int iValTemp = iPow * iC;

		iVal += iValTemp;
		i++;
	}

	return iVal;
}


string Cut0xval(string str)
{
	string strval = str;
	int i0xPos = 0;
	int iDec = 0;
	char cBuf[50] = { 0 };

	if (str.find("0x") == string::npos)
	{
		strval = "";
		return strval;
	}

	i0xPos = str.find("0x");
	strval.erase(i0xPos, 2);

	iDec = HexStringToInt(strval);
	strval = "";

	sprintf(cBuf, "%d", iDec);
	strval += cBuf;

	return strval;
}

string StringDecToHex(string str)
{
	if (str.empty())
	{
		str = "";
		return str;
	}

	string strTemp;
	int iHex = atoi(str.c_str());

	char cBuf[50] = { 0 };
	sprintf(cBuf, "%X", iHex);
	strTemp += cBuf;

	transform(strTemp.begin(), strTemp.end(), strTemp.begin(), ::toupper);

	return strTemp;
}

bool IsNumber(string str)
{
	string strTemp = str;
	int iFlag = 0;

	for (int i = 0; i < strTemp.size(); i++)
	{
		iFlag = isalpha(strTemp[i]);

		if (iFlag != 0)  // 字母
		{
			return false;
		}
	}

	return true;
}

string TakeDiagCodeType(string strRet, string str)
{
	string strTemp = str;
	if (strTemp.empty())
	{
		strTemp = "";
		return strTemp;
	}

	int iPos = strTemp.find(strRet);
	if (-1 == iPos)
	{
		strTemp = "";
		return strTemp;
	}

	strTemp.erase(0, iPos);


}


string UTF8ToGB(const char* str)
{
	string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

void ReadTest(string strFilePath)
{

	ifstream ifs;

	ifs.open(strFilePath.c_str(), ios::in | ios::out);

	if (!ifs.is_open())
	{
		return;
	}

	string strTemp;
	while (std::getline(ifs, strTemp))
	{
		string str = UTF8ToGB(strTemp.c_str());
		cout << strTemp;
	}
	ifs.close();
}


string FileRead(string strFilePath)
{
	FILE* fp = fopen(strFilePath.c_str(), "r");
	if (NULL == fp)
	{
		return "";
	}

	//while (!feof())
	//{
	//	string str;
	//	//fgets(str,fp);
	//}

	fseek(fp, 0L, SEEK_END);
	int iLen = ftell(fp);
	char* pDataBuf = (char*)malloc(iLen + 1);
	char* p = pDataBuf;
	memset(pDataBuf, 0, iLen + 1);
	fseek(fp, 0L, SEEK_SET);

	char cSingleChar;
	while ((cSingleChar = fgetc(fp)) != EOF)
	{
		cSingleChar = toupper(cSingleChar);
		if ((cSingleChar > 'F' || cSingleChar < 'A') && (cSingleChar > '9' || cSingleChar < '0') && (cSingleChar != ',') && (cSingleChar != 'X'))
		{
			continue;
		}
		*p++ = cSingleChar;
	}

	string strData = pDataBuf;
	free(pDataBuf);
	fclose(fp);
	//remove(strFilePath.c_str());//从文件读取参数化数据后，删除文件

	return strData;
}



void NormalSort(vector<int>& vctInt)
{
	int iCount = 0;

	for (int i = 0; i < vctInt.size(); i++)
	{
		int iTemp = 0;
		for (int j = i + 1; j < vctInt.size(); j++)
		{
			if (vctInt[i] > vctInt[j])
			{
				iCount++;
				iTemp = vctInt[j];
				vctInt[j] = vctInt[i];
				vctInt[i] = iTemp;
			}
		}
	}
}

void BubbSort(vector<int>& vctInt)
{
	int iCount = 0;

	for (int i = 0; i < vctInt.size() - 1; i++)
	{
		for (int j = 0; j < vctInt.size() - 1 - i; j++) // 筛选出最大的数至最后，区间则慢慢变小
		{
			if (vctInt[j] > vctInt[j + 1])
			{
				iCount++;
				int iTemp = vctInt[j + 1];
				vctInt[j + 1] = vctInt[j];
				vctInt[j] = iTemp;
			}
		}
	}
}

/*
	第一次排序：1与4, 2, 6, 7, 3, 5, 10, 9进行比较，最小值为1，无需交换，得到数组[1, 4, 2, 6, 7, 3, 5, 10, 9]
	第二次排序：4与2, 6, 7, 3, 5, 10, 9进行比较，最小值为2, 4与2交换位置，得到数组[1, 2, 4, 6, 7, 3, 5, 10, 9]
	第三次排序：4与6, 7, 3, 5, 10, 9进行比较，最小值为3, 4与3交换位置，得到数组[1, 2, 3, 6, 7, 4, 5, 10, 9]
	第四次排序：6与7, 4, 5, 10, 9进行比较，最小值为4, 6与4交换位置，得到数组[1, 2, 3, 4, 7, 6, 5, 10, 9]
	第五次排序：7与6, 5, 10, 9进行比较，最小值为5， 7与5进行交换，得到数组[1, 2, 3, 4, 5, 6, 7, 10, 9]
	第六次排序：6与7, 10, 9进行比较，最小值为6，无需交换，得到数组[1, 2, 3, 4, 5, 6, 7, 10, 9]
	第七次排序：7与10, 9进行比较，最小值为7，无需交换，得到数组[1, 2, 3, 4, 5, 6, 7, 10, 9]
	第八次排序：10与9进行比较，最小值为9, 10与9进行交换，得到数组[1, 2, 3, 4, 5, 6, 7, 9, 10]
*/
void SelectSort(vector<int>& vctInt)
{
	for (int i = 0; i < vctInt.size(); i++)
	{
		int iMinPos = i;

		for (int j = i + 1; j < vctInt.size(); j++)
		{
			if (vctInt[j] < vctInt[iMinPos]) // 不断去找最小值
			{
				iMinPos = j;
			}
		}

		if (iMinPos != i) // 当前位置与最小值位置交换
		{
			int iTemp = vctInt[i];
			vctInt[i] = vctInt[iMinPos];
			vctInt[iMinPos] = iTemp;
		}
	}
}

void InsertSort(vector<int>& vctInt)
{
	int iTemp = 0;
	int iIndex = 0;
	for (int i = 1; i < vctInt.size(); i++)
	{
		iTemp = vctInt[i];
		iIndex = i - 1;

		while ((vctInt[iIndex] > iTemp) && (iIndex >= 0))
		{
			vctInt[iIndex + 1] = vctInt[iIndex];
			iIndex--;
		}

		vctInt[iIndex + 1] = iTemp;
	}
}

string ReservedBits(double dval, int iNum)
{
	string strNum;
	string strTemp;
	string strval;
	char bufTemp[100] = { 0 };
	char buf[100] = { 0 };

	sprintf(bufTemp, "%d", iNum);
	strNum += bufTemp;

	strTemp = "%." + strNum + "f";

	sprintf(buf, strTemp.c_str(), dval);
	strval += buf;

	return strval;
}

string FileData(string strPath)
{
	FILE* fp = fopen(strPath.c_str(), "r");
	if (NULL == fp)
	{
		return "";
	}

	fseek(fp, 0L, SEEK_END);  //  0 文件开始  1 文件当前指针位置  2 文件结尾  fseek(文件指针,移动多少字节,指针位置)
	int iLen = ftell(fp);  // 求文件长度
	char* pDataBuf = (char*)malloc(iLen + 1);  // 获取内存空间
	char* p = pDataBuf;
	memset(pDataBuf, 0, iLen + 1);   // 初始化空间
	fseek(fp, 0L, SEEK_SET);

	char cSingleChar;
	while ((cSingleChar = fgetc(fp)) != EOF)  // fgetc用于二进制文件读写，无可读字节返回-1
	{
		cSingleChar = toupper(cSingleChar);
		if ((cSingleChar > 'F' || cSingleChar < 'A') && (cSingleChar > '9' || cSingleChar < '0') && (cSingleChar != ',') && (cSingleChar != 'X'))
		{
			continue;
		}
		*p++ = cSingleChar;
	}

	string strData = pDataBuf;
	free(pDataBuf);
	fclose(fp);

	return strData;
}

/*
	给定一个增序排列数组 nums ，你需要在 原地 删除重复出现的元素，使得每个元素最多出现两次，返回移除后数组的新长度。
	不要使用额外的数组空间，你必须在 原地 修改输入数组 并在使用 O(1) 额外空间的条件下完成。
	说明：
	为什么返回数值是整数，但输出的答案是数组呢？
	请注意，输入数组是以“引用”方式传递的，这意味着在函数里修改输入数组对于调用者是可见的。
	你可以想象内部操作如下：
	// nums 是以“引用”方式传递的。也就是说，不对实参做任何拷贝
	int len = removeDuplicates(nums);
	// 在函数里修改输入数组对于调用者是可见的。
	// 根据你的函数返回的长度, 它会打印出数组中该长度范围内的所有元素。
	for (int i = 0; i < len; i++) {
   		print(nums[i]);
	}
	示例 1：
	输入：nums = [1,1,1,2,2,3]
	输出：5, nums = [1,1,2,2,3]
	解释：函数应返回新长度 length = 5, 并且原数组的前五个元素被修改为 1, 1, 2, 2, 3 。 你不需要考虑数组中超出新长度后面的元素。
*/
int removeDuplicates(vector<int>& vctNums)
{
	int iSize = 0;

	for (int i = 0; i < vctNums.size(); i++)
	{
		int iCount = count(vctNums.begin(), vctNums.end(), vctNums[i]);
		while (iCount > 2)
		{
			for (vector<int>::iterator it = vctNums.begin(); it != vctNums.end(); it++)
			{
				if (iCount > 2)
				{
					if (vctNums[i] == *it)
					{
						it = vctNums.erase(it); // 指向删除元素的下一个元素，不然会崩溃
						iCount--;
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	iSize = vctNums.size();

	return iSize;
}
/*
	给你两个字符串 word1 和 word2 。请你从 word1 开始，通过交替添加字母来合并字符串。如果一个字符串比另一个字符串长，就将多出来的字母追加到合并后字符串的末尾。
	返回 合并后的字符串 。
	示例 1：
	输入：word1 = "abc", word2 = "pqr"
	输出："apbqcr"
	解释：字符串合并情况如下所示：
	word1：  a   b   c
	word2：    p   q   r
	合并后：  a p b q c r
	示例 2：
	输入：word1 = "ab", word2 = "pqrs"
	输出："apbqrs"
	解释：注意，word2 比 word1 长，"rs" 需要追加到合并后字符串的末尾。
	word1：  a   b
	word2：    p   q   r   s
	合并后：  a p b q   r   s
	输入：word1 = "abcd", word2 = "pq"
	输出："apbqcd"
	解释：注意，word1 比 word2 长，"cd" 需要追加到合并后字符串的末尾。
	word1：  a   b   c   d
	word2：    p   q
	合并后：  a p b q c   d
*/
string mergeAlternately(string strWordOne, string strWordTwo)
{
	string strRes;

	while (1)
	{
		if (strWordOne.empty() && strWordTwo.empty())
		{
			break;
		}

		if (!strWordOne.empty())
		{
			strRes.push_back(strWordOne.at(0));
			strWordOne.erase(0, 1);
		}

		if (!strWordTwo.empty())
		{
			strRes.push_back(strWordTwo.at(0));
			strWordTwo.erase(0, 1);
		}
	}

	return strRes;
}
/*
	数组中占比超过一半的元素称之为主要元素。给定一个整数数组，找到它的主要元素。若没有，返回-1。
	示例 1：
	输入：[1,2,5,9,5,9,5,5,5]
	输出：5
	示例 2：
	输入：[3,2]
	输出：-1
	示例 3：
	输入：[2,2,1,1,1,2,2]
	输出：2
*/
int majorityElement(vector<int>& vctNums)
{
	int iCount = 0;
	int iHas = 0;
	int iSize = vctNums.size() / 2;


	vector<int>vctTemp;
	vctTemp = vctNums;

	sort(vctTemp.begin(), vctTemp.end());
	vctTemp.erase(unique(vctTemp.begin(), vctTemp.end()), vctTemp.end());

	for (int i = 0; i < vctTemp.size(); i++)
	{
		int iTemp = count(vctNums.begin(), vctNums.end(), vctTemp[i]);
		if (iTemp > iSize)
		{
			iHas++;
			iCount = vctTemp[i];
			break;
		}
	}

	if (0 == iHas)
	{
		iCount = -1;
	}

	return iCount;
}
/*
	给你一个字符串 s，以及该字符串中的一些「索引对」数组 pairs，其中 pairs[i] = [a, b] 表示字符串中的两个索引（编号从 0 开始）。
	你可以 任意多次交换 在 pairs 中任意一对索引处的字符。
	返回在经过若干次交换后，s 可以变成的按字典序最小的字符串
	示例 1:
	输入：s = "dcab", pairs = [[0,3],[1,2]]
	输出："bacd"
	解释：
	交换 s[0] 和 s[3], s = "bcad"
	交换 s[1] 和 s[2], s = "bacd"
	示例 2：
	输入：s = "dcab", pairs = [[0,3],[1,2],[0,2]]
	输出："abcd"
	解释：
	交换 s[0] 和 s[3], s = "bcad"
	交换 s[0] 和 s[2], s = "acbd"
	交换 s[1] 和 s[2], s = "abcd"
*/
string smallestStringWithSwaps(string strStrand, vector<vector<int>>& vctPair)
{
	string strTemp = strStrand;

	for (vector<vector<int>>::iterator it = vctPair.begin(); it != vctPair.end(); it++)
	{
		string strFront;
		string strBack;
		char strTran;

		strFront = strTemp.at(it->at(0));
		strBack = strTemp.at(it->at(1));

		strTran = strTemp.at(it->at(0));
		strTemp.at(it->at(0)) = strTemp.at(it->at(1));
		strTemp.at(it->at(1)) = strTran;
	}

	return strTemp;
}
/*
	数字以0123456789101112131415…的格式序列化到一个字符序列中。在这个序列中，第5位（从下标0开始计数）是5，第13位是1，第19位是4，等等。
	请写一个函数，求任意第n位对应的数字。
	示例 1：
	输入：n = 3
	输出：3
*/
int findNthDigit(int iNum)
{




	return iNum;
}

/*
	给定两个整数 A 和 B，返回任意字符串 S，要求满足：

	S 的长度为 A + B，且正好包含 A 个 'a' 字母与 B 个 'b' 字母；
	子串 'aaa' 没有出现在 S 中；
	子串 'bbb' 没有出现在 S 中。
	输入：A = 4, B = 1
	输出："aabaa"
*/
string strWithout3a3b(int iNumA, int iNumB)
{
	string strA;
	string strB;
	string strval;

	while ((iNumA != strA.size()))
	{
		strA += "a";
	}
	while ((iNumB != strB.size()))
	{
		strB += "b";
	}

	strval = strA + strB;
	if (string::npos != strval.find("aaa") || string::npos != strval.find("bbb"))
	{

	}

	return strval;
}

/*
	给定一个数组，将数组中的元素向右移动 k 个位置，其中 k 是非负数。

	示例 1:

	输入: [1,2,3,4,5,6,7] 和 k = 3
	输出: [5,6,7,1,2,3,4]
	解释:
	向右旋转 1 步: [7,1,2,3,4,5,6]
	向右旋转 2 步: [6,7,1,2,3,4,5]
	向右旋转 3 步: [5,6,7,1,2,3,4]
*/

void rotate(vector<int>& vctNum, int iPos)
{
	if (iPos < 0)
	{
		return;
	}

	for (int i = 0; i < iPos; i++)
	{
		vctNum.insert(vctNum.begin(), vctNum.back());
		vctNum.pop_back();
	}
}
/*
	示例 1：
	输入：names = ["pes","fifa","gta","pes(2019)"]
	输出：["pes","fifa","gta","pes(2019)"]
	解释：文件系统将会这样创建文件名：
	"pes" --> 之前未分配，仍为 "pes"
	"fifa" --> 之前未分配，仍为 "fifa"
	"gta" --> 之前未分配，仍为 "gta"
	"pes(2019)" --> 之前未分配，仍为 "pes(2019)"
	示例 2：
	输入：names = ["gta","gta(1)","gta","avalon"]
	输出：["gta","gta(1)","gta(2)","avalon"]
	解释：文件系统将会这样创建文件名：
	"gta" --> 之前未分配，仍为 "gta"
	"gta(1)" --> 之前未分配，仍为 "gta(1)"
	"gta" --> 文件名被占用，系统为该名称添加后缀 (k)，由于 "gta(1)" 也被占用，所以 k = 2 。实际创建的文件名为 "gta(2)" 。
	"avalon" --> 之前未分配，仍为 "avalon"
*/
vector<string> getFolderNames(vector<string>& vctFileName)
{
	string strTemp;
	string strTran;
	string strNum;

	vector<string>vctStr;
	vector<string>vctTran;
	vector<string>vctRepeat;
	vctStr = vctFileName;

	int iCount = 0;
	int iIndex = 0;

	for (vector<string>::iterator it = vctStr.begin(); it != vctStr.end(); it++)
	{
		iCount = count(vctStr.begin(), vctStr.end(), *it);
		if (iCount > 1)
		{
			vctRepeat.push_back(*it);
			it = vctStr.erase(it);
		}
	}

	for (int j = 0; j < vctRepeat.size(); j++)
	{
		int iNum = 1;
		strNum = to_string(iNum);
		strTran = vctRepeat[j] + "(" + strNum + ")";

		while (vctFileName.end() != find(vctFileName.begin(), vctFileName.end(), strTran))
		{
			iNum++;
			strNum = to_string(iNum);
			strTran = vctRepeat[j] + "(" + strNum + ")";
		}
		vctTran.push_back(strTran);
	}

	vctStr.insert(vctStr.begin(), vctTran.begin(), vctTran.end());

	return vctStr;
}

/*
	给定一个只包含整数的有序数组，每个元素都会出现两次，唯有一个数只会出现一次，找出这个数。

	示例 1:

	输入: [1,1,2,3,3,4,4,8,8]
	输出: 2
*/

int singleNonDuplicate(vector<int>& vctNum)
{
	int ival = 0;
	int iCount = 0;

	for (vector<int>::iterator it = vctNum.begin(); it != vctNum.end(); it++)
	{
		iCount = count(vctNum.begin(), vctNum.end(), *it);
		if (iCount == 1)
		{
			ival = *it;
		}
	}

	return ival;
}

/*
	给定一个字符串，请将字符串里的字符按照出现的频率降序排列。

	示例 1:

	输入:
	"tree"

	输出:
	"eert"

	解释:
	'e'出现两次，'r'和't'都只出现一次。
	因此'e'必须出现在'r'和't'之前。此外，"eetr"也是一个有效的答案。
	示例 2:

	输入:
	"cccaaa"

	输出:
	"cccaaa"

	解释:
	'c'和'a'都出现三次。此外，"aaaccc"也是有效的答案。
	注意"cacaca"是不正确的，因为相同的字母必须放在一起。
	示例 3:

	输入:
	"Aabb"

	输出:
	"bbAa"

	解释:
	此外，"bbaA"也是一个有效的答案，但"Aabb"是不正确的。
	注意'A'和'a'被认为是两种不同的字符。
*/
int CountStr(string str, char strChar)
{
	string strTemp = str;
	int iCount = 0;
	while (string::npos != strTemp.find(strChar))
	{
		iCount++;
		int iPos = strTemp.find(strChar);
		strTemp.erase(strTemp.begin() + iPos);
	}
	return iCount;
}
string AddStr(char chTemp, int iNum)
{
	string strTemp;

	if (1 == iNum)
	{
		strTemp += chTemp;
		return strTemp;
	}

	while (0 != iNum)
	{
		strTemp += chTemp;
		iNum--;
	}

	return strTemp;
}
string frequencySort(string strChar)
{
	vector<char>chVct;
	map<int, vector<char>>mapStr;
	int iCount = 0;
	string strTemp;
	string strval;
	string strFront;
	string strBack;

	for (int i = 0; i < strChar.size(); i++)
	{
		chVct.push_back(strChar[i]);
	}

	sort(chVct.begin(), chVct.end());
	chVct.erase(unique(chVct.begin(), chVct.end()), chVct.end());

	for (int j = 0; j < chVct.size(); j++)
	{
		iCount = count(strChar.begin(), strChar.end(), chVct[j]);
		mapStr[iCount].push_back(chVct[j]);
	}

	for (map<int, vector<char>>::reverse_iterator it = mapStr.rbegin(); it != mapStr.rend(); it++)
	{
		for (vector<char>::reverse_iterator itRe = it->second.rbegin(); itRe != it->second.rend(); itRe++)
		{
			strTemp = AddStr(*itRe, it->first);
			strval += strTemp;
		}
	}

	return strval;
}
string getText(string strText, string strIndex)
{
	vector<pair<string, string>>vctpStr;
	string strTemp;
	string strval;
	strTemp = strText;
	int iCount = 0;

	while (string::npos != strTemp.find("#"))
	{
		if (0 == iCount)
		{
			int iTemp = strTemp.find("|");
			strTemp.erase(strTemp.begin(), strTemp.begin() + iTemp + 1);
		}

		iCount++;

		string strKey;
		string strContent;
		int iPosFront = strTemp.find("#");
		int iPosBack = strTemp.find("|");
		strKey = strTemp.substr(iPosFront + 1, iPosBack - iPosFront - 1);
		strTemp.erase(strTemp.begin(), strTemp.begin() + iPosBack + 1);

		iPosFront = strTemp.find("@");
		strContent = strTemp.substr(0, iPosFront);
		vctpStr.push_back(pair<string, string>(strKey, strContent));
	}

	for (vector<pair<string, string>>::iterator it = vctpStr.begin(); it != vctpStr.end(); it++)
	{
		if (strIndex == it->first)
		{
			strval = it->second;
			break;
		}
	}

	return strval;
}
// "(ed(et(oc))el)"
// leetcode
string reverseParentheses(string s)
{
	stack<char> mains;//主要存放字符串s的栈
	queue<char> tempq;//暂时队列，用于反转操作
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ')')
		{
			while (mains.top() != '(')
			{
				tempq.push(mains.top());
				mains.pop();
			}//遇到')'时，进行反转操作，直到下一个'('停止
			mains.pop();//将'('弹出栈
			while (!tempq.empty())
			{
				mains.push(tempq.front());
				tempq.pop();
			}//反转后放回主栈中
		}
		else
		{
			mains.push(s[i]);
		}//如果不是')'，继续入栈

	}
	int size = mains.size();
	string ret(size, 'a');//设置一个大小和主栈大小相同的字符串用于返回值
	for (int i = size - 1; i >= 0; i--)
	{
		ret[i] = mains.top();
		mains.pop();
	}//将主栈中元素全部放入字符串
	return ret;
}
/*
	给出一个字符串 s（仅含有小写英文字母和括号）。
	请你按照从括号内到外的顺序，逐层反转每对匹配括号中的字符串，并返回最终的结果。
	注意，您的结果中 不应 包含任何括号。
	示例 1：
	输入：s = "(abcd)"
	输出："dcba"
	示例 2：
	输入：s = "(u(love)i)"
	输出："iloveu"
	示例 3：
	输入：s = "(ed(et(oc))el)"
	输出："leetcode"
*/
vector<pair<int, int>>ReturnSignPos(string strStrand) // 返回括号对应位置
{
	vector<pair<int, int>>vctSignPos;
	vector<int>vctTemp;

	string strTemp = strStrand;
	int iSignCount = count(strTemp.begin(), strTemp.end(), '(');

	while (0 != iSignCount)
	{
		int iFront = strTemp.find("(");
		int iBack = strTemp.find(")");
		vctSignPos.push_back(pair<int, int>(iFront, iBack));

		string::iterator itFront;
		string::iterator itBack;
		itFront = find(strTemp.begin(), strTemp.end(), '(');
		itBack = find(strTemp.begin(), strTemp.end(), ')');

		if (itFront != strTemp.end())
		{
			strTemp.erase(itFront);
			strTemp.insert(iFront, "0");
		}

		if (itBack != strTemp.end())
		{
			strTemp.erase(itBack);
			strTemp.insert(iBack, "0");
		}
		iSignCount--;
	}

	for (vector<pair<int, int>>::reverse_iterator it = vctSignPos.rbegin(); it != vctSignPos.rend(); it++)
	{
		vctTemp.push_back(it->second);
	}

	for (int i = 0; i < vctSignPos.size(); i++)
	{
		vctSignPos[i].second = vctTemp[i];
	}

	return vctSignPos;
}

/*
	给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
	给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
	示例 1：
	输入：digits = "23"
	输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
	示例 2：

	输入：digits = ""
	输出：[]
	示例 3：
	["adg","adh","adi","aeg","aeh","aei","afg","afh","afi","bdg","bdh","bdi","beg","beh","bei","bfg","bfh","bfi","cdg","cdh","cdi","ceg","ceh","cei","cfg","cfh","cfi"]
	输入：digits = "2"
	输出：["a","b","c"]
*/
//vector<string> letterCombinations(string strNum)
//{
//	vector<string>vctStr;
//	map<char, vector<string>>mapCombination;
//
//	string strDigits = "23456789";
//	string strChar = "abcdefghijklmnopqrstuvwxyz";
//	string strTemp;
//	string strTrand;
//
//	int iCount = 0;
//	int iDelete = 0;
//	int iDigitSize = strDigits.size();
//	int iCharSize = strChar.size();
//
//	strTemp = strDigits;
//	strTrand = strChar;
//	while (0 != iDigitSize)
//	{
//		iCount++;
//		if (iCount != 6 && iCount != strDigits.size())
//		{
//			mapCombination[strTemp.front()].push_back(strTrand.substr(0, 3));
//			strTrand.erase(0, 3);
//		}
//		else
//		{
//			mapCombination[strTemp.front()].push_back(strTrand.substr(0, 4));
//			strTrand.erase(0, 4);
//		}
//		strTemp.erase(strTemp.begin(), strTemp.begin() + 1);
//		iDigitSize--;
//	}
//
//	if (strNum.size() == 1)
//	{	
//		for (int i = 0; i < mapCombination[strNum.front()].front().size(); i++)
//		{
//			string strTemp;
//			strTemp += mapCombination[strNum.front()].front()[i];
//			vctStr.push_back(strTemp);
//		}
//
//		return vctStr;
//	}
//
//	if (strNum.size() == 0)
//	{
//		return vctStr;
//	}
//
//	int iCountTemp = -1;
//	while (iCountTemp != strNum.size() - 2)
//	{
//		iCountTemp++;
//		vector<string>vctFront;
//		vector<string>vctSecond;
//		vctFront = mapCombination[strNum.at(iCountTemp)];
//		if (iCountTemp != strNum.size() - 1)
//		{
//			vctSecond = mapCombination[strNum.at(iCountTemp + 1)];
//		}
//		else
//		{
//			vctSecond = mapCombination[strNum.at(iCountTemp)];
//		}
//
//		while (vctFront.front().size() != 0)
//		{
//			int iSecondSize = -1;
//			while (iSecondSize != vctSecond.front().size() - 1)
//			{
//				iSecondSize++;
//				string strval;
//				strval += vctFront.front().front();
//				strval += vctSecond.front().at(iSecondSize);
//				vctStr.push_back(strval);
//			}
//
//			vctFront.front().erase(vctFront.front().begin(), vctFront.front().begin() + 1);
//		}
//	}
//
//	return vctStr;
//}

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

void WriteTxt(string strFilePath, string strText)
{
	FILE* fpFile = NULL;

	int iFileLength = 0;

	fpFile = fopen(strFilePath.c_str(), "w+");
	if (fpFile == NULL)
	{
		return;
	}

	iFileLength = strText.size() + 1;

	int iWriteLength = fwrite(strText.c_str(), sizeof(char), iFileLength, fpFile);

	if (fpFile)
	{
		fclose(fpFile);
		return;
	}
}

void AppendText(string strFilePath, string strText)
{
	FILE* fpFile = NULL;

	int iFileLength = 0;

	fpFile = fopen(strFilePath.c_str(), "a+");
	if (fpFile == NULL)
	{
		return;
	}

	iFileLength = strText.size() + 1;

	int iWriteLength = fwrite(strText.c_str(), sizeof(char), iFileLength, fpFile);

	if (fpFile)
	{
		fclose(fpFile);
		return;
	}
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

	return strText;
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

		//if (string::npos != strTemp.find(strIndex))
		//{
		//	vctStrText.push_back(strTemp);
		//}

		vctStrText.push_back(strTemp);
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

//1. 用map记录每个数字按键对应的所有字母
map<char, string> M = {
	{'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"},
	{'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}
};
//2. 存储最终结果和临时结果的变量
vector<string> ans;
string current;

//3. DFS函数，index是生成临时结果字串的下标，
//每一个digits[index]数字对应临时结果current[index]的一位字母
void DFS(int index, string digits)
{
	//出口
	if (index == digits.size())
	{
		ans.push_back(current);
		return;
	}
	//递归调用
	//对于当前输入的第index号数字(digits[index])，
	//枚举其对应的所有字母(M[digits[index]][i])
	for (int i = 0; i < M[digits[index]].size(); i++)
	{
		current.push_back(M[digits[index]][i]);     //临时结果压入一个字母
		DFS(index + 1, digits);         //以在当前位置压入该字母这一“情况”为前提，构造此“分支”的后续结果
		current.pop_back();             //状态还原，例如临时结果从 "ab" -> "a"，下一次循环尝试"ac" 
	}
}

vector<string> letterCombinations(string digits)
{
	if (digits.size() == 0)
	{
		return ans;
	}
	DFS(0, digits);
	return ans;
}


string ChangeEvCommand(string strEvFile, string strSystemReq)
{
	string strCommand;
	string strSystemRes;
	string strEvName;
	string strEvVersion;
	string strEvCommand;
	string strEvReq;
	string strEvRes;
	string strTemp;

	vector<string>vctEvCommand;
	vector<string>vctEvRes;

	int iSignFirstPos = 0;
	int iSignSecondPos = 0;
	int iTemp = 0;
	int iTempArray[100] = {};

	char* chXtemp;
	char chTemp[100] = {};

	if ((strEvFile.empty()) || (string::npos == strEvFile.find("_")) || (2 != count(strEvFile.begin(), strEvFile.end(), '_')))
	{
		return strCommand;
	}

	iSignFirstPos = strEvFile.find("_");
	iSignSecondPos = strEvFile.find("_", iSignFirstPos + 1);

	strEvName = strEvFile.substr(0, iSignSecondPos);
	strEvVersion = strEvFile.substr(iSignSecondPos + 1, strEvFile.size() - iSignSecondPos);

	if (2 != strSystemReq.size())
	{
		return strCommand;
	}

	int iTime = strtol(strSystemReq.c_str(), &chXtemp, 16);

	if ((224 == iTime) || (225 == iTime))
	{
		iTime = iTime + 8;
	}
	else
	{
		iTime = iTime + 106;
	}

	memset(chTemp, 0, 100);
	sprintf(chTemp, "%02X", iTime);
	strSystemRes += chTemp;

	for (int i = 0; i < strEvName.size(); i++)
	{
		iTempArray[i] = strEvName[i];
	}

	int iSize = strEvName.size();
	int iCount = -1;
	while (iSize != 0)
	{
		string strTrand;
		iCount++;
		memset(chTemp, 0, 100);
		sprintf(chTemp, "%02X", iTempArray[iCount]);
		strTrand += chTemp;
		vctEvCommand.push_back(strTrand);
		iSize--;
	}

	strEvReq = "08 07 " + strSystemReq + " 03 22 F1 9E 55 55 55 55";
	strEvReq = "REQ=" + strEvReq;
	iCount = -1;


	while (1)
	{
		iCount++;
		string strResTemp;

		if (0 == iCount)
		{
			int iSizeTemp = vctEvCommand.size();
			iSizeTemp += 4;
			memset(chTemp, 0, 100);
			sprintf(chTemp, "%02X", iSizeTemp);
			strTemp += chTemp;

			strResTemp = "08 07 " + strSystemRes + " " + "10 " + strTemp + " 62 F1 9E";
			strResTemp = "RES=" + strResTemp;

			for (vector<string>::iterator it = vctEvCommand.begin(); it != vctEvCommand.end(); it++)
			{
				if (strResTemp.size() != 36)
				{
					if (!((*it).empty()))
					{
						strResTemp.append(" " + *it);
					}
					*it = "";
				}
				else
				{
					break;
				}
			}
			vctEvRes.push_back(strResTemp);
		}
		else
		{
			strResTemp = "08 07 " + strSystemRes + " " + "**";
			strResTemp = "RES=" + strResTemp;

			for (vector<string>::iterator it = vctEvCommand.begin(); it != vctEvCommand.end(); it++)
			{
				if (strResTemp.size() < 36)
				{
					if (!((*it).empty()))
					{
						strResTemp.append(" " + *it);
					}
					*it = "";
				}
			}
			vctEvRes.push_back(strResTemp);
		}

		int iFlag = 0;
		for (int i = 0; i < vctEvCommand.size(); i++)
		{
			if (!vctEvCommand[i].empty())
			{
				iFlag++;
				break;
			}
		}

		if (iFlag == 0)
		{
			while (vctEvRes.back().size() < 36)
			{
				vctEvRes.back().append(" 55");
			}
			break;
		}
		else
		{
			iFlag = 0;
		}
	}

	int iRESNum = count(vctEvRes.begin(), vctEvRes.end(), "RES");
	iCount = 0;

	for (vector<string>::iterator it = vctEvRes.begin(); it != vctEvRes.end(); it++)
	{
		strTemp.clear();
		if (string::npos != (*it).find("**"))
		{
			iCount++;

			int iPosTemp = (*it).find("**");
			(*it).erase(iPosTemp, 2);

			memset(chTemp, 0, 100);
			sprintf(chTemp, "%d", 20 + iCount);
			strTemp += chTemp;

			(*it).insert(iPosTemp, strTemp);
		}
	}

	iTemp = vctEvRes.back().find("55", 0);
	vctEvRes.back().replace(iTemp, 2, "00");

	strEvCommand += strEvReq;
	for (int i = 0; i < vctEvRes.size(); i++)
	{
		strEvCommand += "\n" + vctEvRes[i];
	}

	strTemp = "REQ=08 07 " + strSystemReq + " 03 22 F1 A2 55 55 55 55" + "\n" + "RES=08 07 " + strSystemRes + " 10 09 62 F1 A2 ";

	memset(iTempArray, 0, 100);
	for (int i = 0; i < strEvVersion.size(); i++)
	{
		iTempArray[i] = strEvVersion[i];
	}

	iSize = strEvVersion.size();
	iCount = -1;
	while (iSize != 0)
	{
		string strTrand;
		iCount++;
		memset(chTemp, 0, 100);
		sprintf(chTemp, "%02X", iTempArray[iCount]);
		strTrand += chTemp;
		strTemp += strTrand + " ";
		iSize--;
	}
	strTemp = strTemp + "\n" + "RES=" + "08 07 " + strSystemRes + " 21 30 30 33 55 55 55 55";
	strCommand = strEvCommand + "\n\n" + strTemp;

	return strCommand;
}

/*
	给定一组非负整数 nums，重新排列它们每个数字的顺序（每个数字不可拆分）使之组成一个最大的整数。
	注意：输出结果可能非常大，所以你需要返回一个字符串而不是整数。
	示例 1：
	输入：nums = [10,2]
	输出："210"
	示例 2：
	输入：nums = [3,30,34,5,9]
	输出："9534330"
	示例 3：
	输入：nums = [1]
	输出："1"
*/
static bool AscIICompare(int iNumOne, int iNumTwo)
{
	return to_string(iNumOne) + to_string(iNumTwo) > to_string(iNumTwo) + to_string(iNumOne);
}
string largestNumber(vector<int>& vctNums)
{
	string strRes;

	sort(vctNums.begin(), vctNums.end(), AscIICompare);

	for (vector<int>::iterator it = vctNums.begin(); it != vctNums.end(); it++)
	{
		strRes += to_string(*it);
	}

	while ((strRes.size() > 1) && (strRes.front() == '0'))
	{
		strRes.erase(0, 1);
	}

	return strRes;
}


/*
	你有 4 张写有 1 到 9 数字的牌。你需要判断是否能通过 *，/，+，-，(，) 的运算得到 24。
	示例 1:
	输入: [4, 1, 8, 7]
	输出: True
	解释: (8-4) * (7-1) = 24
	示例 2:
	输入: [1, 2, 1, 2]
	输出: False
*/
static constexpr double ex = 1e-6;//精度
static constexpr double tag = 24;//目标值

bool dfs(vector<double>& nums)
{
	//边界条件： 没有选择了，直接返回
	if (nums.size() == 0)
	{
		return false;
	}

	//基线条件： 只有一种选择，直接判断结果
	if (nums.size() == 1)
	{
		return abs(tag - nums[0]) < ex;
	}

	//当前能做的选择:挑 2 个数，进行 4 种运算
	for (int i = 0; i < nums.size(); ++i)//挑选第一个数
	{
		for (int j = 0; j < nums.size(); ++j)// 挑选第二个数
		{
			if (i != j)
			{
				vector<double> curnums;//保存剩余元素
				for (int k = 0; k < nums.size(); ++k)
				{
					if (k != i && k != j)
					{
						curnums.emplace_back(nums[k]);
					}
				}
				//尝试每一种运算
				// a+b
				curnums.emplace_back(nums[i] + nums[j]);
				bool isok = dfs(curnums);
				curnums.pop_back();
				if (isok)
				{
					return true;
				}

				// a- b
				curnums.emplace_back(nums[i] - nums[j]);
				isok = dfs(curnums);
				curnums.pop_back();
				if (isok)
				{
					return true;
				}

				// b-a 已遍历了所有 a 和 b 的组合，没必要再进行 b-a
				/*
				curnums.emplace_back( nums[j]-nums[i] );
				isok =  dfs( curnums);
				curnums.pop_back();
				if( isok )  return true;*/
				// a*b
				curnums.emplace_back(nums[i] * nums[j]);
				isok = dfs(curnums);
				curnums.pop_back();
				if (isok)
				{
					return true;
				}

				// a / b
				if (nums[j] != 0)
				{
					curnums.emplace_back(nums[i] / nums[j]);
					isok = dfs(curnums);
					curnums.pop_back();
					if (isok)
					{
						return true;
					}
				}

				// b /a
				/*
				if( nums[i]!= 0 )
				{
					curnums.emplace_back( nums[j] / nums[i] );
					isok =  dfs( curnums);
					curnums.pop_back();
					if( isok)   return true;
				} */
			}
		}
	}
	return false;
}
bool judgePoint24(vector<int>& nums)
{
	vector<double> curnums;
	for (int i = 0; i < nums.size(); i++)
	{
		curnums.emplace_back(double(nums[i]));
	}

	return dfs(curnums);
}

/*
	在一个 平衡字符串 中，'L' 和 'R' 字符的数量是相同的。
	给你一个平衡字符串 s，请你将它分割成尽可能多的平衡字符串。
	返回可以通过分割得到的平衡字符串的 最大数量 。
	示例 1：
	输入：s = "RLRRLLRLRL"
	输出：4
	解释：s 可以分割为 "RL"、"RRLL"、"RL"、"RL" ，每个子字符串中都包含相同数量的 'L' 和 'R' 。
	示例 2：
	输入：s = "RLLLLRRRLR"
	输出：3
	解释：s 可以分割为 "RL"、"LLLRRR"、"LR" ，每个子字符串中都包含相同数量的 'L' 和 'R' 。
	输入：s = "LLLLRRRR"
	输出：1
	解释：s 只能保持原样 "LLLLRRRR".
	示例 4：
	输入：s = "RLRRRLLRLL"
	输出：2
	解释：s 可以分割为 "RL"、"RRRLLRLL" ，每个子字符串中都包含相同数量的 'L' 和 'R' 。
*/
vector<string>vctBalance;
void DFSbalancedStringSplit(string& strBalance, string& strTrand)
{
	if (strBalance.empty())
	{
		return;
	}

	strTrand += strBalance.front();
	strBalance.erase(strBalance.begin(), strBalance.begin() + 1);

	if (count(strTrand.begin(), strTrand.end(), 'R') == count(strTrand.begin(), strTrand.end(), 'L'))
	{
		vctBalance.push_back(strTrand);
		strTrand.clear();
	}

	DFSbalancedStringSplit(strBalance, strTrand);
}
//int balancedStringSplit(string strBalance)
//{
//	vector<string>vctstr;
//	string strTemp = strBalance;
//	string strTrand;
//	int iRes = 0;
//
//	while (!strTemp.empty())
//	{
//		if ('R' == strTemp.front())
//		{
//			strTrand.push_back(strTemp.front());
//		}
//
//		if ('L' == strTemp.front())
//		{
//			strTrand.push_back(strTemp.front());
//		}
//
//		if (!strTrand.empty())
//		{
//			if (count(strTrand.begin(), strTrand.end(), 'R') == count(strTrand.begin(), strTrand.end(), 'L'))
//			{
//				vctstr.push_back(strTrand);
//				iRes++;
//				strTrand.clear();
//			}
//		}
//
//		strTemp.erase(strTemp.begin(), strTemp.begin() + 1);
//	}
//
//	return iRes;
//}
int balancedStringSplit(string strBalance)
{
	string strTemp;
	DFSbalancedStringSplit(strBalance, strTemp);

	return vctBalance.size();
}
/*
	找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。
	说明：
	所有数字都是正整数。
	解集不能包含重复的组合。 
	示例 1:
	输入: n = 7,k = 3
	输出: [[1,2,4]]
	示例 2:
	输入: n = 9,k = 3
	输出: [[1,2,6], [1,3,5], [2,3,4]]
*/
vector<int> vctTemp;
vector<vector<int>>vctAddSum;
void dfs(int iAddRes, int iNumCount, int iCurrent)  // 回溯
{
	if (vctTemp.size() == iNumCount)
	{
		if (iAddRes == 0)
		{
			vctAddSum.push_back(vctTemp);
			return;
		}
		else
		{
			return;
		}
	}

	if (iCurrent > 9 || iCurrent > iAddRes)
	{
		return;
	}
	vctTemp.push_back(iCurrent);
	dfs(iNumCount, iAddRes - iCurrent, iCurrent + 1);
	vctTemp.pop_back();
	dfs(iNumCount, iAddRes, iCurrent + 1);
}
vector<vector<int>> combinationSum3(int iAddRes, int iNumCount)
{
	dfs(iAddRes, iNumCount, 1);
	return vctAddSum;
}





/*
	你一个字符串 s 和一个 长度相同 的整数数组 indices
	请你重新排列字符串 s ，其中第 i 个字符需要移动到 indices[i] 指示的位置
	返回重新排列后的字符串。
	示例 1：
	输入：s = "codeleet", indices = [4,5,6,7,0,2,1,3]
	输出："leetcode"
	解释：如图所示，"codeleet" 重新排列后变为 "leetcode" 。
	示例 2：
	输入：s = "abc", indices = [0,1,2]
	输出："abc"
	解释：重新排列后，每个字符都还留在原来的位置上。
*/
string restoreString(string strTrand, vector<int>& vctIndices)
{
	vector<pair<int, char>>pvctIndex;

	for (int i = 0; i < vctIndices.size(); i++)
	{
		pvctIndex.push_back(make_pair(vctIndices[i], strTrand.at(i)));
	}

	for (vector<pair<int, char>>::iterator it = pvctIndex.begin(); it != pvctIndex.end(); it++)
	{
		strTrand.at(it->first) = it->second;
	}

	return strTrand;
}


vector<string>SplitStrText(string strText) // 分割文本
{
	vector<string>vctStrTemp;

	string strTemp;
	strTemp = strText;

	int iCount = 0;

	while (string::npos != strTemp.find("@#"))
	{
		iCount++;

		string strTrand;
		int iPosTemp = 0;
		int iPosSecond = 0;

		if (1 == iCount)
		{
			iPosTemp = strTemp.find("@#");
			strTrand = strTemp.substr(0, iPosTemp);
			strTemp.erase(strTemp.begin(), strTemp.begin() + iPosTemp - 1);
			vctStrTemp.push_back(strTrand);
		}
		else
		{
			iPosTemp = strTemp.find("@#");
			iPosSecond = strTemp.find("@#", iPosTemp + 1);

			if (-1 == iPosSecond)
			{
				strTrand = strTemp.substr(iPosTemp, strTemp.size() - 1);
				strTemp.erase(iPosTemp, strTemp.size() - 1);

				if (string::npos != strTrand.find('"'))
				{
					strTrand.erase(strTrand.begin() + strTrand.find('"'), strTrand.end());
				}

				vctStrTemp.push_back(strTrand);
			}
			else
			{
				strTrand = strTemp.substr(iPosTemp, iPosSecond - 1);
				strTemp.erase(iPosTemp, iPosSecond - 1);
				vctStrTemp.push_back(strTrand);
			}
		}
	}

	for (int i = 0; i < vctStrTemp.size(); i++)
	{
		if (0 != i)
		{
			if ((string::npos == vctStrTemp[i].find("@#")) && (string::npos == vctStrTemp[i].find('|')))
			{
				vctStrTemp.clear();
				return vctStrTemp;
			}
		}
		else
		{
			if (string::npos == vctStrTemp[i].find('|'))
			{
				vctStrTemp.clear();
				return vctStrTemp;
			}
		}
	}

	return vctStrTemp;
}

string JudgeIndexAudiOrVw(string strCbinaryIndex) // 判断这个ID属于奥迪还是大众
{
	string strVwGfsPath = "D:\\Platform\\maker\\GS_VW\\GFS\\XTOOL_0056\\CN_GFS.TXT";
	string strAudiGfsPath = "D:\\Platform\\maker\\GS_VW\\GFS\\XTOOL_0041\\CN_GFS.TXT";
	string strVwText;
	string strAudiText;
	string strRes;

	strAudiText = GetAllFileText(strAudiGfsPath);
	strVwText = GetAllFileText(strVwGfsPath);

	if ((strAudiText.empty()) || (strVwText.empty()))
	{
		strRes = "";
	}
	else
	{
		if (string::npos != strAudiText.find(strCbinaryIndex))
		{
			strRes = "AUDI";
		}
		else if (string::npos != strVwText.find(strCbinaryIndex))
		{
			strRes = "VW";
		}
		else
		{
			strRes = "";
		}
	}

	return strRes;
}

string GetXtoolLastIndex(string strModel, string strFilePath) // 获取最后一行
{
	string strRes;

	FILE* pFile = NULL;

	char* chLine = NULL;

	int iFileLen = 0;
	pFile = fopen(strFilePath.c_str(), "r");
	if (NULL == pFile)
	{
		return strRes;
	}

	fseek(pFile, 0, SEEK_END); // 移动指针至文件尾部
	iFileLen = ftell(pFile); // 计算文件长度
	chLine = (char*)malloc(iFileLen + 1); // 获取内存空间 长度???
	memset(chLine, 0, iFileLen + 1); // 初始化内存空间
	//fseek(pFile, 0L, SEEK_SET); // 移动指针至文件头部

	int iFileCount = -2;
	string strIndex;

	if ("Xtool" == strModel)
	{
		while (1)
		{
			if ('#' == fgetc(pFile))
			{
				break;
			}
			else
			{
				char chTemp = fgetc(pFile);
				strIndex += chTemp;
			}

			iFileCount--;
			fseek(pFile, iFileCount, SEEK_END);  // 指针从末尾向前递减，找出最后一行
		}

		strRes = UTF8ToGB(strIndex.c_str());  // 可能要转编码?
		reverse(strRes.begin(), strRes.end());

		if (string::npos != strRes.find('|'))
		{
			strRes.erase(strRes.begin(), strRes.begin() + strRes.find('|') + 1);
		}
		else
		{
			return "";
		}
	}
	else if ("CNText" == strModel)
	{
		while (1)
		{
			if ('X' == fgetc(pFile))
			{
				break;
			}
			else
			{
				char chTemp = fgetc(pFile);
				strIndex += chTemp;
			}

			iFileCount--;
			fseek(pFile, iFileCount, SEEK_END);
		}

		strRes = UTF8ToGB(strIndex.c_str());
		reverse(strRes.begin(), strRes.end());
		strRes = "VX," + strRes;

		if (string::npos != strRes.find('"'))
		{
			strRes.erase(strRes.begin() + strRes.find('"'), strRes.end());
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}

	while ((string::npos != strRes.find('"')) || (string::npos != strRes.find('\n')) || (string::npos != strRes.find(" ")) || (string::npos != strRes.find('?')) || (string::npos != strRes.find('\t')))
	{
		if (string::npos != strRes.find('"'))
		{
			strRes.replace(strRes.find('"'), 1, "");
		}

		if (string::npos != strRes.find('\n'))
		{
			strRes.replace(strRes.find('\n'), 1, "");
		}

		if (string::npos != strRes.find(" "))
		{
			strRes.replace(strRes.find(" "), 1, "");
		}

		if (string::npos != strRes.find('?'))
		{
			strRes.replace(strRes.find('?'), 1, "");
		}

		if (string::npos != strRes.find('\t'))
		{
			strRes.replace(strRes.find('\t'), 1, "");
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

	return strRes;
}


string VxVehicleIndexAddOne(string strModel, string strVehicle, string& str0xVehicle) // 处理CNText 和 Xtool056，041的ID
{
	string strTime = "VX,VEHICLE,";
	string strTrand = strVehicle;
	string strTemp;

	int iVehicle = 0;
	char chTemp[100] = {};
	char* chXp;

	while ((string::npos != strTrand.find(strTime)) || (string::npos != strTrand.find(',')) || (string::npos != strTrand.find("0x")))
	{
		if (string::npos != strTrand.find(strTime))
		{
			strTrand.replace(strTrand.find(strTime), strTime.size(), "");
		}

		if (string::npos != strTrand.find(','))
		{
			strTrand.replace(strTrand.find(','), 1, "");
		}

		if (string::npos != strTrand.find("0x"))
		{
			strTrand.replace(strTrand.find("0x"), 2, "");
		}
	}

	iVehicle = strtol(strTrand.c_str(), &chXp, 16);

	strTrand.clear();

	if ("Same" == strModel)
	{
		sprintf(chTemp, "%06X", iVehicle);
		strTrand += chTemp;
	}
	else if ("Add" == strModel)
	{
		sprintf(chTemp, "%06X", iVehicle + 1);
		strTrand += chTemp;

		memset(chTemp, 0, 100);
		sprintf(chTemp, "%08X", iVehicle + 1);
		strTemp += chTemp;

		while (!strTemp.empty()) // 加0x
		{
			str0xVehicle += "0x" + strTemp.substr(0, 2);
			strTemp.erase(0, 2);

			if (strTemp.empty())
			{
				str0xVehicle = "VX,VEHICLE," + str0xVehicle;
				break;
			}
			else
			{
				str0xVehicle += ",";
			}
		}
	}
	else
	{
		strTrand = "";
	}

	return strTrand;
}



/*
	给定一个非空的字符串，判断它是否可以由它的一个子串重复多次构成。给定的字符串只含有小写英文字母，并且长度不超过10000。
	示例 1:
	输入: "abab"
	输出: True
	解释: 可由子字符串 "ab" 重复两次构成。
	示例 2:
	输入: "aba"
	输出: False
	示例 3:
	输入: "abcabcabcabc"
	输出: True
	*/
	//KMP里标准构建next数组的过程
void getnext(int* next, int len, const string& str)
{
	next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
	int k = -1;//k初始化为-1
	for (int q = 1; q <= len - 1; q++)
	{
		while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
		{
			k = next[k];//往前回溯
		}
		if (str[k + 1] == str[q])//如果相同，k++
		{
			k = k + 1;
		}
		next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
	}
}
bool repeatedSubstringPattern(string s)
{
	if (s.size() == 0)
	{
		return false;
	}
	//int inextTemp = s.size();
	//int nex[s.size()];

	int* next = new int[s.size()];
	int len = s.size();

	getnext(next, len, s);

	//最长相等前后缀的长度为：next[len-1]+1
	//数组长度: len
	//如果len%(len-(next[len-1]+1))==0,说明数组的长度整除，说明该字符串有重复的子字符串
	if (next[len - 1] != -1 && len % (len - (next[len - 1] + 1)) == 0)
	{
		return true;
	}

	return false;
}


/*
	字符串 S 由小写字母组成。我们要把这个字符串划分为尽可能多的片段，同一字母最多出现在一个片段中。返回一个表示每个字符串片段的长度的列表。
	示例：
	输入：S = "ababcbacadefegdehijhklij"
	输出：[9,7,8]
	解释：
	划分结果为 "ababcbaca", "defegde", "hijhklij"。
	每个字母最多出现在一个片段中。
	像 "ababcbacadefegde", "hijhklij" 的划分是错误的，因为划分的片段数较少
*/
int partitionLabels_FindMaxPos(char chval, string strTrand)
{
	int iPosRes;
	int iCount = 0;
	int iTime = 0;

	if (string::npos == strTrand.find(chval, iCount))
	{
		return -1;
	}

	while (string::npos != strTrand.find(chval, iCount + 1))
	{
		if (0 != iTime)
		{
			int iPosTemp = strTrand.find(chval, iCount + 1);
			if (-1 != iPosTemp)
			{
				iCount = iPosTemp;
			}
		}
		iTime++;
	}

	iPosRes = iCount;
	return iPosRes;
}

int iPosMaxRes;
vector<int> partitionLabels(string strTrand)
{
	vector<int>vctRes;
	int iTemp = 0;

	for (int i = 0; i < strTrand.size(); i++)
	{
		int iPosMaxTemp = 0;
		string strTemp;
		string strTime;

		iPosMaxRes = partitionLabels_FindMaxPos(strTrand.at(i), strTrand);

		strTemp = strTrand.substr(strTrand.find(strTrand.at(i)), (strTrand.size() - (strTrand.find(strTrand.at(i)))) - (strTrand.size() - iPosMaxRes - 1));
		for (int j = 0; j < strTemp.size(); j++)
		{
			// ababcbacadefegdehijhklij
			iPosMaxTemp = partitionLabels_FindMaxPos(strTemp.at(j), strTrand);
			if (iPosMaxTemp > iPosMaxRes)
			{
				strTime = strTrand.substr(iTemp + 1, iPosMaxTemp - iTemp);
				strTemp = strTime;
				iPosMaxRes = iPosMaxTemp;
			}
		}

		iTemp = iPosMaxRes;

		i = iPosMaxTemp;

		vctRes.push_back((int)strTemp.size());
	}

	return vctRes;
}


/*
	有重复字符串的排列组合。编写一种方法，计算某字符串的所有排列组合
	示例1:
	 输入：S = "qqe"
	 输出：["eqq","qeq","qqe"]
	示例2:
	 输入：S = "ab"
	 输出：["ab", "ba"]
	*/
	/*
	* 深度优先算法
	result = []
	def backtrack(路径, 选择列表):
		if 满足结束条件:
			result.add(路径)
			return

		for 选择 in 选择列表:
			做选择
			backtrack(路径, 选择列表)
			撤销选择

	1、路径：也就是已经做出的选择
	2、选择列表：也就是你当前可以做的选择
	3、结束条件：也就是到达决策树底层，无法再做选择的条件
	*/

vector<string>vctAns;
void backracking(string& strTrand, int iStart)
{
	if (iStart == strTrand.size())
	{
		vctAns.push_back(strTrand);
	}
	for (int i = iStart; i < strTrand.size(); ++i)
	{
		swap(strTrand[i], strTrand[iStart]);
		backracking(strTrand, iStart + 1);
		swap(strTrand[i], strTrand[iStart]);
	}
}
vector<string> permutation2(string strTrand)
{
	backracking(strTrand, 0);
	sort(vctAns.begin(), vctAns.end());
	auto it = unique(vctAns.begin(), vctAns.end());
	vctAns.erase(it, vctAns.end());
	return ans;
}

/*
	给定一个非负整数数组，a1, a2, ..., an, 和一个目标数，S。现在你有两个符号 + 和 -。对于数组中的任意一个整数，你都可以从 + 或 -中选择一个符号添加在前面
	返回可以使最终数组和为目标数 S 的所有添加符号的方法数
	示例：
	输入：nums: [1, 1, 1, 1, 1], S: 3
	输出：5
	解释：
	-1+1+1+1+1 = 3
	+1-1+1+1+1 = 3
	+1+1-1+1+1 = 3
	+1+1+1-1+1 = 3
	+1+1+1+1-1 = 3
	一共有5种方法让最终目标和为3

	def backtrack(路径, 选择列表):
		if 满足结束条件:
			result.add(路径)
			return

		for 选择 in 选择列表:
			做选择
			backtrack(路径, 选择列表)
			撤销选择

	1、路径：也就是已经做出的选择
	2、选择列表：也就是你当前可以做的选择
	3、结束条件：也就是到达决策树底层，无法再做选择的条件
*/
vector<int>vctfindTargetSumWaysOriginal;
vector<vector<int>>vctvctfindTargetSumWaysOriginalTemp;
set<vector<int>>vctfindTargetSumWaysAns;
void findTargetSumWays_backracking(vector<int>& vctNum, int iStart, int iSumRes)
{
	if (iSumRes == accumulate(vctNum.begin(), vctNum.end(), 0))
	{
		vctfindTargetSumWaysAns.insert(vctNum);
		vctvctfindTargetSumWaysOriginalTemp.push_back(vctNum);
		//vctNum = vctfindTargetSumWaysOriginal;
	}

	for (int i = iStart; i < vctNum.size(); i++)
	{
		vctNum.at(i) = -(vctNum.at(i));
		swap(vctNum.at(i), vctNum.at(iStart));
		findTargetSumWays_backracking(vctNum, iStart + 1, iSumRes);
		vctNum.at(i) = -(vctNum.at(i));
		swap(vctNum.at(i), vctNum.at(iStart));
	}
}

//int findTargetSumWays(vector<int>& vctNum, int iSumRes) 
//{
//	vector<int>vctTemp;
//	int iRes = 0;
//	vctfindTargetSumWaysOriginal = vctNum;
//	findTargetSumWays_backracking(vctNum,0,iSumRes);
//
//	iRes = vctfindTargetSumWaysAns.size();
//
//	vctfindTargetSumWaysAns.clear();
//	vctfindTargetSumWaysOriginal.clear();
//	return iRes;
//}
int dfsfindTargetSumWays(vector<int>& nums, int target, int left)
{
	if (target == 0 && left == nums.size()) return 1;
	if (left >= nums.size()) return 0;
	int ans = 0;
	ans += dfsfindTargetSumWays(nums, target - nums[left], left + 1);
	ans += dfsfindTargetSumWays(nums, target + nums[left], left + 1);
	return ans;
}

int findTargetSumWays(vector<int>& nums, int S)
{
	return dfsfindTargetSumWays(nums, S, 0);
}

/*
	给你一个数组 arr ，请你将每个元素用它右边最大的元素替换，如果是最后一个元素，用 -1 替换。
	完成所有替换操作后，请你返回这个数组。
	示例 1：
	输入：arr = [17,18,5,4,6,1]
	输出：[18,6,6,6,1,-1]
	解释：
	- 下标 0 的元素 --> 右侧最大元素是下标 1 的元素 (18)
	- 下标 1 的元素 --> 右侧最大元素是下标 4 的元素 (6)
	- 下标 2 的元素 --> 右侧最大元素是下标 4 的元素 (6)
	- 下标 3 的元素 --> 右侧最大元素是下标 4 的元素 (6)
	- 下标 4 的元素 --> 右侧最大元素是下标 5 的元素 (1)
	- 下标 5 的元素 --> 右侧没有其他元素，替换为 -1
	示例 2：
	输入：arr = [400]
	输出：[-1]
	解释：下标 0 的元素右侧没有其他元素。
*/
vector<int> replaceElements(vector<int> vctNum)
{
	vector<int>vctIntRes;
	vector<int>vctTemp;
	int iCountNum = 0;

	if (vctNum.size() <= 1)
	{
		vctIntRes.push_back(-1);
		return vctIntRes;
	}

	vctTemp = vctNum;
	while (!vctTemp.empty())
	{
		vector<int>vctTime;
		int iTemp = vctTemp.front();
		vctTemp.erase(vctTemp.begin(), vctTemp.begin() + 1);
		vctTime = vctTemp;

		if (!vctTemp.empty())
		{
			iTemp = *(max_element(vctTemp.begin(), vctTemp.end()));
			vctIntRes.push_back(iTemp);
			vctTemp = vctTime;
		}
	}

	if (vctTemp.empty())
	{
		vctIntRes.push_back(-1);
	}

	return vctIntRes;
}

/*
	给定一个字符串，逐个翻转字符串中的每个单词。
	说明：
	无空格字符构成一个 单词 。
	输入字符串可以在前面或者后面包含多余的空格，但是反转后的字符不能包括。
	如果两个单词间有多余的空格，将反转后单词间的空格减少到只含一个。
	示例 1：
	输入："the sky is blue"
	输出："blue is sky the"
	示例 2：
	输入："  hello world!  "
	输出："world! hello"
	解释：输入字符串可以在前面或者后面包含多余的空格，但是反转后的字符不能包括。
	示例 3：
	输入："a good   example"
	输出："example good a"
	解释：如果两个单词间有多余的空格，将反转后单词间的空格减少到只含一个。
	示例 4：
	输入：s = "  Bob    Loves  Alice   "
	输出："Alice Loves Bob"
	示例 5：
	输入：s = "Alice does not even like bob"
	输出："bob like even not does Alice"
*/
vector<string>reverseWords_FindWords(string strTrand)
{
	vector<string>vctStrRes;
	char chSign = ' ';

	while (string::npos != strTrand.find(chSign))
	{
		int iCount = 0;
		int iPosTemp = 0;
		string strTemp;
		iPosTemp = strTrand.find(chSign);
		strTemp = strTrand.substr(0, iPosTemp);

		if ((strTemp.empty()) || (string::npos != strTemp.find(chSign)))
		{
			strTrand.replace(iPosTemp, 1, "");
			continue;
		}

		for (string::iterator it = strTemp.begin(); it != strTemp.end(); it++)
		{
			if ((!isalpha(*it)) && (!isdigit(*it)))
			{
				iCount++;
				strTrand.erase(strTrand.begin(), strTrand.begin() + iPosTemp);
				break;
			}
		}

		if (0 == iCount)
		{
			strTrand.erase(strTrand.begin(), strTrand.begin() + iPosTemp);
			vctStrRes.push_back(strTemp);
		}
	}

	if (!strTrand.empty())
	{
		vctStrRes.push_back(strTrand);
	}

	return vctStrRes;
}

string reverseWords(string strTrand)
{
	string strRes;
	vector<string>vctStrRes;
	int iCount = 0;

	vctStrRes = reverseWords_FindWords(strTrand);

	for (vector<string>::reverse_iterator rit = vctStrRes.rbegin(); rit != vctStrRes.rend(); rit++)
	{
		iCount++;
		strRes += *rit;
		if (iCount != vctStrRes.size())
		{
			strRes += " ";
		}
	}

	return strRes;
}


class mycin
{
public:
	bool bTestFunction(string str, string& str1, int iSign = 0);
	bool btst11(string str, string& str1);
	void voidtest();
};

void mycin::voidtest()
{

}

void voidtest()
{

}

bool mycin::bTestFunction(string str, string& str1, int iSign /* = 0 */)
{
	str1 = "55";
	return true;
}

void FunctionTest(bool(mycin::* bf)(string, string&, int), string str2, string& str3, int iSign) // 调用类的成员函数指针
{
	mycin* cintemp = new mycin;
	(cintemp->*bf)(str2, str3, iSign);
}

void FuncitonTestvoid(void (mycin::* function)())
{
	mycin* cintemp = new mycin;
	(cintemp->*function)();
}

void (*GetCINTRODUCTFunction(string strName))()
{
	if ("1" == strName)
	{
		return &voidtest;
	}

	return nullptr;
}



/*
给定两个整数 n 和 k，返回 1 ... n 中所有可能的 k 个数的组合。:
输入: n = 4, k = 2
输出:
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
*/
vector<int>vctIntPath;
vector<vector<int>>dvctIntRes;
void BackTrack_Combine(int iInput, int iNumCount, int iStart)
{
	/*
		def backtrack(路径, 选择列表):
		if 满足结束条件:
			result.add(路径)
			return

		for 选择 in 选择列表:
			做选择
			backtrack(路径, 选择列表)
			撤销选择

		1、路径：也就是已经做出的选择
		2、选择列表：也就是你当前可以做的选择
		3、结束条件：也就是到达决策树底层，无法再做选择的条件
	*/

	//回溯终止条件
	if (vctIntPath.size() == iNumCount)
	{
		dvctIntRes.push_back(vctIntPath);
		return;
	}

	for (int i = iStart; i <= iInput; i++)
	{
		vctIntPath.push_back(i);//处理节点
		BackTrack_Combine(iInput, iNumCount, i + 1);
		vctIntPath.pop_back();//回溯
	}
}
vector<vector<int>>combine(int iInput, int iNumCount)
{
	BackTrack_Combine(iInput, iNumCount, 1);
	return dvctIntRes;
}

void Execute_combine()
{
	vector<vector<int>>dvctTemp = combine(4, 2);
}

/*
	你有一套活字字模 tiles，其中每个字模上都刻有一个字母 tiles[i]。返回你可以印出的非空字母序列的数目
	注意：本题中，每个活字字模只能使用一次
	示例 1：

	输入："AAB"
	输出：8
	解释：可能的序列为 "A", "B", "AA", "AB", "BA", "AAB", "ABA", "BAA"
	示例 2：
	输入："AAABBC"
	输出：188
*/
vector<string>vctStringPath;
vector<vector<string>>dvctStringPath;

void BackTracking_numTilePossibilities(string strTiles, int iStart)
{
	if (iStart > strTiles.size())
	{
		dvctStringPath.push_back(vctStringPath);
		return;
	}

	for (int i = iStart; i <= strTiles.size(); i++)
	{
		if (strTiles[i] != '\0')
		{
			string strTemp;
			strTemp += strTiles[i];

			vctStringPath.push_back(strTemp);
		}

		BackTracking_numTilePossibilities(strTiles, iStart + 1);

		if (!vctStringPath.empty())
		{
			vctStringPath.pop_back();
		}
	}
}

void GetFolderNames(string strFilePath, vector<string>& vctFileNames)
{
	long long hFile = 0; // Win7中用long，但在Win10中用long long,否则会崩溃

	//文件信息    
	struct _finddata_t fileinfo;//用来存储文件信息的结构体    

	string strFileTemp;

	if ((hFile = _findfirst(strFileTemp.assign(strFilePath).append("\\*").c_str(), &fileinfo)) != -1)  //第一次查找  
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))  //如果查找到的是文件夹  
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找  
				{
					vctFileNames.push_back(strFileTemp.assign(strFilePath).append("\\").append(fileinfo.name));
					GetFolderNames(strFileTemp.assign(strFilePath).append("\\").append(fileinfo.name), vctFileNames);
				}
			}
			else //如果查找到的不是是文件夹   
			{
				vctFileNames.push_back(strFileTemp.assign(fileinfo.name));  //将文件路径保存，也可以只保存文件名:    p.assign(path).append("\\").append(fileinfo.name)  
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile); //结束查找  
	}
}


string SinglePrecisionFloatToReverseTSting(float f)
{
	string strReturn = "";

	unsigned char* b = (unsigned char*)&f;
	char ch1[10];

	for (int i = 3; i >= 0; i--)
	{
		sprintf(ch1, "%02X", b[i]);
		strReturn += ch1;
	}

	return strReturn;
}

string SinglePrecisionFloatToForwardTSting(float f)
{
	string strReturn = "";

	unsigned char* b = (unsigned char*)&f;
	char ch1[10];

	for (int i = 0; i < 4; i++)
	{
		sprintf(ch1, "%02X", b[i]);
		strReturn += ch1;
	}

	return strReturn;
}

string XTOOLY_02DA13_AddBinSpace(string strIndentation)
{
	string strRes;

	if ((strIndentation.empty()) || (0 != strIndentation.size() % 2))
	{
		return strRes;
	}

	while (!strIndentation.empty())
	{
		string strTemp = strIndentation.substr(0, 2);
		strTemp.insert(strTemp.size(), " ");
		strRes += strTemp;
		strIndentation.erase(strIndentation.begin(), strIndentation.begin() + 2);
	}

	strRes.erase(strRes.begin() + strRes.size() - 1, strRes.end());

	return strRes;
}

string XTOOLY_02DA13_ReverseBin(string strBin)
{
	string strRes;

	vector<string>vctSubstr;

	if ((strBin.empty()) || (strBin.size() < 2))
	{
		return strRes;
	}

	while (0 != strBin.size() % 2)
	{
		strBin.insert(0, "0");
	}

	while (!strBin.empty())
	{
		string strTemp = strBin.substr(0, 2);
		vctSubstr.push_back(strTemp);
		strBin.erase(strBin.begin(), strBin.begin() + 2);
	}

	for (vector<string>::reverse_iterator rit = vctSubstr.rbegin(); rit != vctSubstr.rend(); rit++)
	{
		strRes += (*rit);
	}

	return strRes;
}

struct SWheelEdgeval
{
	int iMinval;
	int iMaxval;
	int iFrontLeft;
	int iFrontRight;
	int iBackLeft;
	int iBackRight;
};

vector<string>XTOOLY_02D933_GetInputCharacter(int iFrontLeft, int iFrontRight, int iBackLeft, int iBackRight)
{
	vector<string>vctCharacter;

	vector<int>vctIntTemp{ iFrontLeft,iFrontRight,iBackLeft,iBackRight };

	char chArray[100] = { 0 };

	for (vector<int>::iterator itInt = vctIntTemp.begin(); itInt != vctIntTemp.end(); itInt++)
	{
		string strTemp;
		string strTrand;

		int iTemp = 0;
		int iCountTemp = 0;

		iTemp = (*itInt) / 4;

		memset(chArray, 0, sizeof(char));
		sprintf(chArray, "%02X", iTemp);
		strTemp += chArray;

		if (2 == strTemp.size())
		{
			for (string::iterator itStr = strTemp.begin(); itStr != strTemp.end(); itStr++)
			{
				if (0 == iCountTemp)
				{
					strTrand += (*itStr);
					strTrand = "0" + strTrand;
				}
				else
				{
					/*
						<Offset>0</Offset>
						<Factor>1</Factor>
						<Denominator>4</Denominator>
					*/

					// 小数用8填充??
					if (0 != ((*itInt) % 4))
					{
						strTrand += (*itStr);
						strTrand += "8";
					}
					else
					{
						strTrand += (*itStr);
						strTrand += "0";
					}
				}

				iCountTemp++;
			}

			vctCharacter.push_back(strTrand);
		}
	}

	return vctCharacter;
}


vector<string> ADASGetInputCharacter(SWheelEdgeval sEdgeval, string strAlgorithm = "")
{
	vector<string>vctCharacter;
	vector<string>vctCalcSign{ "+","-","*","/" };

	vector<int>vctIntTemp{ sEdgeval.iFrontLeft,sEdgeval.iFrontRight,sEdgeval.iBackLeft,sEdgeval.iBackRight };

	char chArray[100] = { 0 };

	for (vector<int>::iterator itInt = vctIntTemp.begin(); itInt != vctIntTemp.end(); itInt++)
	{
		string strTemp;
		string strAlgorithmTemp;
		string strTrand;

		vector<string>vctAlgorithm;

		int iTemp = 0;
		int iCountTemp = 0;

		bool bIsDecimal = false;

		strAlgorithmTemp = strAlgorithm;

		if (strAlgorithmTemp.empty())
		{
			iTemp = *itInt;
		}
		else
		{
			if (string::npos != strAlgorithmTemp.find("@")) // 多个
			{
				while (string::npos != strAlgorithmTemp.find("@"))
				{
					vctAlgorithm.push_back(strAlgorithmTemp.substr(0, strAlgorithmTemp.find("@")));
					strAlgorithmTemp.erase(strAlgorithmTemp.begin(), strAlgorithmTemp.begin() + strAlgorithmTemp.find("@") + 1);

					if (!strAlgorithmTemp.empty())
					{
						vctAlgorithm.push_back(strAlgorithmTemp);
					}
				}
			}
			else
			{
				vctAlgorithm.push_back(strAlgorithmTemp);
			}

			for (vector<string>::iterator itAlgor = vctAlgorithm.begin(); itAlgor != vctAlgorithm.end(); itAlgor++)
			{
				for (vector<string>::iterator itSign = vctCalcSign.begin(); itSign != vctCalcSign.end(); itSign++)
				{
					if (string::npos != (*itAlgor).find(*itSign))
					{
						string strSnap;
						double dSnap = 0;
						double dTemp = 0;

						strSnap = (*itAlgor).substr((*itAlgor).find(*itSign) + 1, (*itAlgor).size());
						dSnap = atof(strSnap.c_str());

						if ("+" == (*itSign))
						{
							dTemp = (*itInt) + dSnap;
						}
						else if ("-" == (*itSign))
						{
							dTemp = (*itInt) - dSnap;
						}
						else if ("*" == (*itSign))
						{
							dTemp = (*itInt) * dSnap;
						}
						else if ("/" == (*itSign))
						{
							dTemp = (*itInt) / dSnap;
						}

						if (ceil(dTemp) != floor(dTemp)) // 整数
						{
							bIsDecimal = true;
						}
						else
						{
							bIsDecimal = false;
						}

						iTemp = (int)dTemp;

						break;
					}
				}
			}
		}

		memset(chArray, 0, sizeof(char));
		sprintf(chArray, "%02X", iTemp);
		strTemp += chArray;

		if (2 == strTemp.size())
		{
			for (string::iterator itStr = strTemp.begin(); itStr != strTemp.end(); itStr++)
			{
				if (0 == iCountTemp)
				{
					strTrand += (*itStr);
					strTrand = "0" + strTrand;
				}
				else
				{
					// 小数用8填充??
					if (bIsDecimal)
					{
						strTrand += (*itStr);
						strTrand += "8";
					}
					else
					{
						strTrand += (*itStr);
						strTrand += "0";
					}
				}

				iCountTemp++;
			}

			vctCharacter.push_back(strTrand);
		}
	}

	return vctCharacter;
}



void ThreadForTestNull()
{
	for (int i = 0; i < 50; i++)
	{
		cout << "dingChild" << i << endl;
	}
}

void ThreadForTestParameter(string strParameter)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "dingChild" << strParameter << endl;
	}

	strParameter = "ParameterChange";
}

void ThreadForTestParameterQuote(string& strParameter)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "dingChild" << strParameter << endl;
	}

	strParameter = "ParameterChange";
}

/*
	获取线程的基本信息
*/
int GetThreadInfor(thread thTemp, string strModel)
{
	int iTemp = 0;

	/*
		获取当前CPU可开启的线程数
	*/
	if ("CPUThreadNum" == strModel)
	{
		iTemp = thTemp.hardware_concurrency();
	}

	/*
		获取当前线程的ID
	*/
	if ("CurrentThreadId" == strModel)
	{
		auto auTemp = thTemp.get_id();
	}

	return iTemp;
}

void ThreadMutuallyExclusive()
{
	for (int i = 0; i < 10; i++)
	{
		string strText = to_string(i) + "\n";
		AppendText("F:\\引导开发\\V12.21免测发行\\TEST.txt", "ThreadOne: " + strText);
	}
}

void ThreadMutuallyExclusive2()
{
	for (int i = 0; i < 10; i++)
	{
		string strText = to_string(i) + "\n";
		AppendText("F:\\引导开发\\V12.21免测发行\\TEST.txt", "ThreadTwo: " + strText);
	}
}

/*
	线程直接调用无参函数
*/
void CreadThreadNullParameter(void (*Function)())
{
	mutex muThread;

	thread thTemp(Function);

	//muThread.lock();
	lock_guard<mutex>Threadlock(muThread);
	for (int i = 0; i < 10; i++)
	{
		string strText = to_string(i) + "\n";
		AppendText("F:\\引导开发\\V12.21免测发行\\TEST.txt", "ThreadTwo: " + strText);
	}
	//muThread.unlock();

	/*
		join 在线程创建完成后，等待线程执行结束，并回收它的资源
	*/
	thTemp.join();


	/*
		detach 在创建线程后，马上执行，用于把被创建的线程与做创建动作的线程分离，分离的线程变为后台线程
		detach 一个线程只能使用一次，使用完，线程就是空的了
	*/
	//thOne.detach();

	string str;
}

/*
	线程调用有参函数
*/
void CreadThreadHaveParameter(void (*Function)(string), string strParameter)
{
	thread thTemp(Function, strParameter);
	thTemp.join();

	//cout << "Return Parameter: " << strTemp << endl;
}

/*
	线程调用有参引用传递的函数
*/
void CreadThreadHaveParameterQuote(void (*Function)(string&), string strParameter)
{
	/*
		线程传入引用的参数
		需要使用 std::ref() 传递
	*/
	thread thTemp(Function, std::ref(strParameter));
	thTemp.join();

	//cout << "Return Parameter: " << strTemp << endl;
}




class MyLinkedList {
public:
	//定义链表节点结构体
	struct LinkedNode
	{
		int val;
		LinkedNode* next;
		LinkedNode(int val) :val(val), next(nullptr) {}
	};

	//初始化链表
	MyLinkedList()
	{
		m_dummyHead = new LinkedNode(0);
		m_size = 0;
	}

	//获取到第index个节点数值，如果index是非法数值直接返回-1，注意index是从0开始的，第0个节点就是头结点
	int get(int index)
	{
		if (index > m_size - 1 || index < 0)
		{
			return -1;
		}
		LinkedNode* cur = m_dummyHead->next;
		while (index--)//注意--index会陷入死循环
		{
			cur = cur->next;
		}
		return cur->val;
	}

	//在链表最前面插入一个结点，插入完成后，新插入的结点为链表的新的头结点
	void addAtHead(int val)
	{
		LinkedNode* newNode = new LinkedNode(val);
		LinkedNode* cur = m_dummyHead;
		newNode->next = cur->next;
		cur->next = newNode;
		m_size++;//注意++
	}

	//在链表最后面添加一个结点
	void addAtTail(int val)
	{
		LinkedNode* newNode = new LinkedNode(val);
		LinkedNode* cur = m_dummyHead;
		while (cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next = newNode;
		m_size++;//注意++
	}

	//在第index个节点之前插入一个新节点，例如index为0，那么新插入的节点为链表的新头结点
	//如果index等于链表的长度，则说明是新插入和结点为链表的尾结点
	//如果index大于链表的长度，则返回空
	void addAtIndex(int index, int val)
	{
		if (index > m_size)
		{
			return;
		}
		LinkedNode* newNode = new LinkedNode(val);
		LinkedNode* cur = m_dummyHead;
		while (index--)
		{
			cur = cur->next;
		}
		newNode->next = cur->next;
		cur->next = newNode;
		m_size++;//注意++
	}

	//删除第index个节点，如果index大于等于链表的长度，直接return，注意index是从0开始的
	void deleteAtIndex(int index)
	{
		if (index >= m_size || index < 0)
		{
			return;
		}
		LinkedNode* cur = m_dummyHead;
		while (index--)
		{
			cur = cur->next;
		}
		LinkedNode* tmp = cur->next;
		cur->next = cur->next->next;
		delete tmp;
		m_size--;//注意--
	}
private:
	int m_size;
	LinkedNode* m_dummyHead;
};

struct ListNode
{
	int val;
	ListNode* next;
	ListNode* m_listNode;

	//ListNode() // 构造函数将next置为空来判断值，避免非法访问
	//{
	//	val = 0;
	//	next = nullptr;
	//	this->m_listNode = nullptr;
	//}

	ListNode() : val(0), next(nullptr), m_listNode(nullptr) {}; // 构造函数将next置为空来判断值，避免非法访问

	void push_back(int iValue, bool bIsLoop = false);
	void push_back(int iPos, int iValue);
	void push_front(int iValue);
	void deletelist(int iPosList);
	void sort();
	void clear();
	int size();
	bool isRing();
	vector<int>getAllValue();
};

void ListNode::push_back(int iValue, bool bIsLoop /* = false */)
{
	if (this->m_listNode == nullptr)
	{
		this->m_listNode = new ListNode;
		this->m_listNode->val = iValue;
	}
	else
	{
		ListNode* listTemp = new ListNode;
		listTemp = this->m_listNode;

		while (listTemp->next != nullptr)
		{
			listTemp = listTemp->next;
		}

		listTemp->next = new ListNode;
		listTemp->next->val = iValue;

		// 以环形链表结尾
		if (bIsLoop)
		{
			listTemp->next->next = new ListNode;
			listTemp->next->next = this->m_listNode;
		}
	}
}

void ListNode::push_front(int iValue)
{
	if (this->m_listNode == nullptr)
	{
		this->m_listNode = new ListNode;
		this->m_listNode->val = iValue;
	}
	else
	{
		ListNode* listTemp = new ListNode;

		listTemp->val = iValue;
		listTemp->next = this->m_listNode;
		this->m_listNode = listTemp;
	}
}

void ListNode::deletelist(int iPosList)
{
	if (this->m_listNode == nullptr)
	{
		return;
	}
	else if (iPosList < 0)
	{
		return;
	}

	ListNode* listTemp = new ListNode;
	ListNode* listPrev = new ListNode; // 当前删除节点的上一个节点

	int iListSize = 0;
	int iCount = 0;

	if (0 == iPosList) // 删除头节点
	{
		if (this->m_listNode->next != nullptr)
		{
			this->m_listNode = this->m_listNode->next;
			return;
		}
		else
		{
			this->m_listNode = nullptr;
			return;
		}
	}

	listTemp = this->m_listNode;

	while (listTemp != nullptr)
	{
		iListSize++;
		listTemp = listTemp->next;
	}

	if (iPosList > iListSize - 1) // 大于SIZE时不做操作
	{
		return;
	}

	if (iPosList == iListSize - 1) // 删除尾节点
	{
		listPrev = this->m_listNode;

		while (iCount != iListSize - 2)
		{
			listPrev = listPrev->next;
			iCount++;
		}

		listPrev->next = nullptr; // 将尾节点的上一个节点置为空，即是尾节点的删除
	}
	else // 删除中间节点
	{
		listPrev = this->m_listNode;

		while (iCount != iPosList - 1) // 获取到上一个节点开始的链表
		{
			listPrev = listPrev->next;
			iCount++;
		}

		listPrev->next = listPrev->next->next; // 上一个节点指向当前删除节点的下一个节点，令删除的节点释放，即为删除
	}
}

void ListNode::sort()
{
	if (this->m_listNode == nullptr)
	{
		return;
	}

	vector<int>vctSort;

	ListNode* listTemp = new ListNode;

	listTemp = this->m_listNode;

	while (listTemp != nullptr)
	{
		vctSort.push_back(listTemp->val);
		listTemp = listTemp->next;
	}

	std::sort(vctSort.begin(), vctSort.end());
}

void ListNode::clear()
{
	ListNode* listTemp = new ListNode;
	this->m_listNode = listTemp;
	delete listTemp;
}

int ListNode::size()
{
	ListNode* listTemp = new ListNode;

	int iSize = 0;

	if (this->m_listNode == nullptr)
	{
		iSize = 0;
	}
	else
	{
		listTemp = this->m_listNode;

		while (listTemp != nullptr)
		{
			iSize++;
			listTemp = listTemp->next;
		}
	}

	return iSize;
}

bool ListNode::isRing()
{
	if (this->m_listNode == nullptr)
	{
		return false;
	}

	ListNode* listFast = new ListNode;
	ListNode* listSlow = new ListNode;

	listFast = this->m_listNode;
	listSlow = this->m_listNode;

	// 核心思想是，如果是环形链表快指针一定会追上慢指针
	while ((listFast != nullptr) && (listSlow != nullptr) && (listFast->next != nullptr))
	{
		listSlow = listSlow->next;

		if (listFast->next != nullptr)
		{
			listFast = listFast->next->next;
		}

		if (listFast == listSlow)
		{
			return true;
		}
	}

	return false;
}

std::vector<int> ListNode::getAllValue()
{
	vector<int>vctValue;

	ListNode* listTemp = new ListNode;

	if (this->m_listNode = nullptr)
	{
		return vctValue;
	}

	listTemp = this->m_listNode;

	while (listTemp != nullptr)
	{
		vctValue.push_back(listTemp->val);
		listTemp = listTemp->next;
	}

	return vctValue;
}

void Execute_ListNode()
{
	ListNode* listOne = new ListNode;
	ListNode* listTwo = new ListNode;
	ListNode* listRes = new ListNode;

	listOne->push_back(1);
	listOne->push_back(2);
	listOne->push_back(0);
	listOne = listOne->m_listNode;

	listTwo->push_back(1);
	listTwo->push_back(2);
	listTwo->push_back(0);
	listTwo = listTwo->m_listNode;

	listOne->push_back(3);
	listOne->push_back(3);
	listOne->push_back(2);
	listOne->push_back(3, true);

	bool bIsTemp = listOne->isRing();

	listOne->deletelist(5);
	listOne = listOne->m_listNode;
}


class DingListNode
{
public:
	DingListNode()
	{
		m_ListNode = nullptr;
		m_Size = 0;
	}

	void Push_back(int iInsert)
	{
		if (m_ListNode == nullptr) // 头节点直接赋值
		{
			m_Size++;
			m_ListNode = new ListNode;
			m_ListNode->val = iInsert;
		}
		else
		{
			m_Size++;

			ListNode* listTemp = new ListNode;
			listTemp = m_ListNode;

			while (listTemp->next != nullptr) // 找到最后一个结点
			{
				listTemp = listTemp->next;
			}

			listTemp->next = new ListNode; // 建立插入后的新的最后一个节点进行赋值
			listTemp->next->val = iInsert;

			ListFree(listTemp);
		}
	}

	void DeletePos(int iPos)
	{
		if (m_ListNode == nullptr) // 空链表返回
		{
			return;
		}

		if (m_ListNode->next == nullptr) // 头节点置为空
		{
			m_ListNode->next = new ListNode;
			m_ListNode = m_ListNode->next;
		}
		else
		{
			// 建立两个结点
			// 一个是当前位置的上一个结点
			// 一个是当前位置的下一个结点
			// 如果要删除位置的结点不是头节点或者尾结点，将上一个结点指向下一个结点即为删除

			ListNode* listPrev = new ListNode;
			ListNode* listnext = new ListNode;

			int iPosCount = 0;

			listPrev = m_ListNode;
			listnext = m_ListNode;

			while (iPosCount != iPos - 1) // 上一个结点
			{
				iPosCount++;
				listPrev = listPrev->next;
			}

			iPosCount = 0;

			while (iPosCount != iPos + 1) // 下一个结点
			{
				iPosCount++;
				listnext = listnext->next;
			}

			listPrev->next = new ListNode; // 上一个结点指向下一个结点
			listPrev->next = listnext;

			m_ListNode = listPrev;

			ListFree(listPrev);
			ListFree(listnext);
		}
	}

	void ListFree(ListNode* listNode)
	{
		listNode = nullptr;
		delete listNode;
	}

	int size()
	{
		return m_Size;
	}
public:
	ListNode* m_ListNode; // 记录链表
	int m_Size;
};

/*
	输入两个递增排序的链表，合并这两个链表并使新链表中的节点仍然是递增排序的
	示例1：
	输入：1->2->4, 1->3->4
	输出：1->1->2->3->4->4
*/
ListNode* mergeTwoLists_Splicing(vector<int>vctSort)
{
	ListNode* listTemp = new ListNode;
	ListNode* listSplicing = new ListNode;

	sort(vctSort.begin(), vctSort.end());

	listTemp = listSplicing;
	for (int i = 0; i < vctSort.size(); i++)
	{
		if (0 != i)
		{
			listTemp->next = new ListNode;
			listTemp = listTemp->next;
		}

		listTemp->val = vctSort.at(i);
	}

	return listSplicing;
}
ListNode* mergeTwoLists(ListNode* listOne, ListNode* listTwo)
{
	ListNode* listRes = new ListNode;
	ListNode* listTemp = new ListNode;

	vector<int>vctSort;

	if ((listOne == nullptr) && (listTwo == nullptr))
	{
		listRes = nullptr;
		return listRes;
	}

	if (listOne == nullptr)
	{
		listTemp = listTwo;
		while (listTemp != nullptr)
		{
			vctSort.push_back(listTemp->val);
			listTemp = listTemp->next;
		}

		listRes = mergeTwoLists_Splicing(vctSort);
		return listRes;
	}

	if (listTwo == nullptr)
	{
		listTemp = listOne;
		while (listTemp != nullptr)
		{
			vctSort.push_back(listTemp->val);
			listTemp = listTemp->next;
		}

		listRes = mergeTwoLists_Splicing(vctSort);
		return listRes;
	}

	// 找到最后一个节点
	listTemp = listOne;
	while (listTemp->next != nullptr)
	{
		listTemp = listTemp->next;
	}

	// 拼接两个链表
	listTemp->next = new ListNode;
	listTemp->next = listTwo;

	// 获取所有值
	listTemp = listOne;
	while (listTemp != nullptr)
	{
		vctSort.push_back(listTemp->val);
		listTemp = listTemp->next;
	}

	// 将排序后的值重新赋给一个新的链表
	listRes = mergeTwoLists_Splicing(vctSort);

	return listRes;
}

void Execute_mergeTwoLists()
{
	ListNode* listOne = new ListNode;
	ListNode* listTwo = new ListNode;
	ListNode* listVal = new ListNode;

	listVal = mergeTwoLists(listOne, listTwo);
}

ListNode* CreateList(int iNum)
{
	ListNode listHead;
	ListNode* listStart = &listHead;

	for (int i = 0; i < iNum; i++)
	{
		listStart->next = new ListNode;
		listStart->next->val = i;
		listStart = listStart->next;
	}

	listStart->next = NULL;
	return listHead.next;
}



int myAtoi(string strval)
{
	string strTemp;

	vector<pair<int, int>>vctInt;

	int ival = 0;

	long long lReslut = 0;

	bool bIsNegative = false;

	while (string::npos != strval.find(" "))
	{
		strval.replace(strval.find(" "), 1, "");
	}

	if ((string::npos != strval.find("+")) && (string::npos != strval.find("-")))
	{
		lReslut = 0;
		return (int)lReslut;
	}

	if (string::npos != strval.find("+"))
	{
		strval.erase(strval.begin(), strval.begin() + strval.find("+") + 1);
	}

	if (string::npos != strval.find("-"))
	{
		strval.erase(strval.begin(), strval.begin() + strval.find("-") + 1);
		bIsNegative = true;
	}

	if (string::npos != strval.find("."))
	{
		strval = strval.substr(0, strval.find("."));
	}

	for (int i = 0; i < strval.size(); i++)
	{
		if (0 == i)
		{
			if ((strval[i] < '0') || (strval[i] > '9'))
			{
				lReslut = 0;
				return (int)lReslut;
			}
			else
			{
				strTemp.push_back(strval[i]);
			}
		}
		else
		{
			if ((strval[i] >= '0') && (strval[i] <= '9'))
			{
				strTemp.push_back(strval[i]);
			}
		}
	}

	if (strTemp.empty())
	{
		lReslut = 0;
		return (int)lReslut;
	}

	for (int i = 0; i < strTemp.size(); i++)
	{
		int iTemp = 0;
		iTemp = strTemp[i] - '0'; // 关键要素 -- 退格  char - '0'等于数值本身
		vctInt.push_back(make_pair(iTemp, strTemp.size() - i - 1));
	}

	for (vector<pair<int, int>>::iterator pairit = vctInt.begin(); pairit != vctInt.end(); pairit++)
	{
		int iTemp = 0;
		long long iSign = 1;
		int iCount = pairit->second;

		while (0 != iCount)
		{
			iSign = iSign * 10;
			iCount--;
		}

		lReslut += (pairit->first * iSign);
	}

	if (bIsNegative)
	{
		lReslut = -lReslut;
	}

	if (lReslut < INT_MIN)
	{
		return INT_MIN;
	}
	else if (lReslut > INT_MAX)
	{
		return INT_MAX;
	}

	return (int)lReslut;
}

/*
	int left = 0, right = ...;

	while(...) {
		int mid = (right + left) / 2;
		if (nums[mid] == target) {
			...
		} else if (nums[mid] < target) {
			left = ...
		} else if (nums[mid] > target) {
			right = ...
		}
	}
	return ...;
*/
int SearchTwo(vector<int>& vctInt, int iTarget)
{
	int iLeft = 0;
	int iRight = 0;
	int iMiddle = 0;
	int iResPos = -1;

	if (vctInt.empty())
	{
		return -1;
	}

	for (int i = 0; i < vctInt.size() - 1; i++)
	{
		for (int j = 0; j < vctInt.size() - 1 - i; j++)
		{
			int iOne = vctInt[j];
			int iTwo = vctInt[j + 1];

			if (iOne > iTwo)
			{
				int iTemp = vctInt[j];
				vctInt[j] = vctInt[j + 1];
				vctInt[j + 1] = iTemp;
			}
		}
	}

	iRight = vctInt.size() - 1;

	while (iLeft <= iRight)
	{
		iMiddle = (iLeft + iRight) / 2;

		int iTemp = vctInt.at(iMiddle);

		if (iTemp < iTarget) // 当中位置的数小于查找数
		{
			iLeft = iMiddle + 1; // 左边界变为右边界，在较大数的右边界查找
		}
		else if (iTemp > iTarget) // 当中位置的数大于查找数
		{
			iRight = iMiddle - 1; // 右边界变为左边界，在较小数的左边界查找
		}
		else if (iTemp == iTarget)
		{
			return iMiddle;
		}
	}

	return iResPos;
}

void Execute_SearchTwo()
{
	vector<int>vctInt{ 5,6,4,1,3 };

	int iTemp = SearchTwo(vctInt, 1);
}


int binarySearch2(vector<int> vctArryay, int iSize, int iTarget)
{
	int iLow = 0;
	int iHigh = 0;
	int iMiddle = 0;

	iHigh = iSize;

	while (iLow < iHigh)
	{
		iMiddle = (iLow + iHigh) / 2;

		if (iTarget == vctArryay[iMiddle])
		{
			return iMiddle;
		}
		else if (iTarget < vctArryay[iMiddle])
		{
			iHigh = iMiddle;
		}
		else if (iTarget > vctArryay[iMiddle])
		{
			iLow = iMiddle + 1;
		}
	}

	return -1;
}

void Execute_binarySearch2()
{
	vector<int>vctInt{ 1,3,5,3,6 };

	int iTemp = binarySearch2(vctInt, vctInt.size(), 3);
}

void Gettest(vector<int>vctInt = vector<int>())
{

}

void func2(int*& pb)
{
	pb = new int(5);
}

void func1()
{
	int* pa = nullptr;
	int itemp = 0;
	func2(pa);
	delete pa;
}

int search2(vector<int>& vctNum, int iTarget)
{
	int iPos = 0;
	int iLeft = 0;
	int iRight = 0;
	int iMiddle = 0;

	if (vctNum.empty())
	{
		return -1;
	}

	iRight = vctNum.size() - 1;

	while (iLeft <= iRight)
	{
		iMiddle = (iLeft + iRight) / 2;

		if (iTarget == vctNum.at(iMiddle))
		{
			return iMiddle;
		}
		else if (iTarget < vctNum.at(iMiddle))
		{
			iRight = iMiddle - 1;
		}
		else if (iTarget > vctNum.at(iMiddle))
		{
			iLeft = iMiddle + 1;
		}
	}

	iPos = -1;

	return iPos;
}

void Execute_CMyIni()
{
	CMyIni m_Ini;

	string strTemp;

	vector<string>vctStrTemp;

	if (!m_Ini.open("D:\\Platform\\maker\\GS_VW\\data\\XTOOL_0000\\XTOOL0306.txt", "0x41,0x04,0x00,0x00,0x03,0x02"))
	{
		return;
	}

	strTemp = m_Ini.getText("FUNCTION00", "SysTemId");

	vctStrTemp = m_Ini.getGroupText("FUNCTION02", "Prompt");

	vector<pair<string, string>>pairtemp = m_Ini.getSecionsVct("FUNCTION05");
	vector<pair<string, vector<string>>>vctpairtemp = m_Ini.getGroupSecionsVct("FUNCTION04");
	map<string, string>maptemp = m_Ini.getSecionsMap("FUNCTION05");
	map<string, vector<string>>mapvcttemp = m_Ini.getGroupSecionsMap("FUNCTION04");

	m_Ini.close();

	m_Ini.setWriteIni("F:\\WriteIni\\test.txt");
	m_Ini.setHead("SETTING");
	m_Ini.setChildText("FunctionMenuNum", "16");

	m_Ini.setHead("FUNCTION00");
	vctStrTemp.push_back("0002");
	vctStrTemp.push_back("00");
	m_Ini.setGroupChildText("Closeval", vctStrTemp);

	maptemp.clear();
	maptemp.insert(make_pair("Title", "0035"));
	maptemp.insert(make_pair("SysTemId", "5F"));
	maptemp.insert(make_pair("Prompt", "001F"));
	maptemp.insert(make_pair("ProtocolType", "01"));
	m_Ini.setAllGroupChild(maptemp);
	m_Ini.save();
}


/*
	给定一个数组，找出其中最小的K个数。例如数组元素是4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4。
	0 <= k <= input.length <= 10000
	0 <= input[i] <= 10000
*/
vector<int> GetLeastNumbers_Solution(vector<int> vctInput, int iNum)
{
	vector<int>vctRes;

	if (0 == iNum)
	{
		return vctRes;
	}
	else if (vctInput.empty())
	{
		return vctRes;
	}
	else if (iNum > vctInput.size())
	{
		return vctRes;
	}

	for (int i = 0; i < vctInput.size(); i++)
	{
		for (int j = i + 1; j < vctInput.size(); j++)
		{
			if (vctInput[i] > vctInput[j])
			{
				int iTemp = vctInput[i];
				vctInput[i] = vctInput[j];
				vctInput[j] = iTemp;
			}
		}
	}

	for (int i = 0; i < iNum; i++)
	{
		vctRes.push_back(vctInput[i]);
	}

	return vctRes;
}

void Execute_GetLeastNumbers_Solution()
{
	vector<int>vctInt{ 4,1,8,6,2,5 };

	vctInt = GetLeastNumbers_Solution(vctInt, 3);
}

/*
	给定一个整数数组，判断是否存在重复元素
	如果存在一值在数组中出现至少两次，函数返回 true 。如果数组中每个元素都不相同，则返回 false
	示例 1:
	输入: [1,2,3,1]
	输出: true
*/
bool containsDuplicate2(vector<int>& vctNum)
{
	sort(vctNum.begin(), vctNum.end());

	for (int i = 0; i < vctNum.size(); i++)
	{
		if (i != vctNum.size() - 1)
		{
			if (vctNum[i] == vctNum[i + 1])
			{
				return true;
			}
		}
	}

	return false;
}

/*
	实际生活中，经常需要求一些问题的“可行解”和“最优解”，这就是所谓的“最优化”问题。
	一般来说，每个最优化问题都包含一组“限制条件”和一个“目标函数”，符合限制条件的问题求解方案称为可行解，使目标函数取得最佳值（最大或最小）的可行解称为最优解
	求解最优化问题的算法很多，例如穷举、搜索、动态规划等。贪心法也是求解这类问题的一种常用方法

	1. 贪心法的基本思想
	贪心法是从问题的某个初始解出发，采用逐步构造最优解的方法，向给定的目标前进。
	在每一个局部阶段，都做一个“看上去”最优的决策，并期望通过每一次所做的局部最优选择产生出一个全局最优解。
	做出贪心决策的依据称为“贪心策略”。要注意的是，贪心策略一旦做出，就不可再更改。

	与递推不同的是，贪心严格意义上说只是一种策略或方法，而不是算法。
	推进的每一步不是依据某一个固定的递推式，而是做一个当时“看似最佳”的贪心选择（操作），
	不断将问题归纳为更小的相似子问题。所以，归纳、分析、选择正确合适的贪心策略，是解决贪心问题的关键。

	2. 贪心法的正确性证明
	对于一个问题，如果想用贪心法求解，首先要想到基于某种“序”或者“规则”的贪心策略。

	其次还要能证明其正确性。要严格证明一个贪心算法的正确性是很困难的，目前最有效的一种方法叫“矩阵胚理论”，但是很复杂。
	信息学竞赛中常用的贪心证明方法，一般有反证法、构造法、调整法。
	其实，即使一个贪心算法是不完全正确的，也可以努力寻找一些调整方法，或制定多种贪心策略，通过调整优化、
	比较择优来争取得到最优解，甚至也可以先得到一个“较优”解，然后，在此基础上进行搜索剪枝或分支定界
*/

/*
	给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和
	示例 1：
	输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
	输出：6
	解释：连续子数组 [4,-1,2,1] 的和最大，为 6
	示例 2：
	输入：nums = [1]
	输出：1
	示例 3：
	输入：nums = [0]
	输出：0
*/
int maxSubArray(vector<int>& vctNum)
{
	int iPre = 0;
	int iMaxAns = vctNum.at(0);

	for (int i = 0; i < vctNum.size(); i++)
	{
		iPre = max(iPre + vctNum[i], vctNum[i]);
		iMaxAns = max(iMaxAns, iPre);
	}

	return iMaxAns;
}

void Execute_maxSubArray()
{
	vector<int>vctIntArray{ 3,0,3,2,-4,0,-3,2,2,0,-1,-5 };

	int iTemp = maxSubArray(vctIntArray);
}

/*
	给你一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有和为 0 且不重复的三元组
	注意：答案中不可以包含重复的三元组。
	示例 1：
	输入：nums = [-1,0,1,2,-1,-4]
	输出：[[-1,-1,2],[-1,0,1]]
*/
vector<vector<int>> threeSum2(vector<int>& vctNum)
{
	vector<vector<int>>mvctRes;

	for (int i = 0; i < vctNum.size(); i++)
	{
		for (int j = i + 1; j < vctNum.size(); j++)
		{
			for (int k = j + 1; k < vctNum.size(); k++)
			{
				if (0 == (vctNum[i] + vctNum[j] + vctNum[k]))
				{
					vector<int>vctTemp;
					vctTemp.push_back(vctNum[i]);
					vctTemp.push_back(vctNum[j]);
					vctTemp.push_back(vctNum[k]);

					sort(vctTemp.begin(), vctTemp.end());
					mvctRes.push_back(vctTemp);
				}
			}
		}
	}

	sort(mvctRes.begin(), mvctRes.end());
	mvctRes.erase(unique(mvctRes.begin(), mvctRes.end()), mvctRes.end());

	return mvctRes;
}

/*
	实现一个算法，确定一个字符串 s 的所有字符是否全都不同
	示例 1：
	输入: s = "leetcode"
	输出: false
	示例 2：
	输入: s = "abc"
	输出: true
*/
bool isUnique(string strTrand)
{
	if (strTrand.empty())
	{
		return true;
	}

	sort(strTrand.begin(), strTrand.end());

	for (int i = 0; i < strTrand.size(); i++)
	{
		if ((i + 1) < strTrand.size())
		{
			if (strTrand.at(i) == strTrand.at(i + 1))
			{
				return false;
			}
		}
	}

	return true;
}

/*
	给定两个字符串 s1 和 s2，请编写一个程序，确定其中一个字符串的字符重新排列后，能否变成另一个字符串。
	示例 1：
	输入: s1 = "abc", s2 = "bca"
	输出: true
	示例 2：
	输入: s1 = "abc", s2 = "bad"
	输出: false
*/
void CheckPermutation_BubbleSort(string& strSort)
{
	for (int i = 0; i < strSort.size(); i++)
	{
		for (int j = i + 1; j < strSort.size(); j++)
		{
			if (strSort.at(i) > strSort.at(j))
			{
				char chTemp = strSort.at(i);
				strSort.at(i) = strSort.at(j);
				strSort.at(j) = chTemp;
			}
		}
	}
}

bool CheckPermutation(string strOne, string strTwo)
{
	CheckPermutation_BubbleSort(strOne);
	CheckPermutation_BubbleSort(strTwo);

	if (strOne != strTwo)
	{
		return false;
	}

	return true;
}

/*
	URL化。编写一种方法，将字符串中的空格全部替换为%20。假定该字符串尾部有足够的空间存放新增字符，并且知道字符串的“真实”长度
	示例 1：
	输入："Mr John Smith    ", 13
	输出："Mr%20John%20Smith"
	示例 2：
	输入："               ", 5
	输出："%20%20%20%20%20"
*/
string replaceSpaces(string strTrand, int iLength)
{
	string strRes;
	int iLastStrPos = -1;

	for (int i = strTrand.size() - 1; i > 0; i--)
	{
		if (strTrand.at(i) != ' ')
		{
			iLastStrPos = i;
			break;
		}
	}

	if (-1 == iLastStrPos)
	{
		iLastStrPos = strTrand.size();
	}
	else
	{
		iLastStrPos += 1;
	}

	for (int i = 0; i < iLastStrPos; i++)
	{
		if (strTrand.at(i) == ' ')
		{
			strRes += "%20";
		}
		else
		{
			strRes.push_back(strTrand.at(i));
		}
	}

	return strRes;
}

/*
	字符串压缩。利用字符重复出现的次数，编写一种方法，实现基本的字符串压缩功能。比如，字符串aabcccccaaa会变为a2b1c5a3。
	若“压缩”后的字符串没有变短，则返回原先的字符串。你可以假设字符串中只包含大小写英文字母（a至z）。
	示例1:
	输入："aabcccccaaa"
	输出："a2b1c5a3"
	示例2:
	输入："abbccd"
	输出："abbccd"
	解释："abbccd"压缩后为"a1b2c2d1"，比原字符串长度更长
*/
string compressString(string strTrand)
{
	string strRes;
	string strTemp;

	vector<vector<char>>mvctGroup;

	strTemp = strTrand;

	if ((strTrand.empty()) || (1 == strTrand.size()))
	{
		return strTrand;
	}

	while (!strTrand.empty())
	{
		vector<char>vctTemp;
		char chTemp;
		chTemp = strTrand.at(0);

		if (chTemp == strTrand.at(0))
		{
			while (chTemp == strTrand.at(0))
			{
				vctTemp.push_back(strTrand.at(0));
				strTrand.erase(strTrand.begin(), strTrand.begin() + 1);

				if (strTrand.empty())
				{
					break;
				}
			}
		}

		mvctGroup.push_back(vctTemp);
	}

	for (vector<vector<char>>::iterator mit = mvctGroup.begin(); mit != mvctGroup.end(); mit++)
	{
		for (vector<char>::iterator it = (*mit).begin(); it != (*mit).end(); it++)
		{
			strRes += (*it);
			strRes += to_string((*mit).size());
			break;
		}
	}

	if (strRes.size() >= strTemp.size())
	{
		strRes = strTemp;
	}

	return strRes;
}

void Execute_compressString()
{
	string strTemp = compressString("aabcccccaaa");
}

/*
	编写代码，移除未排序链表中的重复节点。保留最开始出现的节点。
	示例1:
	输入：[1, 2, 3, 3, 2, 1]
	输出：[1, 2, 3]
	示例2:
	输入：[1, 1, 1, 1, 2]
	输出：[1, 2]
*/
ListNode* removeDuplicateNodes(ListNode* listHead)
{
	ListNode* listRes = nullptr;

	vector<int>vctTemp;

	if (listHead == nullptr)
	{
		return listRes;
	}

	vctTemp.push_back(listHead->val);

	while (listHead->next != nullptr)
	{
		listHead = listHead->next;
		vctTemp.push_back(listHead->val);
	}

	sort(vctTemp.begin(), vctTemp.end());
	vctTemp.erase(unique(vctTemp.begin(), vctTemp.end()), vctTemp.end());

	for (vector<int>::iterator it = vctTemp.begin(); it != vctTemp.end(); it++)
	{
		if (listRes == nullptr)
		{
			listRes = new ListNode;
			listRes->val = *it;
		}
		else
		{
			ListNode* listTemp = new ListNode;
			listTemp = listRes;

			while (listTemp->next != nullptr)
			{
				listTemp = listTemp->next;
			}

			listTemp->next = new ListNode;
			listTemp->next->val = *it;
		}
	}

	return listRes;
}

void Execute_removeDuplicateNodes()
{
	ListNode* listOne = new ListNode;
	ListNode* listRes = new ListNode;

	listOne->push_back(1);
	listOne->push_back(2);
	listOne->push_back(3);
	listOne->push_back(3);
	listOne->push_back(2);
	listOne->push_back(1);
	listOne = listOne->m_listNode;

	listRes = removeDuplicateNodes(listOne);
}

/*
	编写一个函数，检查输入的链表是否是回文的
	示例 1：
	输入： 1->2
	输出： false
	示例 2：
	输入： 1->2->2->1
	输出： true
*/
bool isPalindromelist(ListNode* listHead)
{
	vector<int>vctOriginal;

	while (listHead != nullptr)
	{
		vctOriginal.push_back(listHead->val);
		listHead = listHead->next;
	}

	if (vctOriginal.empty())
	{
		return true;
	}

	vector<int>vctLast(vctOriginal);

	reverse(vctLast.begin(), vctLast.end());

	if (vctLast != vctOriginal)
	{
		return false;
	}

	return true;
}

/*
	给定一个链表，如果它是有环链表，实现一个算法返回环路的开头节点。

	如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
	为了表示给定链表中的环，我们使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
	如果 pos 是 -1，则在该链表中没有环。注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
*/
/*
	此题就是寻找环形链表的入环点

	没有环路的链表是这样的：
	3->2->0->4->null
	这里输入head = [3,2,0,-4], pos = 1,表示输入的链表是这样的：
	3->2->0->4
	   ^     |
	   |_____|
*/
ListNode* detectCycle(ListNode* pHead)
{
	if (pHead == nullptr)
	{
		return pHead;
	}

	ListNode* pQuick = nullptr;
	ListNode* pSlow = nullptr;
	ListNode* pNodeMeet = nullptr;

	pQuick = pHead;
	pSlow = pHead;
	pNodeMeet = pHead;

	if (pNodeMeet->next == nullptr)
	{
		return nullptr;
	}

	while ((pQuick != nullptr) && (pSlow != nullptr))
	{
		if (pSlow->next != nullptr)
		{
			pSlow = pSlow->next;
		}

		if (pQuick->next->next != nullptr)
		{
			pQuick = pQuick->next->next;
		}

		if (pQuick == pSlow) // 快指针追上了慢指针就说明这个链表是有环的
		{
			while (pQuick != pNodeMeet) // 相遇点指针和头指针同时后退，知道再次相遇，此次的相遇就是入环点
			{
				pQuick = pQuick->next;
				pNodeMeet = pNodeMeet->next;
			}

			return pNodeMeet;
		}
	}

	return nullptr;
}

void Execute_detectCycle()
{
	ListNode* listOne = new ListNode;
	ListNode* listTwo = new ListNode;
	ListNode* listRes = new ListNode;

	listOne->push_back(1);
	listOne->push_back(2, true);
	listOne = listOne->m_listNode;
	listRes = detectCycle(listOne);
}

/*
	给定一个字符串，编写一个函数判定其是否为某个回文串的排列之一
	回文串是指正反两个方向都一样的单词或短语。
	排列是指字母的重新排列回文串不一定是字典当中的单词。

	示例1：
	输入："tactcoa"
	输出：true（排列有"tacocat"、"atcocta"，等等）

	回文序列的特点就是对称
	因此，如果字符串长度为偶数，那么字符串中出现的字符的个数一定都是偶数个
	如果字符串长度为奇数，那么符串中除了中心字符是奇数个以外，其余字符均为偶数个

	如果字符串里的char字符出现的个数是偶数，则是对称性
*/
bool canPermutePalindrome(string strPalindrome)
{
	if (strPalindrome.empty())
	{
		return false;
	}
	else if (1 == strPalindrome.size())
	{
		return true;
	}

	vector<char>vctPalindrome;

	set<char>setPalindrome;

	int iPalindrome = 0;

	for (int i = 0; i < strPalindrome.size(); i++)
	{
		vctPalindrome.push_back(strPalindrome.at(i));
		setPalindrome.insert(strPalindrome.at(i));
	}

	if (1 == setPalindrome.size())
	{
		return true;
	}

	for (set<char>::iterator it = setPalindrome.begin(); it != setPalindrome.end(); it++)
	{
		if (0 == (count(vctPalindrome.begin(), vctPalindrome.end(), *it)) % 2) // 求字符是否出现次数是否是偶数
		{
			iPalindrome++;
		}
	}

	if (iPalindrome < setPalindrome.size() - 1) // 排除 aa b cc 中间的一个数
	{
		return false;
	}

	return true;
}

void Execute_canPermutePalindrome()
{
	bool bTemp = canPermutePalindrome("acdbcba");
}

/*
	给定两个用链表表示的整数，每个节点包含一个数位。
	这些数位是反向存放的，也就是个位排在链表首部。
	编写函数对这两个整数求和，并用链表形式返回结果。

	示例：
	输入：(7 -> 1 -> 6) + (5 -> 9 -> 2)，即617 + 295
	输出：2 -> 1 -> 9，即912
	进阶：思考一下，假设这些数位是正向存放的，又该如何解决呢?

	示例：
	输入：(6 -> 1 -> 7) + (2 -> 9 -> 5)，即617 + 295
	输出：9 -> 1 -> 2，即912
*/
ListNode* addTwoNumbers(ListNode* plistOne, ListNode* plistTwo)
{
	ListNode* pNodeRes = nullptr;

	bool bIsAddOne = false;

	// 关键就是求相加的进位
	// 即便是反向存放的，只要进位正确，也能相加
	while ((plistOne != nullptr) || (plistTwo != nullptr) || (bIsAddOne))
	{
		int iOneValue = 0;
		int iTwoValue = 0;
		int iAddResValue = 0;

		if (plistOne != nullptr)
		{
			iOneValue = plistOne->val;
			plistOne = plistOne->next;
		}

		if (plistTwo != nullptr)
		{
			iTwoValue = plistTwo->val;
			plistTwo = plistTwo->next;
		}

		iAddResValue = iOneValue + iTwoValue;

		if (bIsAddOne)
		{
			iAddResValue += 1;
		}

		if (iAddResValue >= 10)
		{
			iAddResValue = iAddResValue % 10;
			bIsAddOne = true;
		}
		else
		{
			bIsAddOne = false;
		}

		if (pNodeRes == nullptr)
		{
			pNodeRes = new ListNode;
			pNodeRes->val = iAddResValue;
		}
		else
		{
			ListNode* pListTemp = nullptr;
			pListTemp = pNodeRes;

			while (pListTemp->next != nullptr)
			{
				pListTemp = pListTemp->next;
			}

			pListTemp->next = new ListNode;
			pListTemp->next->val = iAddResValue;
			pListTemp = nullptr;
			delete pListTemp;
		}
	}

	return pNodeRes;
}

void Execute_addTwoNumbers()
{
	ListNode* listOne = new ListNode;
	ListNode* listTwo = new ListNode;
	ListNode* listRes = new ListNode;

	listTwo->push_back(1);
	listTwo->push_back(9);
	listTwo = listTwo->m_listNode;
	listRes = addTwoNumbers(listOne, listTwo);
}



/*
	整数转换。编写一个函数，确定需要改变几个位才能将整数A转成整数B。

	示例1:
	输入：A = 29 （或者0b11101）, B = 15（或者0b01111）
	输出：2

	示例2:
	输入：A = 1，B = 2
	输出：2

	&：与，同1为1，其他为0
	|：或，只要有1，就是1，其他为0
	^：异或，只要相同就是0，其他为1
*/
int convertInteger(int iOneVale, int iTwoValue)
{
	int iChangeNum = 0;
	unsigned int iXorValue = 0;

	iXorValue = iOneVale ^ iTwoValue;

	// 统计1的个数
	while (iXorValue != 0)
	{
		iXorValue = iXorValue & (iXorValue - 1); // 此操作是移除掉最后一位，直至数值为0
		iChangeNum++;
	}

	return iChangeNum;
}

void Execute_convertInteger()
{
	int iTemp = convertInteger(29, 15);
}

/*
	设计一个栈，除了常规栈支持的pop与push函数以外，还支持min函数，该函数返回栈元素中的最小值
	执行push、pop和min操作的时间复杂度必须为O(1)
*/
class MinStack
{
public:
	/** initialize your data structure here. */
	MinStack()
	{
		m_iSize = 0;
		m_pStack = nullptr;
	}
	void push(int x)
	{
		if (0 == m_iSize)
		{
			m_iSize += 1;
			m_pStack = new int[1];
			m_pStack[0] = x;
			return;
		}

		int* pStack = nullptr;

		// 留出第一位作为最后插入的数，栈的特性
		pStack = new int[m_iSize + 1];
		for (int i = 0; i < m_iSize; i++)
		{
			pStack[i + 1] = m_pStack[i];
		}

		pStack[0] = x;
		m_pStack = nullptr;
		m_iSize += 1;
		m_pStack = pStack;

		pStack = nullptr;
		delete pStack;
	}
	void pop()
	{
		if (0 == m_iSize)
		{
			return;
		}

		int* pStack = nullptr;

		pStack = new int[m_iSize - 1];
		for (int i = 0; i < m_iSize; i++)
		{
			if (0 != i)
			{
				pStack[i - 1] = m_pStack[i];
			}
		}

		m_iSize--;

		m_pStack = nullptr;
		m_pStack = pStack;
		pStack = nullptr;
		delete pStack;
	}
	int top()
	{
		if (0 == m_iSize)
		{
			return 0;
		}

		return m_pStack[0];
	}
	int getMin()
	{
		if (0 == m_iSize)
		{
			return 0;
		}

		vector<int>vctTemp;
		for (int i = 0; i < m_iSize; i++)
		{
			vctTemp.push_back(m_pStack[i]);
		}

		sort(vctTemp.begin(), vctTemp.end());
		return vctTemp.at(0);
	}

	int m_iSize;
	int* m_pStack;
};

class MyQueue
{
public:
	/** Initialize your data structure here. */
	MyQueue()
	{
		m_iSize = 0;
		m_pQueue = nullptr;
	}
	/** Push element x to the back of queue. */
	void push(int x)
	{
		if (0 == m_iSize)
		{
			m_pQueue = new int[1];
			m_pQueue[0] = x;
			m_iSize++;
			return;
		}

		int* pQueueTemp = new int[m_iSize + 1];

		for (int i = 0; i < m_iSize; i++)
		{
			pQueueTemp[i] = m_pQueue[i];
		}

		pQueueTemp[m_iSize] = x;
		m_pQueue = nullptr;
		m_pQueue = pQueueTemp;
		pQueueTemp = nullptr;
		delete pQueueTemp;

		m_iSize++;
	}
	/** Removes the element from in front of queue and returns that element. */
	int pop()
	{
		int iTop = 0;
		iTop = peek();

		if (m_iSize > 0)
		{
			int* pQueueTemp = new int[m_iSize - 1];

			for (int i = 0; i < m_iSize; i++)
			{
				if (0 != i)
				{
					pQueueTemp[i - 1] = m_pQueue[i];
				}
			}

			m_pQueue = nullptr;
			m_pQueue = pQueueTemp;
			pQueueTemp = nullptr;
			delete pQueueTemp;

			m_iSize--;
		}

		return iTop;
	}
	/** Get the front element. */
	int peek()
	{
		if (0 == m_iSize)
		{
			return 0;
		}

		return m_pQueue[0];
	}
	/** Returns whether the queue is empty. */
	bool empty()
	{
		if (0 == m_iSize)
		{
			return true;
		}

		return false;
	}

	int m_iSize;
	int* m_pQueue;
};

struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

/*
	给定一个有序整数数组，元素各不相同且按升序排列，编写一个算法，创建一棵高度最小的二叉搜索树。

	示例:
	给定有序数组: [-10,-3,0,5,9],

	一个可能的答案是：[0,-3,9,-10,null,5]，它可以表示下面这个高度平衡二叉搜索树：

	因为数组是从小到大排列，假设123，在123数组中根节点是他们的中点2，如果数组长度很长就要用到递归，
	举个例子1234567，4是中点，此时分割数组123是左子树，567是右子树，依次类推就能使二叉树的高度为最小。
*/
TreeNode* perm(vector<int>& nums, int l, int r)
{
	if (l > r)
	{
		return nullptr;
	}
	int m = (l + r) / 2;
	TreeNode* root = new TreeNode(nums[m]);
	root->left = perm(nums, l, m - 1);
	root->right = perm(nums, m + 1, r);
	return root;
}
TreeNode* sortedArrayToBST(vector<int>& nums)
{
	return perm(nums, 0, nums.size() - 1);
}

/*
	配对交换。
	编写程序，交换某个整数的奇数位和偶数位，尽量使用较少的指令（也就是说，位0与位1交换，位2与位3交换，以此类推）。

	示例1:
	输入：num = 2（或者0b10）
	输出 1 (或者 0b01)

	示例2:
	输入：num = 3
	输出：3

	int -> 32位
	奇数位全1 -> 0101.... 表示为 0x55555555
	偶数位全1 -> 1010.... 表示为 0xaaaaaaaa
	ans = (提取奇数位 << 1) + (提取偶数位 >> 1)
*/
int exchangeBits(int iNum)
{
	return (((iNum & 0x55555555) << 1) | ((iNum & 0xaaaaaaaa) >> 1));
}

void Execute_exchangeBits()
{
	int iTemp = exchangeBits(1);
}

class CShallowCopy
{
public:
	CShallowCopy(const char* pCopy)
	{
		int iLength = strlen(pCopy);
		this->m_pCopy = (char*)malloc(iLength);
		this->m_pCopy = pCopy;
		cout << m_pCopy << endl;
	}

	~CShallowCopy() { delete m_pCopy; }

	const char* m_pCopy;
};

void Execute_CShallowCopy()
{
	CShallowCopy cshCopy("ding");
	CShallowCopy cshCopyCopy(cshCopy);
}

class CDeepCopy
{
public:
	CDeepCopy(const char* pCopy)
	{
		int iLength = strlen(pCopy);
		m_pCopy = (char*)malloc(iLength);
		m_pCopy = pCopy;
		cout << m_pCopy << endl;
	}

	CDeepCopy(CDeepCopy& cCopy) { cCopy.m_pCopy = nullptr; }

	~CDeepCopy() { delete m_pCopy; }

	const char* m_pCopy;
};

void Execute_CDeepCopy()
{
	CDeepCopy cdeCopy("ding");
	CDeepCopy cdeCopyCopy(cdeCopy);
}

void Execute_CMysql()
{
	CMysql cSql;

	SSqlParamSign spara;

	vector<string>vctStrTemp;
	string strError;
	string strTemp;

	spara.strHost = "localhost";
	spara.strUser = "root";
	spara.strPassword = "123456";
	spara.strDbName = "dingtest";
	spara.iPort = 3306;

	if (!cSql.isPing())
	{
		if (!cSql.init(spara))
		{
			strError = cSql.m_strSqlError;
		}
	}

	if (!cSql.query("select * from xtool0005vw where TextIndex like '0x7C,0x29,0x7A,0x9A,0xE1,0x3D'"))
	{
		strError = cSql.m_strSqlError;
	}

	vctStrTemp = cSql.getQueryAll();

	strTemp = cSql.getQueryLine("0x5D,0xA7,0xA4,0x50,0x59,0x0F");
}


/*
	示例 1:
	给定 matrix =
	[
	  [1,2,3],
	  [4,5,6],
	  [7,8,9]
	]

	原地旋转输入矩阵，使其变为:
	[
	  [7,4,1],
	  [8,5,2],
	  [9,6,3]
	]
*/
void rotate(vector<vector<int>>& vctMatrix)
{
	vector<vector<int>> vctArray = vctMatrix;

	for (int i = 0; i < vctMatrix.size(); i++)
	{
		for (int j = 0; j < vctMatrix.size(); j++)
		{
			vctArray[j][vctMatrix.size() - i - 1] = vctMatrix[i][j];
		}
	}

	vctMatrix = vctArray;
}

void Execute_rotate()
{
	vector<int>vctOne{ 1,2,3 };
	vector<int>vctTwo{ 4,5,6 };
	vector<int>vctThree{ 7,8,9 };
	vector<vector<int>>dvctTemp;

	dvctTemp.push_back(vctOne);
	dvctTemp.push_back(vctTwo);
	dvctTemp.push_back(vctThree);
	rotate(dvctTemp);
}

/*
	栈排序。 编写程序，对栈进行排序使最小元素位于栈顶。
	最多只能使用一个其他的临时栈存放数据，但不得将元素复制到别的数据结构（如数组）中
	该栈支持如下操作：push、pop、peek 和 isEmpty。当栈为空时，peek 返回 -1
*/
class SortedStack
{
public:
	SortedStack()
	{
		m_iSize = 0;
		m_pIntArray = nullptr;
	}
	void push(int iValue)
	{
		if (m_pIntArray == nullptr)
		{
			m_pIntArray = new int[1];
			m_pIntArray[0] = iValue;
			m_iSize++;
			return;
		}

		int* pIntTemp = new int[m_iSize + 1];

		for (int i = 0; i < m_iSize; i++)
		{
			pIntTemp[i] = m_pIntArray[i];
		}

		pIntTemp[m_iSize] = iValue;
		for (int i = 0; i < m_iSize + 1; i++)
		{
			for (int j = i + 1; j < m_iSize + 1; j++)
			{
				if (pIntTemp[i] > pIntTemp[j])
				{
					int iTemp = pIntTemp[i];
					pIntTemp[i] = pIntTemp[j];
					pIntTemp[j] = iTemp;
				}
			}
		}

		m_pIntArray = nullptr;
		m_pIntArray = pIntTemp;
		pIntTemp = nullptr;
		delete pIntTemp;

		m_iSize++;
	}
	void pop()
	{
		if (0 == m_iSize)
		{
			return;
		}

		int* pIntArray = new int[m_iSize - 1];

		for (int i = 0; i < m_iSize; i++)
		{
			if (0 != i)
			{
				pIntArray[i - 1] = m_pIntArray[i];
			}
		}

		m_pIntArray = nullptr;
		m_pIntArray = pIntArray;
		pIntArray = nullptr;
		delete pIntArray;

		m_iSize--;
	}
	int peek()
	{
		if (0 == m_iSize)
		{
			return -1;
		}

		return m_pIntArray[0];
	}
	bool isEmpty()
	{
		if (0 == m_iSize)
		{
			return true;
		}

		return false;
	}

	int m_iSize;
	int* m_pIntArray;
};

void Execute_SortedStack()
{
	SortedStack mstack;
	mstack.push(4);
	mstack.push(1);
	mstack.push(2);
	mstack.push(3);
	mstack.pop();
}

/*
	给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。

	回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。例如，121 是回文，而 123 不是。

	示例 1：
	输入：x = 121
	输出：true

	示例 2：
	输入：x = -121
	输出：false
	解释：从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。

	示例 3：
	输入：x = 10
	输出：false
	解释：从右向左读, 为 01 。因此它不是一个回文数。

	示例 4：
	输入：x = -101
	输出：false
*/
bool isPalindrome(int iPalindrome)
{
	if ((iPalindrome >= 0) && (iPalindrome < 10))
	{
		return true;
	}

	if (iPalindrome < 0)
	{
		return false;
	}

	vector<int>vctInt;

	int iLeft = 0;
	int iRight = 0;

	while (0 < iPalindrome)
	{
		int iTemp = 0;
		iTemp = iPalindrome % 10;
		vctInt.push_back(iTemp);

		iPalindrome = iPalindrome / 10;
	}

	iRight = vctInt.size() - 1;

	while (iLeft != vctInt.size() - 1)
	{
		if (vctInt[iLeft] != vctInt[iRight])
		{
			return false;
		}

		iLeft++;
		iRight--;
	}

	return true;
}

void Execute_isPalindrome()
{
	bool bIsTemp = isPalindrome(123);
}

// 链表反转
ListNode* LinkedlistReversal(ListNode* plistHead)
{
	ListNode* plistReverse = nullptr;

	vector<int>vctOrigin;

	if (plistHead == nullptr)
	{
		return plistReverse;
	}

	while (plistHead != nullptr)
	{
		vctOrigin.push_back(plistHead->val);
		plistHead = plistHead->next;
	}

	for (int i = 0; i < vctOrigin.size(); i++)
	{
		if (plistReverse == nullptr)
		{
			plistReverse = new ListNode;
			plistReverse->val = vctOrigin[i];
		}
		else
		{
			ListNode* plistTemp = new ListNode;
			plistTemp->val = vctOrigin[i];

			plistTemp->next = new ListNode;
			plistTemp->next = plistReverse;

			plistReverse = nullptr;
			plistReverse = plistTemp;
			plistTemp = nullptr;
			delete plistTemp;
		}
	}

	return plistReverse;
}

void Execute_LinkedlistReversal()
{
	ListNode* listOne = new ListNode;
	ListNode* listTwo = new ListNode;
	ListNode* listRes = new ListNode;

	listOne->push_back(1);
	listOne->push_back(9);
	listOne->push_back(9);
	listOne = listOne->m_listNode;
	listRes = LinkedlistReversal(listOne);
}

/*
	给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。

	示例 1：
	输入：nums = [1,2,3]
	输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
*/

/*
void backtracking(参数)
{
	if (终⽌条件)
	{
		存放结果;
		return;
	}

	for (选择：本层集合中元素（树中节点孩⼦的数量就是集合的⼤⼩）)
	{
		处理节点;
		backtracking(路径，选择列表); // 递归
		回溯，撤销处理结果
	}
}
*/


vector<vector<int>>dvctDfs;
vector<int>vctDfs;

void backtrace(vector<int>& vctArray, vector<int> vctSign)
{
	if (vctDfs.size() == vctArray.size())
	{  //终止条件为path大小达到数组大小
		dvctDfs.push_back(vctDfs);
		return;
	}

	for (int i = 0; i < vctArray.size(); i++)
	{
		//if (vctUse[i] == 1)
		//{
		//	continue; //标记数组判断
		//}

		//vctUse[i] = 1;

		vctDfs.push_back(vctArray[i]);
		backtrace(vctArray, vctSign);   //回溯
		vctDfs.pop_back();

		//vctUse[i] = 0;
	}
}

vector<vector<int>> permute(vector<int>& vctNum)
{
	dvctDfs.clear();
	vctDfs.clear();
	vector<int>vctSign(vctNum.size(), 0);
	backtrace(vctNum, vctSign);
	return dvctDfs;
}

void Execute_permute()
{
	vector<int>vctIntTemp;
	vector<vector<int>>dvctTemp;

	vctIntTemp.push_back(1);
	vctIntTemp.push_back(2);
	vctIntTemp.push_back(3);
	dvctTemp = permute(vctIntTemp);
}

void TcpIpFunciton()
{
	WSADATA wsadata;

	int iResult = 0;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

	sockaddr sockAddress;
	sockaddr_in sockAddressIn;

	SOCKET m_sock = 0;

	iResult = send(m_sock, "11", 2, 0);



}

/*
	编写一个函数来查找字符串数组中的最长公共前缀
	如果不存在公共前缀，返回空字符串 ""。

	示例 1：
	输入：strs = ["flower","flow","flight"]
	输出："fl"

	示例 2：
	输入：strs = ["dog","racecar","car"]
	输出：""
*/
string longestCommonPrefix(vector<string>& vctStr)
{
	string strRes;

	if (vctStr.empty())
	{
		return strRes;
	}
	else if (1 == vctStr.size())
	{
		return vctStr[0];
	}

	for (int i = 0; i < vctStr[0].size(); i++)
	{
		char chFronti = vctStr[0][i];

		for (int j = 0; j < vctStr.size(); j++)
		{
			char chFrontj = vctStr[j][i];

			if (chFronti != chFrontj)
			{
				return strRes;
			}
		}

		strRes += chFronti;
	}

	return strRes;
}

void Execute_longestCommonPrefix()
{
	vector<string>vctStrTemp;
	string strTemp;

	vctStrTemp.push_back("flower");
	vctStrTemp.push_back("flow");
	vctStrTemp.push_back("flight");
	strTemp = longestCommonPrefix(vctStrTemp);
}

/*
	这道题是被称为最长公共子序列的问题（LCS，Longest Common Subsequence）的著名问题。这道题我们是用动态规划的思想来做的。
	我们先拿第一组测试用例，asdf 与 adfsd 作为例子来看一下这道题的思路。上图！！

	j / i	0	1(a)	2(s)	3(d)	4(f)
	0	    0	0	0	0	0
	1(a)	0	1	1	1	1
	2(d)	0	1	1	2	2
	3(f)	0	1	1	2	3
	4(s)	0	1	2	2	3
	5(d)	0	1	2	2	3

	做这种题，我们要用一个二维数组（dp[MAX_N][MAX_N]）来存放每一个状态的值。如图所示，横向代表i、纵向代表j，那么，每一个网格的值是怎么来的呢。
	在这里我们把每一个状态即dp[i][j] 看做 s1 … si 和 t1 … tj 的LCS的长度。由此我们，s1 … s(i+1) 和 t1 … t(j+1) 对应的公共子列长度可能是：

	当s(i+1) == t(j+1),在 s1 … si 和 t1 … tj 的公共子列末尾追加上s(i+1) 。

	否则则可能是 s1 … si 和 t1 … t(j+1) 的公共子列或者 s1 … s(i+1) 和 t1 … tj 的公共子列最大值。

	对应以下一个公式：
*/
/*
	给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。

	一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。
	例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。
	两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。

	示例 1：
	输入：text1 = "abcde", text2 = "ace"
	输出：3
	解释：最长公共子序列是 "ace" ，它的长度为 3 。

	示例 2：
	输入：text1 = "abc", text2 = "abc"
	输出：3
	解释：最长公共子序列是 "abc" ，它的长度为 3 。

	示例 3：
	输入：text1 = "abc", text2 = "def"
	输出：0
	解释：两个字符串没有公共子序列，返回 0 。

	动态规划公式:
	dp[i + 1][j + 1] =
	{
		dp[i][j] + 1,(s[i] == t[j])
		max(dp[i][j + 1],dp[i + 1][j]),(其他)
	}
*/
int longestCommonSubsequence(string strTextOne, string strTextTwo)
{
	int iLength = 0;

	vector<vector<int>>vctDynamic(strTextOne.size() + 1, vector<int>(strTextTwo.size() + 1, 0));

	for (int i = 0; i < strTextOne.size(); i++)
	{
		for (int j = 0; j < strTextTwo.size(); j++)
		{
			if (strTextOne.at(i) == strTextTwo.at(j))
			{
				vctDynamic[i + 1][j + 1] = vctDynamic[i][j] + 1;
			}
			else
			{
				vctDynamic[i + 1][j + 1] = max(vctDynamic[i][j + 1], vctDynamic[i + 1][j]);
			}
		}
	}

	iLength = vctDynamic.back().back();
	return iLength;
}

void Execute_longestCommonSubsequence()
{
	int iTemp = longestCommonSubsequence("ezupkr", "ubmrapg");
}

/*
	输入两个链表，找出它们的第一个公共节点。

	输入：intersectVal = 8, listA = [4,1,8,4,5], listB = [5,0,1,8,4,5], skipA = 2, skipB = 3
	输出：Reference of the node with value = 8

	输入解释：相交节点的值为 8 （注意，如果两个列表相交则不能为 0）。
	从各自的表头开始算起，链表 A 为 [4,1,8,4,5]，链表 B 为 [5,0,1,8,4,5]。
	在 A 中，相交节点前有 2 个节点；在 B 中，相交节点前有 3 个节点。
*/
ListNode* getIntersectionNode(ListNode* plistOne, ListNode* plistTwo)
{
	if ((plistOne == nullptr) || (plistTwo == nullptr))
	{
		return nullptr;
	}

	ListNode* plistSlow = plistOne;
	ListNode* plistFast = plistTwo;

	while (plistSlow != plistFast)
	{
		//plistSlow = plistSlow == nullptr ? plistTwo : plistSlow->next;
		//plistFast = plistFast == nullptr ? plistOne : plistFast->next;

		if (plistSlow == nullptr)
		{
			plistSlow = plistTwo;
		}
		else
		{
			plistSlow = plistSlow->next;
		}

		if (plistFast == nullptr)
		{
			plistFast = plistOne;
		}
		else
		{
			plistFast = plistFast->next;
		}
	}

	return plistSlow;
}

/*
	字符串轮转。给定两个字符串s1和s2，请编写代码检查s2是否为s1旋转而成（比如，waterbottle是erbottlewat旋转后的字符串）

	示例1:
	输入：s1 = "waterbottle", s2 = "erbottlewat"
	输出：True

	erbottlewat erbottlewat

*/
bool isFlipedString(string strOne, string strTwo)
{
	string strAdd;

	if (strOne.size() != strTwo.size())
	{
		return false;
	}

	strAdd = strTwo + strTwo;
	if (string::npos != strAdd.find(strOne))
	{
		return true;
	}

	return false;
}

union uValue
{
	int iOne;
	int iTwo;
	int iThree;
	int iFour;
};

/*
	若链表中的某个节点，既不是链表头节点，也不是链表尾节点，则称其为该链表的「中间节点」。

	假定已知链表的某一个中间节点，请实现一种算法，将该节点从链表中删除。

	例如，传入节点 c（位于单向链表 a->b->c->d->e->f 中），将其删除后，剩余链表为 a->b->d->e->f
*/
void deleteNode(ListNode* plistNode)
{
	if (plistNode == nullptr)
	{
		return;
	}

	if (plistNode->next == nullptr)
	{
		delete plistNode;
		plistNode = nullptr;
		return;
	}

	plistNode->val = plistNode->next->val;
	plistNode->next = plistNode->next->next;
}

template<typename TOne, typename TTwo>
class CTemplate
{
public:
	CTemplate(TOne tOne, TTwo tTwo) : TValueOne(tOne), TValueTwo(tTwo) {}

	void print();

private:
	TOne TValueOne;
	TTwo TValueTwo;
};

template<typename TOne, typename TTwo>
void CTemplate<TOne, TTwo>::print()
{
	cout << TValueOne << TValueTwo << endl;
}

void Execute_CTemplate()
{
	CTemplate<string, int>ctemp("1", 1);
	ctemp.print();
}

/*
	给你一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有和为 0 且不重复的三元组

	注意：答案中不可以包含重复的三元组

	示例 1：
	输入：nums = [-1,0,1,2,-1,-4]
	输出：[[-1,-1,2],[-1,0,1]]

	示例 2：
	输入：nums = []
	输出：[]

	示例 3：
	输入：nums = [0]
	输出：[]
*/
vector<vector<int>> threeSum(vector<int>& nums)
{
	int n = nums.size();
	sort(nums.begin(), nums.end());
	vector<vector<int>> ans;

	// 枚举 a
	for (int first = 0; first < n; ++first)
	{
		// 需要和上一次枚举的数不相同
		if (first > 0 && nums[first] == nums[first - 1])
		{
			continue;
		}
		// c 对应的指针初始指向数组的最右端
		int third = n - 1;
		int target = -nums[first];

		// 枚举 b
		for (int second = first + 1; second < n; ++second)
		{
			// 需要和上一次枚举的数不相同
			if (second > first + 1 && nums[second] == nums[second - 1])
			{
				continue;
			}
			// 需要保证 b 的指针在 c 的指针的左侧
			while (second < third && nums[second] + nums[third] > target)
			{
				--third;
			}
			// 如果指针重合，随着 b 后续的增加
			// 就不会有满足 a+b+c=0 并且 b<c 的 c 了，可以退出循环
			if (second == third)
			{
				break;
			}
			if (nums[second] + nums[third] == target)
			{
				ans.push_back({ nums[first], nums[second], nums[third] });
			}
		}
	}
	return ans;
}

void Execute_threeSum()
{
	vector<int>vctIntTemp;
	vector<vector<int>>dvctTemp;

	vctIntTemp.push_back(-1);
	vctIntTemp.push_back(0);
	vctIntTemp.push_back(1);
	vctIntTemp.push_back(2);
	vctIntTemp.push_back(-1);
	vctIntTemp.push_back(-4);
	dvctTemp = threeSum(vctIntTemp);
}


struct SHashNode
{
	int key;
	int value;
	SHashNode* next;
	SHashNode() : next(nullptr) {}
};


/*
	输入：
	["MyHashMap", "put", "put", "get", "get", "put", "get", "remove", "get"]
	[[], [1, 1], [2, 2], [1], [3], [2, 1], [2], [2], [2]]
	输出：
	[null, null, null, 1, -1, null, 1, null, -1]
*/
class MyHashMap
{
public:
	/** Initialize your data structure here. */
	MyHashMap()
	{

	}

	/** value will always be non-negative. */
	void put(int key, int value)
	{
		if (m_pHashNode == nullptr)
		{
			m_pHashNode->key = key;
			m_pHashNode->value = value;
			return;
		}

		SHashNode* pHashTemp = m_pHashNode;

		while (pHashTemp->next != nullptr)
		{
			if (pHashTemp->key == key)
			{
				pHashTemp->value = value;
				return;
			}

			pHashTemp = pHashTemp->next;
		}

		pHashTemp->next = new SHashNode;
		pHashTemp->key = key;
		pHashTemp->value = value;
	}

	/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
	int get(int key)
	{
		if (m_pHashNode == nullptr)
		{
			return -1;
		}

		SHashNode* pHashTemp = m_pHashNode;

		while (pHashTemp->next != nullptr)
		{
			if (pHashTemp->key == key)
			{
				return pHashTemp->value;
			}

			pHashTemp = pHashTemp->next;
		}

		return -1;
	}

	/** Removes the mapping of the specified value key if this map contains a mapping for the key */
	void remove(int key)
	{
		if (m_pHashNode == nullptr)
		{
			return;
		}

		SHashNode* pHashTemp = m_pHashNode;
		SHashNode* pHashPrev = m_pHashNode;

		while (pHashTemp->next != nullptr)
		{
			pHashTemp = pHashTemp->next;

			if (pHashTemp->key == key)
			{

			}

			pHashPrev = pHashPrev->next;
		}
	}

	SHashNode* m_pHashNode;
};

/*
	无重复字符串的排列组合。编写一种方法，计算某字符串的所有排列组合，字符串每个字符均不相同

	示例1:
	输入：S = "qwe"
	输出：["qwe", "qew", "wqe", "weq", "ewq", "eqw"]

	示例2:
	输入：S = "ab"
	输出：["ab", "ba"]

		def backtrack(路径, 选择列表):
		if 满足结束条件:
			result.add(路径)
			return

		for 选择 in 选择列表:
			做选择
			backtrack(路径, 选择列表)
			撤销选择

		1、路径：也就是已经做出的选择
		2、选择列表：也就是你当前可以做的选择
		3、结束条件：也就是到达决策树底层，无法再做选择的条件
*/

vector<string>g_vctBackTrack;

void permutation_BackTracking(string strTrand, int iStart)
{
	// 结束条件，做出插入操作
	if (iStart == strTrand.size())
	{
		g_vctBackTrack.push_back(strTrand);
	}

	for (int i = iStart; i < strTrand.size(); i++)
	{
		// 进行选择
		swap(strTrand.at(i), strTrand.at(iStart));

		// 进行回溯
		permutation_BackTracking(strTrand, iStart + 1);
	}
}

vector<string> permutation(string strTrand)
{
	permutation_BackTracking(strTrand, 0);

	// 对于有重复字符串排列的操作
	sort(g_vctBackTrack.begin(), g_vctBackTrack.end());
	g_vctBackTrack.erase(unique(g_vctBackTrack.begin(), g_vctBackTrack.end()), g_vctBackTrack.end());
	return g_vctBackTrack;
}

void Execute_permutation()
{
	vector<string>vctStrTemp;
	vctStrTemp = permutation("qqe");
}

vector<string> permutation_algorithm(string strTrand)
{
	vector<string>vctRes;

	if (strTrand.empty())
	{
		return vctRes;
	}

	// 在使用全排列时，需要对数组进行升序排列
	sort(strTrand.begin(), strTrand.end());

	vctRes.push_back(strTrand);
	while (next_permutation(strTrand.begin(), strTrand.end()))
	{
		vctRes.push_back(strTrand);
	}

	return vctRes;
}

/*
	编写一个函数，不用临时变量，直接交换numbers = [a, b]中a与b的值

	示例：
	输入: numbers = [1,2]
	输出: [2,1]

	按位与：同1为1
	按位或：有1则1
	异或：不同则1
*/
vector<int> swapNumbers(vector<int>& vctNumber)
{
	if (vctNumber.empty())
	{
		return vctNumber;
	}

	int iXor = 0;

	iXor = vctNumber[0] ^ vctNumber[1];
	vctNumber[0] = iXor ^ vctNumber[0];
	vctNumber[1] = iXor ^ vctNumber[1];

	return vctNumber;
}

void Execute_swapNumbers()
{
	vector<int>vctIntTemp;
	vctIntTemp.push_back(1);
	vctIntTemp.push_back(2);
	vctIntTemp = swapNumbers(vctIntTemp);
}

/*
	稀疏数组搜索。有个排好序的字符串数组，其中散布着一些空字符串，编写一种方法，找出给定字符串的位置

	示例1:
	输入: words = ["at", "", "", "", "ball", "", "", "car", "", "","dad", "", ""], s = "ta"
	输出：-1
	说明: 不存在返回-1。

	其实也是二分查找
*/
int findString(vector<string>& vctWords, string strFind)
{
	if (vctWords.empty())
	{
		return -1;
	}

	int iMiddle = 0;
	int iLeft = 0;
	int iRight = 0;

	iRight = vctWords.size() - 1;

	while (iLeft <= iRight)
	{
		iMiddle = (iLeft + iRight) / 2;

		int iTemp = iMiddle;

		while ((iMiddle <= iRight) && (vctWords[iMiddle].empty()))
		{
			iMiddle++;
		}

		if (iMiddle > iRight)
		{
			iRight = iTemp - 1;
			continue;
		}

		if (vctWords[iMiddle] == strFind)
		{
			return iMiddle;
		}
		else if (vctWords[iMiddle] < strFind)
		{
			iLeft = iMiddle + 1;
		}
		else
		{
			iRight = iMiddle - 1;
		}
	}

	return -1;
}

void Execute_findString()
{
	vector<string>vctStrTemp;
	vctStrTemp.push_back("at");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("ball");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("car");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("dad");
	vctStrTemp.push_back("");
	vctStrTemp.push_back("");

	int iTemp = findString(vctStrTemp, "ta");
}

/*
	设计一个方法，找出任意指定单词在一本书中的出现频率。
	你的实现应该支持如下操作：

	WordsFrequency(book)构造函数，参数为字符串数组构成的一本书
	get(word)查询指定单词在书中出现的频率

	示例：
	WordsFrequency wordsFrequency = new WordsFrequency({"i", "have", "an", "apple", "he", "have", "a", "pen"});
	wordsFrequency.get("you"); //返回0，"you"没有出现过
	wordsFrequency.get("have"); //返回2，"have"出现2次
	wordsFrequency.get("an"); //返回1
	wordsFrequency.get("apple"); //返回1
	wordsFrequency.get("pen"); //返回1
*/
class WordsFrequency
{
public:
	WordsFrequency(vector<string>& vctBook)
	{
		m_unmapInt.clear();

		for (vector<string>::iterator it = vctBook.begin(); it != vctBook.end(); it++)
		{
			if (!m_unmapInt[*it])
			{
				m_unmapInt[*it] = 1;
			}
			else
			{
				m_unmapInt[*it]++;
			}
		}
	}

	int get(string strWord)
	{
		return m_unmapInt[strWord];
	}

	unordered_map<string, int>m_unmapInt;
};

void Execute_WordsFrequency()
{
	vector<string>vctStrTemp;

	vctStrTemp.push_back("a");
	vctStrTemp.push_back("ab");
	vctStrTemp.push_back("bbb");
	vctStrTemp.push_back("bbb");
	vctStrTemp.push_back("bc");

	WordsFrequency wordsm(vctStrTemp);
	int iTemp = wordsm.get("bbb");
}

/*
	搜索旋转数组。给定一个排序后的数组，包含n个整数，但这个数组已被旋转过很多次了，次数不详。
	请编写代码找出数组中的某个元素，假设数组元素原先是按升序排列的。若有多个相同元素，返回索引值最小的一个。

	示例1:
	输入: arr = [15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14], target = 5
	输出: 8（元素5在该数组中的索引）

	示例2:
	输入：arr = [15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14], target = 11
	输出：-1 （没有找到）
*/
int search(vector<int>& vctArray, int iTarget)
{
	if (vctArray.empty())
	{
		return -1;
	}

	int iMiddle = 0;
	int iLeft = 0;
	int iRight = 0;

	iRight = vctArray.size() - 1;

	while (iLeft <= iRight)
	{
		iMiddle = (iLeft + iRight) / 2;

		if (iTarget == vctArray.at(iMiddle))
		{
			return iMiddle;
		}
		else if (vctArray.at(iMiddle) < iTarget)
		{
			iLeft = iMiddle + 1;
		}
		else if (vctArray.at(iMiddle) > iTarget)
		{
			iRight = iMiddle - 1;
		}
	}

	return -1;
}
























#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

//#define DEBUGTEST

int main()
{
	initializer_list<int>InitListArray;
	vector<string>vctStrTemp;
	vector<vector<string>>dvctStrTemp;
	vector<int>vctIntTemp;
	vector<vector<int>>dvctTemp;
	string strTemp;
	int iTemp = 0;
	short shTemp = 0;
	long long llTemp = 0;
	float fTemp = 2.5;
	double dTemp = 2.5454;
	char chTemp;
	const char* pStr;
	char* pchTemp;
	char ArrayStr[100] = { 0 };
	bool bIsTemp = false;

	vctIntTemp.push_back(15);
	vctIntTemp.push_back(16);
	vctIntTemp.push_back(19);
	vctIntTemp.push_back(1);
	vctIntTemp.push_back(2);
	vctIntTemp.push_back(3);

	iTemp = search(vctIntTemp, 15);

	// 单词出现的频率
	Execute_WordsFrequency();

	// 稀疏字符串数组二分查找
	Execute_findString();

	// 不使用临时变量直接交换两个数值
	Execute_swapNumbers();

	// 求字符串全排列 - 回溯
	Execute_permutation();

	// 找三元组
	Execute_threeSum();

	// 实现模板
	Execute_CTemplate();

	// 动态规划 - 求最长公共子序列
	Execute_longestCommonSubsequence();

	// 求最长公共前缀
	Execute_longestCommonPrefix();

	// 全排列 - 回溯
	Execute_permute();

	// socket 
	TcpIpFunciton();

	// 是否是回文数
	Execute_isPalindrome();

	// 实现栈排序的类
	Execute_SortedStack();

	// 原地旋转矩阵
	Execute_rotate();

	// 交换整数的奇数位和偶数位
	Execute_exchangeBits();

	// 需要改变几个位才能从A-B
	Execute_convertInteger();

	// 求回文串的排列
	Execute_canPermutePalindrome();

	// 浅拷贝
	Execute_CShallowCopy();

	// 深拷贝
	Execute_CDeepCopy();

	// 自实现MYSQL接口
	Execute_CMysql();

	// 链表反转
	Execute_LinkedlistReversal();

	// 链表反向求和
	Execute_addTwoNumbers();

	// 判断链表是否是环形链表，并求出入环点
	Execute_detectCycle();

	// 自实现链表功能
	Execute_ListNode();

	// 移除链表重复节点
	Execute_removeDuplicateNodes();

	// 字符串压缩
	Execute_compressString();

	// 求最大和的连续数组
	Execute_maxSubArray();

	// 选择排序
	SelectSort(vctIntTemp);

	// 最小的k个数
	Execute_GetLeastNumbers_Solution();

	// 二分查找
	Execute_SearchTwo();

	// 二分查找
	Execute_binarySearch2();

	// 自实现Ini类功能
	Execute_CMyIni();

	// 实现atoi函数
	iTemp = myAtoi("-123");

	CreadThreadNullParameter(ThreadMutuallyExclusive);

	// 合并两个链表
	Execute_mergeTwoLists();

	// k个组合 - 回溯
	Execute_combine();

	// 类的函数指针
	FunctionTest(&mycin::bTestFunction, "1", strTemp, 0);

	return 0;
}


