#pragma once

#include "ReadNetList.h"

#include <string>
using std::vector;
using std::string;

class ReadNetListContext
{
private:
	ReadNetList* p_ReadNetList = nullptr;     // 基类指针
public:
	string netListDir;            // 网表文件地址
	string netListType = "";      // 网表文件类型
	ReadNetListContext() = default;
	ReadNetListContext(const string& netlistType, const string& dir);    // （文件类型，文件地址）
	~ReadNetListContext();
	ReadNetList* getNetListp();                           // 返回指向特定派生类对象的基类指针
}; 
