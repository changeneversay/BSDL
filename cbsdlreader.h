#ifndef CBSDLREADER_H
#define CBSDLREADER_H
#include <string>
#include<vector>
using namespace std;
class CBsdlReader
{
private:
	vector<vector<string>> port_info;
	vector<string> port_name;
	vector<string>use_info;
	vector<string>end_info;
	vector<string>constant_name;
	vector<vector<string>>constant_info;
	vector<vector<string>>attribute_info;
	vector<string>attribute_TDI;
	vector<string>attribute_TDO;
	vector<string>attribute_TMS;
	vector<string>attribute_TCK;
	vector<string>attribute_COMPONENT_CONFORMANCE;
	vector<string>attribute_PIN_MAP;
	vector<string>attribute_INSTRUCTION_LENGTH;
	vector<string>attribute_INSTRUCTION_OPCODE;
	vector<string>attribute_INSTRUCTION_CAPTURE;
	vector<string>attribute_IDCODE_REGISTER;
	vector<string>attribute_USERCODE_REGISTER;
	vector<string>attribute_REGISTER_ACCESS;
	vector<string>attribute_BOUNDARY_LENGTH;
	vector<string>attribute_BOUNDARY_REGISTER;
	vector<vector<string>>attribute_BR_info;
	vector<string>Generic_info;
public:
	CBsdlReader() {};
	
	void  ProcessPort(string temp);//处理port 关键字
	void  ProcessBsdlFile(string BsdlFileContent);
	string  SkipProcess(string BsdlFileContent);//跳过无用的信息。
	void  ProcessEnd(string temp);//处理 entity 关键字
	void  ProcessGeneric(string temp);//处理 generic 关键字
	void  ProcessUse(string temp);//处理 use 关键字
	void  ProcessConstant(string temp);//处理  constant 关键字
	void  ProcessAttribute(string temp);//处理attribute关键字
	virtual ~CBsdlReader() = default;

};
























#endif // !CBSDLREADER.H
#pragma once
