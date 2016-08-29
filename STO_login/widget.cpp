#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(500, 430);
    this->setWindowTitle(QString::fromLocal8Bit("StoGodע���¼����@20160825"));

    m_signUpWiget = new SignUpWidget;

    isOk = false;

    m_signUpWiget->hide();

    //������ʽ�û�������  �����ġ������пո�
    QRegExp regExpUserName("[^\u4E00-\u9FA5^| ]{5,16}");
    m_RegExpValidatorUserName = new QRegExpValidator(regExpUserName, this);
    ui->lineEdit_Account->setValidator(m_RegExpValidatorUserName);
    //������6-20λ��ĸ�����ֺͷ����������������ַ���ϣ����ִ�Сд����ĸ��ͷ
    //QRegExp regExpPassWord("^[a-zA-Z]\w{6,20}$");
    QRegExp regExpPassWord("[^\u4E00-\u9FA5^| ]{6,20}");    //������
    m_RegExpValidatorPassWord = new QRegExpValidator(regExpPassWord, this);
    ui->lineEdit_Password->setValidator(m_RegExpValidatorPassWord);

    //!���ü��̰����¼�
    ui->lineEdit_Account->installEventFilter(this);
    ui->lineEdit_Password->installEventFilter(this);
    ui->pushButton_Login->installEventFilter(this);
    ui->pushButton_Exit->installEventFilter(this);
    ui->pushButton_SignUp->installEventFilter(this);
    ui->pushButton_ForgotPassword->installEventFilter(this);
    QWidget::installEventFilter(this);

    updateConnectNetStatus();

    updateConnectDBStatus();
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->lineEdit_Account || target == ui->lineEdit_Password
     || target == ui->pushButton_Login || target == ui->pushButton_Exit
     || target == ui->pushButton_SignUp || target == ui->pushButton_ForgotPassword
     )
    {
        // �����¼�
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Escape)          //Esc��
            {
                exitWidget();               //�˳�����
                return true;
            }
            if ((keyEvent->key() == Qt::Key_Enter)          //�س���
                || (keyEvent->key() == Qt::Key_Return))     //���ؼ�
            {
                if (target == ui->lineEdit_Account)
                {
                    ui->lineEdit_Password->setFocus();
                    ui->lineEdit_Password->selectAll();
                    return true;
                }
                if (target == ui->lineEdit_Password)
                {
                    ui->pushButton_Login->setFocus();

                    loginAccount();         //��½�˻�
                    return true;
                }
                if (target == ui->pushButton_Login)
                {
                    loginAccount();         //��½�˻�
                    return true;
                }
                if (target == ui->pushButton_SignUp)
                {
                    signUpAccount();        //ע���˻�
                    return true;
                }
                if (target == ui->pushButton_ForgotPassword)
                {
                    forgotPassword();       //��������
                    return true;
                }
                if (target == ui->pushButton_Exit)
                {
                    exitWidget();           //�˳�����
                    return true;
                }
            }
        }
    }

    return QWidget::eventFilter(target, event);
}

void Widget::resizeEvent(QResizeEvent *)
{
    this->setAutoFillBackground(true);

    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush("#cdcbff"));
    this->setPalette(palette);
}

// �ж�IP��ַ���˿��Ƿ����ߣ������������Ƿ����������ӰٶȲ���
bool Widget::IPLive(QString ip, int port)
{
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100����û�����������жϲ�����
    return tcpClient.waitForConnected(100);
}

// ������������״̬, 1000ms
void Widget::updateConnectNetStatus()
{
    //�������������Ƿ�����
    if(IPLive("www.baidu.com", 80))
    {
        NetConnectFlag = true;
        //qDebug() << "connect baidu OK!";
    }
    else
    {
        NetConnectFlag = false;
        //qDebug() << "connect baidu fail!";
    }
    //
    QTimer::singleShot(1000 , this , SLOT(updateConnectNetStatus()));
}

