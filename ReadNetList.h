#pragma once


#include <vector>
#include <string>

using std::vector;
using std::string;

class ReadNetList
{
public:
	ReadNetList();
	virtual ~ReadNetList();

	virtual size_t getNetNum() = 0;
	virtual size_t getComponentNum() = 0;
	virtual vector<vector<string>> getNetListInfo() = 0;
	virtual vector<vector<string>> getComponentInfo() = 0;
	virtual bool outfile_to_txt(const string& file_path_name) = 0;
};

