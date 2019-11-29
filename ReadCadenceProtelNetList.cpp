//--------------------------------------------------------------------
// Code by WangYuan; Date:2019/9/5
// Input：网表文件地址+文件名
// Output：器件信息、网络信息、器件数量、网络数量
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
	// 实现把网表文件一次全部读入到string里：
	ifstream inFile_NetList(netListDir);
	ostringstream temp;
	temp << inFile_NetList.rdbuf();
	string netListFileContent = temp.str();

	// 定义正则表达式
	// VS不能识别表达式?<=，有什么办法呢！！真的很烦，简直就是在增加我的工作量
	regex bracket("\\(([\\s\\S]+?)(?=\\))");                                       // 一对小括号
	regex squareBracket("\\[([\\s\\S]+?)(?=\\])");                                 // 一对中括号
	regex keywordDESIGNATOR("DESIGNATOR\n([-\\w\\+\\.]+?)(?=\n)");                 // DESIGNATOR和后面的元器件名
	regex keywordPARTTYPE("PARTTYPE\n([\\s\\S]+?)(?=\n)");                         // PARTTYPE和后面的元器件信息
	regex begOfALine("\n[^\\(][-\\w\\+\\.]+(?=\\s)");                              // 每行开头的内容直到遇到空格
	regex VCC_GND("(\\d+\\.*\\d*V)|VCC|GND");                                      // 电源和地标志，几.几V 或 VCC 或GND

	//regex keywordDESIGNATOR("(?<=DESIGNATOR\n)([\\s\\S]*?)(?=\n)");              // 器件名
	//regex keywordPARTTYPE("(?<=PARTTYPE\n)([\\s\\S]*?)(?=\n)");                  // 器件信息
	//regex begOfALine("(?<=\n)\S*");                                              // 每行开头的内容直到遇到空格
	//regex netInfo("^[[:alpha:]]{1,2}[[:digit:]]{1,2}-[[:digit:]]{1,2}");         // 节点：网络名-引脚

	// 提取器件信息
	// 匹配每一对方括号里的内容
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), squareBracket), end_it; it != end_it; ++it)
	{
		// 取出匹配到的某一个方括号内的内容
		string str_squareBracket = it->str();
		vector<string> tempVector;
		smatch result;
		// 匹配器件名 关键字DESIGNATOR
		if (regex_search(str_squareBracket, result, keywordDESIGNATOR))
		{
			string str_DESIGNATOR = result.str();
			// 把前面的关键字删掉（加换行符一共11个字符）
			str_DESIGNATOR.erase(0, 11);
			// 把器件名放进临时一维容器
			tempVector.push_back(str_DESIGNATOR);
		}
		// 匹配元器件信息 关键字PARTTYPE
		if (regex_search(str_squareBracket, result, keywordPARTTYPE))
		{
			string str_PARTTYPE = result.str();
			// 把前面的关键字删掉（加换行符一共9个字符）
			str_PARTTYPE.erase(0, 9);
			// 把器件名放进临时一维容器
			tempVector.push_back(str_PARTTYPE);
		}
		// 一个元器件的相关信息存完了
		componentInfo.push_back(tempVector);
	}

	// 提取网络信息
	for (sregex_iterator it(netListFileContent.begin(), netListFileContent.end(), bracket), end_it; it != end_it; ++it)
	{
		// 取出匹配到的某一个小括号内的内容
		string str_bracket = it->str();
		vector<string> tempVector;
		// 判断第0行是否为电源或地，用一个标志来记录是当前第几行
		int i = 0;
		// 用一个临时字符串来存电源与地标志
		string tempGndOrVcc = "";
		// 把一个小括号内的网络信息一行一行存进一维容器
		for (sregex_iterator it1(str_bracket.begin(), str_bracket.end(), begOfALine), end_it1; it1 != end_it1; ++it1)
		{
			// 取出小括号内的取出每行
			string str_net = it1->str();
			// 把最前面的换行符删掉（加换行符一共1个字符）
			str_net.erase(0, 1);
			tempVector.push_back(str_net);
			// 如果是第0行，判断网络名是否是 几.几V 或 VCC 或GND
			if (!i)
			{
				// 判断是否电源或地
				smatch result;
				if (regex_search(str_net, result, VCC_GND))
				{
					// 是 电源或地
					string str_VCC_GND = result.str();
					if (!str_VCC_GND.compare("GND"))
					{
						// 是 地 ，令 电源与地标志 ="GND"
						tempGndOrVcc = "GND";
					}
					else
					{
						// 不是 地，令 电源与地标志 ="VCC"
						tempGndOrVcc = "VCC";
					}
				}
				else
				{
					// 不是电源与地
					tempGndOrVcc = "";
				}
			}
			// 行数+1
			++i;
		}
		// 在一维容器的末尾存入 电源与地标志
		tempVector.push_back(tempGndOrVcc);
		// 一个网络的信息存完了，把这个网络信息存进二维容器
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