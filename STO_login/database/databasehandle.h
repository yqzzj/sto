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

    //!ע���û�
    //��ע���û������ݲ�����,�ֶ�����UserID
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

    //��ע���û������ݲ�����,�Ե�������UserID
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

    //��ѯ�û��Ƿ��ѱ�ע��
    static int inquireUserNameExist(const QString &userName);

    //��ѯ�û�����
    static QString inquireUserPassword(const QString &userName);

    //��ѯ�û�ID
    static QString inquireUserID(const QString &userName);

    //��ѯ�û��Ƿ��Ѿ���½
    static QString inquireLoginFlag(const QString &userName);

    //�����û���½��ʾ,����1\����0
    static bool updateLoginFlag(const QString &userName,
                                   const QString &loginFlag);

    //��ѯ�ֻ����Ƿ��ѱ�ע��
    static int inquireMobilePhoneNumExist(const QString &phone);

    //��ѯ�����Ƿ��ѱ�ע��
    static int inquireEmailNumExist(const QString &email);

signals:

public slots:

};

#endif // DATABASEHANDLE_H
