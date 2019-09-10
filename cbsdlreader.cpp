#include"cbsdlreader.h"
#include<vector>
#include<regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include<cstdio>
#include<iterator>
#include<cctype>
//应实现对BSDL文件数据分类以及显示
using namespace std;

void CBsdlReader::ProcessBsdlFile(string BsdlFileContent)
{
	regex keyword_port("port\\s\\([\\s\\S]*([\\W]\\);){1}");
	regex keyword_constant("constant([\\s\\S]*?);");
	regex keyword_attribute("^attribute");
	regex keyword_use("use\\s([\\s\\S]*?)([\\w];)");
	regex keyword_end("^end");
	regex keyword_generic("^generic");
	smatch result_port;
	smatch result_constant;
	smatch result_use;
	smatch result_entity;
	smatch result_attribute;
	smatch result_genertic;
	string text = SkipProcess(BsdlFileContent);//实现对 -- 的忽略 并输出一个没有--注释类的string
	if (regex_search(text, result_port, keyword_port))//遍历匹配port关键字
	{
		string m_temp = result_port.str();
		ProcessPort(m_temp);
	}
	if (regex_search(text, result_use, keyword_use))//遍历匹配use关键字
	{
		string m_temp = result_use.str();
		ProcessPort(m_temp);
	}
	if (regex_search(text, result_constant, keyword_constant))//遍历匹配constant关键字
	{
		string m_temp = result_constant.str();
		ProcessPort(m_temp);
	}
}
void CBsdlReader::ProcessPort(string temp)
{
	temp.erase(0, 6);//删除"port (" 六个字符
	string::iterator it_str = temp.begin();
	string temp_string = "";
	//char temp_string_end;
	char m_temp_char = NULL;
	char m_depend_char = NULL;
	int i = 0;
	int j = 0;
	bool depend = (*it_str == ')') && (!isdigit(*prev(it_str)));
	/*bool end_bool = (*prev(it_str) = ' ') || (*prev(it_str) = '\n');*/
	while ((depend == 0) && (it_str != temp.end()))
	{
		while ((*it_str != ':') && (it_str != temp.end()))//在遇见冒号之前存储各管脚名
		{   //temp_string = *next(it_str);//找了半天，用迭代器的next函数指向迭代器的下一个元素
			m_depend_char = *it_str;
			if ((isalpha(*it_str)) || (isdigit(*it_str)) || (m_depend_char == 95))//确定存进去的是字母或者数字或下划线
			{
				temp_string = temp_string + *it_str;
				++it_str;
				m_depend_char = *it_str;
				depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//要更新啊！！！前面那个是固定的！！！
			}
			else
			{
				if (*it_str == ',')
				{
					port_name.push_back(temp_string);
					cout << temp_string << ",";//读取元件
					++it_str;//逗号下一个
					i = 1;
					temp_string = "";//清除字符串数据
					m_depend_char = *it_str;
					depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//要更新啊！！！前面那个是固定的！！！
				}
				else if ((depend != 0) && (it_str != temp.end()))
				{
					break;
				}
				else
				{
					++it_str;
					m_depend_char = *it_str;
					depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//要更新啊！！！前面那个是固定的！！！
				}
			}
		}//跳出循环时it_str为冒号
		if ((i != 1) && (it_str != temp.end()))//避免重复存储
		{
			port_name.push_back(temp_string);
			cout << temp_string << ":";//读取元件
			++it_str;//冒号下一个
			temp_string = "";//清除字符串数据
			depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//要更新啊！！！前面那个是固定的！！！
		}
		while ((*it_str != ';') && (it_str != temp.end()))
		{
			depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//要更新啊！！！前面那个是固定的！！！
			if (*it_str == ')')
			{
				if (depend == 1)//括号前面不为数字
				{
					j = 1;
					break;
				}
				else//    ）前面为数字
				{
					temp_string = temp_string + *it_str;
					++it_str;
					continue;//将前面是数字的括号存起来，然后进入下一个循环
				}
			}
			else
			{
				temp_string = temp_string + *it_str;
				++it_str;
			}
		}//此时it_str为 ; 或者为结束前的 )
		cout << temp_string <<endl;//读取属性
		port_name.push_back(temp_string);//将管脚属性存入port_name里面
		temp_string = "";//清除字符串数据
		if ((j != 1)&&(it_str != temp.end()))//此时*it_str为 ;
		{
			++it_str;//;下一位
			bool pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			while(pack && (it_str != temp.end()))
			{
				++it_str;
				m_temp_char = *it_str;
				pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			}//跳出此循环时*it_str为非空格.非换行符
		}

	}
	port_info.push_back(port_name);//再将port_name打包存入port_info里面
	it_str = temp.end();
}   

void CBsdlReader::ProcessUse(string temp)
{

}
//void CBsdlReader::ProcessConstant(string temp)
//{
//
//}








	

string CBsdlReader::SkipProcess(string BsdlFileContent)//实现对 -- 的忽略 并输出一个没有--注释类的string（此时可能最后一行仍有注释,但不影响）
{
	regex keyword_skip("\\-([\\s\\S]*?)(?=\n)");
	smatch result_skip;
	string::iterator it_str = BsdlFileContent.begin();
	string temp_string = "";
	string all_text = "";
	string m_save = "";
	while (it_str != BsdlFileContent.end())
	{
		while ((*it_str != '\n') && (it_str != BsdlFileContent.end()))//提取每一行的全部数据
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}//此时*it_str为\n或者结束
		if (it_str != BsdlFileContent.end())
		{
			temp_string = temp_string + *it_str;//将换行符给temp_string
			if (regex_search(temp_string, result_skip, keyword_skip))//若匹配到某行存在需要跳过的注释类
			{
				m_save = result_skip.str();
				temp_string = temp_string.replace(temp_string.find(m_save), m_save.length(), "");//find返回字符串初始位置指针，第二个返回长度，第三个替代为换行符
			}
			all_text = all_text + temp_string;
			temp_string = "";//重置
			++it_str;//指针指向下一行
		}
	}//此时可能最后一行仍有注释,但不影响
	return all_text;
}









