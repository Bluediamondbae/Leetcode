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
#include <io.h>
#include <map>
#include <sstream>
#include <atltime.h>
#include <algorithm>
#include <ios>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <iomanip>
#include <math.h>
#include <direct.h>
#include <string>
#include <numeric>
#include <initializer_list>




#define BUF_SIZE 1024


using namespace std;
using namespace pugi;
using namespace this_thread;

//#define SEARCHJAVATEXT
#define  LEETCODE


vector<string>vctNodeStr;
vector<string>vctFolderStr;
set<string>setStr;

int iChildNode = 0;
int iAttriNode = 0;
vector<string>vctRes;
set<pair<string, string>>setCallID;



void PushNodeStr() {}
template<typename T, typename ... Args>
void PushNodeStr(const T& value, const Args &... args)
{
	vctNodeStr.push_back(value);
	PushNodeStr(args...);
}

void WriteXml(string strHeadNode, string strChildNode, string strAttriNode, string strAttriName, string strAttriValue, string strValueNodeName, string ValueNodeValue);

void ReadXml();

struct SMCDDbRequestRef
{
	string strIdRef;
};

struct SRef
{
	string strIdRef;
};

struct SDbNcamedObject
{
	string strSubNod;
	struct SRef Sref;
};

struct SDbDiagComPrimitive
{
	string str6;
	string strIdRef;
	string strSubNod;
	string strDOCREF;
	struct SMCDDbRequestRef Smcddbrequestref;
	struct SDbNcamedObject Sdbncamedobject;
};

