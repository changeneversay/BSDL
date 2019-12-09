/*ע������:
��ʼ�������޸�main�������infile_bsdl·����dir·����
�Լ�connenct_database.cpp�ļ���process_database���������ݿ���û��������루Ϊ�Լ������е��û��������룩*/




#include"cbsdlreader.h"
#include "connect_database.h"
#include "ReadProtelNetList.h"
#include "ReadCadenceProtelNetList.h"
#include "ReadNetListContext.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <string>
#include <list>
#include<cctype>
#include <winsock.h>
#include <mysql.h>
#pragma comment(lib,"libmySQL.lib")  //Ҫ�Ӳ�Ȼ��������
using namespace std;
int main()
{
	ifstream inFile_BSDL("C:/Users/changeneversay/Desktop/�߽�ɨ��/BS�ļ�/bsdl�ļ�/SN74BCT8244A.bsdl");
	ostringstream temp;
	temp << inFile_BSDL.rdbuf();
	string BsdlFileContent = temp.str();
	CBsdlReader* reader = new CBsdlReader;
	reader->ProcessBsdlFile(BsdlFileContent);//ִ��BSDL�ļ�����


	string dir = "C:/Users/changeneversay/Desktop/�߽�ɨ��/BScan_Demo/BScan_Demo_2.NET";
	string type = "DefaultType";
	ReadNetListContext* p = new ReadNetListContext(type, dir);
	ReadNetList* p0 = p->getNetListp();      // ָ�� ������Ӧ����������� �Ļ���ָ��
	auto netNum = p0->getNetNum();
	//cout << netNum << endl;
	auto compoNum = p0->getComponentNum();
	//cout << compoNum << endl;
	vector<vector<string>> netListInfo = p0->getNetListInfo();         // һ������ȫ����Ϣ����һ��vector<string>��
	vector<vector<string>> componentInfo = p0->getComponentInfo();       // ����������Ƽ�����
	auto c = reader->Get_BRdata();
	auto a = reader->Get_portdata();
	auto b = reader->Get_constantdata();
	auto end_info = reader->Get_Enddata();
	MyDataBase m;
	string y = m.Process_database(a,b,c,end_info,netListInfo,componentInfo);
	m.Process_Chain(y);
	delete reader;
	reader = nullptr;
	delete p;
	p = nullptr;
	p0 = nullptr;
	system("pause");
	return 0;
}