#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>

#include "signupwidget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool eventFilter(QObject *target, QEvent *event);

    void resizeEvent(QResizeEvent *);

    //判断IP地址及端口是否在线
    bool IPLive(QString ip, int port);

    //登陆账户
    void loginAccount();
    //注册账户
    void signUpAccount();
    //退出程序
    void exitWidget();
    //忘记密码
    void forgotPassword();

    bool isOk;

public slots:
    //更新网络连接状态
    void updateConnectNetStatus();

    //更新上报连接数据库状态
    void updateConnectDBStatus();

private slots:
    void on_pushButton_SignUp_clicked();

    void on_pushButton_ForgotPassword_clicked();

    void on_pushButton_Login_clicked();

    void on_pushButton_Exit_clicked();

private:
    Ui::Widget *ui;

    SignUpWidget *m_signUpWiget;

    QRegExpValidator *m_RegExpValidatorUserName;
    QRegExpValidator *m_RegExpValidatorPassWord;
};

#endif // WIDGET_H
