//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/9/5
// Input�������ļ���ַ+�ļ���
// Output��������Ϣ��������Ϣ��������������������
//--------------------------------------------------------------------

#include "ReadCadenceProtelNetList.h"
#include "ReadNetList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <string>
#include <exception>

using namespace std;

ReadCadenceProtelNetList::ReadCadenceProtelNetList(string netListDir)
{
	// ʵ�ְ������ļ�һ��ȫ�����뵽string�
	ifstream inFile_NetList(netListDir);
	ostringstream temp;
	temp << inFile_NetList.rdbuf();
	string netListFileContent = temp.str();

	// ����������ʽ
	// VS����ʶ����ʽ?<=����ʲô�취�أ�����ĺܷ�����ֱ�����������ҵĹ�����
	regex bracket("\\(([\\s\\S]+?)(?=\\))");                                       // һ��С����
	regex squareBracket("\\[([\\s\\S]+?)(?=\\])");                                 // һ��������
	regex keywordDESIGNATOR("DESIGNATOR\n([-\\w\\+\\.]+?)(?=\n)");                 // DESIGNATOR�ͺ����Ԫ������
	regex keywordPARTTYPE("PARTTYPE\n([\\s\\S]+?)(?=\n)");                         // PARTTYPE�ͺ����Ԫ������Ϣ
	regex begOfALine("\n[^\\(][-\\w\\+\\.]+(?=\\s)");                              // ÿ�п�ͷ������ֱ�������ո�
	regex VCC_GND("(\\d+\\.*\\d*V)|VCC|GND");                                      // ��Դ�͵ر�־����.��V �� VCC ��GND

	//regex keywordDESIGNATOR("(?<=DESIGNATOR\n)([\\s\\S]*?)(?=\n)");              // ������
	//regex keywordPARTTYPE("(?<=PARTTYPE\n)([\\s\\S]*?)(?=\n)");                  // ������Ϣ
	//regex begOfALine("(?<=\n)\S*");                                              // ÿ�п�ͷ������ֱ�������ո�
	//regex netInfo("^[[:alpha:]]{1,2}[[:digit:]]{1,2}-[[:digit:]]{1,2}");         // �ڵ㣺������-����

	// ��ȡ������Ϣ
	// ƥ��ÿһ�Է������������
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), squareBracket), end_it; it != end_it; ++it)
	{
		// ȡ��ƥ�䵽��ĳһ���������ڵ�����
		string str_squareBracket = it->str();
		vector<string> tempVector;
		smatch result;
		// ƥ�������� �ؼ���DESIGNATOR
		if (regex_search(str_squareBracket, result, keywordDESIGNATOR))
		{
			string str_DESIGNATOR = result.str();
			// ��ǰ��Ĺؼ���ɾ�����ӻ��з�һ��11���ַ���
			str_DESIGNATOR.erase(0, 11);
			// ���������Ž���ʱһά����
			tempVector.push_back(str_DESIGNATOR);
		}
		// ƥ��Ԫ������Ϣ �ؼ���PARTTYPE
		if (regex_search(str_squareBracket, result, keywordPARTTYPE))
		{
			string str_PARTTYPE = result.str();
			// ��ǰ��Ĺؼ���ɾ�����ӻ��з�һ��9���ַ���
			str_PARTTYPE.erase(0, 9);
			// ���������Ž���ʱһά����
			tempVector.push_back(str_PARTTYPE);
		}
		// һ��Ԫ�����������Ϣ������
		componentInfo.push_back(tempVector);
	}

	// ��ȡ������Ϣ
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), bracket), end_it; it != end_it; ++it)
	{
		// ȡ��ƥ�䵽��ĳһ��С�����ڵ�����
		string str_bracket = it->str();
		vector<string> tempVector;
		// �жϵ�0���Ƿ�Ϊ��Դ��أ���һ����־����¼�ǵ�ǰ�ڼ���
		int i = 0;
		// ��һ����ʱ�ַ��������Դ��ر�־
		string tempGndOrVcc = "";
		// ��һ��С�����ڵ�������Ϣһ��һ�д��һά����
		for (sregex_iterator it1(str_bracket.begin(), str_bracket.end(), begOfALine), end_it1; it1 != end_it1; ++it1)
		{
			// ȡ��С�����ڵ�ȡ��ÿ��
			string str_net = it1->str();
			// ����ǰ��Ļ��з�ɾ�����ӻ��з�һ��1���ַ���
			str_net.erase(0, 1);
			tempVector.push_back(str_net);
			// ����ǵ�0�У��ж��������Ƿ��� ��.��V �� VCC ��GND
			if (!i)
			{
				// �ж��Ƿ��Դ���
				smatch result;
				if (regex_search(str_net, result, VCC_GND))
				{
					// �� ��Դ���
					string str_VCC_GND = result.str();
					if (!str_VCC_GND.compare("GND"))
					{
						// �� �� ���� ��Դ��ر�־ ="GND"
						tempGndOrVcc = "GND";
					}
					else
					{
						// ���� �أ��� ��Դ��ر�־ ="VCC"
						tempGndOrVcc = "VCC";
					}
				}
				else
				{
					// ���ǵ�Դ���
					tempGndOrVcc = "";
				}
			}
			// ����+1
			++i;
		}
		// ��һά������ĩβ���� ��Դ��ر�־
		tempVector.push_back(tempGndOrVcc);
		// һ���������Ϣ�����ˣ������������Ϣ�����ά����
		netListInfo.push_back(tempVector);
	}
}


