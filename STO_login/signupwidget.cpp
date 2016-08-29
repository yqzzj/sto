#include "signupwidget.h"
#include "ui_signupwidget.h"
#include "ui_clausedialog.h"

SignUpWidget::SignUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpWidget)
{
    ui->setupUi(this);
    this->setFixedSize(430, 400);
    this->setWindowTitle(QString::fromLocal8Bit("StoGod�˺�ע�����"));

/***
* �趨��������������QMessageBox�ؼ�(���治�˳���������������, exec())
* ���ô���ģ̬�Ի���,Ҫ��ô���û�и���,������Ч
* ���Ե����Ѿ�����ģ̬ģʽ�ĶԻ���,����ʹ��WindowModalģʽ����!
* this->setWindowModality(Qt::WindowModal);
***/
    //ģ̬ģʽ�ĶԻ���
    this->setWindowModality(Qt::ApplicationModal);

    ui->label_usernameUnit->hide();
    ui->label_phoneUnit->hide();
    ui->label_emailUnit->hide();
    ui->label_pw1Unit->hide();
    ui->label_pw2Unit->hide();

    resetSignUpWidget();

/////////////// ������ʽ ///////////////

    //������ʽ����IP
    QRegExp regExpIP("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    m_REVIP = new QRegExpValidator(regExpIP, this);

    //�û�����������
    QRegExp regExpUserName("[^\u4E00-\u9FA5^| ]{5,16}");
    m_REVUserName = new QRegExpValidator(regExpUserName, this);
    ui->lineEdit_UserName->setValidator(m_REVUserName);

    //�ֻ���
    //QRegExp regExpPhone("^(13[0-9]|14[5|7]|15[0|1|2|3|5|6|7|8|9]|18[0|1|2|3|5|6|7|8|9])\d{8}$");
    QRegExp regExpPhone("^1(3[0-9]|4[57]|5[0-35-9]|7[01678]|8[0-9])\\d{8}$");
    m_REVPhone = new QRegExpValidator(regExpPhone, this);
    ui->lineEdit_Phone->setValidator(m_REVPhone);

    //�����ַ
    //QRegExp regExpEmail("^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$");   //js��
    //QRegExp regExpEmail("^[A-Za-zd]+([-_.][A-Za-zd]+)*@([A-Za-zd]+[-.])+[A-Za-zd]{2,5}$");
    QRegExp regExpEmail("^([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$");
    m_REVEmail = new QRegExpValidator(regExpEmail, this);
    ui->lineEdit_Email->setValidator(m_REVEmail);

    //������6-20λ��ĸ�����ֺͷ����������������ַ���ϣ����ִ�Сд����ĸ��ͷ
    //QRegExp regExpPassWord("^[a-zA-Z]\w{6,20}$");
    QRegExp regExpPassWord("[^\u4E00-\u9FA5^| ]{6,20}");        //������
    m_REVPassWord = new QRegExpValidator(regExpPassWord, this);
    ui->lineEdit_Password1->setValidator(m_REVPassWord);
    ui->lineEdit_Password2->setValidator(m_REVPassWord);

///////////////

    //!���ü��̰����¼�
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

    //��ȡ����IP��Ϣ����
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
        // �����¼�
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Escape)          //Esc��
            {
                // ��ԭ����ؼ�������Ϣ
                resetSignUpWidget();

                this->hide();
                return true;
            }
            if ((keyEvent->key() == Qt::Key_Enter)          //�س���
                || (keyEvent->key() == Qt::Key_Return))     //���ؼ�
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

                    confirmSignUpUser();            //ȷ��ע���˻�
                    return true;
                }
                if (target == ui->pushButton_signup)
                {
                    confirmSignUpUser();            //ȷ��ע���˻�
                    return true;
                }
                if (target == ui->pushButton_clause)
                {
                    lookSignUpClause();             //�鿴����
                    return true;
                }
                if (target == ui->pushButton_exit)
                {
                    // ��ԭ����ؼ�������Ϣ
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

//! �˳���ǰ����ʱ�����ý���ؼ���Ϣ
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

//�ж��û�����Ϣ
bool SignUpWidget::checkUserNameInfo(const QString &username)
{
    if(username.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n����������ģ�����Ϊ5-16λ�û�����\n"));
        ui->lineEdit_UserName->clear();
        ui->lineEdit_UserName->setFocus();
        return false;
    }
    else
    {
        if(!m_REVUserName->regExp().exactMatch(username))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\nע���˻���ʽ��ƥ�䣡\n"));
            ui->lineEdit_UserName->clear();
            ui->lineEdit_UserName->setFocus();
            return false;
        }

        //��ѯ���ݿ��û��Ƿ����
        int userExist = DatabaseHandle::inquireUserNameExist(username);

        if(userExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n��ǰ�˻��Ѵ��ڣ������������û�����\n"));
            ui->lineEdit_UserName->clear();
            ui->lineEdit_UserName->setFocus();
            return false;
        }
    }

    return true;
}
//�ж��ֻ�����Ϣ
bool SignUpWidget::checkPhoneInfo(const QString &phone)
{
    if(phone.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�������ֻ��ţ�\n"));
        ui->lineEdit_Phone->clear();
        ui->lineEdit_Phone->setFocus();
        return false;
    }
    else
    {
        if(!m_REVPhone->regExp().exactMatch(phone))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����ֻ��Ÿ�ʽ����ȷ��\n"));
            ui->lineEdit_Phone->clear();
            ui->lineEdit_Phone->setFocus();
            return false;
        }

        //��ѯ�ֻ����Ƿ����
        int phoneExist = DatabaseHandle::inquireMobilePhoneNumExist(phone);

        if(phoneExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n��ǰ�ֻ����ѱ�ע�ᣬ�����������ֻ��ţ�\n"));
            ui->lineEdit_Phone->clear();
            ui->lineEdit_Phone->setFocus();
            return false;
        }
    }

    return true;
}
//�ж�������Ϣ
bool SignUpWidget::checkEmailInfo(const QString &email)
{
    if(email.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����������ַ��\n"));
        ui->lineEdit_Email->clear();
        ui->lineEdit_Email->setFocus();
        return false;
    }
    else
    {
        if(!m_REVEmail->regExp().exactMatch(email))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n���������ַ��ʽ����ȷ��\n"));
            ui->lineEdit_Email->clear();
            ui->lineEdit_Email->setFocus();
            return false;
        }

        //��ѯ�����ַ�Ƿ����
        int emailExist = DatabaseHandle::inquireEmailNumExist(email);

        if(emailExist == 1)
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n��ǰ�����ַ�ѱ�ע�ᣬ�����������µ������ַ��\n"));
            ui->lineEdit_Email->clear();
            ui->lineEdit_Email->setFocus();
            return false;
        }
    }

    return true;
}
//�ж�������Ϣ
bool SignUpWidget::checkPasswordInfo(const QString &password)
{
    if(password.isEmpty())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n���������룡\n"));
        ui->lineEdit_Password1->clear();
        ui->lineEdit_Password1->setFocus();
        return false;
    }
    else
    {
        if(!m_REVPassWord->regExp().exactMatch(password))
        {
            myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n���������ʽ����ȷ��\n"));
            ui->lineEdit_Password1->clear();
            ui->lineEdit_Password1->setFocus();
            return false;
        }
    }

    return true;
}

