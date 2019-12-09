#include"cbsdlreader.h"/*解析BSDL文件的主程序,包含各种关键字的解析,并分类保存在不同的vector里面*/
#include<vector>
#include<regex>
#include<iostream>
#include <fstream>
#include <sstream>
#include<string>
#include<cstdio>
#include<iterator>
#include<cctype>
//应实现对BSDL文件数据分类以及显示vector<std::string>attribute_INSTRUCTION_OPCODE_info
using namespace std;
//void  readthefile();
void Process_COMPONENT_CONFORMANCE(regex keyword_COMPONENT_CONFORMANCE, smatch result_COMPONENT_CONFORMANCE, string temp, vector<string>& attribute_COMPONENT_CONFORMANCE);
void ProcessPIN_MAP(regex keyword_PIN_MAP, smatch result_PIN_MAP, string temp, vector<string>& attribute_PIN_MAP);
void ProcessTAP(regex keyword_tap, smatch result_tap, string temp, vector<string>& attribute_tap);
void ProcessTCK(regex keyword_tap, smatch result_tap, string temp, vector<string>& attribute_tap);
void ProcessQuo(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo);
void Process_INSTRUCTION_CAPTURE(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo);
void Process_IDCODE_REGISTER(regex keyword_IDCODE_REGISTER, smatch result_IDCODE_REGISTER, string temp, vector<string>& attribute_IDCODE_REGISTER);
void Process_USERCODE_REGISTER(regex keyword_USERCODE_REGISTER, smatch result_USERCODE_REGISTER, string temp, vector<string>& attribute_USERCODE_REGISTER);
void ProcessREGISTER_ACCESS(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo);
void Process_BOUNDARY_REGISTER(regex keyword_quo, smatch result_quo, string temp, vector<vector<string>>& attribute_BR_info);
void Process_INSTRUCTION_OPCODE(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo, vector<vector<string>>& attribute_BR_info);
void CBsdlReader::ProcessBsdlFile(string BsdlFileContent)//提供外部接口    并读取存储函数
{
	string m_attribute = "";
	regex keyword_port("port\\s\\([\\s\\S]*([\\W]\\);){1}");
	regex keyword_constant("constant([\\s\\S]*?);");
	regex keyword_attribute("attribute([\\s\\S]*?);");
	regex keyword_use("use\\s([\\s\\S]*?)([\\w];)");
	regex keyword_end("end([\\s\\S]*?);");
	regex keyword_generic("generic([\\s\\S]*?);");
	smatch result_port;
	smatch result_constant;
	smatch result_use;
	smatch result_entity;
	smatch result_generic;
	smatch result_end;
	string text = SkipProcess(BsdlFileContent);//实现对 -- 的忽略 并输出一个没有--注释类的string
	//if (regex_search(text, result_generic, keyword_generic))//遍历匹配generic关键字
	//{
	//	string m_port = result_generic.str();
	//	ProcessGeneric(m_port);
	//}
	if (regex_search(text, result_port, keyword_port))//遍历匹配port关键字
	{
		string m_port = result_port.str();
		ProcessPort(m_port);
	}
	//if (regex_search(text, result_use, keyword_use))//遍历匹配use关键字
	//{
	//	string m_temp = result_use.str();
	//	ProcessUse(m_temp);
	//}
	for (sregex_iterator it(text.begin(), text.end(), keyword_attribute), end_it; it != end_it; ++it)//匹配attribute关键字
	{
		m_attribute = m_attribute + it->str();
	}
	ProcessAttribute(m_attribute);
	if (regex_search(text, result_constant, keyword_constant))//遍历匹配constant关键字
	{
		string m_temp = result_constant.str();
		ProcessConstant(m_temp);
	}
	if (regex_search(text, result_end, keyword_end))
	{
		string m_temp = result_end.str();
		ProcessEnd(m_temp);
	}
	
	//view_BSDL();
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
		vector<string> port_name;//前面为管脚名   最后一个为管脚属性
		while ((*it_str != ':') && (it_str != temp.end()))//在遇见冒号之前存储各管脚名
		{
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
					++it_str;//逗号下一个
				/*	i = 1;*/
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
		//if ((i != 1) && (it_str != temp.end()))//避免重复存储
		if(it_str != temp.end())
		{
			port_name.push_back(temp_string);
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
		port_name.push_back(temp_string);//将管脚属性存入port_name里面
		temp_string = "";//清除字符串数据
		if ((j != 1) && (it_str != temp.end()))//此时*it_str为 ;
		{
			++it_str;//;下一位
			bool pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			while (pack && (it_str != temp.end()))
			{
				++it_str;
				m_temp_char = *it_str;
				pack = (m_temp_char == ' ') || (m_temp_char == '\n');
			}//跳出此循环时*it_str为非空格.非换行符
		}
	    port_info.push_back(port_name);//再将port_name打包存入port_info里面
	}
	//port_info.push_back(port_name);//再将port_name打包存入port_info里面
	it_str = temp.end();
}
void CBsdlReader::ProcessConstant(const string& temp)
{
	auto it_str = temp.begin();
	int i = 0;
	int j = 0;
	string temp_string = "";
	while (*it_str != '"')
	{
		++it_str;
	}
	++it_str;//引号前面的不要了
	while (*it_str != ';')
	{
		/*vector<string>constant_name;*/
		while ((*it_str != '\n') && (*it_str != ';'))
		{
			i = 0;
			vector<string>constant_name;//第一为管脚名，第二为物理管脚序号
			while ((*it_str != ',') && (*it_str != ';'))
			{
				//while ((*it_str != ':') && (*it_str != '\n') && (*it_str != ';') && (j == 0))
				while ((*it_str != ':') && (*it_str != '\n') && (*it_str != ';'))
				{
					if ((*it_str == '_') || (isalpha(*it_str)) || (isalnum(*it_str)))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					else if ((*it_str == ',') || (*it_str == ';'))
					{
						break;
					}
					else
					{
						++it_str;
					}
				}//出来为: 或\n 或;
				if (*it_str == ';')
				{
					break;
				}
				if (*it_str == ':')
				{
					++it_str;
					constant_name.push_back(temp_string);
					temp_string = "";
				}
				//if ((isalnum(*it_str)) && (j == 1))
				if (isalnum(*it_str))
				{
					//j = 0;
					while (isalnum(*it_str))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					constant_name.push_back(temp_string);
					temp_string = "";
				}
				if (*it_str == '(')
				{
					++it_str;
					while (*it_str != ')')
					{
						if (isalnum(*it_str))
						{
							while (isalnum(*it_str))
							{
								temp_string = temp_string + *it_str;
								++it_str;
							}
							constant_name.push_back(temp_string);
							temp_string = "";
						}
						else
						{
							++it_str;
						}
					}
				}
				if ((*it_str == ',') || (*it_str == '\n'))
				{
					break;
				}
				else
				{
					++it_str;
				}
			}//出来为逗号或者\n或者)
			if (*it_str == ';')
			{
				constant_info.push_back(constant_name);
				break;
			}
			if ((*it_str == '\n') && (i == 0))
			{
				i = 1;
				constant_info.push_back(constant_name);
				temp_string = "";
				break;
			}
			if ((*it_str != '\n') && (i == 0))
			{
				i = 1;
				constant_info.push_back(constant_name);
				temp_string = "";
				++it_str;
			}
			else
			{
				temp_string = "";
				++it_str;
			}

		}
		if (*it_str != ';')
		{
			++it_str;
		}
	}
	it_str = temp.end();
	temp_string = "";
}

void CBsdlReader::ProcessAttribute(string temp)
{
	const regex keyword_COMPONENT_CONFORMANCE("COMPONENT_CONFORMANCE([\\s\\S]*?);");
	const regex keyword_PIN_MAP("PIN_MAP([\\s\\S]*?);");
	const regex keyword_TDI("TAP_SCAN_IN([\\s\\S]*?);");
	const regex keyword_TDO("TAP_SCAN_MODE([\\s\\S]*?);");
	const regex keyword_TMS("TAP_SCAN_OUT([\\s\\S]*?);");
	const regex keyword_TCK("TAP_SCAN_CLOCK([\\s\\S]*?);");
	const regex keyword_INSTRUCTION_LENGTH("INSTRUCTION_LENGTH([\\s\\S]*?);");
	const regex keyword_INSTRUCTION_OPCODE("INSTRUCTION_OPCODE([\\s\\S]*?);");
	const regex keyword_INSTRUCTION_CAPTURE("INSTRUCTION_CAPTURE([\\s\\S]*?);");
	const regex keyword_INSTRUCTION_DISABLE("INSTRUCTION_DISABLE([\\s\\S]*?);");
	const regex keyword_INSTRUCTION_GUARD("INSTRUCTION_GUARD([\\s\\S]*?);");
	const regex keyword_REGISTER_ACCESS("REGISTER_ACCESS([\\s\\S]*?);");
	const regex keyword_BOUNDARY_LENGTH("BOUNDARY_LENGTH([\\s\\S]*?);");
	const regex keyword_BOUNDARY_REGISTER("BOUNDARY_REGISTER([\\s\\S]*?);");
	const regex keyword_IDCODE_REGISTER("IDCODE_REGISTER([\\s\\S]*?);");
	const regex keyword_USERCODE_REGISTER("USERCODE_REGISTER([\\s\\S]*?);");
	const smatch result_IDCODE_REGISTER;
	const smatch result_USERCODE_REGISTER;
	const smatch result_PIN_MAP;
	const smatch result_COMPONENT_CONFORMANCE;
	const smatch result_TDI;
	const smatch result_TDO;
	const smatch result_TMS;
	const smatch result_TCK;
	const smatch result_INSTRUCTION_LENGTH;
	const smatch result_INSTRUCTION_OPCODE;
	const smatch result_INSTRUCTION_CAPTURE;
	const smatch result_INSTRUCTION_DISABLE;
	const smatch result_INSTRUCTION_GUARD;
	const smatch result_REGISTER_ACCESS;
	const smatch result_BOUNDARY_LENGTH;
	const smatch result_BOUNDARY_REGISTER;
	//Process_COMPONENT_CONFORMANCE(keyword_COMPONENT_CONFORMANCE, result_COMPONENT_CONFORMANCE, temp, attribute_COMPONENT_CONFORMANCE);
	//ProcessPIN_MAP(keyword_PIN_MAP, result_PIN_MAP, temp, attribute_PIN_MAP);
	//ProcessTAP(keyword_TDI, result_TDI, temp, attribute_TDI);
	//ProcessTAP(keyword_TMS, result_TMS, temp, attribute_TMS);
	//ProcessTAP(keyword_TDO, result_TDO, temp, attribute_TDO);
	//ProcessTCK(keyword_TCK, result_TCK, temp, attribute_TCK);
	//ProcessTAP(keyword_INSTRUCTION_LENGTH, result_INSTRUCTION_LENGTH, temp, attribute_INSTRUCTION_LENGTH);//INSTRUCTION_LENGTH关键字和TAP相似  可以直接用TAP函数
	//Process_INSTRUCTION_OPCODE(keyword_INSTRUCTION_OPCODE, result_INSTRUCTION_OPCODE, temp, attribute_INSTRUCTION_OPCODE, attribute_INSTRUCTION_OPCODE_info);
	//Process_INSTRUCTION_CAPTURE(keyword_INSTRUCTION_CAPTURE, result_INSTRUCTION_CAPTURE, temp, attribute_INSTRUCTION_CAPTURE);
	//Process_IDCODE_REGISTER(keyword_IDCODE_REGISTER, result_IDCODE_REGISTER, temp, attribute_IDCODE_REGISTER);
	//Process_USERCODE_REGISTER(keyword_USERCODE_REGISTER, result_USERCODE_REGISTER, temp, attribute_USERCODE_REGISTER);
	//ProcessREGISTER_ACCESS(keyword_REGISTER_ACCESS, result_REGISTER_ACCESS, temp, attribute_REGISTER_ACCESS);
	//ProcessTAP(keyword_BOUNDARY_LENGTH, result_BOUNDARY_LENGTH, temp, attribute_BOUNDARY_LENGTH);
	Process_BOUNDARY_REGISTER(keyword_BOUNDARY_REGISTER, result_BOUNDARY_REGISTER, temp,attribute_BR_info);
}
void CBsdlReader::ProcessEnd(string temp)
{
	temp.erase(0, 4);
	string::iterator it_str = temp.begin();
	string temp_string = "";
	while (*it_str != ';')
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	end_info.push_back(temp_string);
	temp_string = "";
	it_str = temp.end();
}
string CBsdlReader::SkipProcess(string& BsdlFileContent)//实现对 -- 的忽略 并输出一个没有--注释类的string（此时可能最后一行仍有注释,但不影响）
{
	regex keyword_skip("\\-([\\s\\S]*?)(?=\n)");//匹配所有的注释内容
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
void ProcessTAP(regex keyword_tap, smatch result_tap, string temp, vector<string>& attribute_tap)
{
	if (regex_search(temp, result_tap, keyword_tap))//TDI
	{
		string m_tap = result_tap.str();
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != ':')
		{
			++it_str;
		}
		++it_str;
		while (*it_str != ';')
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		temp_string.erase(0, 11);
		attribute_tap.push_back(temp_string);
		it_str = m_tap.end();
		temp_string = "";
	}
}
void ProcessTCK(regex keyword_tap, smatch result_tap, string temp, vector<string>& attribute_tap)
{
	if (regex_search(temp, result_tap, keyword_tap))
	{
		string m_tap = result_tap.str();
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '(')
		{
			++it_str;
		}
		++it_str;
		while (*it_str != ',')
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_tap.push_back(temp_string);//存入括号第一个信息
		++it_str;
		temp_string = "";
		while (*it_str != ')')
		{
			if (isalpha(*it_str))
			{
				temp_string = temp_string + *it_str;
				++it_str;
			}
			else
			{
				++it_str;
			}
		}
		attribute_tap.push_back(temp_string);//存入括号第二个信息
		it_str = m_tap.end();
		temp_string = "";
	}
}
void ProcessQuo(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo)
{
	if (regex_search(temp, result_quo, keyword_quo))
	{
		string m_tap = result_quo.str();
		int i = 0;
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//引号前面的不要了
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			while (*it_str != '\n')
			{
				if (isupper(*it_str))
				{
					while ((isupper(*it_str)) && (*it_str != ';'))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);//存入指令名称
					temp_string = "";
				}
				if (*it_str == '(')
				{
					++it_str;
					while ((isdigit(*it_str)) && (it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);//存入指令代码
					temp_string = "";
				}
				if (*it_str == ';')
				{
					i = 1;
					break;

				}
				++it_str;
			}
			if (i != 1)
			{
				++it_str;
			}
		}
		it_str = temp.end();
	}
}
void Process_INSTRUCTION_CAPTURE(regex keyword_INSTRUCTION_CAPTURE, smatch result_INSTRUCTION_CAPTURE, string temp, vector<string>& attribute_INSTRUCTION_CAPTURE)
{
	if (regex_search(temp, result_INSTRUCTION_CAPTURE, keyword_INSTRUCTION_CAPTURE))
	{
		string m_tap = result_INSTRUCTION_CAPTURE.str();
		string temp_string = "";

		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//引号前面的不要了
		while ((isdigit(*it_str)) && (it_str != m_tap.end()))
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_INSTRUCTION_CAPTURE.push_back(temp_string);//直接将数字存入
		temp_string = "";
		it_str = temp.end();
	}
}
void Process_IDCODE_REGISTER(regex keyword_IDCODE_REGISTER, smatch result_IDCODE_REGISTER, string temp, vector<string>& attribute_IDCODE_REGISTER)
{
	if (regex_search(temp, result_IDCODE_REGISTER, keyword_IDCODE_REGISTER))
	{
		string m_tap = result_IDCODE_REGISTER.str();
		int i = 0;
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//引号前面的不要了
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			while (*it_str != '\n')
			{
				if ((isdigit(*it_str)) && (it_str != m_tap.end()))
				{
					while ((isdigit(*it_str)) && (it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_IDCODE_REGISTER.push_back(temp_string);
					temp_string = "";
				}
				if (*it_str != ';')
				{
					++it_str;
				}
				else
				{
					break;
				}

			}
			if (*it_str == ';')
			{
				i = 1;
				break;
			}
			if (i != 1)
			{
				++it_str;
			}
		}
		it_str = temp.end();
	}
}
void Process_USERCODE_REGISTER(regex keyword_USERCODE_REGISTER, smatch result_USERCODE_REGISTER, string temp, vector<string>& attribute_USERCODE_REGISTER)
{
	if (regex_search(temp, result_USERCODE_REGISTER, keyword_USERCODE_REGISTER))
	{
		string m_tap = result_USERCODE_REGISTER.str();
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//引号前面的不要了
		while ((*it_str != '"') && (it_str != m_tap.end()))
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_USERCODE_REGISTER.push_back(temp_string);
		temp_string = "";
		it_str = temp.end();
	}
}
void ProcessREGISTER_ACCESS(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo)
{
	if (regex_search(temp, result_quo, keyword_quo))
	{
		string m_tap = result_quo.str();
		int i = 0;
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		char m_tempr = NULL;
		bool depend = (m_tempr == '_') || (m_tempr == '[') || (m_tempr == ']');
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//引号前面的不要了
		m_tempr = *it_str;
		depend = (m_tempr == '_') || (m_tempr == '[') || (m_tempr == ']');
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			while ((*it_str != '\n') && (*it_str != ';'))
			{
				if ((!isblank(*it_str)) && (*it_str != '(') && (*it_str != ';'))
				{
					if ((isalpha(*it_str)) || (isalnum(*it_str)) || (depend))
					{
						while ((isalpha(*it_str)) || (isalnum(*it_str)) || (depend))
						{
							temp_string = temp_string + *it_str;
							++it_str;
							m_tempr = *it_str;
							depend = (m_tempr == '_') || (m_tempr == '[') || (m_tempr == ']');
						}
						attribute_quo.push_back(temp_string);//存入指令名称
						temp_string = "";
					}
				}
				if (*it_str == '(')
				{
					++it_str;
					while (*it_str != ')')
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);//存入指令数据
					temp_string = "";
				}
				if (*it_str == ';')
				{
					i = 1;
					break;
				}
				++it_str;
			}
			if (*it_str == ';')
			{
				i = 1;
				break;
			}
			if (i != 1)
			{
				++it_str;
			}
		}
		it_str = temp.end();
	}
}
void Process_BOUNDARY_REGISTER(regex keyword_quo, smatch result_quo, string temp, vector<vector<string>>& attribute_BR_info)
{
	if (regex_search(temp, result_quo, keyword_quo))
	{
		vector<string> temp_v;
		string m_tap = result_quo.str();
		int i = 0;
		int j = 0;
		int k = 0;
		/*	int m = 0;*/
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		char m_char = NULL;
		m_char = *it_str;
		bool depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_') || (m_char == '(') || (m_char == ')');
		bool pick = (j == 1) && (*it_str == ')');
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//第一个引号前面的不要了
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			vector<string> attribute_quo;
			while ((*it_str != '\n') && (*it_str != ';'))
			{
				
				if ((isdigit(*it_str)) && (*prev(it_str) == '"'))//存储引号里面的开头数字
				{
					while ((isdigit(*it_str)) && (it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);
					temp_string = "";
				}
				if (*it_str == '(')
				{
					++it_str;
					m_char = *it_str;
					depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
					pick = (j == 1) && (*it_str == ')');
					while (*it_str != ')')
					{
						
						while (((*it_str != ',') && (*it_str != ')')) || pick)
						{
							if (depend || (*it_str == '(') || (*it_str == ')'))
							{
								if (depend)
								{
									temp_string = temp_string + *it_str;
									++it_str;
									pick = (j == 1) && (*it_str == ')');
									m_char = *it_str;
									depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
								}
								if (*it_str == '(')
								{
									temp_string = temp_string + *it_str;
									++it_str;
									j = 1;
									pick = (j == 1) && (*it_str == ')');
									m_char = *it_str;
									depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
								}
								if ((*it_str == ')') && (j == 1))
								{
									temp_string = temp_string + *it_str;
									++it_str;
									j = 0;
									pick = (j == 1) && (*it_str == ')');
									m_char = *it_str;
									depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
								}
							}
							else
							{
								++it_str;
								m_char = *it_str;
								pick = (j == 1) && (*it_str == ')');
								depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
							}
						}
						if (*it_str == ',')
						{
							attribute_quo.push_back(temp_string);
							temp_string = "";
							++it_str;
							pick = (j == 1) && (*it_str == ')');
							m_char = *it_str;
							depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
						}
						if (*it_str == ')')
						{
							attribute_quo.push_back(temp_string);
							temp_string = "";
							break;
						}
					}
					temp_string = "";
					++it_str;
					pick = (j == 1) && (*it_str == ')');
					m_char = *it_str;
					depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
				}//出来为 ）的下一位
				++it_str;
				temp_v = attribute_quo;
			}//出来为\n或;
			if (*it_str == '\n')
			{
				attribute_BR_info.push_back(attribute_quo);
				++it_str;
				k = 1;
			}
		}
		if (k != 1)
		{
			attribute_BR_info.push_back(temp_v);
			//vector<string> attribute_quo;
		}
		if (*it_str == ';')
		{
			attribute_BR_info.push_back(temp_v);
			//vector<string> attribute_quo;
		}
		temp_string = "";
		it_str = temp.end();
	}
}
void Process_COMPONENT_CONFORMANCE(regex keyword_COMPONENT_CONFORMANCE, smatch result_COMPONENT_CONFORMANCE, string temp, vector<string>& attribute_COMPONENT_CONFORMANCE)
{
	if (regex_search(temp, result_COMPONENT_CONFORMANCE, keyword_COMPONENT_CONFORMANCE))//保存显示COMPONENT_CONFORMANCE字串属性
	{
		string m_COMPONENT_CONFORMANCE = result_COMPONENT_CONFORMANCE.str();
		string temp_string = "";
		string::iterator it_str = m_COMPONENT_CONFORMANCE.begin();
		while (*it_str != '"')
		{
			++it_str;
		}//*it_str此刻为"
		++it_str;
		while (*it_str != '"')//存储引号内为数字  字母  下滑线的部分
		{
			char m_depend_char = *it_str;
			if ((isalpha(*it_str)) || (isdigit(*it_str)) || (m_depend_char == 95))//确定存进去的是字母或者数字或下划线
			{
				temp_string = temp_string + *it_str;
				++it_str;
				m_depend_char = *it_str;
			}
			else
			{
				++it_str;
			}
		}
		attribute_COMPONENT_CONFORMANCE.push_back(temp_string);
		temp_string = "";
		it_str = m_COMPONENT_CONFORMANCE.end();
	}
}
void ProcessPIN_MAP(regex keyword_PIN_MAP, smatch result_PIN_MAP, string temp, vector<string>& attribute_PIN_MAP)
{
	if (regex_search(temp, result_PIN_MAP, keyword_PIN_MAP))
	{
		string m_PIN_MAP = result_PIN_MAP.str();
		string temp_string = "";
		m_PIN_MAP.erase(0, 7);
		string::iterator it_str = m_PIN_MAP.begin();
		while (!isupper(*it_str))
		{
			++it_str;
		}
		while (*it_str != ';')
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_PIN_MAP.push_back(temp_string);
		it_str = m_PIN_MAP.end();
		temp_string = "";
	}
}

