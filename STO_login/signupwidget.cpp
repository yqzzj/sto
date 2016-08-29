#include "signupwidget.h"
#include "ui_signupwidget.h"
#include "ui_clausedialog.h"

SignUpWidget::SignUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpWidget)
{
    ui->setupUi(this);
    this->setFixedSize(430, 400);
    this->setWindowTitle(QString::fromLocal8Bit("StoGod账号注册界面"));

/***
* 设定界面属性类似于QMessageBox控件(界面不退出不能有其他操作, exec())
* 设置窗体模态对话框,要求该窗体没有父类,否则无效
* 可以抵消已经存在模态模式的对话框,可以使用WindowModal模式抵消!
* this->setWindowModality(Qt::WindowModal);
***/
    //模态模式的对话框
    this->setWindowModality(Qt::ApplicationModal);

    ui->label_usernameUnit->hide();
    ui->label_phoneUnit->hide();
    ui->label_emailUnit->hide();
    ui->label_pw1Unit->hide();
    ui->label_pw2Unit->hide();

    resetSignUpWidget();

/////////////// 正则表达式 ///////////////

    //正则表达式限制IP
    QRegExp regExpIP("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    m_REVIP = new QRegExpValidator(regExpIP, this);

    //用户名，非中文
    QRegExp regExpUserName("[^\u4E00-\u9FA5^| ]{5,16}");
    m_REVUserName = new QRegExpValidator(regExpUserName, this);
    ui->lineEdit_UserName->setValidator(m_REVUserName);

    //手机号
    //QRegExp regExpPhone("^(13[0-9]|14[5|7]|15[0|1|2|3|5|6|7|8|9]|18[0|1|2|3|5|6|7|8|9])\d{8}$");
    QRegExp regExpPhone("^1(3[0-9]|4[57]|5[0-35-9]|7[01678]|8[0-9])\\d{8}$");
    m_REVPhone = new QRegExpValidator(regExpPhone, this);
    ui->lineEdit_Phone->setValidator(m_REVPhone);

    //邮箱地址
    //QRegExp regExpEmail("^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$");   //js版
    //QRegExp regExpEmail("^[A-Za-zd]+([-_.][A-Za-zd]+)*@([A-Za-zd]+[-.])+[A-Za-zd]{2,5}$");
    QRegExp regExpEmail("^([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$");
    m_REVEmail = new QRegExpValidator(regExpEmail, this);
    ui->lineEdit_Email->setValidator(m_REVEmail);

    //密码由6-20位字母，数字和符号至少两种以上字符组合，区分大小写，字母开头
    //QRegExp regExpPassWord("^[a-zA-Z]\w{6,20}$");
    QRegExp regExpPassWord("[^\u4E00-\u9FA5^| ]{6,20}");        //非中文
    m_REVPassWord = new QRegExpValidator(regExpPassWord, this);
    ui->lineEdit_Password1->setValidator(m_REVPassWord);
    ui->lineEdit_Password2->setValidator(m_REVPassWord);

///////////////

    //!配置键盘按键事件
    ui->lineEdit_UserName->installEventFilter(this);
    ui->lineEdit_Phone->installEventFilter(this);
    ui->lineEdit_Email->installEventFilter(this);
    ui->lineEdit_Password1->installEventFilter(this);
    ui->lineEdit_Password2->installEventFilter(this);
    ui->pushButton_signup->installEventFilter(this);
    ui->pushButton_clause->installEventFilter(this);
    ui->pushButton_exit->installEventFilter(this);
    QWidget::installEventFilter(this);

    m_publicNetworkIP = "NULL";

    //获取本机IP信息链表
    local_ip.clear();
    //local_ip = getLocalHostIP();
    //qDebug_NetworkInterfaceInfo();
}

SignUpWidget::~SignUpWidget()
{
    delete ui;
}

bool SignUpWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->lineEdit_UserName || target == ui->lineEdit_Phone
     || target == ui->lineEdit_Email || target == ui->lineEdit_Password1
     || target == ui->lineEdit_Password2 || target == ui->pushButton_signup
     || target == ui->pushButton_clause || target == ui->pushButton_exit
     )
    {
        // 键盘事件
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Escape)          //Esc键
            {
                // 还原清理控件输入信息
                resetSignUpWidget();

                this->hide();
                return true;
            }
            if ((keyEvent->key() == Qt::Key_Enter)          //回车键
                || (keyEvent->key() == Qt::Key_Return))     //返回键
            {
                if (target == ui->lineEdit_UserName)
                {
                    ui->lineEdit_Phone->setFocus();
                    ui->lineEdit_Phone->selectAll();
                    return true;
                }
                if (target == ui->lineEdit_Phone)
                {
                    ui->lineEdit_Email->setFocus();
                    ui->lineEdit_Email->selectAll();
                    return true;
                }
                if (target == ui->lineEdit_Email)
                {
                    ui->lineEdit_Password1->setFocus();
                    ui->lineEdit_Password1->selectAll();
                    return true;
                }
                if (target == ui->lineEdit_Password1)
                {
                    ui->lineEdit_Password2->setFocus();
                    ui->lineEdit_Password2->selectAll();
                    return true;
                }
                if (target == ui->lineEdit_Password2)
                {
                    ui->pushButton_signup->setFocus();

                    confirmSignUpUser();            //确认注册账户
                    return true;
                }
                if (target == ui->pushButton_signup)
                {
                    confirmSignUpUser();            //确认注册账户
                    return true;
                }
                if (target == ui->pushButton_clause)
                {
                    lookSignUpClause();             //查看条款
                    return true;
                }
                if (target == ui->pushButton_exit)
                {
                    // 还原清理控件输入信息
                    resetSignUpWidget();

                    this->hide();
                    return true;
                }
            }
        }
    }

    return QWidget::eventFilter(target, event);
}

