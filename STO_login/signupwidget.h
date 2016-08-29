#ifndef SIGNUPWIDGET_H
#define SIGNUPWIDGET_H

#include <QWidget>
#include "GlobalData.h"
#include "database/databasehandle.h"

#include <QRegExpValidator>
#include <QRegExp>

#include <QNetworkInterface>
#include <QtNetwork>
#include <QHostAddress>

#include <QKeyEvent>
#include <QEvent>
#include <QMouseEvent>

namespace Ui {
class SignUpWidget;
}

class SignUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpWidget(QWidget *parent = 0);
    ~SignUpWidget();

    bool eventFilter(QObject *target, QEvent *event);

    void resizeEvent(QResizeEvent *);

    void resetSignUpWidget();

    //确认注册账号
    void confirmSignUpUser();
    //查看注册条款
    void lookSignUpClause();

    //判断用户名信息
    bool checkUserNameInfo(const QString &);
    //判断手机号信息
    bool checkPhoneInfo(const QString &);
    //判断邮箱信息
    bool checkEmailInfo(const QString &);
    //判断密码信息
    bool checkPasswordInfo(const QString &);


    //获取本机MAC地址
    QString getLocalMachineMAC();

    //获取本机公网IP
    QString getPublicNetworkIP();

    //获取网页所有源代码
    void GetHtml(QString url);

    //打印接口网络信息
    void qDebug_NetworkInterfaceInfo();

    //获取本地IP
    QStringList getLocalHostIP();

private slots:
    void on_pushButton_signup_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_clause_clicked();

private:
    Ui::SignUpWidget *ui;

    QRegExpValidator *m_REVIP;
    QRegExpValidator *m_REVUserName;
    QRegExpValidator *m_REVPhone;
    QRegExpValidator *m_REVEmail;
    QRegExpValidator *m_REVPassWord;

    QStringList local_ip;               //本地IP
    QString m_publicNetworkIP;          //本机外网IP
};

#endif // SIGNUPWIDGET_H
