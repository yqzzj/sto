#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setFixedSize(500, 430);
    this->setWindowTitle(QString::fromLocal8Bit("StoGod注册登录界面@20160825"));

    m_signUpWiget = new SignUpWidget;

    isOk = false;

    m_signUpWiget->hide();

    //正则表达式用户名设置  非中文、不能有空格
    QRegExp regExpUserName("[^\u4E00-\u9FA5^| ]{5,16}");
    m_RegExpValidatorUserName = new QRegExpValidator(regExpUserName, this);
    ui->lineEdit_Account->setValidator(m_RegExpValidatorUserName);
    //密码由6-20位字母，数字和符号至少两种以上字符组合，区分大小写，字母开头
    //QRegExp regExpPassWord("^[a-zA-Z]\w{6,20}$");
    QRegExp regExpPassWord("[^\u4E00-\u9FA5^| ]{6,20}");    //非中文
    m_RegExpValidatorPassWord = new QRegExpValidator(regExpPassWord, this);
    ui->lineEdit_Password->setValidator(m_RegExpValidatorPassWord);

    //!配置键盘按键事件
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
        // 键盘事件
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Escape)          //Esc键
            {
                exitWidget();               //退出程序
                return true;
            }
            if ((keyEvent->key() == Qt::Key_Enter)          //回车键
                || (keyEvent->key() == Qt::Key_Return))     //返回键
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

                    loginAccount();         //登陆账户
                    return true;
                }
                if (target == ui->pushButton_Login)
                {
                    loginAccount();         //登陆账户
                    return true;
                }
                if (target == ui->pushButton_SignUp)
                {
                    signUpAccount();        //注册账户
                    return true;
                }
                if (target == ui->pushButton_ForgotPassword)
                {
                    forgotPassword();       //忘记密码
                    return true;
                }
                if (target == ui->pushButton_Exit)
                {
                    exitWidget();           //退出程序
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

// 判断IP地址及端口是否在线，即网络连接是否正常，连接百度测试
bool Widget::IPLive(QString ip, int port)
{
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(100);
}

// 更新网络连接状态, 1000ms
void Widget::updateConnectNetStatus()
{
    //测试网络连接是否正常
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

// 更新上报连接数据库状态, 1000ms
void Widget::updateConnectDBStatus()
{
    if(NetConnectFlag)      //网络连接
    {
        if(dbConnectFlag)   //数据库连接
        {
            ui->label_connectStatus->hide();
            ui->label_connectStatus->setText(QString::fromLocal8Bit("网络连接正常！"));
        }
        else
        {
            ui->label_connectStatus->show();
            ui->label_connectStatus->setText(QString::fromLocal8Bit("连接服务器出现问题，请重新启动程序或联系售后！"));
        }
    }
    else
    {
        ui->label_connectStatus->show();
        ui->label_connectStatus->setText(QString::fromLocal8Bit("网络连接出现问题，请检查网络连接或防火墙，并重新启动程序！"));
    }

    //
    QTimer::singleShot(1000 , this , SLOT(updateConnectDBStatus()));
}

// 登陆账户
void Widget::loginAccount()
{
    if(!dbConnectFlag)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请检查网络连接或者查看防火墙设置！\n"));
        return;
    }

    QString username = ui->lineEdit_Account->text();

    if(username.isEmpty() || ui->lineEdit_Password->text().isEmpty() )
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n用户名或密码不能为空！\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Account->setFocus();
        return;
    }

    //查询数据库用户是否存在
    int userExist = DatabaseHandle::inquireUserNameExist(username);

    if(userExist == 0)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n当前账户不存在！\n"));
        ui->lineEdit_Account->clear();
        ui->lineEdit_Account->setFocus();
        return;
    }

    if(!m_RegExpValidatorPassWord->regExp()
            .exactMatch(ui->lineEdit_Password->text()))
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n账户密码格式不匹配！\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Password->setFocus();
        return;
    }

    //查询账户密码是否匹配
    QString passwd = DatabaseHandle::inquireUserPassword(username);

    //查询用户是否登陆，查看登陆标示
#if 0
    QString loginFlag = DatabaseHandle::inquireLoginFlag(username);

    if(loginFlag.toInt() == GoOnlineFlag)
    {
        //同一台机器上不能同时登陆两次
        //不同机器上登陆如何将已经登录的账户下线(需要服务器和客户端共同来实现)
        //
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n当前账户已经登陆，不能重复登陆！\n"));
        return;
    }
#endif

    if(passwd == ui->lineEdit_Password->text())
    {
        //用户上线更新登陆标示
        bool ok = DatabaseHandle::updateLoginFlag(username, "1");
        isOk = ok;
        qDebug() << "isOk: " << isOk;
        if(ok)
        {
            currentLoginID = DatabaseHandle::inquireUserID(username).toInt();
            qDebug() << "current Login userID: " << currentLoginID;

            //myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n账户'%1'登陆成功！\n").arg(username));

            //!显示并进入主界面
            ui->lineEdit_Account->clear();
            ui->lineEdit_Password->clear();
            ui->lineEdit_Account->setFocus();
            exitWidget();
        }
        else
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n账户上线登陆标示更新失败！\n"));
        }
    }
    else
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n密码输入错误，请重新输入！\n"));
        ui->lineEdit_Password->clear();
        ui->lineEdit_Password->setFocus();
    }
}

// 注册账号
void Widget::signUpAccount()
{
    if(dbConnectFlag)
        m_signUpWiget->show();
    else
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请检查网络连接或者查看防火墙设置！\n"));
}

// 退出程序
void Widget::exitWidget()
{
    //弹出提示对话框，选择是否退出程序
//    int ok = myHelper::ShowMessageBoxQuesionStandard(QString::fromLocal8Bit("\n 确认要退出当前程序吗？ \n"));

//    if(ok == QMessageBox::No)
//        return;

    this->close();
    qDebug() << "Widget close!";

    qApp->quit();
    qDebug() << "quit App!";
}

// 忘记密码
void Widget::forgotPassword()
{
    if(!dbConnectFlag)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请检查网络连接或者查看防火墙设置！\n"));
        return;
    }
}

///////////////// pushButton按键事件 /////////////////

// 进入注册账号界面
void Widget::on_pushButton_SignUp_clicked()
{
    signUpAccount();
}

// 找回密码
void Widget::on_pushButton_ForgotPassword_clicked()
{
    forgotPassword();
}

// 登陆
void Widget::on_pushButton_Login_clicked()
{
    loginAccount();
}

//退出
void Widget::on_pushButton_Exit_clicked()
{
    exitWidget();
}
