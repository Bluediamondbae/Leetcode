#ifndef _FISH_INI_FILE_
#define _FISH_INI_FILE_

#include <string>
#include <vector>

using namespace std;

class IniFileSTL
{
public:
	explicit IniFileSTL(const string& fileName);
	~IniFileSTL(void);
	bool ReadFile(void);

	//使用eg: mysql    ip 127.0.0.1        mysql port 3306
	//其中value是数据如果读取失败时候的默认值
	string ReadString(const string& section, const string& key, const string& value);
	int    ReadInt(const string& section, const string& key, int value);
	bool WriteString(const string& section, const string& key, const string& value);
	bool WriteInt(const string& section, const string& key, int value);
	bool RemoveSection(const string& section);
	bool RemoveKey(const string& section, const string& key);
	bool WriteFile(void);


private:
	static string Trim(const string& str);
	static string LTrim(const string& str);
	static string RTrim(const string& str);
private:
	string m_fileName;
	vector<string> m_vctLine;
};


#endif

// #include <stdio.h>
// #include <iostream>
// #include "IniFileSTL.h"

// int main(int argc, char **argv)
// {
//     IniFileSTL ini("./test.ini");
//      std::cout<<"use default value:"<<std::endl;
//     std::cout<<ini.ReadString("system","ipaddr", "10.70.101.134")<<std::endl;
//     std::cout<<ini.ReadInt("system","tcpport", 7050 )<<std::endl;
//      std::cout<<"input data,and read again:"<<std::endl;
//     ini.WriteString("system","ipaddr", "10.70.101.125");
//     ini.WriteInt("system","tcpport", 8080 );
//     std::cout<<ini.ReadString("system","ipaddr", "10.70.101.134")<<std::endl;
//     std::cout<<ini.ReadInt("system","tcpport", 7050 )<<std::endl;
//     return 0;
// }