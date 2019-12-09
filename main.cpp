/*注意事项:
初始运行请修改main函数里的infile_bsdl路径，dir路径，
以及connenct_database.cpp文件内process_database中连接数据库的用户名与密码（为自己电脑中的用户名与密码）*/




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
#pragma comment(lib,"libmySQL.lib")  //要加不然会编译出错
using namespace std;
int main()
{
	ifstream inFile_BSDL("C:/Users/changeneversay/Desktop/边界扫描/BS文件/bsdl文件/SN74BCT8244A.bsdl");
	ostringstream temp;
	temp << inFile_BSDL.rdbuf();
	string BsdlFileContent = temp.str();
	CBsdlReader* reader = new CBsdlReader;
	reader->ProcessBsdlFile(BsdlFileContent);//执行BSDL文件解析


	string dir = "C:/Users/changeneversay/Desktop/边界扫描/BScan_Demo/BScan_Demo_2.NET";
	string type = "DefaultType";
	ReadNetListContext* p = new ReadNetListContext(type, dir);
	ReadNetList* p0 = p->getNetListp();      // 指向 解析相应网表的派生类 的基类指针
	auto netNum = p0->getNetNum();
	//cout << netNum << endl;
	auto compoNum = p0->getComponentNum();
	//cout << compoNum << endl;
	vector<vector<string>> netListInfo = p0->getNetListInfo();         // 一个网络全部信息存在一个vector<string>中
	vector<vector<string>> componentInfo = p0->getComponentInfo();       // 存放器件名称及类型
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