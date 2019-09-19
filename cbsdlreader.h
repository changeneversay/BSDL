#ifndef CBSDLREADER_H
#define CBSDLREADER_H
#include <string>
#include<vector>
class CBsdlReader
{
private:
	std::vector<std::vector<std::string>>port_info;
	std::vector<std::string> port_name;
	std::vector<std::string>use_info;
	std::vector<std::string>end_info;
	std::vector<std::string>constant_name;
	std::vector<std::vector<std::string>>constant_info;
	std::vector<std::string>attribute_TDI;
	std::vector<std::string>attribute_TDO;
	std::vector<std::string>attribute_TMS;
	std::vector<std::string>attribute_TCK;
	std::vector<std::string>attribute_COMPONENT_CONFORMANCE;
	std::vector<std::string>attribute_PIN_MAP;
	std::vector<std::string>attribute_INSTRUCTION_LENGTH;
	std::vector<std::string>attribute_INSTRUCTION_OPCODE;
	std::vector<std::string>attribute_INSTRUCTION_CAPTURE;
	std::vector<std::string>attribute_IDCODE_REGISTER;
	std::vector<std::string>attribute_USERCODE_REGISTER;
	std::vector<std::string>attribute_REGISTER_ACCESS;
	std::vector<std::string>attribute_BOUNDARY_LENGTH;
	std::vector<std::string>attribute_BOUNDARY_REGISTER;
	std::vector<std::vector<std::string>>attribute_BR_info;
	std::vector<std::string>Generic_info;
public:
	CBsdlReader() {};
	void  ProcessPort(std::string temp);//处理port 关键字
	void  ProcessBsdlFile(std::string BsdlFileContent);
	std::string  SkipProcess(std::string BsdlFileContent);//跳过无用的信息。
	void  ProcessEnd(std::string temp);//处理 entity 关键字
	void  ProcessGeneric(std::string temp);//处理 generic 关键字
	void  ProcessUse(std::string temp);//处理 use 关键字
	void  ProcessConstant(std::string temp);//处理  constant 关键字
	void  ProcessAttribute(std::string temp);//处理attribute关键字
	virtual ~CBsdlReader() = default;

};
























#endif // !CBSDLREADER.H
#pragma once
