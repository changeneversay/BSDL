#include <winsock.h>
#include <mysql.h>
#pragma comment(lib,"libmySQL.lib")  //要加不然会编译出错
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

class MyDataBase
{
private:
	MYSQL* sql = nullptr;
	MYSQL_RES* res = nullptr;
	MYSQL_ROW row = nullptr;
	bool connect_flag;
	vector<string>Chain_U;
	vector<string>Chain_TDO;
	vector<string>Chain_TDI;
	vector<string>Mark;
	vector<vector<string>>Chain_info;
public:
	MyDataBase();
	MyDataBase(MYSQL* mysql);
	~MyDataBase();
	void connect(const string& host, const string& user, const string& password,
		const string& database = "mysql", unsigned int port = 0,
		const char* unix_socket = nullptr, unsigned long client_flag = 0);
	void disconnect();
	void create_database(const string& database);
	void use_database(const string& database);
	void delete_database(const string& database);
	void create_table(const string& table, const string& elements);
	vector<string> select_U(const string& limits);
	vector<string> select_constant_io(const string& str1, const string& IOname);
	vector<string> select_net_name(const string& Node_name);
	vector<string> select_node_name(const string& Net_name);
	vector<string> select_Utype(const string& limits);
	vector<string> select_componment_trans(const string& trans_name);
	void insert_table(const string& table, const string& value);
	void insert_port_table(const string& table, const string& one, const string& two, const string& col1,const string& col2);
	void delete_table(const string& table, const string& value);
	void delete_table(const string& table);
	void update_table(const string& table, const string& value, const string& limits);
	void Alter_table(const string& table, const string& op, const string& name_type);
	void query(const string& limits, const string& command);
	string Process_database(const vector<vector<string>>& a, const vector<vector<string>>& b, const vector<vector<string>>& c, const vector<string>& end_info, const vector<vector<string>>& d, const vector<vector<string>>& e);
	void insert_vector(const vector<vector<string>>& port_v, const vector<vector<string>>& constant_info, const vector<vector<string>>& attribute_BR_info);
	void insert_BR_table(const string& table, const string& one, const string& two, const string& three, const string& four, const string& five, const string& six, const string& seven, const string& eight, const string& col1, const string& col2, const string& col3, const string& col4, const string& col5, const string& col6, const string& col7, const string& col8);
	void insert_BR_table(const string& table, const string& one, const string& two, const string& three, const string& four, const string& five, const string& col1, const string& col2, const string& col3, const string& col4, const string& col5);
	void insert_chain_table(const string& table, const string& one, const string& two, const string& three, const string& four, const string& col1, const string& col2, const string& col3, const string& col4);
	void Process_select(const string& str1);
	void Process_Chain_road(const size_t& x,MyDataBase db, const string& str1, const string& temp_str, int chain_num,vector<string>& Mark, vector<vector<string>>& Chain_info);
	vector<vector<string>> Process_All_Chain(MyDataBase db, const string& str1);
	
};