//! ȷ��ע���˺�
void SignUpWidget::confirmSignUpUser()
{
    QString insertFlag;

    //int userID = 1;                 //�˻�IDϵͳ����(�Ե���)
    QString loginFlag = "0";        //ע��Ĭ�ϵ�¼��ʾΪ0
    QString loginIP = "null";       //��ǰ����IP
    QString loginMAC = "null";      //��ǰע����Ϣ
    QString userLevel = "1";        //�û�Ȩ�޵ȼ�,Ĭ�����Ȩ��1
    QString usedTimes = "30";       //�˻�ʹ��ʱ�䣬Ĭ����30��
    QString registrationTime;       //ע��ʱ��
    //QString dueTime;                //����ʱ��

    QString username = ui->lineEdit_UserName->text();
    QString m_phone = ui->lineEdit_Phone->text();
    QString m_email = ui->lineEdit_Email->text();
    QString m_password1 = ui->lineEdit_Password1->text();
    QString m_password2 = ui->lineEdit_Password2->text();

    //!���ж�ע����Ϣ��������
    //�ж��û�����Ϣ
    if(!checkUserNameInfo(username))
    {
        return;
    }
    //�ж��ֻ���Ϣ
    if(!checkPhoneInfo(m_phone))
    {
        return;
    }
    //�ж�������Ϣ
    if(!checkEmailInfo(m_email))
    {
        return;
    }
    //�ж�����1��Ϣ
    if(!checkPasswordInfo(m_password1))
    {
        return;
    }

    //�ж�����1������2�Ƿ�һ��
    if(m_password1 != m_password2)
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�����������벻ƥ�䣬���������룡\n"));
        return;
    }

    //!���ж��Ƿ�ͬ��ע����������
    if(!ui->checkBox_agree->isChecked())
    {
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n����StoGodע���������ע�ᣡ\n"));
        return;
    }

    //��ȡ����MAC��ַ
    //getLocalMachineMAC();

    //��ȡ��������IP
    loginIP = getPublicNetworkIP();;

    //ע��ʱ��
    QDateTime time = QDateTime::currentDateTime();              //��ȡϵͳ���ڵ�ʱ��
    registrationTime = time.toString("yyyy-MM-dd hh:mm:ss");    //������ʾ��ʽ

    //!���жϺõ�ע����Ϣ���뵽���ݿ���
    //ϵͳ�ֶ�����userID
