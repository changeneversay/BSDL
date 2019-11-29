#pragma once

#include "ReadNetList.h"

#include <string>
using std::vector;
using std::string;

class ReadNetListContext
{
private:
	ReadNetList* p_ReadNetList = nullptr;     // ����ָ��
public:
	string netListDir;            // �����ļ���ַ
	string netListType = "";      // �����ļ�����
	ReadNetListContext() = default;
	ReadNetListContext(const string& netlistType, const string& dir);    // ���ļ����ͣ��ļ���ַ��
	~ReadNetListContext();
	ReadNetList* getNetListp();                           // ����ָ���ض����������Ļ���ָ��
}; 
