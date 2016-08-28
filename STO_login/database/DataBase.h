#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>   //ֻ��������ģ��

#include "GlobalData.h"

#define DB_VERSION              "QSQLITE"               //SQLite
#define DB_VERSION_MYSQL        "QMYSQL"                //MYSql
#define DB_VERSION_ODBC         "QODBC"                 //SQL Server2012

#define INTERFACEIP_STO         "139.196.47.109"        //Ĭ�Ϸ�����IP
#define INTERFACEIP_Local       "localhost"             //����IP
#define INTERFACEPORT           "3306"                  //�˿ں�
#define INTERFACEPORT_INT       3306                    //�˿ں�(int)

#define INTERFACEUSERNAME       "root"                  //�������û���
#define INTERFACEPASSWORD       "Sto123456"             //��������
//#define INTERFACEPASSWORD       "123456"                //��������

#define DB_NAME                 "StoGodSystemDB"        //���ݿ�����
#define DB_NAME_TEST            "test"                  //���ݿ�����

#define CHINESELIBRARY_TABLE    "ChineselibrarySYS"     //���ĺ����ֿ�
#define USERMANAGEMENT_TABLE    "UserManagement"        //�û���Ϣ���

//���ݿ����ʾ
enum DatabaseTableFlag{
            UserManagementTableFlag = 1,                //�û���Ϣ���
            UserMCreateTableFlag = 2,                   //�û���Ϣ��񴴽���ʾ
            UserMSignUpTableFlag = 3,                   //ע�����û�
            UserMLoginTableFlag = 4,                    //�û���½
};

/* Ŀǰ��Ҫ���ǵ����� @20160824
 *  1��ʵ���Զ��������绹���ֶ����ӣ�ʵ�ַ�ʽ����Ŀǰ��¼�������ݿ�ʧ�ܣ�ֻ�����´����
 *  2��ͬһ��������ͬʱ��½���ε�ǰ�˻���ʵ�ַ�ʽ�����ǻ�ȡ����MAC��ַ��������Ҫ��ϵ�½״̬
 *  3����μ�ʱ��ȷ�ĸ����˻��ĵ�½״̬�� ������������
 *  4����ͬ������½�˻������ʵ�ֽ�֮ǰ�ĵ�½�˻������ߣ�
 * ......
 *
*/

/*=====�û���Ϣ���=====
 * �û�ID			UserID                	//Ψһ�ԣ�ϵͳ������
 * �û���			UserName                //Ψһ�ԣ����
 * �û�����			UserPassword
 * �ֻ���			MobilePhoneNum			//ҲҪ��Ψһ��
 * ����				EmailNum				//ҲҪ��Ψһ��
 * ��½��ʾ			LoginFlag				//�쳣�������ʶ����ĵ�½��ʾ?
 * ��½IP			LoginIP					//��ǰ��������IP,ʶ����ص�½
 * ����MAC��ַ		LoginMAC				//ʶ�����MAC��ַ,ͬһ�˺�ֻ�ܵ�½һ��
 * Ȩ�޼���			UserLevel				//��ͨע��Ȩ����1
 * Ȩ��ʱ��			UsedTimes
 * ע��ʱ��			RegistrationTime
 * ����ʱ��DueTime
 * ��ǰ��½ʱ��CurrentLoginTime
 * �ϴε�¼ʱ��LastLoginTime//�Ƿ��ǵ��������µı��洢�ⲿ����Ϣ
*/

#ifdef DB_VERSION_MYSQL
//!mysql ID�ֶ��Ե��� AUTO_INCREMENT
//UserID              int AUTO_INCREMENT primary key(mysql)

#define USERMANAGEMENT_TABLE_CREATE  "CREATE TABLE UserManagement \
(\
    UserID              int AUTO_INCREMENT primary key,\
    UserName            varchar(50) not null unique,\
    UserPassword        varchar(50) not null,\
    MobilePhoneNum      varchar(50) not null unique,\
    EmailNum            varchar(50) not null unique,\
    LoginFlag           numeric(1,0) not null DEFAULT 0,\
    LoginIP             varchar(50) not null,\
    LoginMAC            varchar(50) not null,\
    UserLevel           varchar(10) not null,\
    UsedTimes           varchar(50) not null,\
    RegistrationTime    varchar(50) not null\
);"

#else
//!sqlite�´������,ID�ֶ�ֵ�Զ�����,������INTEGER����,�ؼ���AUTOINCREMENT,��SQL Server������ͬ!
//UserID              INTEGER primary key AUTOINCREMENT(sqlite)

#define USERMANAGEMENT_TABLE_CREATE  "CREATE TABLE UserManagement \
(\
    UserID              INTEGER primary key AUTOINCREMENT,\
    UserName            varchar(50) not null unique,\
    UserPassword        varchar(50) not null,\
    MobilePhoneNum      varchar(50) not null unique,\
    EmailNum            varchar(50) not null unique,\
    LoginFlag           numeric(1,0) not null DEFAULT 0,\
    LoginIP             varchar(50) not null,\
    LoginMAC            varchar(50) not null,\
    UserLevel           varchar(10) not null,\
    UsedTimes           varchar(50) not null,\
    RegistrationTime    varchar(50) not null\
);"
#endif

//�����ֿ��񴴽�
#define CHINESELIBRARY_TABLE_CREATE  "CREATE TABLE ChineselibrarySYS \
(\
    id      INTEGER PRIMARY KEY AUTOINCREMENT, \
    py      TEXT,                \
    word    TEXT              \
);"

//��ѯ���ĺ��ֿ�(ƴ���뺺��)
#define INQUIRE_PY_WORD_DATA        "select PY, WORD from ChineselibrarySYS"

#endif // DATABASE_H
