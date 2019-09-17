// ConsoleApplication12.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include"cbsdlreader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <string>
#include <list>
#include<cctype>

using namespace std;

int main()
{
	ifstream inFile_BSDL("C:/Users/changeneversay/Desktop/边界扫描/BS文件/bsdl文件/EPM3032AT44.bsdl");
	ostringstream temp;
	temp << inFile_BSDL.rdbuf();
	string BsdlFileContent = temp.str();
	CBsdlReader* d =new CBsdlReader;
	d->ProcessBsdlFile(BsdlFileContent);
	delete d;
	d = nullptr;
	system("pause");
	return 0;
}