void SignUpWidget::resizeEvent(QResizeEvent *)
{
    this->setAutoFillBackground(true);

    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush("#c1ffc5"));
    this->setPalette(palette);
}

//! 退出当前界面时，重置界面控件信息
void SignUpWidget::resetSignUpWidget()
{
    ui->lineEdit_UserName->setFocus();

    ui->lineEdit_UserName->clear();
    ui->lineEdit_Phone->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_Password1->clear();
    ui->lineEdit_Password2->clear();

    ui->checkBox_agree->setChecked(true);
}

//判断用户名信息
bool SignUpWidget::checkUserNameInfo(const QString &username)
{
    if(username.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请输入非中文，长度为5-16位用户名！\n"));
        ui->lineEdit_UserName->clear();
        ui->lineEdit_UserName->setFocus();
        return false;
    }
    else
    {
        if(!m_REVUserName->regExp().exactMatch(username))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n注册账户格式不匹配！\n"));
            ui->lineEdit_UserName->clear();
            ui->lineEdit_UserName->setFocus();
            return false;
        }

        //查询数据库用户是否存在
        int userExist = DatabaseHandle::inquireUserNameExist(username);

        if(userExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n当前账户已存在，请重新输入用户名！\n"));
            ui->lineEdit_UserName->clear();
            ui->lineEdit_UserName->setFocus();
            return false;
        }
    }

    return true;
}
//判断手机号信息
bool SignUpWidget::checkPhoneInfo(const QString &phone)
{
    if(phone.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请输入手机号！\n"));
        ui->lineEdit_Phone->clear();
        ui->lineEdit_Phone->setFocus();
        return false;
    }
    else
    {
        if(!m_REVPhone->regExp().exactMatch(phone))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n输入手机号格式不正确！\n"));
            ui->lineEdit_Phone->clear();
            ui->lineEdit_Phone->setFocus();
            return false;
        }

        //查询手机号是否存在
        int phoneExist = DatabaseHandle::inquireMobilePhoneNumExist(phone);

        if(phoneExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n当前手机号已被注册，请重新输入手机号！\n"));
            ui->lineEdit_Phone->clear();
            ui->lineEdit_Phone->setFocus();
            return false;
        }
    }

    return true;
}
//判断邮箱信息
bool SignUpWidget::checkEmailInfo(const QString &email)
{
    if(email.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请输入邮箱地址！\n"));
        ui->lineEdit_Email->clear();
        ui->lineEdit_Email->setFocus();
        return false;
    }
    else
    {
        if(!m_REVEmail->regExp().exactMatch(email))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n输入邮箱地址格式不正确！\n"));
            ui->lineEdit_Email->clear();
            ui->lineEdit_Email->setFocus();
            return false;
        }

        //查询邮箱地址是否存在
        int emailExist = DatabaseHandle::inquireEmailNumExist(email);

        if(emailExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n当前邮箱地址已被注册，请重新输入新的邮箱地址！\n"));
            ui->lineEdit_Email->clear();
            ui->lineEdit_Email->setFocus();
            return false;
        }
    }

    return true;
}
//判断密码信息
bool SignUpWidget::checkPasswordInfo(const QString &password)
{
    if(password.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n请输入密码！\n"));
        ui->lineEdit_Password1->clear();
        ui->lineEdit_Password1->setFocus();
        return false;
    }
    else
    {
        if(!m_REVPassWord->regExp().exactMatch(password))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n输入密码格式不正确！\n"));
            ui->lineEdit_Password1->clear();
            ui->lineEdit_Password1->setFocus();
            return false;
        }
    }

    return true;
}