//    insertFlag = DatabaseHandle::insert_UserManagementTable(userID, username, m_password1,
//                                                            m_phone, m_email, loginFlag, loginIP,
//                                                            loginMAC, userLevel, usedTimes,
//                                                            registrationTime);
    //�Ե�������userID
    insertFlag = DatabaseHandle::insert_UserManagementTable_Auto(username, m_password1,
                                                            m_phone, m_email, loginFlag, loginIP,
                                                            loginMAC, userLevel, usedTimes,
                                                            registrationTime);

    if(insertFlag == DATABASE_WRITE_OK)
    {
        qDebug() << "insert_UserManagementTable_Auto OK!";
        myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n�û�ע��ɹ���\n"));

        resetSignUpWidget();

        this->hide();       //ע��ɹ�֮�󣬹رմ���
    }
    else
    {
        qDebug() << "insert_UserManagementTable_Auto fail!";
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("\n�û�ע��ʧ�ܣ�\n"));
    }
}

//! �鿴ע������
void SignUpWidget::lookSignUpClause()
{
    //myHelper::ShowMessageBoxInfo(QString::fromLocal8Bit("\n��Ȩ����StoGod����ֹ���⴫����\n"));

    QDialog *dialog = new QDialog;
    Ui::ClauseDialog dialog_ui;
    dialog_ui.setupUi(dialog);

    dialog->setWindowTitle(QString::fromLocal8Bit("StoGod�û�ע���ͬ����"));
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

///////////////// pushButton�����¼� /////////////////

// ע���˺�
void SignUpWidget::on_pushButton_signup_clicked()
{
    confirmSignUpUser();
}

// �˳�ע�ᣬ���������ʾ�Ի����ж��Ƿ��˳�ע�����
void SignUpWidget::on_pushButton_exit_clicked()
{
    // ��ԭ����ؼ�������Ϣ
    resetSignUpWidget();

    this->hide();
}

// ע����������
void SignUpWidget::on_pushButton_clause_clicked()
{
    lookSignUpClause();
}

//////////////////////������Ϣ��ȡ�ӿں���///////////////////////

// ��ȡ����MAC��ַ��������
QString SignUpWidget::getLocalMachineMAC()
{
    return "1";
}

// ��ȡ��ҳ����Դ���룬������ȡ��Ҫ��IP��ַ
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

    //ת��ʶ�������ַ�
    QString responseData_str = QString::fromLocal8Bit(responseData);
    qDebug() << "responseData_str: " << responseData_str;

    int index_postion = responseData_str.indexOf(QString::fromLocal8Bit("����IP��ַ�ǣ�"));
    qDebug() << " index_postion =  "<< index_postion;

    //IP��ַ��ʽxxx.xxx.xxx.xxx   //���15���ַ�
    //��ȡ����IP��ַ���ַ���,[IP]
    QString containIP = responseData_str.mid(index_postion + 8, 17);
    qDebug() << " containIP =  "<< containIP;

    //������ȡIP��ַ����Ҫ�ǻ�ȡ[]��λ��
    int index1 = containIP.indexOf("[");
    qDebug() << " index1 =  "<< index1;

    int index2 = containIP.indexOf("]");
    qDebug() << " index2 =  "<< index2;

    //��ȡ������IP��ַ
    m_publicNetworkIP = containIP.mid(index1 + 1, index2 - 1);
    qDebug() << " m_publicNetworkIP = "<< m_publicNetworkIP;

#if 0    //!��������д���ļ�
    QFile file("GetHtml.txt");

    if(file.fileName().isEmpty()){
        return;
    }

    if(!file.open(QFile::WriteOnly | QIODevice::Text))
    {
       return;
    }

    //д���ݵ��ļ�
    QTextStream out(&file);
    out << responseData;
    file.close();
#endif
}

