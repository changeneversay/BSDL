#include "connect_database.h"
#include"cbsdlreader.h"
#include<string>
#include<iostream>
#include<vector>
using namespace std;
void insert_info(MyDataBase mdb, const string& tem2, const string& tem3, const string& tem4, const vector<vector<string>>& port_v, const vector<vector<string>>& constant_v, const vector<vector<string>>& br_v, const vector<vector<string>>& d, const vector<vector<string>>& e);
void MyDataBase::disconnect() {    //断开连接
	if (sql)
		mysql_close(sql);
	sql = nullptr;
	res = nullptr;
	connect_flag = false;
	cout << "disconnect success!" << endl;
}

MyDataBase::MyDataBase() {          //初始化
	sql = mysql_init(nullptr);
	if (!sql)
		cout << "init error!" << endl;
	connect_flag = false;
}

MyDataBase::MyDataBase(MYSQL* mysql) {      //初始化，构造函数的重载
	sql = mysql_init(mysql);
	if (!sql)
		cout << "init error!" << endl;
	connect_flag = false;
}

MyDataBase::~MyDataBase() {
	if (connect_flag)
		disconnect();
	sql = nullptr;
	mysql_free_result(res);
	res = nullptr;
}

void MyDataBase::connect(const string& host, const string& user, const string& password, const string& database,
	unsigned int port, const char* unix_socket, unsigned long client_flag) {
	if (!sql) {
		cout << "sql has not been initialized!" << endl;          //判断是否初始化
		return;
	}
	if (!mysql_real_connect(sql, host.c_str(), user.c_str(), password.c_str(), database.c_str(),     //进行连接
		port, unix_socket, client_flag)) {
		cout << "connect error!" << endl;
		return;
	}
	cout << "connect success!" << endl;
}
void MyDataBase::create_database(const string& database) 
{                     //传入的名字作为被创建新的数据库名
	string str = "create database if not exists " + database;
	if (mysql_query(sql, str.c_str()))
	{
		cout << "create database error!" << endl;
		return;
	}
	cout << "create database success!" << endl;
}

void MyDataBase::use_database(const string& database)
{                        //调用数据库
	string str = "use " + database;
	if (mysql_query(sql, str.c_str())) {
		cout << "use database error!" << endl;
		return;
	}
	cout << "use " << database << " now!" << endl;
}



void MyDataBase::show_table() {                                            //查看表
	string str = "show tables";
	if (mysql_query(sql, str.c_str())) {
		cout << "show tables error!" << endl;
		return;
	}
	showres();
}

void MyDataBase::create_table(const string& table, const string& elements) {       //创造表，1、表名   2、成员，成员格式为     列名 类型，列名 类型，..... 
	string str = "create table " + table + "(" + elements + ")";
	if (mysql_query(sql, str.c_str())) {
		cout << mysql_error(sql) << endl;
		cout << "create table error!" << endl;
		return;
	}
	cout << "create table success!" << endl;
}

void  MyDataBase::selectitem(const string& table, const string& value)
{
	string str = "select "  +value  + " from " +table;//查询表的数据，  1、表名     2、字段名 ，多段用逗号隔开
	cout << str << endl;
	if (mysql_query(sql, str.c_str()))
	{
		cout <<mysql_error(sql)<<endl;
		cout << "select error!" << endl;
		//return {};
	}
	else
	{
		cout << str << " success" << endl;
	}
	//vector<vector<string>> ret;
	//res = mysql_use_result(sql);//mysql_use_result() 初始化一个一行一行地结果集合的检索。
	//while ((row = mysql_fetch_row(res)) != nullptr)
	//{
	//	unsigned int i = 0;
	//	vector<string> temp;
	//	while (i < mysql_num_fields(res))
	//		temp.push_back(row[i++]);
	//	ret.push_back(temp);
	//}
	//mysql_free_result(res);
	//res = nullptr;
	//return ret;
}

