#include "databasehandle.h"

DatabaseHandle::DatabaseHandle(QObject *parent) :
    QObject(parent)
{
    //输出本系统可用数据库驱动
    qDebug() << "SqlDatabase available drivers:";
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
        qDebug() << driver;
}

//创建数据库
bool DatabaseHandle::createDBConnection()
{
#ifdef DB_VERSION_MYSQL
    //!连接mysql
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase(DB_VERSION_MYSQL);

    //db.setHostName(INTERFACEIP_Local);          //数据库本地IP
    db.setHostName(INTERFACEIP_STO);            //数据库公网IP
    db.setPort(INTERFACEPORT_INT);              //端口
    db.setDatabaseName(DB_NAME);                //数据库名称
    db.setUserName(INTERFACEUSERNAME);          //用户名
    db.setPassword(INTERFACEPASSWORD);          //用户密码
#else
    //!sqlite
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase(DB_VERSION);
    db.setDatabaseName(DB_NAME);
#endif
    if(!db.open())
    {
        dbConnectFlag = false;
        qDebug() << "open Database fail...";
        return false;
    }
    else
    {
        dbConnectFlag = true;
        qDebug() << "open Database OK...";
        execDBSql(USERMANAGEMENT_TABLE_CREATE, UserMCreateTableFlag);

        return true;
    }
}

//关闭数据库
void DatabaseHandle::closeDBConnection()
{
    QSqlDatabase::database().close();
    qDebug() << "close database!";
}

/**
 * @brief 字符串转化为sql参数,如'CLA_A'
 * @author cc
 * @param in
 * @return QString
 */
QString &DatabaseHandle::cat_str_sql(const QString &in)
{
    static QString out;
    out =  " '";
    out += in;
    out += "' ";
    return out;
}

/**
 * @brief   执行数据库语句
 * @author cc
 * @param   sqlStr        数据库操作语句
 * @param   tableFlag     表格标志类型
 * @return  QString       返回字符串,执行状态
 */
QString DatabaseHandle::execDBSql(const QString &sqlStr, const int tableFlag)
{
    if(tableFlag == UserManagementTableFlag)
        qDebug() << "execDBSql UserManagementTableFlag!";
    else if(tableFlag == UserMCreateTableFlag)
        qDebug() << "execDBSql UserMCreateTableFlag!";
    else if(tableFlag == UserMSignUpTableFlag)
        qDebug() << "execDBSql UserMSignUpTableFlag!";
    else if(tableFlag == UserMLoginTableFlag)
        qDebug() << "execDBSql UserMLoginTableFlag!";

    QSqlQuery query;

    if(!query.exec(sqlStr))
    {
        qDebug() << query.lastError().text();
        return DATABASE_WRITE_NO;
    }
    else
    {
        qDebug() << "execDBSql OK!";
        return DATABASE_WRITE_OK;
    }
}

//新注册用户将数据插入表格,手动分配UserID
QString DatabaseHandle::insert_UserManagementTable(const int UserID,
                                                   const QString &UserName,
                                                   const QString &UserPassword,
                                                   const QString &MobilePhoneNum,
                                                   const QString &EmailNum,
                                                   const QString &LoginFlag,
                                                   const QString &LoginIP,
                                                   const QString &LoginMAC,
                                                   const QString &UserLevel,
                                                   const QString &UsedTimes,
                                                   const QString &RegistrationTime)
{
    QString sql_temp;
    sql_temp = "INSERT INTO UserManagement (UserID, UserName, UserPassword,\
                MobilePhoneNum, EmailNum, LoginFlag, LoginIP, LoginMAC,\
                UserLevel, UsedTimes, RegistrationTime) VALUES( ";
    //sql_temp += cat_str_sql(UserID);
    sql_temp += QString::number(UserID);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UserName);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UserPassword);
    sql_temp += ", ";
    sql_temp += cat_str_sql(MobilePhoneNum);
    sql_temp += ", ";
    sql_temp += cat_str_sql(EmailNum);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginFlag);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginIP);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginMAC);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UserLevel);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UsedTimes);
    sql_temp += ", ";
    sql_temp += cat_str_sql(RegistrationTime);
    sql_temp += "); ";

    qDebug() << "insert_UserManagementTable: " << sql_temp;

    return execDBSql(sql_temp, UserMSignUpTableFlag);
}

