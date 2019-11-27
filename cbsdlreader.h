#ifndef CBSDLREADER_H
#define CBSDLREADER_H
#include <string>
#include<vector>
class CBsdlReader
{
private:
	std::vector<std::vector<std::string>>port_info;
	//std::vector<std::string> port_name;//ǰ��Ϊ�ܽ���   ���һ��Ϊ�ܽ�����
	std::vector<std::string>use_info;
	std::vector<std::string>end_info;
	//std::vector<std::string>constant_name;//��һΪ�ܽ������ڶ�Ϊ����ܽ����
	std::vector<std::vector<std::string>>constant_info;
	std::vector<std::string>attribute_TDI;//true
	std::vector<std::string>attribute_TDO;
	std::vector<std::string>attribute_TMS;
	std::vector<std::string>attribute_TCK;
	std::vector<std::string>attribute_COMPONENT_CONFORMANCE;//�ò���
	std::vector<std::string>attribute_PIN_MAP;//�ò���
	std::vector<std::string>attribute_INSTRUCTION_LENGTH;
	std::vector<std::string>attribute_INSTRUCTION_OPCODE;//��һλָ���� �����Ϊָ�����
	std::vector<std::vector<std::string>>attribute_INSTRUCTION_OPCODE_info;
	std::vector<std::string>attribute_INSTRUCTION_CAPTURE;//
	std::vector<std::string>attribute_IDCODE_REGISTER;
	std::vector<std::string>attribute_USERCODE_REGISTER;
	std::vector<std::string>attribute_REGISTER_ACCESS;
	std::vector<std::string>attribute_BOUNDARY_LENGTH;
	std::vector<std::vector<std::string>>attribute_BR_info;
	std::vector<std::string>Generic_info;
public:
	CBsdlReader() {};
	void  ProcessPort(std::string temp);//����port �ؼ���
	void  ProcessBsdlFile(std::string BsdlFileContent);
	std::string  SkipProcess(std::string& BsdlFileContent);//�������õ���Ϣ��
	void  ProcessEnd(std::string temp);//���� entity �ؼ���
	void  ProcessGeneric(const std::string& temp);//���� generic �ؼ���
	void  ProcessUse(std::string temp);//���� use �ؼ���
	void  ProcessConstant(const std::string& temp);//����  constant �ؼ���
	void  ProcessAttribute(std::string temp);//����attribute�ؼ���
	void  view_BSDL();
	std::vector<std::vector<std::string>> Get_portdata();
	std::vector<std::vector<std::string>> Get_constantdata();
	std::vector<std::vector<std::string>> Get_BRdata();
	virtual ~CBsdlReader() = default;

};
























#endif // !CBSDLREADER.H
#pragma once