//! 确认注册账号
void SignUpWidget::confirmSignUpUser()
{
    QString insertFlag;

    //int userID = 1;                 //账户ID系统分配(自递增)
    QString loginFlag = "0";        //注册默认登录标示为0
    QString loginIP = "null";       //当前公网IP
    QString loginMAC = "null";      //当前注册信息
    QString userLevel = "1";        //用户权限等级,默认最低权限1
    QString usedTimes = "30";       //账户使用时间，默认是30天
    QString registrationTime;       //注册时间
    //QString dueTime;                //到期时间

    QString username = ui->lineEdit_UserName->text();
    QString m_phone = ui->lineEdit_Phone->text();
    QString m_email = ui->lineEdit_Email->text();
    QString m_password1 = ui->lineEdit_Password1->text();
    QString m_password2 = ui->lineEdit_Password2->text();

    //!先判断注册信息的完整度
    //判断用户名信息
    if(!checkUserNameInfo(username))
    {
        return;
    }
    //判断手机信息
    if(!checkPhoneInfo(m_phone))
    {
        return;
    }
    //判断邮箱信息
    if(!checkEmailInfo(m_email))
    {
        return;
    }
    //判断密码1信息
    if(!checkPasswordInfo(m_password1))
    {
        return;
    }

    //判断密码1与密码2是否一致
    if(m_password1 != m_password2)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n两次输入密码不匹配，请重新输入！\n"));
        return;
    }

    //!后判断是否同意注册条款声明
    if(!ui->checkBox_agree->isChecked())
    {
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n接受StoGod注册条款才能注册！\n"));
        return;
    }

    //获取本机MAC地址
    //getLocalMachineMAC();

    //获取本机公网IP
    loginIP = getPublicNetworkIP();;

    //注册时间
    QDateTime time = QDateTime::currentDateTime();              //获取系统现在的时间
    registrationTime = time.toString("yyyy-MM-dd hh:mm:ss");    //设置显示格式

    //!将判断好的注册信息插入到数据库中
    //系统手动分配userID
//    insertFlag = DatabaseHandle::insert_UserManagementTable(userID, username, m_password1,
//                                                            m_phone, m_email, loginFlag, loginIP,
//                                                            loginMAC, userLevel, usedTimes,
//                                                            registrationTime);
    //自递增分配userID
    insertFlag = DatabaseHandle::insert_UserManagementTable_Auto(username, m_password1,
                                                            m_phone, m_email, loginFlag, loginIP,
                                                            loginMAC, userLevel, usedTimes,
                                                            registrationTime);

    if(insertFlag == DATABASE_WRITE_OK)
    {
        qDebug() << "insert_UserManagementTable_Auto OK!";
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n用户注册成功！\n"));

        resetSignUpWidget();

        this->hide();       //注册成功之后，关闭窗口
    }
    else
    {
        qDebug() << "insert_UserManagementTable_Auto fail!";
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n用户注册失败！\n"));
    }
}

//! 查看注册条款
void SignUpWidget::lookSignUpClause()
{
    //myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n版权所有StoGod，禁止恶意传播！\n"));

    QDialog *dialog = new QDialog;
    Ui::ClauseDialog dialog_ui;
    dialog_ui.setupUi(dialog);

    dialog->setWindowTitle(QString::fromLocal8Bit("StoGod用户注册合同条款"));
    dialog->setModal(true);

    dialog->show();

    if(dialog->exec()){
        qDebug() << "StoGod user registration contract terms OK!";
    }else{
        qDebug() << "StoGod user registration contract terms NO!";
    }

    delete dialog;
    dialog = NULL;
}

///////////////// pushButton按键事件 /////////////////

// 注册账号
void SignUpWidget::on_pushButton_signup_clicked()
{
    confirmSignUpUser();
}

// 退出注册，可以添加提示对话框判断是否退出注册界面
void SignUpWidget::on_pushButton_exit_clicked()
{
    // 还原清理控件输入信息
    resetSignUpWidget();

    this->hide();
}

// 注册条款声明
void SignUpWidget::on_pushButton_clause_clicked()
{
    lookSignUpClause();
}

//////////////////////本机信息获取接口函数///////////////////////

// 获取本机MAC地址，待完善
QString SignUpWidget::getLocalMachineMAC()
{
    return "1";
}

