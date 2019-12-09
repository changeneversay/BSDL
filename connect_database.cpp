#include "connect_database.h"/*������BSDL�ļ����������������ݿ�,���ɲ�����·�����ؿɲ�������*/
#include"cbsdlreader.h"
#include<string>
#include<iostream>
#include<vector>
#include<cctype>
using namespace std;
void insert_ChainNet_info(MyDataBase db, const vector<vector<string>>& Net_inout, const vector<vector<string>>& Net_inout_Hang, const vector<vector<string>>& Net_Out_Unkown, const vector<vector<string>>& Net_in_hang, const vector<vector<string>>& Net_In_Unkown);
void inNet(MyDataBase db, const vector<string>& Out_node, const vector<string>& In_node, const vector<vector<string>>& chain_info, const vector<vector<string>>& Net_inout, vector<vector<string>>& Net_in_Hang, vector<vector<string>>& Net_In_Unkown);//���chain_u����ֻ��һ��Ԫ��
void outNet(MyDataBase db, const vector<string>& Out_node, const vector<string>& In_node, const vector<vector<string>>& chain_info, vector<vector<string>>& Net_inout, vector<vector<string>>& Net_out_Hang, vector<vector<string>>& Net_Out_Unkown);
void insert_chain_info(MyDataBase db, const vector<vector<string>>& Chain_info);
void insert_info(MyDataBase mdb, const string& tem2, const string& tem3, const string& tem4, const vector<vector<string>>& port_v, const vector<vector<string>>& constant_v, const vector<vector<string>>& br_v, const vector<vector<string>>& d, const vector<vector<string>>& e);
void MyDataBase::disconnect() {    //�Ͽ�����
	if (sql)
		mysql_close(sql);
	sql = nullptr;
	res = nullptr;
	connect_flag = false;
	cout << "disconnect success!" << endl;
}

MyDataBase::MyDataBase() {          //��ʼ��
	sql = mysql_init(nullptr);
	if (!sql)
		cout << "init error!" << endl;
	connect_flag = false;
}

