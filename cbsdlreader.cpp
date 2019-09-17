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
void Process_COMPONENT_CONFORMANCE(regex keyword_COMPONENT_CONFORMANCE, smatch result_COMPONENT_CONFORMANCE, string temp, vector<string>attribute_COMPONENT_CONFORMANCE);
void ProcessPIN_MAP(regex keyword_PIN_MAP, smatch result_PIN_MAP, string temp,vector<string> attribute_PIN_MAP);
void ProcessTAP(regex keyword_tap, smatch result_tap, string temp, vector<string>attribute_tap);
void ProcessTCK(regex keyword_tap, smatch result_tap, string temp, vector<string>attribute_tap);
void ProcessQuo(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo);
void Process_INSTRUCTION_CAPTURE(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo);
void Process_IDCODE_REGISTER(regex keyword_IDCODE_REGISTER, smatch result_IDCODE_REGISTER, string temp, vector<string>attribute_IDCODE_REGISTER);
void Process_USERCODE_REGISTER(regex keyword_USERCODE_REGISTER, smatch result_USERCODE_REGISTER, string temp, vector<string>attribute_USERCODE_REGISTER);
void ProcessREGISTER_ACCESS(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo);
void Process_BOUNDARY_REGISTER(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo,vector<vector<string>>attribute_BR_info);
void CBsdlReader::ProcessBsdlFile(string BsdlFileContent)//�ṩ�ⲿ�ӿ�    ����ȡ�洢����
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
	string text = SkipProcess(BsdlFileContent);//ʵ�ֶ� -- �ĺ��� �����һ��û��--ע�����string
	if (regex_search(text, result_generic, keyword_generic))//����ƥ��generic�ؼ���
	{
		string m_port = result_generic.str();
		ProcessGeneric(m_port);
	}
	if (regex_search(text, result_port, keyword_port))//����ƥ��port�ؼ���
	{
		string m_port = result_port.str();
		ProcessPort(m_port);
	}
	if (regex_search(text, result_use, keyword_use))//����ƥ��use�ؼ���
	{
		string m_temp = result_use.str();
		ProcessUse(m_temp);
	}
	for (sregex_iterator it(text.begin(), text.end(), keyword_attribute), end_it; it != end_it; ++it)
	{
		m_attribute = m_attribute + it->str();
	}
	ProcessAttribute(m_attribute);
	if (regex_search(text, result_constant, keyword_constant))//����ƥ��constant�ؼ���
	{
		string m_temp = result_constant.str();
		ProcessConstant(m_temp);
	}
	if (regex_search(text, result_end, keyword_end))
	{
		string m_temp = result_end.str();
		ProcessEnd(m_temp);
	}
}
void  CBsdlReader::ProcessGeneric(string temp)
{
	string temp_string = "";
	string::iterator it_str = temp.begin();
	while (*it_str != '"')
	{
		++it_str;
	}
	++it_str;//����ǰ��Ĳ�Ҫ��
	while ((isdigit(*it_str)) || (isalpha(*it_str)))
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	Generic_info.push_back(temp_string);//ֱ�ӽ����ִ���
	cout << "Generic of EPM3032AT44  is " << temp_string << endl;
	temp_string = "";
	it_str = temp.end();
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
		{   
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
		cout << temp_string << endl;//��ȡ����
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
void CBsdlReader::ProcessConstant(string temp)
{
	string::iterator it_str = temp.begin();
	int i = 0;
	int j = 0;
	string temp_string = "";
	cout << "CONSTANT: " << endl;
	while (*it_str != '"')
	{
		++it_str;
	}
	++it_str;//����ǰ��Ĳ�Ҫ��
	while (*it_str != ';')
	{
		while ((*it_str != '\n')&&( *it_str != ';'))
		{
			i = 0;
			while ((*it_str != ',') && (*it_str != ';'))
			{
				while ((*it_str != ':')&&(*it_str != '\n') && (*it_str != ';')&&(j == 0))
				{
					j = 1;
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
				}//����Ϊ: ��\n ��;
				if (*it_str == ';')
				{
					break;
				}
				if (*it_str == ':')
				{
					++it_str;
					constant_name.push_back(temp_string);
					cout << temp_string << "  ";
					temp_string = "";
				}
				if ((isalnum(*it_str))&&(j ==1))
				{
					j = 0;
					while (isalnum(*it_str))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					constant_name.push_back(temp_string);
					cout << temp_string << "  ";
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
							cout << temp_string << "  ";
							temp_string = "";
						}
						else
						{
							++it_str;
						}
					}
				}
				if ((*it_str == ',')||(*it_str =='\n'))
				{
					break;
				}
				else
				{
					++it_str;
				}
			}//����Ϊ���Ż���\n����)
			if (*it_str == ';')
			{
				break;
			}
			if ((*it_str == '\n')&&( i == 0))
			{
				i = 1;
				constant_info.push_back(constant_name);
				temp_string = "";
				break;
			}
			if ((*it_str != '\n')&& (i == 0))
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
	++it_str;//.����һλ
	cout << "use:" << temp_string << ".";
	temp_string = "";
	use_info.push_back(temp_string);
	while(*it_str !=';')
	{
		temp_string = temp_string + *it_str;
		++it_str;
	}
	use_info.push_back(temp_string);
	cout <<temp_string << endl;
	temp_string = "";
	it_str = temp.end();
}
void CBsdlReader::ProcessAttribute(string temp)
{
	regex keyword_COMPONENT_CONFORMANCE("COMPONENT_CONFORMANCE([\\s\\S]*?);");
	regex keyword_PIN_MAP("PIN_MAP([\\s\\S]*?);");
	regex keyword_TDI("TAP_SCAN_IN([\\s\\S]*?);");
	regex keyword_TDO("TAP_SCAN_MODE([\\s\\S]*?);");
	regex keyword_TMS("TAP_SCAN_OUT([\\s\\S]*?);");
	regex keyword_TCK("TAP_SCAN_CLOCK([\\s\\S]*?);");
	regex keyword_INSTRUCTION_LENGTH("INSTRUCTION_LENGTH([\\s\\S]*?);");
	regex keyword_INSTRUCTION_OPCODE("INSTRUCTION_OPCODE([\\s\\S]*?);");
	regex keyword_INSTRUCTION_CAPTURE("INSTRUCTION_CAPTURE([\\s\\S]*?);");
	regex keyword_INSTRUCTION_DISABLE("INSTRUCTION_DISABLE([\\s\\S]*?);");
	regex keyword_INSTRUCTION_GUARD("INSTRUCTION_GUARD([\\s\\S]*?);");
	regex keyword_REGISTER_ACCESS("REGISTER_ACCESS([\\s\\S]*?);");
	regex keyword_BOUNDARY_LENGTH("BOUNDARY_LENGTH([\\s\\S]*?);");
	regex keyword_BOUNDARY_REGISTER("BOUNDARY_REGISTER([\\s\\S]*?);"); 
	regex keyword_IDCODE_REGISTER("IDCODE_REGISTER([\\s\\S]*?);");
	regex keyword_USERCODE_REGISTER("USERCODE_REGISTER([\\s\\S]*?);");
	smatch result_IDCODE_REGISTER;
	smatch result_USERCODE_REGISTER;
	smatch result_PIN_MAP;
	smatch result_COMPONENT_CONFORMANCE;
	smatch result_TDI;
	smatch result_TDO;
	smatch result_TMS;
	smatch result_TCK;
	smatch result_INSTRUCTION_LENGTH;
	smatch result_INSTRUCTION_OPCODE;
	smatch result_INSTRUCTION_CAPTURE;
	smatch result_INSTRUCTION_DISABLE;
	smatch result_INSTRUCTION_GUARD;
	smatch result_REGISTER_ACCESS;
	smatch result_BOUNDARY_LENGTH;
	smatch result_BOUNDARY_REGISTER;
	Process_COMPONENT_CONFORMANCE(keyword_COMPONENT_CONFORMANCE, result_COMPONENT_CONFORMANCE, temp, attribute_COMPONENT_CONFORMANCE);
	ProcessPIN_MAP(keyword_PIN_MAP, result_PIN_MAP, temp, attribute_PIN_MAP);
	ProcessTAP(keyword_TDI, result_TDI, temp, attribute_TDI);
	ProcessTAP(keyword_TMS, result_TMS, temp, attribute_TMS); 
	ProcessTAP(keyword_TDO, result_TDO, temp, attribute_TDO);
	ProcessTCK(keyword_TCK, result_TCK, temp, attribute_TCK);
	ProcessTAP(keyword_INSTRUCTION_LENGTH, result_INSTRUCTION_LENGTH, temp, attribute_INSTRUCTION_LENGTH);//INSTRUCTION_LENGTH�ؼ��ֺ�TAP����  ����ֱ����TAP����
	ProcessQuo(keyword_INSTRUCTION_OPCODE, result_INSTRUCTION_OPCODE, temp, attribute_INSTRUCTION_OPCODE);
	Process_INSTRUCTION_CAPTURE(keyword_INSTRUCTION_CAPTURE, result_INSTRUCTION_CAPTURE, temp, attribute_INSTRUCTION_CAPTURE);
	Process_IDCODE_REGISTER(keyword_IDCODE_REGISTER, result_IDCODE_REGISTER, temp, attribute_IDCODE_REGISTER);
	Process_USERCODE_REGISTER(keyword_USERCODE_REGISTER, result_USERCODE_REGISTER, temp, attribute_USERCODE_REGISTER);
	ProcessREGISTER_ACCESS(keyword_REGISTER_ACCESS, result_REGISTER_ACCESS, temp, attribute_REGISTER_ACCESS);
	ProcessTAP(keyword_BOUNDARY_LENGTH, result_BOUNDARY_LENGTH, temp, attribute_BOUNDARY_LENGTH);
	Process_BOUNDARY_REGISTER(keyword_BOUNDARY_REGISTER, result_BOUNDARY_REGISTER, temp, attribute_BOUNDARY_REGISTER, attribute_BR_info);
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
	cout << "end " << temp_string;
	end_info.push_back(temp_string);
	temp_string = "";
	it_str = temp.end();
}
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
void ProcessTAP(regex keyword_tap, smatch result_tap,string temp,vector<string>attribute_tap)
{
	if (regex_search(temp, result_tap, keyword_tap))//TDI
	{
		string m_tap = result_tap.str();
		cout << m_tap << endl;
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
void ProcessTCK(regex keyword_tap, smatch result_tap, string temp, vector<string>attribute_tap)
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
		attribute_tap.push_back(temp_string);//�������ŵ�һ����Ϣ
		cout << "attribute TAP_SCAN_CLOCK of TCK  : signal is (" << temp_string << ",";
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
		attribute_tap.push_back(temp_string);//�������ŵڶ�����Ϣ
		cout << temp_string << ");" << endl;
		it_str = m_tap.end();
		temp_string = "";
	}
}
void ProcessQuo(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo)
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
		++it_str;//����ǰ��Ĳ�Ҫ��
		while ((*it_str != ';')&&(it_str != m_tap.end()))
		{
			while (*it_str != '\n')
			{
				if (isupper(*it_str))
				{
					while ((isupper(*it_str))&& (*it_str != ';'))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);//����ָ������
					cout << temp_string << "         ";
					temp_string = "";
				}
				if (*it_str == '(')
				{
					++it_str;
					while ((isdigit(*it_str))&&(it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);//����ָ�����
					cout << temp_string << endl;
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
void Process_INSTRUCTION_CAPTURE(regex keyword_INSTRUCTION_CAPTURE, smatch result_INSTRUCTION_CAPTURE, string temp, vector<string>attribute_INSTRUCTION_CAPTURE)
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
		++it_str;//����ǰ��Ĳ�Ҫ��
		while ((isdigit(*it_str)) && (it_str != m_tap.end()))
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_INSTRUCTION_CAPTURE.push_back(temp_string);//ֱ�ӽ����ִ���
		cout << "INSTRUCTION_CAPTURE of EPM3032AT44 : entity is " << temp_string << endl;
		temp_string = "";
		it_str = temp.end();
	}
}
void Process_IDCODE_REGISTER(regex keyword_IDCODE_REGISTER, smatch result_IDCODE_REGISTER, string temp, vector<string>attribute_IDCODE_REGISTER)
{
	if (regex_search(temp, result_IDCODE_REGISTER, keyword_IDCODE_REGISTER))
	{
		string m_tap = result_IDCODE_REGISTER.str();
		cout << m_tap << endl;
		int i = 0;
		string temp_string = "";
		string::iterator it_str = m_tap.begin();
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//����ǰ��Ĳ�Ҫ��
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
void Process_USERCODE_REGISTER(regex keyword_USERCODE_REGISTER, smatch result_USERCODE_REGISTER, string temp, vector<string>attribute_USERCODE_REGISTER)
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
		++it_str;//����ǰ��Ĳ�Ҫ��
		while ((*it_str != '"') && (it_str != m_tap.end()))
		{
			temp_string = temp_string + *it_str;
			++it_str;
		}
		attribute_USERCODE_REGISTER.push_back(temp_string);
		cout << temp_string << endl;
		temp_string = "";
		it_str = temp.end();
	}
}
void ProcessREGISTER_ACCESS(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo)
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
		++it_str;//����ǰ��Ĳ�Ҫ��
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
						attribute_quo.push_back(temp_string);//����ָ������
						cout << temp_string << "         ";
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
					attribute_quo.push_back(temp_string);//����ָ������
					cout << temp_string << endl;
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
void Process_BOUNDARY_REGISTER(regex keyword_quo, smatch result_quo, string temp, vector<string>attribute_quo, vector<vector<string>>attribute_BR_info)
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
		char m_char = NULL;
		m_char = *it_str;
		bool depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_') || (m_char == '(') || (m_char == ')');
		bool pick = (j == 1) && (*it_str == ')');
		while (*it_str != '"')
		{
			++it_str;
		}
		++it_str;//��һ������ǰ��Ĳ�Ҫ��
		while ((*it_str != ';') && (it_str != m_tap.end()))
		{
			while ((*it_str != '\n') && (*it_str != ';'))
			{
				if ((isdigit(*it_str)) && (*prev(it_str) == '"'))//�洢��������Ŀ�ͷ����
				{
					while ((isdigit(*it_str)) && (it_str != m_tap.end()))
					{
						temp_string = temp_string + *it_str;
						++it_str;
					}
					attribute_quo.push_back(temp_string);
					cout << temp_string << ":  ";
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
						while (((*it_str != ',') && (*it_str != ')'))||pick)
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
							cout << temp_string << "    ";
							temp_string = "";
							++it_str;
							pick = (j == 1) && (*it_str == ')');
							m_char = *it_str;
							depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
						}
						if (*it_str == ')')
						{
							attribute_quo.push_back(temp_string);
							cout << temp_string<<"    ";
							temp_string = "";
							break;
						}
					}
					temp_string = "";
					++it_str;
					pick = (j == 1) && (*it_str == ')');
					m_char = *it_str;
					depend = (m_char == '*') || (isalpha(m_char)) || (isalnum(m_char)) || (m_char == '_');
				}//����Ϊ ������һλ
				++it_str;
			}//����Ϊ\n��;
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
void Process_COMPONENT_CONFORMANCE(regex keyword_COMPONENT_CONFORMANCE,smatch result_COMPONENT_CONFORMANCE, string temp, vector<string>attribute_COMPONENT_CONFORMANCE)
{
	if (regex_search(temp, result_COMPONENT_CONFORMANCE, keyword_COMPONENT_CONFORMANCE))//������ʾCOMPONENT_CONFORMANCE�ִ�����
	{
		string m_COMPONENT_CONFORMANCE = result_COMPONENT_CONFORMANCE.str();
		string temp_string = "";
		string::iterator it_str = m_COMPONENT_CONFORMANCE.begin();
		while (*it_str != '"')
		{
			++it_str;
		}//*it_str�˿�Ϊ"
		++it_str;
		while (*it_str != '"')//�洢������Ϊ����  ��ĸ  �»��ߵĲ���
		{
			char m_depend_char = *it_str;
			if ((isalpha(*it_str)) || (isdigit(*it_str)) || (m_depend_char == 95))//ȷ�����ȥ������ĸ�������ֻ��»���
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
		cout << "attribute COMPONENT_CONFORMANCE of EPM3032AT44 :entity is " << temp_string << endl;
		temp_string = "";
		it_str = m_COMPONENT_CONFORMANCE.end();
	}
}
void ProcessPIN_MAP(regex keyword_PIN_MAP, smatch result_PIN_MAP, string temp, vector<string> attribute_PIN_MAP)
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
		cout << "attribute PIN_MAP of EPM3032AT44 : entity is " << temp_string << endl;
		temp_string = "";
	}
}
