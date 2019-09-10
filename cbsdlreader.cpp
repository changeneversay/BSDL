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
//Ӧʵ�ֶ�BSDL�ļ����ݷ����Լ���ʾ
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
	string text = SkipProcess(BsdlFileContent);//ʵ�ֶ� -- �ĺ��� �����һ��û��--ע�����string
	if (regex_search(text, result_port, keyword_port))//����ƥ��port�ؼ���
	{
		string m_temp = result_port.str();
		ProcessPort(m_temp);
	}
	if (regex_search(text, result_use, keyword_use))//����ƥ��use�ؼ���
	{
		string m_temp = result_use.str();
		ProcessPort(m_temp);
	}
	if (regex_search(text, result_constant, keyword_constant))//����ƥ��constant�ؼ���
	{
		string m_temp = result_constant.str();
		ProcessPort(m_temp);
	}
}
void CBsdlReader::ProcessPort(string temp)
{
	temp.erase(0, 6);//ɾ��"port (" �����ַ�
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
		while ((*it_str != ':') && (it_str != temp.end()))//������ð��֮ǰ�洢���ܽ���
		{   //temp_string = *next(it_str);//���˰��죬�õ�������next����ָ�����������һ��Ԫ��
			m_depend_char = *it_str;
			if ((isalpha(*it_str)) || (isdigit(*it_str)) || (m_depend_char == 95))//ȷ�����ȥ������ĸ�������ֻ��»���
			{
				temp_string = temp_string + *it_str;
				++it_str;
				m_depend_char = *it_str;
				depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//Ҫ���°�������ǰ���Ǹ��ǹ̶��ģ�����
			}
			else
			{
				if (*it_str == ',')
				{
					port_name.push_back(temp_string);
					cout << temp_string << ",";//��ȡԪ��
					++it_str;//������һ��
					i = 1;
					temp_string = "";//����ַ�������
					m_depend_char = *it_str;
					depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//Ҫ���°�������ǰ���Ǹ��ǹ̶��ģ�����
				}
				else if ((depend != 0) && (it_str != temp.end()))
				{
					break;
				}
				else
				{
					++it_str;
					m_depend_char = *it_str;
					depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//Ҫ���°�������ǰ���Ǹ��ǹ̶��ģ�����
				}
			}
		}//����ѭ��ʱit_strΪð��
		if ((i != 1) && (it_str != temp.end()))//�����ظ��洢
		{
			port_name.push_back(temp_string);
			cout << temp_string << ":";//��ȡԪ��
			++it_str;//ð����һ��
			temp_string = "";//����ַ�������
			depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//Ҫ���°�������ǰ���Ǹ��ǹ̶��ģ�����
		}
		while ((*it_str != ';') && (it_str != temp.end()))
		{
			depend = (*it_str == ')') && (!isdigit(*prev(it_str)));//Ҫ���°�������ǰ���Ǹ��ǹ̶��ģ�����
			if (*it_str == ')')
			{
				if (depend == 1)//����ǰ�治Ϊ����
				{
					j = 1;
					break;
				}
				else//    ��ǰ��Ϊ����
				{
					temp_string = temp_string + *it_str;
					++it_str;
					continue;//��ǰ�������ֵ����Ŵ�������Ȼ�������һ��ѭ��
				}
			}
			else
			{
				temp_string = temp_string + *it_str;
				++it_str;
			}
		}//��ʱit_strΪ ; ����Ϊ����ǰ�� )
		cout << temp_string <<endl;//��ȡ����
		port_name.push_back(temp_string);//���ܽ����Դ���port_name����
		temp_string = "";//����ַ�������
		if ((j != 1)&&(it_str != temp.end()))//��ʱ*it_strΪ ;
		{
			++it_str;//;��һλ
			bool pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			while(pack && (it_str != temp.end()))
			{
				++it_str;
				m_temp_char = *it_str;
				pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			}//������ѭ��ʱ*it_strΪ�ǿո�.�ǻ��з�
		}

	}
	port_info.push_back(port_name);//�ٽ�port_name�������port_info����
	it_str = temp.end();
}   

void CBsdlReader::ProcessUse(string temp)
{

}
//void CBsdlReader::ProcessConstant(string temp)
//{
//
//}








	

string CBsdlReader::SkipProcess(string BsdlFileContent)//ʵ�ֶ� -- �ĺ��� �����һ��û��--ע�����string����ʱ�������һ������ע��,����Ӱ�죩
{
	regex keyword_skip("\\-([\\s\\S]*?)(?=\n)");
	smatch result_skip;
	string::iterator it_str = BsdlFileContent.begin();
	string temp_string = "";
	string all_text = "";
	string m_save = "";
	while (it_str != BsdlFileContent.end())
	{
		while ((*it_str != '\n') && (it_str != BsdlFileContent.end()))//��ȡÿһ�е�ȫ������
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}//��ʱ*it_strΪ\n���߽���
		if (it_str != BsdlFileContent.end())
		{
			temp_string = temp_string + *it_str;//�����з���temp_string
			if (regex_search(temp_string, result_skip, keyword_skip))//��ƥ�䵽ĳ�д�����Ҫ������ע����
			{
				m_save = result_skip.str();
				temp_string = temp_string.replace(temp_string.find(m_save), m_save.length(), "");//find�����ַ�����ʼλ��ָ�룬�ڶ������س��ȣ����������Ϊ���з�
			}
			all_text = all_text + temp_string;
			temp_string = "";//����
			++it_str;//ָ��ָ����һ��
		}
	}//��ʱ�������һ������ע��,����Ӱ��
	return all_text;
}