vector<string> MyDataBase::select_U( const string& limits) 
{
	string m1 = "\"";
	string te2 = m1 + limits + m1;
	string str = "SELECT componment_name FROM componment_info where componment_type=" + m1+limits+m1;
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) 
	{
		cout << mysql_error(sql) << endl;
		cout << "select error!" << endl;
		return{};
	}
	else
	{
		cout << str << " success" << endl;
		vector<string> ret;
		res = mysql_use_result(sql);
		while ((row = mysql_fetch_row(res)) != nullptr)
		{
			unsigned int i = 0;
			ret.push_back(row[i++]);
		}
		//cout << ret[0].c_str() << ret[1].c_str() << ret[2].c_str() << ret[3].c_str();
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}

	/*vector<string> ret;
	res = mysql_use_result(sql);
	while ((row = mysql_fetch_row(res)) != nullptr) 
	{
		unsigned int i = 0;
		ret.push_back(row[i++]);
	}
	mysql_free_result(res);
	res = nullptr;
	return ret;*/
}

void MyDataBase::showres() {
	res = mysql_use_result(sql);
	cout << "****************The result is:****************" << endl;
	while ((row = mysql_fetch_row(res)) != nullptr) {
		unsigned int i = 0;
		while (i < mysql_num_fields(res))
			cout << row[i++] << "\t";
		cout << endl;
	}
	cout << "**********************************************" << endl;
	mysql_free_result(res);
	res = nullptr;//展示结果
}

void MyDataBase::insert_table(const string& table, const string& value) {  //插入， 1、表名     2、数据格式   data1,data2,data3......
	string str = "insert into " + table + " values (" + value + ")";

	if (mysql_query(sql, str.c_str())) {
		cout << "insert error!" << endl;
		return;
	}
	cout << "insert success!" << endl;
}

void MyDataBase::insert_port_table(const string& table, const string& one,const string& two, const string& col1,const string& col2 ) 
{
	string str = "insert into " + table + "(" + col1 +","+col2 + ") values (" +"\""+ one +"\""+","+"\""+ two +"\""+ ")";
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "insert error!" << endl;
		return;
	}
	cout << "insert success!" << endl;
}
void MyDataBase::insert_BR_table(const string& table, const string& one, const string& two, const string& three, const string& four, const string& five, const string& six, const string& seven, const string& eight, const string& col1, const string& col2, const string& col3, const string& col4, const string& col5, const string& col6, const string& col7, const string& col8)
{
	string str = "insert into " + table + "(" + col1 + "," + col2 + "," + col3+"," + col4+"," + col5+"," + col6+"," + col7+ "," + col8+") values (" + "\"" + one + "\"" + "," + "\"" + two + "\"" + "," + "\"" + three+"\"" + "," +"\""+ four+"\"" + "," +"\""+ five+"\"" + "," +"\""+ six+"\"" + "," +"\""+ seven+"\""+","+"\""+eight+"\""+")";
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "insert error!" << endl;
		return;
	}
	cout << "insert success!" << endl;
}
void MyDataBase::insert_BR_table(const string& table, const string& one, const string& two, const string& three, const string& four, const string& five,const string& col1, const string& col2, const string& col3, const string& col4,const string& col5)
{
	string str = "insert into " + table + "(" + col1 + "," + col2 + "," + col3 + "," + col4 + ","+col5+ ") values (" + "\"" + one + "\"" + "," + "\"" + two + "\"" +","+"\"" + three + "\"" +","+"\""+ four + "\""  +","+"\""+five+"\""+ ")";
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "insert error!" << endl;
		return;
	}
	cout << "insert success!" << endl;
}


void MyDataBase::delete_table(const string& table, const string& value) {
	string str = "delete from " + table + " where " + value;
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "delete error!" << endl;
		return;
	}
	cout << "delete success!" << endl;
}
void MyDataBase::delete_table(const string& table) {
	string str = "drop table " + table;
	if (mysql_query(sql, str.c_str())) {
		cout << "delete error!" << endl;
		return;
	}
	cout << "delete success!" << endl;
}

