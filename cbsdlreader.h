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
	void  ProcessPort(string temp);//����port �ؼ���
	void  ProcessBsdlFile(string BsdlFileContent);
	string  SkipProcess(string BsdlFileContent);//�������õ���Ϣ��
	//void  ProcessEnd();//���� entity �ؼ���
	//void  ProcessGeneric();//���� generic �ؼ���
	//
	void  ProcessUse(string temp);//���� use �ؼ���
	//void  ProcessConstant(string temp);//����  constant �ؼ���
	//void  ProcessAttribute();//����attribute�ؼ���
	virtual ~CBsdlReader() = default;

};
























#endif // !CBSDLREADER.H
#pragma once
