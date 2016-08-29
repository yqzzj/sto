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

    //�ж�IP��ַ���˿��Ƿ�����
    bool IPLive(QString ip, int port);

    //��½�˻�
    void loginAccount();
    //ע���˻�
    void signUpAccount();
    //�˳�����
    void exitWidget();
    //��������
    void forgotPassword();

    bool isOk;

public slots:
    //������������״̬
    void updateConnectNetStatus();

    //�����ϱ��������ݿ�״̬
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