void Process_INSTRUCTION_OPCODE(regex keyword_quo, smatch result_quo, string temp, vector<string>& attribute_quo, vector<vector<string>>& attribute_BR_info)
{
	if (regex_search(temp, result_quo, keyword_quo))
	{
		string m_tap = result_quo.str();

		int i = 0;
		int j = 0;
		int k = 0;
		/*	int m = 0;*/
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//第一个引号前面的不要了
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			while ((*it_str != '\n') && (*it_str != ';'))
			{
				if ((isalpha(*it_str)) && (*prev(it_str) == '"'))//存储引号里面的开头指令名
				{
					while ((isalpha(*it_str)) && (it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);
					temp_string = "";
				}
				if (*it_str == '(')
				{
					while (*it_str != ')')
					{
						while ((*it_str != ',') && (*it_str != ')'))
						{
							if (isalnum(*it_str))
							{
								temp_string = temp_string + *it_str;
								++it_str;
							}
							else
							{
								++it_str;
							}
						}
						if (*it_str == ',')
						{
							attribute_quo.push_back(temp_string);
							temp_string = "";
							++it_str;
						}
					}
					attribute_BR_info.push_back(attribute_quo);
					temp_string = "";
					++it_str;
				}//出来为 ）的下一位
				++it_str;
			}//出来为\n或;
			if (*it_str == '\n')
			{
				attribute_BR_info.push_back(attribute_quo);
				++it_str;
				k = 1;
			}
		}
		if (k != 1)
		{
			attribute_BR_info.push_back(attribute_quo);
		}
		temp_string = "";
		it_str = temp.end();
	}
}
void  CBsdlReader::view_BSDL()
{
	for (auto i = 0; i != port_info.size();i++)
	{
		for (auto j = 0; j != port_info[i].size(); j++)
		{
			if (j == (port_info[i].size() -2))
			{
				cout << port_info[i][j] << ": ";
			}
			else if (j == (port_info[i].size() - 1))
			{
				cout << port_info[i][j] << endl;
			}
			else
			{
				cout << port_info[i][j] <<",";
			}
		}
	}
	for (auto i = 0; i != constant_info.size(); i++)
	{
		for (auto j = 0; j != constant_info[i].size(); j++)
		{
			if (j == 0)
			{
				cout <<i<<'\t'<<j<<"***"<< constant_info[i][j] << ": ";
			}
			else
			{
				cout << constant_info[i][j] <<"............."<<i<<" "<<j<<" "<< endl;
			}
		}
	}
	for (auto i = 0; i != attribute_BR_info.size(); i++)
	{
		for (auto j = 0; j != attribute_BR_info[i].size(); j++)
		{
			cout << attribute_BR_info[i][j] <<"   ";
		}
		cout << endl;
	}
}
void  CBsdlReader::ProcessGeneric(const string& temp)
{
	string temp_string = "";
	auto it_str = temp.begin();
	while (*it_str != '"')
	{
		++it_str;
	}
	++it_str;//引号前面的不要了
	while ((isdigit(*it_str)) || (isalpha(*it_str)))
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	Generic_info.push_back(temp_string);//直接将数字存入
	temp_string = "";
	it_str = temp.end();
}
void CBsdlReader::ProcessUse(string temp)
{
	temp.erase(0, 4);
	string::iterator it_str = temp.begin();
	string temp_string = "";
	while (*it_str != '.')
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	++it_str;//.的下一位
	temp_string = "";
	use_info.push_back(temp_string);
	while (*it_str != ';')
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	use_info.push_back(temp_string);
	temp_string = "";
	it_str = temp.end();
}
vector<vector<string>>  CBsdlReader::Get_portdata()
{
	return port_info;
}
vector<vector<string>>  CBsdlReader::Get_constantdata()
{
	return constant_info;
}
vector<vector<string>>  CBsdlReader::Get_BRdata()
{
	return attribute_BR_info;
}
vector<string>  CBsdlReader::Get_Enddata()
{
	return end_info;
}