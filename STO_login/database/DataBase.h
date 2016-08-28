#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>   //只读的数据模型

#include "GlobalData.h"

#define DB_VERSION              "QSQLITE"               //SQLite
#define DB_VERSION_MYSQL        "QMYSQL"                //MYSql
#define DB_VERSION_ODBC         "QODBC"                 //SQL Server2012

#define INTERFACEIP_STO         "139.196.47.109"        //默认服务器IP
#define INTERFACEIP_Local       "localhost"             //本地IP
#define INTERFACEPORT           "3306"                  //端口号
#define INTERFACEPORT_INT       3306                    //端口号(int)

#define INTERFACEUSERNAME       "root"                  //服务器用户名
#define INTERFACEPASSWORD       "Sto123456"             //访问密码
//#define INTERFACEPASSWORD       "123456"                //访问密码

#define DB_NAME                 "StoGodSystemDB"        //数据库名称
#define DB_NAME_TEST            "test"                  //数据库名称

#define CHINESELIBRARY_TABLE    "ChineselibrarySYS"     //中文汉字字库
#define USERMANAGEMENT_TABLE    "UserManagement"        //用户信息表格

//数据库表格标示
enum DatabaseTableFlag{
            UserManagementTableFlag = 1,                //用户信息表格
            UserMCreateTableFlag = 2,                   //用户信息表格创建标示
            UserMSignUpTableFlag = 3,                   //注册新用户
            UserMLoginTableFlag = 4,                    //用户登陆
};

/* 目前需要考虑的问题 @20160824
 *  1、实现自动连接网络还是手动连接，实现方式！如目前登录连接数据库失败，只能重新打开软件
 *  2、同一机器不能同时登陆两次当前账户，实现方式！考虑获取本机MAC地址，但是需要配合登陆状态
 *  3、如何及时正确的更新账户的登陆状态？ 此问题待解决！
 *  4、不同机器登陆账户，如何实现将之前的登陆账户先下线？
 * ......
 *
*/

/*=====用户信息表格=====
 * 用户ID			UserID                	//唯一性，系统给分配
 * 用户名			UserName                //唯一性，外键
 * 用户密码			UserPassword
 * 手机号			MobilePhoneNum			//也要是唯一性
 * 邮箱				EmailNum				//也要是唯一性
 * 登陆标示			LoginFlag				//异常掉线如何识别更改登陆标示?
 * 登陆IP			LoginIP					//当前电脑外网IP,识别异地登陆
 * 机器MAC地址		LoginMAC				//识别机器MAC地址,同一账号只能登陆一次
 * 权限级别			UserLevel				//普通注册权限是1
 * 权限时间			UsedTimes
 * 注册时间			RegistrationTime
 * 到期时间DueTime
 * 当前登陆时间CurrentLoginTime
 * 上次登录时间LastLoginTime//是否考虑单独创建新的表格存储这部分信息
*/

#ifdef DB_VERSION_MYSQL
//!mysql ID字段自递增 AUTO_INCREMENT
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
//!sqlite下创建表格,ID字段值自动递增,必须是INTEGER类型,关键字AUTOINCREMENT,与SQL Server有所不同!
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

//中文字库表格创建
#define CHINESELIBRARY_TABLE_CREATE  "CREATE TABLE ChineselibrarySYS \
(\
    id      INTEGER PRIMARY KEY AUTOINCREMENT, \
    py      TEXT,                \
    word    TEXT              \
);"

//查询中文汉字库(拼音与汉字)
#define INQUIRE_PY_WORD_DATA        "select PY, WORD from ChineselibrarySYS"

#endif // DATABASE_H
