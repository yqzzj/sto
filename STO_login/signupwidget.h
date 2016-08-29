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

    //ȷ��ע���˺�
    void confirmSignUpUser();
    //�鿴ע������
    void lookSignUpClause();

    //�ж��û�����Ϣ
    bool checkUserNameInfo(const QString &);
    //�ж��ֻ�����Ϣ
    bool checkPhoneInfo(const QString &);
    //�ж�������Ϣ
    bool checkEmailInfo(const QString &);
    //�ж�������Ϣ
    bool checkPasswordInfo(const QString &);


    //��ȡ����MAC��ַ
    QString getLocalMachineMAC();

    //��ȡ��������IP
    QString getPublicNetworkIP();

    //��ȡ��ҳ����Դ����
    void GetHtml(QString url);

    //��ӡ�ӿ�������Ϣ
    void qDebug_NetworkInterfaceInfo();

    //��ȡ����IP
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

    QStringList local_ip;               //����IP
    QString m_publicNetworkIP;          //��������IP
};

#endif // SIGNUPWIDGET_H
