#ifndef CBSDLREADER_H
#define CBSDLREADER_H
#include <string>
#include<vector>
using namespace std;
class CBsdlReader
{
private:
	
public:
	CBsdlReader() {};
	vector<vector<string>> port_info;
	vector<string> port_name;
	vector<string>use_info;
	vector<string>constant_name;
	vector<vector<string>>constant_info;
	void  ProcessPort(string temp);//处理port 关键字
	void  ProcessBsdlFile(string BsdlFileContent);
	string  SkipProcess(string BsdlFileContent);//跳过无用的信息。
	//void  ProcessEnd();//处理 entity 关键字
	//void  ProcessGeneric();//处理 generic 关键字
	//
	void  ProcessUse(string temp);//处理 use 关键字
	//void  ProcessConstant(string temp);//处理  constant 关键字
	//void  ProcessAttribute();//处理attribute关键字
	virtual ~CBsdlReader() = default;

};
























#endif // !CBSDLREADER.H
#pragma once