//新注册用户将数据插入表格,自递增分配UserID
QString DatabaseHandle::insert_UserManagementTable_Auto(const QString &UserName,
                                                        const QString &UserPassword,
                                                        const QString &MobilePhoneNum,
                                                        const QString &EmailNum,
                                                        const QString &LoginFlag,
                                                        const QString &LoginIP,
                                                        const QString &LoginMAC,
                                                        const QString &UserLevel,
                                                        const QString &UsedTimes,
                                                        const QString &RegistrationTime)
{
    QString sql_temp;
    sql_temp = "INSERT INTO UserManagement (UserName, UserPassword,\
                MobilePhoneNum, EmailNum, LoginFlag, LoginIP, LoginMAC,\
                UserLevel, UsedTimes, RegistrationTime) VALUES( ";
//    sql_temp += QString::number(UserID);
//    sql_temp += ", ";
    sql_temp += cat_str_sql(UserName);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UserPassword);
    sql_temp += ", ";
    sql_temp += cat_str_sql(MobilePhoneNum);
    sql_temp += ", ";
    sql_temp += cat_str_sql(EmailNum);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginFlag);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginIP);
    sql_temp += ", ";
    sql_temp += cat_str_sql(LoginMAC);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UserLevel);
    sql_temp += ", ";
    sql_temp += cat_str_sql(UsedTimes);
    sql_temp += ", ";
    sql_temp += cat_str_sql(RegistrationTime);
    sql_temp += "); ";

    qDebug() << "insert_UserManagementTable_Auto: " << sql_temp;

    return execDBSql(sql_temp, UserMSignUpTableFlag);
}

//查询用户是否已被注册
int DatabaseHandle::inquireUserNameExist(const QString &userName)
{
    QSqlQuery query;
    QString queryString = QString("Select COUNT(*) from UserManagement \
                                   where UserName = '%1' ").arg(userName);
    int m_Exist = 0;
    if(query.exec(queryString)){

        while(query.next()){
            m_Exist = query.value(0).toInt();
        }
    }

    return m_Exist;
}

//查询用户密码
QString DatabaseHandle::inquireUserPassword(const QString &userName)
{
    QSqlQuery query;
    QString queryString = QString("Select UserPassword from UserManagement \
                                   where UserName = '%1' ").arg(userName);
    QString passwd;
    if(query.exec(queryString)){

        while(query.next()){
            passwd = query.value(0).toString();
        }
    }

    return passwd;
}

//查询用户ID,UserID本身属性是int,自递增
QString DatabaseHandle::inquireUserID(const QString &userName)
{
    QSqlQuery query;
    QString queryString = QString("Select UserID from UserManagement \
                                   where UserName = '%1' ").arg(userName);
    QString userID;
    if(query.exec(queryString)){

        while(query.next()){
            userID = query.value(0).toString();
        }
    }

    return userID;
}

//查询用户是否已经登陆
QString DatabaseHandle::inquireLoginFlag(const QString &userName)
{
    QSqlQuery query;
    QString queryString = QString("Select LoginFlag from UserManagement \
                                   where UserName = '%1' ").arg(userName);
    QString loginFlag;
    if(query.exec(queryString)){

        while(query.next()){
            loginFlag = query.value(0).toString();
        }
    }

    return loginFlag;
}

//更新用户登陆标示,上线1\下线0
bool DatabaseHandle::updateLoginFlag(const QString &userName,
                                        const QString &loginFlag)
{
    QSqlQuery query;

    QString queryString = QString("update UserManagement set LoginFlag = '%1' \
                                    where UserName = '%2' ")
                                    .arg(loginFlag).arg(userName);

    if(!query.exec(queryString))
        return false;
    return true;
}

//查询手机号是否已被注册
int DatabaseHandle::inquireMobilePhoneNumExist(const QString &phone)
{
    QSqlQuery query;
    QString queryString = QString("Select COUNT(*) from UserManagement \
                                   where MobilePhoneNum = '%1' ").arg(phone);
    int m_Exist = 0;
    if(query.exec(queryString)){

        while(query.next()){
            m_Exist = query.value(0).toInt();
        }
    }

    return m_Exist;
}

//查询邮箱是否已被注册
int DatabaseHandle::inquireEmailNumExist(const QString &email)
{
    QSqlQuery query;
    QString queryString = QString("Select COUNT(*) from UserManagement \
                                   where EmailNum = '%1' ").arg(email);
    int m_Exist = 0;
    if(query.exec(queryString)){

        while(query.next()){
            m_Exist = query.value(0).toInt();
        }
        qDebug() << "EmailNum m_Exist: " << m_Exist;
    }

    return m_Exist;
}
