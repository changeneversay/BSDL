//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/8/30
// Input�������ļ���ַ+�ļ���
// Output��������Ϣ��������Ϣ��������������������
//--------------------------------------------------------------------


#include "ReadProtelNetList.h"
#include "ReadNetList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <string>

using namespace std;



ReadProtelNetList::ReadProtelNetList(string netListDir)
{
	// ʵ�ְ������ļ�һ��ȫ�����뵽string�
	// �������ļ������ļ�������
	// ���ļ�����������ݸ�string��
	// ��string����������������ݵĿ�������netListFileContent
	ifstream inFile_NetList(netListDir);
	ostringstream temp;
	temp << inFile_NetList.rdbuf();
	string netListFileContent = temp.str();

	// ����������ʽ
	regex bracket("\\(([\\s\\S]*?)\\)\n");
	//   regex bracketEnd("\\)");
	regex squareBracket("\\[([\\s\\S]*?)\\]\n");
	//regex squareBracketEnd("\\]");
	// ��㣺������-����
	//regex netInfo("^[[:alpha:]]{1,2}[[:digit:]]{1,2}-[[:digit:]]{1,2}");


	// ��ȡ������Ϣ
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), squareBracket), end_it; it != end_it; ++it)
	{
		// �� [ ���200��char����temp��
		//auto pos = it->suffix().str().length();
		//pos = pos > 200 ? 200 : pos;
		//string temp = it->suffix().str().substr(1, pos);
		string temp = it->str();
		temp.erase(0, 2);
		string::iterator it_str = temp.begin();

		// ���������it��ûָ�� ] ����˵�����������Ϣ��û����
		vector<string> temp_vector;
		int i = 0;
		while (*it_str != ']')
		{
			string temp_string;
			while (*it_str != '\n')
			{
				// ��0���������������һ�У��ڶ��У��ǲ�Ʒ����
				if ((i == 0) | (i == 2))
				{
					char temp_char = *it_str;
					temp_string = temp_string + temp_char;
				}
				++it_str;

			}
			// �����0��2�У��ʹ�����
			if ((i == 0) | (i == 2))
			{
				temp_vector.push_back(temp_string);
			}
			// ����ѭ������ʱ *it_str = '\n'����Ҫ�õ�����ָ����һ��char
			++it_str;
			// ����+1
			++i;
		}
		// ��i��������Ϣ�Ѵ���
		componentInfo.push_back(temp_vector);
		// �õ�����ָ��tempĩβ�����������
		it_str = temp.end();
	}

	// ��ȡ������Ϣ
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), bracket), end_it; it != end_it; ++it)
	{
		// �� ( ���500��char����temp��
		//auto pos = it->suffix().str().length();
		//pos = pos > 500 ? 500 : pos;
		//string temp = it->suffix().str().substr(1, pos);
		string temp = it->str();
		temp.erase(0, 2);
		string::iterator it_str = temp.begin();

		// ���������it��ûָ�� ) ����˵���������Ľ�㻹û����
		vector<string> temp_vector;
		while (*it_str != ')')
		{
			// ��0����������������һ����һ�����
			string temp_string;
			while (*it_str != '\n')
			{
				char temp_char = *it_str;
				temp_string = temp_string + temp_char;
				++it_str;

			}
			temp_vector.push_back(temp_string);
			// ����ѭ������ʱ *it_str = '\n'����Ҫ�õ�����ָ����һ��char
			++it_str;
		}
		// ��i��������Ϣ�Ѵ���
		netListInfo.push_back(temp_vector);
		// �õ�����ָ��tempĩβ�����������
		it_str = temp.end();
	}

	// ��netListInfoÿ�к�����ӵ�Դ��ر�־
	for (unsigned int i = 0; i != netListInfo.size(); ++i)
	{
		string str = netListInfo[i][0];
		if (str == "GND")
		{
			netListInfo[i].push_back("GND");
		}
		else if (str == "VCC" || str == "3.3V" || str == "5V")
		{
			netListInfo[i].push_back("VCC");
		}
		else
		{
			netListInfo[i].push_back("");
		}
	}

}


ReadProtelNetList::~ReadProtelNetList()
{
}


size_t ReadProtelNetList::getNetNum()
{
	netNum = netListInfo.size();
	return netNum;
}

size_t  ReadProtelNetList::getComponentNum()
{
	componentNum = componentInfo.size();
	return componentNum;
}

vector<vector<string>> ReadProtelNetList::getNetListInfo()
{
	return netListInfo;
}

vector<vector<string>> ReadProtelNetList::getComponentInfo()
{
	return componentInfo;
}

bool ReadProtelNetList::outfile_to_txt(const string& file_path_name)
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