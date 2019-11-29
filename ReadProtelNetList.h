#pragma once

#include "ReadNetList.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class ReadProtelNetList :
	public ReadNetList
{
private:
	size_t netNum = NULL;                                 // ���������
	size_t componentNum = NULL;                           // ���������
	vector<vector<string>> netListInfo;         // һ������ȫ����Ϣ����һ��vector<string>��
	vector<vector<string>> componentInfo;       // ����������Ƽ�����

public:
	ReadProtelNetList() = default;
	ReadProtelNetList(string netListDir);
	virtual ~ReadProtelNetList();

	virtual size_t getNetNum();
	virtual size_t getComponentNum();
	virtual vector<vector<string>> getNetListInfo();
	virtual vector<vector<string>> getComponentInfo();
	virtual bool outfile_to_txt(const string & file_path_name);
};
