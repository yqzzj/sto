#ifndef DATABASEHANDLE_H
#define DATABASEHANDLE_H

#include <QSqlError>
#include <QMutex>

#include "database/DataBase.h"

class DatabaseHandle : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandle(QObject *parent = 0);

    static bool createDBConnection();
    static void closeDBConnection();

    static QString &cat_str_sql(const QString &in);

    static QString execDBSql(const QString &sqlStr, const int tableFlag);

    //!注册用户
    //新注册用户将数据插入表格,手动分配UserID
    static QString insert_UserManagementTable(const int UserID,
                                     const QString &UserName,
                                     const QString &UserPassword,
                                     const QString &MobilePhoneNum,
                                     const QString &EmailNum,
                                     const QString &LoginFlag,
                                     const QString &LoginIP,
                                     const QString &LoginMAC,
                                     const QString &UserLevel,
                                     const QString &UsedTimes,
                                     const QString &RegistrationTime);

    //新注册用户将数据插入表格,自递增分配UserID
    static QString insert_UserManagementTable_Auto(const QString &UserName,
                                     const QString &UserPassword,
                                     const QString &MobilePhoneNum,
                                     const QString &EmailNum,
                                     const QString &LoginFlag,
                                     const QString &LoginIP,
                                     const QString &LoginMAC,
                                     const QString &UserLevel,
                                     const QString &UsedTimes,
                                     const QString &RegistrationTime);

    //查询用户是否已被注册
    static int inquireUserNameExist(const QString &userName);

    //查询用户密码
    static QString inquireUserPassword(const QString &userName);

    //查询用户ID
    static QString inquireUserID(const QString &userName);

    //查询用户是否已经登陆
    static QString inquireLoginFlag(const QString &userName);

    //更新用户登陆标示,上线1\下线0
    static bool updateLoginFlag(const QString &userName,
                                   const QString &loginFlag);

    //查询手机号是否已被注册
    static int inquireMobilePhoneNumExist(const QString &phone);

    //查询邮箱是否已被注册
    static int inquireEmailNumExist(const QString &email);

signals:

public slots:

};

#endif // DATABASEHANDLE_H
