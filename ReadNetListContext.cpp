//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/9/9
// Input���������͡������ļ���ַ+�ļ���
// Output��ָ�� ������Ӧ����������� �Ļ���ָ��
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
	netListType = netlistType;  // �û�ѡ�����������
	netListDir = dir;           // �û�ѡ��������ַ

	// ���ж��û�ѡ�����������
	// Ϊʲô����switch����أ���Ϊstringû����const char�Ƚϣ�ǿ������ת����ʮ��Σ�գ�ʹ��if�ж��������Խ�����ʽ����ת��
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
		// ���ַ����������û��DESIGNATOR��PARTTYPE�ؼ��֣��еĻ�������Cadence������ɵ�protel��.NET����û�еĻ�������Protel������ɵ�protel��.NET����
		auto pos1 = netListFileContent.find("DESIGNATOR");
		auto pos2 = netListFileContent.find("PARTTYPE");
		// ������Զ��ж���������
		if ((pos1 == netListFileContent.npos) && (pos2 == netListFileContent.npos))
		{
			// û�еĻ�������Protel������ɵ�protel��.NET����
			p_ReadNetList = new ReadProtelNetList(netListDir);
		}
		else
		{
			// �еĻ�������Cadence������ɵ�protel��.NET����
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