void MyDataBase::update_table(const string& table, const string& value, const string& limits) {
	string str = "update " + table + " set " + value + " where " + limits;
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "delete error!" << endl;
		return;
	}
	cout << "update success!" << endl;
}

void MyDataBase::Alter_table(const string& table, const string& op, const string& name_type) {      //1、表名 2、操作 add/drop/modify/change  3、列名 列类型
	string str = "alter table " + table + " " + op + " column " + "name_type";
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "modify error!" << endl;
		return;
	}
	cout << "modify success!" << endl;
}
void MyDataBase::delete_database(const string& database) {
	string str = "drop database " + database;
	if (mysql_query(sql, str.c_str())) {
		cout << "delete database error!" << endl;
		return;
	}
	else
	{
		cout << database << " has been deleted!" << endl;
	}

}

void MyDataBase::query(const string& limits, const string& command) {
	string str = "select " + limits + " from " + command;
	if (mysql_query(sql, str.c_str())) {      //mysql_query()：向数据库发送查询语句,第一个参数为MYSQL变量的地址，第二个参数为"查询语句"
		cout << "query error!" << endl;
		return;
	}
	cout << "query success!" << endl;
}
string MyDataBase::Process_database(const vector<vector<string>>& a, const vector<vector<string>>& b, const vector<vector<string>>& c, const vector<string>& end_info, const vector<vector<string>>& d, const vector<vector<string>>& e)
{
	MyDataBase mdb;
	vector<vector<string>>port_v = a;
	vector<vector<string>>constant_v = b;
	vector<vector<string>>br_v = c;
	string tem1, tem2, tem3, tem4, tem5;
	tem1 = end_info[0].c_str();//器件名称
	tem2 = tem1 + "_port";
	tem3 = tem1 + "_constant";
	tem4 = tem1 + "_boundary_register";
	tem5 = tem1 + "_BSDL_DATA";
	mdb.connect("localhost", "root", "change");
	mdb.create_database(tem5);
	mdb.use_database(tem5);
	mdb.create_table(tem2,"port_name varchar(40),port_character varchar(40)");
	mdb.create_table(tem3, "constant_logic_name varchar(40),constant_physical_name varchar(40)");
	mdb.create_table(tem4, "sign_num varchar(40),BR_type varchar(40),channel_pin varchar(40),func varchar(40),safe_num varchar(40),control_num varchar(40),invain_num varchar(40),state_num varchar(40)");
	mdb.create_table("Net_info", "Net_name varchar(40),Node_name varchar(40)");
	mdb.create_table("componment_info", "componment_name varchar(40),componment_type varchar(40)");
	insert_info(mdb,tem2,tem3,tem4, port_v, constant_v, br_v, d, e);
	//mdb.delete_database(tem5);
	mdb.disconnect();
	return tem1;
}
void MyDataBase::insert_vector(const vector<vector<string>>&port_v, const vector<vector<string>>& constant_info, const vector<vector<string>>& attribute_BR_info)
{
	for (auto i = 0; i != port_v.size(); i++)
	{
		for (auto j = 0; j != port_v[i].size(); j++)
		{
			if (j == (port_v[i].size() - 2))
			{
				cout << port_v[i][j] << ": ";
			}
			else if (j == (port_v[i].size() - 1))
			{
				cout << port_v[i][j] << endl;
			}
			else
			{
				cout << port_v[i][j] << ",";
			}
		}
	}
	for (auto i = 0; i != constant_info.size(); i++)
	{
		for (auto j = 0; j != constant_info[i].size(); j++)
		{
			if (j == 0)
			{
				cout << i << '\t' << j << "***" << constant_info[i][j] << ": ";
			}
			else
			{
				cout << constant_info[i][j] << "............." << i << " " << j << " " << endl;
			}
		}
	}
	for (auto i = 0; i != attribute_BR_info.size(); i++)
	{
		for (auto j = 0; j != attribute_BR_info[i].size(); j++)
		{
			cout << attribute_BR_info[i][j] << "   ";
		}
		cout << endl;
	}
}
void insert_info(MyDataBase mdb,const string & tem2, const string& tem3, const string& tem4,const vector<vector<string>>& port_v, const vector<vector<string>>& constant_v, const vector<vector<string>>& br_v, const vector<vector<string>>& d, const vector<vector<string>>& e)
{
	string temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
	for (auto i = 0; i != port_v.size(); i++)
	{
		for (auto j = 0; j != port_v[i].size(); j++)
		{
			if (j != port_v[i].size() - 1)
			{
				string temp = "";
				size_t x = 0;
				temp = port_v[i][j].c_str();
				temp1 = temp;
				x = port_v[i].size() - 1;
				temp2 = port_v[i][x].c_str();
				mdb.insert_port_table(tem2, temp1, temp2, "port_name", "port_character");
			}
		}
	}
	for (auto i = 0; i != constant_v.size(); i++)
	{
		for (auto j = 0; j != constant_v[i].size(); j++)
		{
			if (j != 0)
			{
				string temp = "";
				temp = constant_v[i][j].c_str();
				temp4 = temp;
				auto m = constant_v[i].size();
				if (m >= 3)
				{
					string mmm = to_string(j);//记住！
					string yyy = ")";
					string xxx = "(";
					temp3 = constant_v[i][0].c_str() + xxx + mmm + yyy;
				}
				else
				{
					temp3 = constant_v[i][0].c_str();
				}
				mdb.insert_port_table(tem3, temp3, temp4, "constant_logic_name", "constant_physical_name");
			}
		}
	}
	for (auto i = 0; i != br_v.size(); i++)
	{
		auto m = br_v[i].size();
		string temp5, temp6, temp7, temp8, temp9, temp10, temp11, temp12;
		if (m > 6)
		{
			temp5 = br_v[i][0].c_str();
			temp6 = br_v[i][1].c_str();
			temp7 = br_v[i][2].c_str();
			temp8 = br_v[i][3].c_str();
			temp9 = br_v[i][4].c_str();
			temp10 = br_v[i][5].c_str();
			temp11 = br_v[i][6].c_str();
			temp12 = br_v[i][7].c_str();
			mdb.insert_BR_table(tem4, temp5, temp6, temp7, temp8, temp9, temp10, temp11, temp12, "sign_num ", "BR_type", "channel_pin", "func", "safe_num", "control_num", "invain_num", "state_num");
		}
		else
		{
			temp5 = br_v[i][0].c_str();
			temp6 = br_v[i][1].c_str();
			temp7 = br_v[i][2].c_str();
			temp8 = br_v[i][3].c_str();
			temp9 = br_v[i][4].c_str();
			mdb.insert_BR_table(tem4, temp5, temp6, temp7, temp8, temp9, "sign_num ", "BR_type", "channel_pin", "func", "safe_num");
		}

	}
	for (auto i = 0; i != d.size(); i++)
	{
		for (auto j = 0; j != d[i].size() - 1; j++)
		{
			if (j != 0)
			{
				string temp = "";
				temp = d[i][j].c_str();
				temp6 = temp;
				temp5 = d[i][0].c_str();
				mdb.insert_port_table("Net_info", temp5, temp6, "Net_name", "Node_name");
			}
		}
	}
	for (auto i = 0; i != e.size(); i++)
	{
		temp7 = e[i][0].c_str();
		temp8 = e[i][1].c_str();
		mdb.insert_port_table("componment_info", temp7, temp8, "componment_name", "componment_type");
	}
}
void MyDataBase::Process_select(const string& str1)
{
	MyDataBase db;
	db.connect("localhost", "root", "change");
	db.use_database(str1+"_BSDL_DATA");
	vector<string>v1 = db.select_U(str1);
	cout << v1[0].c_str() <<"   "<< v1[1].c_str()<<"   " << v1[2].c_str() <<"   "<< v1[3].c_str()<< endl;
	//db.selectitem("net_info", "*");
	db.disconnect();
}