// ��ȡ��������IP��ַ,��ҳԴ�����ļ�
QString SignUpWidget::getPublicNetworkIP()
{
    QString IP = "NULL";

    GetHtml("http://www.ip138.com/ips1388.asp");

    IP = m_publicNetworkIP;

#if 0       //��ȡ��ҳԴ���ļ�,����
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

    //IP��ַ��ʽxxx.xxx.xxx.xxx   //���15���ַ�
    QString temp = data.mid(index1 + 3, 15);
    qDebug() << " temp =  "<< temp;

    int index2 = temp.indexOf(QRegExp("[0-9]"), 0);
    qDebug() << " index2 =  "<< index2;

//    if(!m_REVIP->regExp().exactMatch(temp))   //������ʽ��ƥ�䣿����
//    {
//        IP = temp;
//        qDebug() << " regExp IP =  "<< IP;
//    }
//    else
//    {
//        qDebug() << " regExp IP fail! ";
//        //�����ַ���
//    }

    IP = temp;
#endif

    return IP;
}

// ��ȡ����ip��ַ����ȡ��̬IP"192.168."���Σ����ԣ�
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
            //IPv4��ַ
            if (address.toString().contains("127.0.")
                || !address.toString().startsWith("192.168."))
            {
                continue;
            }
            temp.append(address.toString());
        }
    }

//!����
    //��ȡ��¼�û�
    QString name = qgetenv("USERNAME");
    qDebug() << "Test:User Name:" << name;

    //QHostInfo�����ã���ȡ��������Ҳ����ͨ��������������IP��ַ������ͨ��IP��ַ�����������������
    QString localHostName = QHostInfo::localHostName();
    qDebug() << "Test:LocalHostName:" << localHostName;

    //��ȡIP��ַ
    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug() << "Test:IP Address:" << info.addresses();

    return temp;
}

//��ӡ���ؽӿ�������Ϣ
void SignUpWidget::qDebug_NetworkInterfaceInfo()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    //��ȡ��������ӿڵ��б�
    foreach(QNetworkInterface interface,list)
    {  //����ÿһ������ӿ�
        qDebug() << "Device: "<<interface.name();
        //�豸��
        qDebug() << "HardwareAddress: "<<interface.hardwareAddress();
        //Ӳ����ַ
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        //��ȡIP��ַ��Ŀ�б�ÿ����Ŀ�а���һ��IP��ַ��һ�����������һ���㲥��ַ
        foreach(QNetworkAddressEntry entry,entryList)
        {//����ÿһ��IP��ַ��Ŀ
            //IP��ַ
            qDebug()<<"IP Address: "<< entry.ip().toString();
            //��������
            qDebug()<<"Netmask: "<< entry.netmask().toString();
            //�㲥��ַ
            qDebug()<<"Broadcast: "<< entry.broadcast().toString();
        }
    }
}

//////////////////////////////////////////////////////////////
