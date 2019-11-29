//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/9/9
// Input：网表类型、网表文件地址+文件名
// Output：指向 解析相应网表的派生类 的基类指针
//--------------------------------------------------------------------


#include "ReadNetListContext.h"
#include "ReadProtelNetList.h"
#include "ReadCadenceProtelNetList.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

ReadNetListContext::ReadNetListContext(const string& netlistType, const string& dir)
{
	netListType = netlistType;  // 用户选择的网表类型
	netListDir = dir;           // 用户选择的网表地址

	// 先判断用户选择的网表类型
	// 为什么不用switch语句呢，因为string没法和const char比较，强制类型转换又十分危险，使用if判断条件可以进行隐式类型转换
	if (netListType == "CadenceProtel")
	{
		p_ReadNetList = new ReadCadenceProtelNetList(netListDir);
	}
	else if (netListType == "ProtelProtel")
	{
		p_ReadNetList = new ReadProtelNetList(netListDir);
	}
	else
	{
		ifstream inFile_NetList(netListDir);
		ostringstream temp;
		temp << inFile_NetList.rdbuf();
		string netListFileContent = temp.str();
		// 在字符串里查找有没有DESIGNATOR和PARTTYPE关键字；有的话，就是Cadence软件生成的protel型.NET网表；没有的话，就是Protel软件生成的protel型.NET网表
		auto pos1 = netListFileContent.find("DESIGNATOR");
		auto pos2 = netListFileContent.find("PARTTYPE");
		// 让软件自动判断网表类型
		if ((pos1 == netListFileContent.npos) && (pos2 == netListFileContent.npos))
		{
			// 没有的话，就是Protel软件生成的protel型.NET网表
			p_ReadNetList = new ReadProtelNetList(netListDir);
		}
		else
		{
			// 有的话，就是Cadence软件生成的protel型.NET网表
			p_ReadNetList = new ReadCadenceProtelNetList(netListDir);
		}
	}


}


ReadNetListContext::~ReadNetListContext()
{
	delete p_ReadNetList;
	p_ReadNetList = nullptr;
}

ReadNetList* ReadNetListContext::getNetListp()
{
	return p_ReadNetList;
}