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
	size_t netNum = NULL;                                 // 存放网络数
	size_t componentNum = NULL;                           // 存放器件数
	vector<vector<string>> netListInfo;         // 一个网络全部信息存在一个vector<string>中
	vector<vector<string>> componentInfo;       // 存放器件名称及类型

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