// �����ϱ��������ݿ�״̬, 1000ms
void Widget::updateConnectDBStatus()
{
    if(NetConnectFlag)      //��������
    {
        if(dbConnectFlag)   //���ݿ�����
        {
            ui->label_connectStatus->hide();
            ui->label_connectStatus->setText(QString::fromLocal8Bit("��������������"));
        }
        else
        {
            ui->label_connectStatus->show();
            ui->label_connectStatus->setText(QString::fromLocal8Bit("���ӷ������������⣬�����������������ϵ�ۺ�"));
        }
    }
    else
    {
        ui->label_connectStatus->show();
        ui->label_connectStatus->setText(QString::fromLocal8Bit("�������ӳ������⣬�����������ӻ����ǽ����������������"));
    }

    //
    QTimer::singleShot(1000 , this , SLOT(updateConnectDBStatus()));
}

// ��½�˻�
void Widget::loginAccount()
{
    if(!dbConnectFlag)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����������ӻ��߲鿴����ǽ���ã�\n"));
        return;
    }

    QString username = ui->lineEdit_Account->text();

    if(username.isEmpty() || ui->lineEdit_Password->text().isEmpty() )
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�û��������벻��Ϊ�գ�\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Account->setFocus();
        return;
    }

    //��ѯ���ݿ��û��Ƿ����
    int userExist = DatabaseHandle::inquireUserNameExist(username);

    if(userExist == 0)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n��ǰ�˻������ڣ�\n"));
        ui->lineEdit_Account->clear();
        ui->lineEdit_Account->setFocus();
        return;
    }

    if(!m_RegExpValidatorPassWord->regExp()
            .exactMatch(ui->lineEdit_Password->text()))
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�˻������ʽ��ƥ�䣡\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Password->setFocus();
        return;
    }

    //��ѯ�˻������Ƿ�ƥ��
    QString passwd = DatabaseHandle::inquireUserPassword(username);

    //��ѯ�û��Ƿ��½���鿴��½��ʾ
#if 0
    QString loginFlag = DatabaseHandle::inquireLoginFlag(username);

    if(loginFlag.toInt() == GoOnlineFlag)
    {
        //ͬһ̨�����ϲ���ͬʱ��½����
        //��ͬ�����ϵ�½��ν��Ѿ���¼���˻�����(��Ҫ�������Ϳͻ��˹�ͬ��ʵ��)
        //
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n��ǰ�˻��Ѿ���½�������ظ���½��\n"));
        return;
    }
#endif

    if(passwd == ui->lineEdit_Password->text())
    {
        //�û����߸��µ�½��ʾ
        bool ok = DatabaseHandle::updateLoginFlag(username, "1");
        isOk = ok;
        qDebug() << "isOk: " << isOk;
        if(ok)
        {
            currentLoginID = DatabaseHandle::inquireUserID(username).toInt();
            qDebug() << "current Login userID: " << currentLoginID;

            //myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n�˻�'%1'��½�ɹ���\n").arg(username));

            //!��ʾ������������
            ui->lineEdit_Account->clear();
            ui->lineEdit_Password->clear();
            ui->lineEdit_Account->setFocus();
            exitWidget();
        }
        else
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�˻����ߵ�½��ʾ����ʧ�ܣ�\n"));
        }
    }
    else
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n��������������������룡\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Password->setFocus();
    }
}

// ע���˺�
void Widget::signUpAccount()
{
    if(dbConnectFlag)
        m_signUpWiget->show();
    else
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����������ӻ��߲鿴����ǽ���ã�\n"));
}

// �˳�����
void Widget::exitWidget()
{
    //������ʾ�Ի���ѡ���Ƿ��˳�����
//    int ok = myHelper::ShowMessageBoxQuesionStandard(QString::fromLocal8Bit("\n ȷ��Ҫ�˳���ǰ������ \n"));

//    if(ok == QMessageBox::No)
//        return;

    this->close();
    qDebug() << "Widget close!";

    qApp->quit();
    qDebug() << "quit App!";
}

// ��������
void Widget::forgotPassword()
{
    if(!dbConnectFlag)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����������ӻ��߲鿴����ǽ���ã�\n"));
        return;
    }
}

///////////////// pushButton�����¼� /////////////////

// ����ע���˺Ž���
void Widget::on_pushButton_SignUp_clicked()
{
    signUpAccount();
}

// �һ�����
void Widget::on_pushButton_ForgotPassword_clicked()
{
    forgotPassword();
}

// ��½
void Widget::on_pushButton_Login_clicked()
{
    loginAccount();
}

//�˳�
void Widget::on_pushButton_Exit_clicked()
{
    exitWidget();
}