// 获取网页所有源代码，从中提取需要的IP地址
void SignUpWidget::GetHtml(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();

    //qDebug() << "responseData: " << responseData;

    //转换识别中文字符
    QString responseData_str = QString::fromLocal8Bit(responseData);
    qDebug() << "responseData_str: " << responseData_str;

    int index_postion = responseData_str.indexOf(QString::fromLocal8Bit("您的IP地址是："));
    qDebug() << " index_postion =  "<< index_postion;

    //IP地址格式xxx.xxx.xxx.xxx   //最多15个字符
    //截取包含IP地址的字符串,[IP]
    QString containIP = responseData_str.mid(index_postion + 8, 17);
    qDebug() << " containIP =  "<< containIP;

    //解析提取IP地址，主要是获取[]的位置
    int index1 = containIP.indexOf("[");
    qDebug() << " index1 =  "<< index1;

    int index2 = containIP.indexOf("]");
    qDebug() << " index2 =  "<< index2;

    //提取完整的IP地址
    m_publicNetworkIP = containIP.mid(index1 + 1, index2 - 1);
    qDebug() << " m_publicNetworkIP = "<< m_publicNetworkIP;

#if 0    //!将数据流写入文件
    QFile file("GetHtml.txt");

    if(file.fileName().isEmpty()){
        return;
    }

    if(!file.open(QFile::WriteOnly | QIODevice::Text))
    {
       return;
    }

    //写数据到文件
    QTextStream out(&file);
    out << responseData;
    file.close();
#endif
}

// 获取本机公网IP地址,网页源代码文件
QString SignUpWidget::getPublicNetworkIP()
{
    QString IP = "NULL";

    GetHtml("http://www.ip138.com/ips1388.asp");

    IP = m_publicNetworkIP;

#if 0       //读取网页源码文件,测试
    QString data = "";

    QFile file("GetHtml.txt");

    if(file.fileName().isEmpty()){
        return " GetHtml.txt file is empty!";
    }

    if(!file.open(QFile::ReadOnly | QIODevice::Text))
    {
       return " GetHtml.txt ReadOnly open fail!";
    }

    data = file.readAll();
    file.close();

    int index1 = data.indexOf("ip=");
    qDebug() << " index1 =  "<< index1;

    //IP地址格式xxx.xxx.xxx.xxx   //最多15个字符
    QString temp = data.mid(index1 + 3, 15);
    qDebug() << " temp =  "<< temp;

    int index2 = temp.indexOf(QRegExp("[0-9]"), 0);
    qDebug() << " index2 =  "<< index2;

//    if(!m_REVIP->regExp().exactMatch(temp))   //正则表达式不匹配？？？
//    {
//        IP = temp;
//        qDebug() << " regExp IP =  "<< IP;
//    }
//    else
//    {
//        qDebug() << " regExp IP fail! ";
//        //解析字符串
//    }

    IP = temp;
#endif

    return IP;
}

// 获取本地ip地址，获取静态IP"192.168."网段，测试！
QStringList SignUpWidget::getLocalHostIP()
{
    QStringList temp;
    QList<QHostAddress> list;
    temp.clear();
    list.clear();

    list = QNetworkInterface::allAddresses();

    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            //IPv4地址
            if (address.toString().contains("127.0.")
                || !address.toString().startsWith("192.168."))
            {
                continue;
            }
            temp.append(address.toString());
        }
    }

//!测试
    //获取登录用户
    QString name = qgetenv("USERNAME");
    qDebug() << "Test:User Name:" << name;

    //QHostInfo类作用，获取主机名，也可以通过主机名来查找IP地址，或者通过IP地址来反向查找主机名。
    QString localHostName = QHostInfo::localHostName();
    qDebug() << "Test:LocalHostName:" << localHostName;

    //获取IP地址
    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug() << "Test:IP Address:" << info.addresses();

    return temp;
}

//打印本地接口网络信息
void SignUpWidget::qDebug_NetworkInterfaceInfo()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    //获取所有网络接口的列表
    foreach(QNetworkInterface interface,list)
    {  //遍历每一个网络接口
        qDebug() << "Device: "<<interface.name();
        //设备名
        qDebug() << "HardwareAddress: "<<interface.hardwareAddress();
        //硬件地址
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        //获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
        foreach(QNetworkAddressEntry entry,entryList)
        {//遍历每一个IP地址条目
            //IP地址
            qDebug()<<"IP Address: "<< entry.ip().toString();
            //子网掩码
            qDebug()<<"Netmask: "<< entry.netmask().toString();
            //广播地址
            qDebug()<<"Broadcast: "<< entry.broadcast().toString();
        }
    }
}

//////////////////////////////////////////////////////////////
