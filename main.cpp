#include<mysql.h>
#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#define _CRT_SECURE_NO_WARNINGS
#define Maxsize 100
using namespace std;

void poem();
void search();
void comments();
void login();
void read_db();
void menu();

const char* host = "127.0.0.1";
const char* user = "root";
const char* pw = "123456";
const char* database_name = "db";
const int port = 3306;

typedef struct User {
	int userid;
	string username;
	string userpassword;
}User;
typedef struct User_detail {
	int id;
	string username;
	string userpassword;
}User_detail;
typedef struct pNodeuser {
	User_detail data;
	struct pNodeuser* next;
}pNodeuser, * Linklistuser;
Linklistuser headnode = new pNodeuser;//全局变量

int  mysql_connect() {
	MYSQL* con = mysql_init(NULL);
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
	{
		cout << "Failed to connect to database : Error:" << mysql_error(con);
	}
	return 0;
}
MYSQL* get_connection()
{
	MYSQL* conn;

	// 初始化连接句柄
	conn = mysql_init(NULL);

	//配置为和数据库同步的utf8字符集
	mysql_set_character_set(conn, "gbk");


	if (!mysql_real_connect(conn, host, user,pw, database_name, port, NULL, 0))
	{
		cout << mysql_error(conn) << endl;
		return NULL;
	}
	return conn;

}
void  mysql_insert()
{
	MYSQL m_sql;
	mysql_init(&m_sql);
	Linklistuser p = new pNodeuser;
	p = headnode;
	string s1, s2,s3;
	headnode->next = NULL;
	read_db();
	cout << "请输入用户名：" << endl;
	read3:
	cin >> s1;
	while (p) {
		if (p->data.username == s1) 
		{
			cout << "此用户已存在,请重新输入：" << endl;
			p = headnode;
			goto read3;
		}
		else
		{
			p = p->next;
		}
	}
	cout << "请输入密码：" << endl;
	cin >> s2;
	cout << "请再次输入密码" << endl;
	read4:
	cin >> s3;
	if (s2 != s3) {
		cout << "密码不一致，请重新输入:" << endl;
		goto read4;
	}
	else {
		MYSQL* con = mysql_init(NULL);
		mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
		if (!mysql_real_connect(con, host, user, pw, database_name, port, NULL, 0))
		{
			cout << "Failed to connect to database : Error:" << mysql_error(con);
		}
		char sql[1024];
		sprintf(sql, "insert into user (username,userpassword) values('%s','%s')", s1.c_str(), s2.c_str());
		if (mysql_query(con, sql)) {
			fprintf(stderr, "Failed to insert data:Error:%s", mysql_error(con));
		}
		menu();
	}
}
void save(User u1)
{
	ofstream ofile;
	ofile.open("user.txt", ios::out);
	ofile << u1.username << endl;
	ofile << u1.userpassword<< endl;
	ofile.close();
};



void menu() {
	int y;
	cout << "**********" << endl;
	cout << "请选择：" << endl;
	cout << "1.词句检索" << endl;
	cout << "2.诗词库" << endl;
	cout << "3.诗词评论区" << endl;
	cout << "0.退出登录" << endl;
	cin >> y;
	switch (y) {
	case 1:
		search();
	case 2:
		poem();
	case 3:
		comments();
	case 0:
		login();
	}
}

void pNode_initial(Linklistuser &p){
	p = new pNodeuser;
	p->next = NULL;
};

void read_db(){
	headnode->next = NULL;
	MYSQL m_sql;
	mysql_init(&m_sql);
	if (!mysql_real_connect(&m_sql, "localhost", "root", "123456", "db", 3306, NULL, 0))
	{
		cout << "fail" << endl;
	}
	//读取
	char select[200] = "select * from user ";
	//各种语言字符集,根据需求设定
	mysql_query(&m_sql, "set names gbk");
	mysql_query(&m_sql, select);
	MYSQL_RES* result;
	result = mysql_store_result(&m_sql);
	//打印列表名
	MYSQL_FIELD* fields;
	fields = mysql_fetch_fields(result);  //返回所有列
	auto num = mysql_num_fields(result);
	//取出列表值,并存入链表中
	MYSQL_ROW row;
	unsigned int num_c;
	num_c = mysql_num_fields(result);
	while (row = mysql_fetch_row(result))
	{
		Linklistuser pnodeuser = new pNodeuser;
		pnodeuser->data.id = int(row[0]);
		pnodeuser->data.username = row[1];
		pnodeuser->data.userpassword = row[2];
		pnodeuser->next = headnode->next;
		headnode->next = pnodeuser;
	}
	mysql_free_result(result);
}

//登录界面
void login() {
	int x;
	string s1, s2;
	string account_in;
	cout << "**********" << endl;
	cout << "请选择登录方式，输入对应数字" << endl;
	cout << "1.管理员登录/t/t2.用户登录/t/t3.用户注册" << endl;
	cin >> x;
	if (x == 1) {
		exit1:
		cout << "请输入用户名" << endl;
		cin >> s1;
		if (s1 == "ou")
		{
			exit2:
			cout << "请输入密码" << endl;
			cin >> s2;
			if (s2 == "123")
			{
				cout << "登录成功" << endl;
				menu();
			}
			else 
			{
				cout << "密码输入错误，请重新输入" << endl;
				goto exit2;
			}
		}
		else {
			cout << "用户名输入错误，请重新输入" << endl;
			goto exit1;
		}
	}
	if (x == 2)
	{
		Linklistuser p=new pNodeuser;
		p = headnode;
		read_db();
		cout << "请输入用户名:"<<endl;
		read1:
		cin >> s1;
		while (p)
		{
			if (p->data.username == s1)
			{
				cout << "请输入密码:" << endl;
				read2:
				cin >> s2;
				if (p->data.userpassword == s2)
				{
					cout << "登录成功" << endl;
					menu();
				}
				else 
				{
					cout << "密码错误,请重新输入:" << endl;
					goto read2;
				}
			}
			else
			{
				p = p->next;
			}
		}
		cout << "暂无此用户,请重新输入：" << endl;
		p = headnode;
		goto read1;
	}
	if (x == 3)
	{
		mysql_insert();
	}
}
void poem() {


};
void search() {

};
typedef struct node {
	int id;
	int p_id;
	struct node* lchild;//回复评论
	struct node* rbro;
	struct node* par;
	string comment;
	int poem_id;
	int	lflag;
	int rflag;
}node;
void comments() {

};

int main()
{
	
	login();
	
};