MyDataBase::MyDataBase(MYSQL* mysql) {      //��ʼ�������캯��������
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

void MyDataBase::connect(const string& host, const string& user, const string& password, const string& database,unsigned int port, const char* unix_socket, unsigned long client_flag) 
{
	if (!sql) 
	{
		cout << "sql has not been initialized!" << endl;          //�ж��Ƿ��ʼ��
		return;
	}
	if (!mysql_real_connect(sql, host.c_str(), user.c_str(), password.c_str(), database.c_str(),     //��������
		port, unix_socket, client_flag)) {
		cout << "connect error!" << endl;
		return;
	}
	cout << "connect success!" << endl;
}
void MyDataBase::create_database(const string& database) 
{                     //�����������Ϊ�������µ����ݿ���
	string str = "create database if not exists " + database;
	if (mysql_query(sql, str.c_str()))
	{
		cout << "create database error!" << endl;
		return;
	}
	cout << "create database success!" << endl;
}

void MyDataBase::use_database(const string& database)
{                        //�������ݿ�
	string str = "use " + database;
	if (mysql_query(sql, str.c_str())) {
		cout << "use database error!" << endl;
		return;
	}
	cout << "use " << database << " now!" << endl;
}

void MyDataBase::create_table(const string& table, const string& elements) {       //�����1������   2����Ա����Ա��ʽΪ     ���� ���ͣ����� ���ͣ�..... 
	string str = "create table " + table + "(" + elements + ")";
	if (mysql_query(sql, str.c_str())) {
		cout << mysql_error(sql) << endl;
		cout << "create table error!" << endl;
		return;
	}
	cout << "create table success!" << endl;
}

vector<string> MyDataBase::select_U( const string& limits) 
{
	string m1 = "\"";
	/*string te2 = m1 + limits + m1;*/
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_Utype(const string& limits)
{
	string m1 = "\"";
	/*string te2 = m1 + limits + m1;*/
	string str = "SELECT componment_type FROM componment_info where componment_name=" + m1 + limits + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_constant_io(const string& str1,const string& IOname)
{
	string m1 = "\"";
	string str = "SELECT constant_physical_name FROM "+str1+"_constant where constant_logic_name=" + m1 + IOname + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_constant_logic_name(const string& str1, const string& IOname)
{
	string m1 = "\"";
	string str = "SELECT constant_logic_name FROM " + str1 + "_constant where constant_physical_name=" + m1 + IOname + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_componment_trans(const string& trans_name)
{
	string m1 = "\"";
	string str = "SELECT componment_type FROM componment_info where componment_name=" + m1 + trans_name + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_net_name(const string& Node_name)
{
	string m1 = "\"";
	string str = "SELECT Net_name FROM net_info where Node_name=" + m1 + Node_name + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_node_name(const string& Net_name)//��һ������Ϊ������,�ڶ���ΪU��TDI�������
{
	string m1 = "\"";
	string str = "SELECT  Node_name FROM net_info where Net_name=" + m1 + Net_name + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
vector<string> MyDataBase::select_Chain_U(const string& limits1,const string& limits2)//���,��·���
{
	string m1 = "\"";
	string str = "SELECT componment_u FROM chain_info where chain_num=" + m1 + limits1 + m1 +" and chain_level="+m1+ limits2 + m1;
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
		mysql_free_result(res);
		res = nullptr;
		return ret;
	}
}
void MyDataBase::insert_table(const string& table, const string& value) {  //���룬 1������     2�����ݸ�ʽ   data1,data2,data3......
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
void MyDataBase::insert_Hang_table(const string& table, const string& one, const string& col1)
{
	string str = "insert into " + table + "(" + col1 + ") values (" + "\"" + one + "\"" +  ")";
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
void MyDataBase::insert_chain_table(const string& table, const string& one, const string& two, const string& three, const string& four,const string& col1, const string& col2, const string& col3, const string& col4)
{
	string str = "insert into " + table + "(" + col1 + "," + col2 + "," + col3 + "," + col4 + ") values (" + "\"" + one + "\"" + "," + "\"" + two + "\"" + "," + "\"" + three + "\"" + "," + "\"" + four + "\""  + ")";
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

void MyDataBase::Alter_table(const string& table, const string& op, const string& name_type) {      //1������ 2������ add/drop/modify/change  3������ ������
	string str = "alter table " + table + " " + op + " column " + "name_type";
	cout << str << endl;
	if (mysql_query(sql, str.c_str())) {
		cout << "modify error!" << endl;
		return;
	}
	cout << "modify success!" << endl;
}
void MyDataBase::delete_database(const string& database) 
{
    string str = "drop database if exists " + database;
	if (mysql_query(sql, str.c_str()))
	{
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
	if (mysql_query(sql, str.c_str())) {      //mysql_query()�������ݿⷢ�Ͳ�ѯ���,��һ������ΪMYSQL�����ĵ�ַ���ڶ�������Ϊ"��ѯ���"
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
	tem1 = end_info[0].c_str();//��������
	tem2 = tem1 + "_port";
	tem3 = tem1 + "_constant";
	tem4 = tem1 + "_boundary_register";
	tem5 = tem1 + "_BSDL_DATA";
	mdb.connect("localhost", "root", "change");
	mdb.delete_database(tem5);
	mdb.create_database(tem5);
	mdb.use_database(tem5);
	mdb.create_table(tem2,"port_name varchar(40),port_character varchar(40)");
	mdb.create_table(tem3, "constant_logic_name varchar(40),constant_physical_name varchar(40)");
	mdb.create_table(tem4, "sign_num varchar(40),BR_type varchar(40),channel_pin varchar(40),func varchar(40),safe_num varchar(40),control_num varchar(40),invain_num varchar(40),state_num varchar(40)");
	mdb.create_table("Net_info", "Net_name varchar(40),Node_name varchar(40)");
	mdb.create_table("componment_info", "componment_name varchar(40),componment_type varchar(40)");
	insert_info(mdb,tem2,tem3,tem4, port_v, constant_v, br_v, d, e);
	
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
					string mmm = to_string(j);//��ס��
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
void MyDataBase::Process_Chain(const string& str1)
{
	MyDataBase db;
	db.connect("localhost", "root", "change");
	db.use_database(str1+"_BSDL_DATA");
	vector<vector<string>>Chain = db.Process_All_Chain(db, str1);
	db.create_table("Chain_info", "chain_num varchar(40),chain_level varchar(40),componment_u varchar(40),componment_u_info varchar(40)");
	insert_chain_info(db, Chain);
	vector<string>V1 = Out_node;
	vector<string>V2 = In_node;
	vector<vector<string>>V3 = Chain;
	outNet(db, V1, V2, V3, Net_inout, Net_out_Hang, Net_Out_Unkown);
	inNet(db, V1, V2, V3, Net_inout, Net_in_Hang, Net_In_Unkown);
	db.create_table("T_NET_inout", "Out_Net varchar(40),In_Net varchar(40)");
	db.create_table("T_NET_Hang_out", "Net varchar(40)");//����OUT
	db.create_table("T_NET_Hang_in", "Net varchar(40)"); //����IN
	db.create_table("T_NET_Unkown_Out", "Net varchar(40)");//δ֪�������,�޷����
	db.create_table("T_NET_Unkown_In", "Net varchar(40)");//δ֪��������,�޷����
	insert_ChainNet_info(db, Net_inout, Net_out_Hang,Net_Out_Unkown, Net_in_Hang,Net_In_Unkown);
	db.disconnect();
}
void MyDataBase::Process_Chain_road(const size_t& x,MyDataBase db,const string &str1,const string& temp_str, int chain_num, vector<string>& Mark, vector<vector<string>>& Chain_info)//��������
{
	string tempt1;
	string center = temp_str;
	int turn = 1;
	int C_num = 1;
	string c_num_str = to_string(C_num);
	string next_u = center;
	vector<string>Chain_name;
	string num = to_string(chain_num);
	string chain_num_str = "chain" + num;
	Chain_TDO = db.select_constant_io(str1, "TDO");
	Chain_TDI = db.select_constant_io(str1, "TDI");
	for (auto i = 0; i != x; ++i)
	{
		string q = "-";
		bool pick = 0;
		for (auto v = 0; v != Mark.size(); ++v)
		{
			if (next_u == Mark[v])
			{
				pick = 1;
			}
		}
		if (pick != 1)
		{
			bool Mark_insert = 0;
			Chain_name.push_back(c_num_str);//�������
			++C_num;
			c_num_str = to_string(C_num);
			Chain_name.push_back(chain_num_str);//��������
			Chain_name.push_back(next_u);//��������Ԫ����
			Chain_name.push_back(str1);//����һ������BSDL��Ϣ
			Chain_info.push_back(Chain_name);
			Chain_name.clear();
			tempt1 = next_u + q + Chain_TDO[0];//TDO������ܽ����
			for (auto h : Mark)
			{
				if (next_u == h)
				{
					Mark_insert = 1;
				}
			}
			if (Mark_insert != 1)
			{
				Mark.push_back(next_u);//���
			}
			vector<string>net_name = db.select_net_name(tempt1);//��TDO����������
			vector<string>node_name = db.select_node_name(net_name[0].c_str());//��TDO���������Ĺܽ���
			cout << node_name[0].c_str() << "     " << node_name[1].c_str() << endl;
			if (node_name.size() != 1)
			{
				for (auto it = node_name.begin(); it != node_name.end(); ++it)//node_name������TDO�����ܽ���
				{
					if (*it == tempt1)
					{
						it = node_name.erase(it);
						break;
					}
				}
			}
			string dev1 = node_name[0].c_str();
			dev1.erase(dev1.begin() + 2, dev1.end());//�����ܽŵ�����������
			cout << dev1 << "****" << endl;
			vector<string>trans_type = db.select_componment_trans(dev1);//��������������
			cout << trans_type[0].c_str() << "######" << endl;
			if (trans_type[0] == "HEADER 8X2")
			{
				string temp_str = node_name[0].c_str();
				temp_str.erase(0, 3);
				int m = stoi(temp_str);
				string f;
				cout << m << "&&&&" << endl;
				if (m % 2 == 1)//��Ϊ��
				{
					int t = m + 1;
					f = to_string(t);
				}
				else//��Ϊż
				{
					int t = m - 1;
					f = to_string(t);
				}
				string trans_next_node = dev1 + "-" + f;//͸��Ԫ������һ�߹ܽ���
				cout << trans_next_node << endl;
				vector<string>net_name_next = db.select_net_name(trans_next_node);//Ѱ��͸���ܽ�����������,��һ��Ԫ��
				vector<string>node_name_next = db.select_node_name(net_name_next[0]);//�����������������Ԫ��(�ܽ���)
				if (node_name_next.size() != 1)
				{
					for (auto it = node_name_next.begin(); it != node_name_next.end(); ++it)//node_name������TDO�����ܽ���
					{
						if (*it == trans_next_node)
						{
							it = node_name_next.erase(it);
							break;
						}
					}
				}
				string next_node = node_name_next[0];
				next_node.erase(next_node.begin() + 2, next_node.end());
				vector<string>depend = db.select_Utype(next_node);//�ж���͸��ԭ��������������Ϣ�Ƿ�ΪBSDоƬ
				bool sign = 0;
				string str2 = str1;
				for (auto& c : str2)//Сдת����д
				{
					c = toupper(c);
				}
				if (depend[0] == str2)
				{
					for (auto v = 0; v != Mark.size(); ++v)
					{
						if (next_node == Mark[v])
						{
							sign = 1;
						}
					}
					if (sign != 1)
					{
						string nk = next_node + q + db.select_constant_io(str1, "TDI")[0];
						if (nk == node_name_next[0])
						{
							next_u = next_node;
							continue;
						}
					}
				}
				else
				{
					break;//͸��ԭ��������ΪBSDоƬʱ,����ѭ��������������,��TDI��ʼѰ��
				}
			}
			else if (trans_type[0] == str1)
			{
				string nk = dev1 + q + db.select_constant_io(str1, "TDI")[0];
				if (nk == node_name[0])
				{
					next_u = dev1;
					continue;
				}
				else
				{
					break;//����ΪU�ķ�TDI�ܽ�,�÷�����·����,��������������һ����������·
				}
			}
			else
			{
				break;
				//TDO������U��TRANS
			}
		}
		else
		{
			break;//PICK = 1,��U�ѱ����
		}

	}
	if (Mark.size() != x)
	{
		next_u = center;
		for (auto i = Mark.size() - 1; i != x; ++i)
		{
			bool pick = 0;
			for (auto v = 0; v != Mark.size(); ++v)
			{
				if ((next_u == Mark[v])&&(next_u != center))
				{
					pick = 1;
				}
			}
			if (pick != 1)
			{
				if (next_u != center)
				{
					Chain_name.push_back(c_num_str);//�������
					++C_num;
					c_num_str = to_string(C_num);
					Chain_name.push_back(chain_num_str);//��������
					Chain_name.push_back(next_u);//��������Ԫ����
					Chain_name.push_back(str1);//����һ������BSDL��Ϣ
					Chain_info.push_back(Chain_name);
					Chain_name.clear();
				}
				string q = "-";
				tempt1 = next_u + q + Chain_TDI[0];//TDI������ܽ����
				Mark.push_back(next_u);//���
				vector<string>net_name = db.select_net_name(tempt1);//��TDI����������
				vector<string>node_name = db.select_node_name(net_name[0].c_str());//��TDI���������Ĺܽ���
				cout << node_name[0].c_str() << "     " << node_name[1].c_str() << endl;
				if (node_name.size() != 1)
				{
					for (auto it = node_name.begin(); it != node_name.end(); ++it)//node_name������TDI�����ܽ���
					{
						if (*it == tempt1)
						{
							it = node_name.erase(it);
							break;
						}
					}
				}
				string dev1 = node_name[0].c_str();
				dev1.erase(dev1.begin() + 2, dev1.end());//�����ܽŵ�����������
				cout << dev1 << "****" << endl;
				vector<string>trans_type = db.select_componment_trans(dev1);//��������������
				cout << trans_type[0].c_str() << "######" << endl;
				if (trans_type[0] == "HEADER 8X2")
				{
					string temp_str = node_name[0].c_str();
					temp_str.erase(0, 3);
					int m = stoi(temp_str);
					string f;
					cout << m << "&&&&" << endl;
					if (m % 2 == 1)//��Ϊ��
					{
						int t = m + 1;
						f = to_string(t);
					}
					else//��Ϊż
					{
						int t = m - 1;
						f = to_string(t);
					}
					string trans_next_node = dev1 + "-" + f;//͸��Ԫ������һ�߹ܽ���
					cout << trans_next_node << endl;
					vector<string>net_name_next = db.select_net_name(trans_next_node);//Ѱ��͸���ܽ�����������,��һ��Ԫ��
					vector<string>node_name_next = db.select_node_name(net_name_next[0]);//�����������������Ԫ��(�ܽ���)
					if (node_name_next.size() != 1)
					{
						for (auto it = node_name_next.begin(); it != node_name_next.end(); ++it)//node_name������TDI�����ܽ���
						{
							if (*it == trans_next_node)
							{
								it = node_name_next.erase(it);
								break;
							}
						}
					}
					string next_node = node_name_next[0];
					next_node.erase(next_node.begin() + 2, next_node.end());
					vector<string>depend = db.select_Utype(next_node);//�ж���͸��ԭ��������������Ϣ�Ƿ�ΪBSDLоƬ
					bool sign = 0;
					string str2 = str1;
					for (auto& c : str2)//Сдת����д
					{
						c = toupper(c);
					}
					if (depend[0] == str2)
					{
						for (auto v = 0; v != Mark.size(); ++v)
						{
							if (next_node == Mark[v])
							{
								sign = 1;
							}
						}
						if (sign != 1)
						{
							string nk = next_node + q + db.select_constant_io(str1, "TDO")[0];
							if (nk == node_name_next[0])
							{
								turn = 1;
								next_u = next_node;
								continue;
							}
						}
						else
						{
							break;
						}
					}
					else
					{
						break;//͸��ԭ��������ΪBSDоƬʱ
					}
				}
				else if (trans_type[0] == str1)
				{
					string nk = dev1 + q + db.select_constant_io(str1, "TDO")[0];
					if (nk == node_name[0])
					{
						next_u = dev1;
						continue;
					}
				}
				else
				{
					break;//TDO������U��TRANS,����·����
				}
			}
		}
	}
}
vector<vector<string>>  MyDataBase::Process_All_Chain(MyDataBase db, const string& str1)
{
	bool decide = 0;
	int chain_num = 1;
	Chain_U = db.select_U(str1);//��ʾΪU1 U2
	auto x = Chain_U.size();
	for (auto k = 0; k != Chain_U.size(); ++k)
	{
		for (auto v = 0; v != Mark.size(); ++v)
		{
			if (Chain_U[k]== Mark[v])
			{
				decide = 1;
			}
		}
		if (decide != 1)
		{
			string temp_str = Chain_U[k];
			db.Process_Chain_road(x,db, str1, temp_str, chain_num,Mark, Chain_info);
			chain_num++;
		}
	}
	for (auto a : Chain_info)
	{
		cout << a[0] << endl;
		cout << a[1] << endl;
		cout << a[2] << endl;
		cout << a[3] << endl;
	}
	return Chain_info;
}
void insert_chain_info(MyDataBase db,const vector<vector<string>>& Chain_info)
{
	for (auto i = 0; i != Chain_info.size(); i++)
	{
		string tem1, tem2,tem3,tem4;
		tem1 = Chain_info[i][0];
		tem2 = Chain_info[i][1];
		tem3 = Chain_info[i][2];
		tem4 = Chain_info[i][3];
		db.insert_chain_table("Chain_info", tem1, tem2, tem3, tem4, "chain_num", "chain_level", "componment_u", "componment_u_info");
	}
}
void outNet(MyDataBase db,const vector<string>& Out_node, const vector<string>& In_node,const vector<vector<string>>& chain_info,vector<vector<string>>& Net_inout, vector<vector<string>>& Net_out_Hang, vector<vector<string>>&Net_Out_Unkown)//���chain_u����ֻ��һ��Ԫ��
{
	bool deci = 0;
	for (auto v = 0; v != chain_info.size(); ++v)
	{
		if (deci != 1)
		{
			for (auto j = 0; j != Out_node.size(); ++j)
			{
				vector<string>comp = db.select_Chain_U(chain_info[v][0], chain_info[v][1]);
				vector<string>node = db.select_constant_io(chain_info[v][3], Out_node[j]);
				string m = "-";
				string chain_pin = comp[0] + m + node[0];
				vector<string>net_name = db.select_net_name(chain_pin);//Ѱ�ҹܽ�����������,��һ��Ԫ��
				vector<string>node_name = db.select_node_name(net_name[0]);//�����������������Ԫ��(�ܽ���)��һ��Ԫ��(����������Ԫ�����)
			
				string str4 = chain_info[v][3];
				for (auto& c : str4)//Сдת����д
				{
					c = toupper(c);
				}

				if (node_name.size() != 1)//��Ϊ���ս�
				{
					for (auto it = node_name.begin(); it != node_name.end(); ++it)//node_name���������ܽ���
					{
						if (*it == chain_pin)
						{
							it = node_name.erase(it);//������һ��������
							break;
						}
					}
					string dev1 = node_name[0].c_str();//�����ܽ���
					dev1.erase(dev1.begin() + 2, dev1.end());//�����ܽŵ�����������
					cout << dev1 << "****" << endl;
					vector<string>trans_type = db.select_componment_trans(dev1);//�������������� ��Ϊ��д   header\U\DAC
					cout << trans_type[0].c_str() << "######" << endl;
					if (trans_type[0] == "HEADER 8X2")
					{
						string temp_str = node_name[0].c_str();
						temp_str.erase(0, 3);
						int m = stoi(temp_str);
						string f;
						cout << m << "&&&&" << endl;
						if (m % 2 == 1)//��Ϊ��
						{
							int t = m + 1;
							f = to_string(t);
						}
						else//��Ϊż
						{
							int t = m - 1;
							f = to_string(t);
						}
						string trans_next_node = dev1 + "-" + f;//͸��Ԫ������һ�߹ܽ���
						cout << trans_next_node << endl;
						vector<string>net_name_next = db.select_net_name(trans_next_node);//Ѱ��͸���ܽ���һ������������,��һ��Ԫ��
						vector<string>node_name_next = db.select_node_name(net_name_next[0]);//�����������������Ԫ��(�ܽ���)
						if (node_name_next.size() != 1)
						{
							for (auto it = node_name_next.begin(); it != node_name_next.end(); ++it)//node_name������TDI�����ܽ���
							{
								if (*it == trans_next_node)
								{
									it = node_name_next.erase(it);
									break;
								}
							}
						}
						string next_node = node_name_next[0];//node_name_nextΪ�ܽ���
						string N_tem = next_node;
						vector<string> Net_in = db.select_net_name(node_name_next[0]);
						next_node.erase(next_node.begin() + 2, next_node.end());
						vector<string>depend = db.select_Utype(next_node);//�ж���͸��ԭ��������������Ϣ�Ƿ�ΪBSDLоƬ
						bool sign = 0;
						string str2 = chain_info[v][3];
						for (auto& c : str2)//Сдת����д
						{
							c = toupper(c);
						}
						if (depend[0] == str2)
						{
							string next_node_num = N_tem.erase(0, 3);
							vector<string>logic_name = db.select_constant_logic_name(chain_info[v][3], next_node_num);
							bool tem = 0;
							for (auto x : In_node)
							{
								if (logic_name[0] == x)
								{
									tem = 1;
								}
							}
							if (tem == 1)//͸��Ԫ���ӵ�U��in��
							{ 
								string gr = Net_in[0];
								net_name.push_back(gr);
								Net_inout.push_back(net_name);
							}
							else//͸��ԭ�����ܽӷ���U,δ����in��
							{
								cout << "Wrong!Not connect with U_In bit,please check the circuit board" << endl;
								deci = 1;
								break;
							}
						}
						else
						{
							cout << "wrong ,too many transparent original" << endl;
							deci = 1;
							break;
						}
					}
					else if (trans_type[0] == str4)//OUT_NODEֱ��IN_NODE
					{
						string str5 = node_name[0];
						string next_node_num = str5.erase(0, 3);
						vector<string>logic_name = db.select_constant_logic_name(chain_info[v][3], next_node_num);
						bool tem = 0;
						for (auto x : In_node)
						{
							if (logic_name[0] == x)
							{
								tem = 1;
								break;
							}
						}
						if (tem == 1)//͸��Ԫ���ӵ�U��in��
						{
							string gr = net_name[0];
							net_name.push_back(gr);
							Net_inout.push_back(net_name);
						}
						else//͸��ԭ�����ܽӷ���U,δ����in��
						{
							cout << "Wrong!Not connect with U_In bit,please check the circuit board" << endl;
							deci = 1;
							break;
						}
					}
					//else if()//��ADC/DAC
					//{

					//}
					else//��ʱOut_node���ӵ���Ī�����������,�����սŴ���
					{
					   Net_Out_Unkown.push_back(net_name);
					}
				}
				else//��ʱ�ܽ�Ϊ����̬
				{
				     Net_out_Hang.push_back(net_name);
				}
			}
		}
		else
		{
		    cout << "Fail to Get right Net" << endl;
		    break;
        }
	}
	
}
void insert_ChainNet_info(MyDataBase db, const vector<vector<string>>& Net_inout,const vector<vector<string>>& Net_out_Hang, const vector<vector<string>>&Net_Out_Unkown, const vector<vector<string>>& Net_in_hang, const vector<vector<string>>& Net_In_Unkown)
{
	for (auto i = 0; i != Net_inout.size(); i++)
	{
		string tem1, tem2;
		tem1 = Net_inout[i][0];//OUT
		tem2 = Net_inout[i][1];//IN
		db.insert_port_table("T_NET_inout", tem1, tem2, "Out_Net", "In_Net");
	}
	for (auto i = 0; i != Net_out_Hang.size(); i++)
	{
		string tem1;
		tem1 = Net_out_Hang[i][0];
		db.insert_Hang_table("T_NET_Hang_out", tem1, "NET");
	}
	for (auto i = 0; i != Net_Out_Unkown.size(); i++)
	{
		string tem1;
		tem1 = Net_Out_Unkown[i][0];
		db.insert_Hang_table("T_NET_Unkown_Out", tem1, "NET");
	}
	for (auto i = 0; i != Net_In_Unkown.size(); i++)
	{
		string tem1;
		tem1 = Net_In_Unkown[i][0];
		db.insert_Hang_table("T_NET_Unkown_In", tem1, "NET");
	}
	for (auto i = 0; i != Net_in_hang.size(); i++)
	{
		string tem1;
		tem1 = Net_in_hang[i][0];
		db.insert_Hang_table("T_NET_Hang_in", tem1, "NET");
	}

}
void inNet(MyDataBase db, const vector<string>& Out_node, const vector<string>& In_node, const vector<vector<string>>& chain_info, const vector<vector<string>>& Net_inout, vector<vector<string>>& Net_in_Hang, vector<vector<string>>& Net_In_Unkown)
{
	for (auto v = 0; v != chain_info.size(); ++v)
	{
		for (auto j = 0; j != In_node.size(); ++j)
		{
			vector<string>comp = db.select_Chain_U(chain_info[v][0], chain_info[v][1]);//ȷ����һ��U
			vector<string>node = db.select_constant_io(chain_info[v][3], In_node[j]);//ȷ��U������
			string m = "-";
			string chain_pin = comp[0] + m + node[0];//����
			vector<string>net_name = db.select_net_name(chain_pin);//Ѱ�ҹܽ�����������,��һ��Ԫ��
			bool ghs = 0;
			for (auto m = 0; m != Net_inout.size(); ++m)//�����ɲ�����,ѡ������
			{
				if (net_name[0] == Net_inout[m][1])
				{
					ghs = 1;
					break;
				}
			}
			if (ghs != 1)//�ǿɲ�IN����
			{
				vector<string>node_name = db.select_node_name(net_name[0]);//�����������������Ԫ��(�ܽ���)��һ��Ԫ��(����������Ԫ�����)
				if (node_name.size() != 1)//��Ϊ���ս�
				{
					Net_In_Unkown.push_back(net_name);
				}
				else//��ʱ�ܽ�Ϊ����̬
				{
				    Net_in_Hang.push_back(net_name);
				}
			}
		}
	}
}