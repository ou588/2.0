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
Linklistuser headnode = new pNodeuser;//ȫ�ֱ���

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

	// ��ʼ�����Ӿ��
	conn = mysql_init(NULL);

	//����Ϊ�����ݿ�ͬ����utf8�ַ���
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
	cout << "�������û�����" << endl;
	read3:
	cin >> s1;
	while (p) {
		if (p->data.username == s1) 
		{
			cout << "���û��Ѵ���,���������룺" << endl;
			p = headnode;
			goto read3;
		}
		else
		{
			p = p->next;
		}
	}
	cout << "���������룺" << endl;
	cin >> s2;
	cout << "���ٴ���������" << endl;
	read4:
	cin >> s3;
	if (s2 != s3) {
		cout << "���벻һ�£�����������:" << endl;
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
	cout << "��ѡ��" << endl;
	cout << "1.�ʾ����" << endl;
	cout << "2.ʫ�ʿ�" << endl;
	cout << "3.ʫ��������" << endl;
	cout << "0.�˳���¼" << endl;
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
	//��ȡ
	char select[200] = "select * from user ";
	//���������ַ���,���������趨
	mysql_query(&m_sql, "set names gbk");
	mysql_query(&m_sql, select);
	MYSQL_RES* result;
	result = mysql_store_result(&m_sql);
	//��ӡ�б���
	MYSQL_FIELD* fields;
	fields = mysql_fetch_fields(result);  //����������
	auto num = mysql_num_fields(result);
	//ȡ���б�ֵ,������������
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

//��¼����
void login() {
	int x;
	string s1, s2;
	string account_in;
	cout << "**********" << endl;
	cout << "��ѡ���¼��ʽ�������Ӧ����" << endl;
	cout << "1.����Ա��¼/t/t2.�û���¼/t/t3.�û�ע��" << endl;
	cin >> x;
	if (x == 1) {
		exit1:
		cout << "�������û���" << endl;
		cin >> s1;
		if (s1 == "ou")
		{
			exit2:
			cout << "����������" << endl;
			cin >> s2;
			if (s2 == "123")
			{
				cout << "��¼�ɹ�" << endl;
				menu();
			}
			else 
			{
				cout << "���������������������" << endl;
				goto exit2;
			}
		}
		else {
			cout << "�û��������������������" << endl;
			goto exit1;
		}
	}
	if (x == 2)
	{
		Linklistuser p=new pNodeuser;
		p = headnode;
		read_db();
		cout << "�������û���:"<<endl;
		read1:
		cin >> s1;
		while (p)
		{
			if (p->data.username == s1)
			{
				cout << "����������:" << endl;
				read2:
				cin >> s2;
				if (p->data.userpassword == s2)
				{
					cout << "��¼�ɹ�" << endl;
					menu();
				}
				else 
				{
					cout << "�������,����������:" << endl;
					goto read2;
				}
			}
			else
			{
				p = p->next;
			}
		}
		cout << "���޴��û�,���������룺" << endl;
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
	struct node* lchild;//�ظ�����
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