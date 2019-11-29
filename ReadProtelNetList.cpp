//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/8/30
// Input：网表文件地址+文件名
// Output：器件信息、网络信息、器件数量、网络数量
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
	// 实现把网表文件一次全部读入到string里：
	// 把网表文件读入文件流对象
	// 把文件流对象的内容给string流
	// 把string流对象所保存的内容的拷贝赋给netListFileContent
	ifstream inFile_NetList(netListDir);
	ostringstream temp;
	temp << inFile_NetList.rdbuf();
	string netListFileContent = temp.str();

	// 定义正则表达式
	regex bracket("\\(([\\s\\S]*?)\\)\n");
	//   regex bracketEnd("\\)");
	regex squareBracket("\\[([\\s\\S]*?)\\]\n");
	//regex squareBracketEnd("\\]");
	// 结点：网络名-引脚
	//regex netInfo("^[[:alpha:]]{1,2}[[:digit:]]{1,2}-[[:digit:]]{1,2}");


	// 提取器件信息
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), squareBracket), end_it; it != end_it; ++it)
	{
		// 把 [ 后的200个char放入temp中
		//auto pos = it->suffix().str().length();
		//pos = pos > 200 ? 200 : pos;
		//string temp = it->suffix().str().substr(1, pos);
		string temp = it->str();
		temp.erase(0, 2);
		string::iterator it_str = temp.begin();

		// 如果迭代器it还没指向 ] ，就说明这个器件信息还没读完
		vector<string> temp_vector;
		int i = 0;
		while (*it_str != ']')
		{
			string temp_string;
			while (*it_str != '\n')
			{
				// 第0行是器件名，最后一行（第二行）是产品类型
				if ((i == 0) | (i == 2))
				{
					char temp_char = *it_str;
					temp_string = temp_string + temp_char;
				}
				++it_str;

			}
			// 如果是0、2行，就存下来
			if ((i == 0) | (i == 2))
			{
				temp_vector.push_back(temp_string);
			}
			// 跳出循环，此时 *it_str = '\n'，需要让迭代器指向下一个char
			++it_str;
			// 行数+1
			++i;
		}
		// 第i个器件信息已存完
		componentInfo.push_back(temp_vector);
		// 让迭代器指向temp末尾，免得它乱跑
		it_str = temp.end();
	}

	// 提取网络信息
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), bracket), end_it; it != end_it; ++it)
	{
		// 把 ( 后的500个char放入temp中
		//auto pos = it->suffix().str().length();
		//pos = pos > 500 ? 500 : pos;
		//string temp = it->suffix().str().substr(1, pos);
		string temp = it->str();
		temp.erase(0, 2);
		string::iterator it_str = temp.begin();

		// 如果迭代器it还没指向 ) ，就说明这个网络的结点还没读完
		vector<string> temp_vector;
		while (*it_str != ')')
		{
			// 第0行是网络名，后面一行是一个结点
			string temp_string;
			while (*it_str != '\n')
			{
				char temp_char = *it_str;
				temp_string = temp_string + temp_char;
				++it_str;

			}
			temp_vector.push_back(temp_string);
			// 跳出循环，此时 *it_str = '\n'，需要让迭代器指向下一个char
			++it_str;
		}
		// 第i个网络信息已存完
		netListInfo.push_back(temp_vector);
		// 让迭代器指向temp末尾，免得它乱跑
		it_str = temp.end();
	}

	// 在netListInfo每行后面添加电源与地标志
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
	// 该路径下有该文件，则打开，没有，则新建一个。每次操作均定位到文件末尾
	out_file.open(file_path_name, ofstream::out | ofstream::app);
	// COMPONENT
	// 器件名 : 器件类型
	// 
	// NETS
	// 网络名 : 节点1 节点2 节点3 
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
		// 强制类型转换
		int int_num = static_cast<int>(num);
		for (const auto& j : i)
		{
			if (ii == 0)
			{
				out_file << j << '\x20' << ":" << '\x20';
			}
			else if (ii == int_num - 2)
			{
				// 倒数第二个数存了就该换行了，所以不用输一个空格进文本
				out_file << j;
			}
			else if (ii == int_num - 1)
			{
				// 最后一个元素存的是该网络是否为地或VCC，不将这个元素写进文本
			}
			else
			{
				// 网络间用空格隔开
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