ReadCadenceProtelNetList::~ReadCadenceProtelNetList()
{
}

size_t ReadCadenceProtelNetList::getNetNum()
{
	netNum = netListInfo.size();
	return netNum;
}
size_t ReadCadenceProtelNetList::getComponentNum()
{
	componentNum = componentInfo.size();
	return componentNum;

}
vector<vector<string>> ReadCadenceProtelNetList::getNetListInfo()
{
	return netListInfo;
}
vector<vector<string>> ReadCadenceProtelNetList::getComponentInfo()
{
	return componentInfo;
}
bool  ReadCadenceProtelNetList::outfile_to_txt(const string& file_path_name)
{
	ofstream out_file;
	// ��·�����и��ļ�����򿪣�û�У����½�һ����ÿ�β�������λ���ļ�ĩβ
	out_file.open(file_path_name, ofstream::out | ofstream::app);
	// COMPONENT
	// ������ : ��������
	// 
	// NETS
	// ������ : �ڵ�1 �ڵ�2 �ڵ�3 
	// 
	// END
	out_file << "COMPONENT" << '\n';
	for (const auto& i : componentInfo)
	{
		int ii = 0;
		for (const auto& j : i)
		{
			out_file << j;
			if (ii == 0)
			{
				out_file << '\x20' << ":" << '\x20';
			}
			++ii;
		}
		out_file << '\n';
	}
	out_file << '\n' << "NETS" << '\n';
	for (const auto& i : netListInfo)
	{
		int ii = 0;
		auto num = i.size();
		// ǿ������ת��
		int int_num = static_cast<int>(num);
		for (const auto& j : i)
		{
			if (ii == 0)
			{
				out_file << j << '\x20' << ":" << '\x20';
			}
			else if (ii == int_num - 2)
			{
				// �����ڶ��������˾͸û����ˣ����Բ�����һ���ո���ı�
				out_file << j;
			}
			else if (ii == int_num - 1)
			{
				// ���һ��Ԫ�ش���Ǹ������Ƿ�Ϊ�ػ�VCC���������Ԫ��д���ı�
			}
			else
			{
				// ������ÿո����
				out_file << j << '\x20';
			}
			++ii;
		}
		out_file << '\n';
	}
	out_file << '\n' << "END";
	out_file.close();
	auto status = out_file.fail();
	return status;
}