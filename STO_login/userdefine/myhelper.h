#ifndef MYHELPER_H
#define MYHELPER_H

#include <QObject>
#include <QListWidget>
#include <QSettings>
#include <QWidget>
#include <QDesktopWidget>
#include <QFile>
#include <QFileIconProvider>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QCoreApplication>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QCursor>
#include <QTimer>
#include <QApplication>
#include <QStyleFactory>
#include <QTextStream>
#include <QDebug>
#include <QScrollBar>

//#define SYS_LINUX           //������linuxϵͳ

#ifdef SYS_LINUX
#include <sys/statfs.h>     //��ѯ�ļ�ϵͳ��ص���Ϣ(linux)
#endif

//#define QT4_VERSION         //������Qt4.x,PC��

#ifdef QT4_VERSION
//QT4.x��ģ��
#include <QSound>
#else
//QT5.x��ģ��
#include <QtSerialPort/QSerialPortInfo>     //�ṩ�������ڵĸ��ֽӿ�
#include <QtSerialPort/QSerialPort>         //�ṩ������п��ô��ڵĸ�����Ϣ
#endif

//��ʾ������Ϣ,��ʱ3s�ر���Ϣ
//box.setWindowFlags(Qt::FramelessWindowHint);
#define MESSAGEBOX(strshow) { QMessageBox box ;\
    QTimer::singleShot(3000, &box, SLOT(close()));\
    box.setWindowTitle(tr("����"));\
    box.setIcon(QMessageBox::Warning);\
    box.setText(strshow);\
    box.exec();}

//����������
typedef unsigned long long U64;

class myHelper:public QObject
{
public:

    #if 1       //DDU������������
    //����У��λ
    static int calculateCheckBit(QByteArray temp)
    {
        //����У��λ,Ϊ֡ͷ(��ΪFF)������š��ؼ��š����ݹ�7�ֽ��ۼӺ͵���Ͱ�λ
        //��: & 0xFF; �����ǰ�߸��ֽں�
        int _checkBit = 0;   //�ۼ�֮ǰ����Ϊ0

        for(int i = 0; i < 7; i++)
        {
            _checkBit += temp[i];
        }

        //��Ͱ�λ
        _checkBit = _checkBit & 0xFF;

        //qDebug() << "_checkBit: " << _checkBit;
        //qDebug() << "_checkBit(16): 0x" << QString::number(_checkBit, 16);

        return _checkBit;
    }

    //��ʼ���ֽ�����
    static QByteArray initByteArray(QByteArray temp, int dataType)
    {
        //10�ֽ�������
        temp[0] = 0x55;    //֡ͷ
        temp[1] = 0xAA;
        temp[2] = dataType;

        for(int i = 3; i < 8; i++)
        {
            temp[i] = 0;
        }

        temp[8] = 0xEE;    //֡β
        temp[9] = 0XFF;

        return temp;
    }
    #endif

    //����ȫ��Ϊplastique��ʽ
    static void SetPlastiqueStyle()
    {
        QApplication::setStyle(QStyleFactory::create("Plastique"));
        QApplication::setPalette(QApplication::style()->standardPalette());//�滻Ӧ�ó������ɫ���
    }

#ifdef QT4_VERSION
    //���ú��ʵı����ʽ
    static void SetTextCodec()
    {
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

        //TW-arm
        //QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
        //::setCodecForLocale(QTextCodec::codecForName("GB18030"));

        //smarthome
        QTextCodec *codec = QTextCodec::codecForName("gbk");
        QTextCodec *codec2 = QTextCodec::codecForName("utf8");
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForLocale(codec2);
        QTextCodec::setCodecForCStrings(codec2);
    }

    //���ñ���ΪGB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    }

    //���ñ���ΪUTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    }
#endif

    //���ö�ʱ�رնԻ���(����ƳɶԻ�����ʾ����ʱ)
    static void Show_Time_Message(QString strshow, int m_time)
    {
        QMessageBox box ;
        QTimer::singleShot(m_time, &box, SLOT(close()));
        box.setStyleSheet("font:12pt '����'");
        box.setWindowTitle(tr("��ܰ��ʾ (��ʱ%1s,�رնԻ���)")
                           .arg(m_time / 1000));
        box.setText(strshow);
        box.setIcon(QMessageBox::Information);
        box.addButton(tr("ȷ��"),QMessageBox::ActionRole);
        box.exec();
    }

    //��ʾ��Ϣ�򣬽�ȷ����ť
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        //msg.setStyleSheet("font:12pt '΢���ź�'");
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '΢���ź�'"));
        //msg.setWindowTitle(tr("��ʾ"));
        msg.setWindowTitle(QString::fromLocal8Bit("��ʾ"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        //msg.addButton(tr("ȷ��"),QMessageBox::ActionRole);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::ActionRole);
        msg.exec();
    }

    //��ʾ����򣬽�ȷ����ť
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '΢���ź�'"));
        msg.setWindowTitle(QString::fromLocal8Bit("����"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::ActionRole);
        msg.exec();
    }

    //��ʾѯ�ʿ�ȷ����ȡ����ť
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '΢���ź�'"));
        msg.setWindowTitle(QString::fromLocal8Bit("ѯ��"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);        
        msg.addButton(QString::fromLocal8Bit("ȡ��"),QMessageBox::NoRole);
        msg.addButton(QString::fromLocal8Bit("ȷ��"),QMessageBox::YesRole);

        return msg.exec();
    }

    static int ShowMessageBoxQuesionStandard(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '΢���ź�'"));
        msg.setWindowTitle(QString::fromLocal8Bit("ѯ��"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);        
        msg.addButton(QMessageBox::No);
        msg.addButton(QMessageBox::Yes);

        return msg.exec();
    }

    //16�����ַ���ת�ֽ�����
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; )
        {
#ifdef QT4_VERSION
            hstr=str[i].toAscii();
#else
            hstr=str[i].toLatin1();
#endif

            if(hstr == ' ')
            {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
#ifdef QT4_VERSION
            lstr = str[i].toAscii();
#else
            lstr = str[i].toLatin1();
#endif

            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    //�ֽ�����ת16�����ַ���
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    //�ֽ�����ת��Ϊ�ַ���
    static QString ByteArrayToStr(const QByteArray &data)
    {
        QString tempdata = data.toHex().data(); //�ַ������ȶ���ż��

        return tempdata;
    }

    //16�����ַ���ת10����
    static uchar StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //����ת��Ϊ�ֽ�,test
    static void intToBit(unsigned int a)
    {
        unsigned int b;

        qDebug() <<"������һ����(С��4294967296��������4�ֽ�)��";
        qDebug() <<"���ֽڵ�ֵΪ(10����)��";

        b = a & 0x000000ff;
        qDebug() <<"���ֽ�Ϊ��" << b;

        b = a & 0x0000ff00;
        b/=256;
        qDebug() <<"�ε��ֽ�Ϊ��" << b;

        b = a & 0x00ff0000;
        b/=65536;
        qDebug() <<"�θ��ֽ�Ϊ��" << b;

        b = a & 0xff000000;
        b/=65536*256;
        qDebug() <<"���ֽ�Ϊ��" << b;
    }

    //10�����ַ���ת10����
    static uchar StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //16�����ַ���ת2�����ַ���
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //����У����
    static uchar GetCheckCode(uchar data[],uchar len)
    {
        uchar temp=0;

        for (uchar i=0;i<len;i++)
        {
            temp+=data[i];
        }

        return temp%256;
    }

    //��ʱ
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    //��ȡ��ǰ·��
    static QString GetCurrentPath()
    {
        return QString(QCoreApplication::applicationDirPath()+"/");
    }

#ifdef QT4_VERSION
    //��������
    static void PlaySound(QString soundName)
    {
        QSound::play(soundName);
    }
#endif

    //��ȡ��ǰ����ʱ������
    static QString GetLongDateTime()
    {
        QDateTime time=QDateTime::currentDateTime();
        return time.toString("yyyy��MM��dd�� hhʱmm��ss�� dddd");
    }

    //����ַ
    static void OpenUrlOrExe(QString url)
    {
        QDesktopServices::openUrl(QUrl(url));
    }

    //�ж��Ƿ�����
    static bool IsLeapYear(QString strDate)
    {
        int year=strDate.left(4).toInt();
        if(((year % 4 ==0) && (year % 100 !=0 ))|| (year % 400 ==0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

#ifdef SYS_LINUX
    //�鿴���̿ռ�(linux��ʹ��)
    static int GetStorageInfo(char *MountPoint,     //�洢��һ������
                              int *Capacity,        //��Ҫ��ȡ�Ŀռ��С
                              int type)             //��ȡʲô���͵Ŀռ�
    {
        struct statfs statFS;   //ϵͳstat�Ľṹ��
        U64 usedBytes = 0;
        U64 freeBytes = 0;
        U64 totalBytes = 0;
        U64 endSpace = 0;       //Ҫ��ȡ���͵Ŀռ��С

        if(statfs(MountPoint, &statFS) == -1)
        {//��ȡ������״̬
            printf("statfs failed for path->[%s]\n", MountPoint);
            return -1;
        }

        //����������, ���ֽ�Ϊ��λ
        totalBytes = (U64)statFS.f_blocks * (U64)statFS.f_frsize;
        //ʣ��ռ����������ֽ�Ϊ��λ
        freeBytes = (U64)statFS.f_bfree * (U64)statFS.f_frsize;
        //���ÿռ����������ֽ�Ϊ��λ
        usedBytes = (U64)(totalBytes - freeBytes);

        switch( type )
        {
            case 1:
            endSpace = totalBytes/1024;     //��KBΪ��λ��������
            break;

            case 2:
            endSpace = usedBytes/1024;      //��KBΪ��λ�����ÿռ�
            break;

            case 3:
            endSpace = freeBytes/1024;      //��KBΪ��λ��ʣ��ռ�
            break;

            case 4:
            endSpace = totalBytes >> 20;    //��MBΪ��λ��������
            break;

            case 5:
            endSpace = usedBytes >> 20;     //��MBΪ��λ�����ÿռ�
            break;

            case 6:
            endSpace = freeBytes >> 20;     //��MBΪ��λ��ʣ��ռ�
            break;

            case 7:
            endSpace = totalBytes >> 30;    //��GBΪ��λ��������
            break;

            case 8:
            endSpace = usedBytes >> 30;     //��GBΪ��λ�����ÿռ�
            break;

            case 9:
            endSpace = freeBytes >> 30;     //��GBΪ��λ��ʣ��ռ�
            break;

            default:
            //*Capacity = endSpace;       // 0
            return -1;
        }

        *Capacity = endSpace;
        return 0;
    }
#endif
};

class FormHelper:public QObject
{
public:    

    //��ť���ͼƬ
    static void SetButtonBackGround(QPushButton *button, QPixmap picturepath)
    {
        button->setFixedSize( picturepath.width(), picturepath.height());
        button->setIcon(QIcon(picturepath));
        button->setFlat(true);
        button->setIconSize(QSize(picturepath.width(),picturepath.height()));;
        button->setToolTip("");
    }

    //���������ʾ
    static void FormInCenter(QWidget *frm)
    {
        QDesktopWidget desktop;
        int screenX=desktop.availableGeometry().width();
        int screenY=desktop.availableGeometry().height();
        int wndX=frm->width();
        int wndY=frm->height();
        QPoint movePoint(screenX/2-wndX/2,screenY/2-wndY/2);
        frm->move(movePoint);
    }

    //����û����󻯰�ť
    static void FormNoMaxButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }

    //����û����󻯺���С����ť
    static void FormOnlyCloseButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinMaxButtonsHint);
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //���ô���͸����
    static void FormOpacity(QWidget *frm,double value)
    {
        frm->setWindowOpacity(value);
    }

    //���岻�ܸı��С
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //����ǰ����ʾ
    static void FormOnTop(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnTopHint);
    }

    //��������ʾ
    static void FormOnBottom(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnBottomHint);
    }

    //��ȡ�����С
    static QSize GetDesktopSize()
    {
        QDesktopWidget desktop;
        return QSize(desktop.availableGeometry().width(),desktop.availableGeometry().height());
    }

    //����ָ������Ϊplastique��ʽ
    static void SetPlastiqueStyle(QWidget *frm)
    {
        frm->setStyle(QStyleFactory::create("Plastique"));
    }

    static void SetCustomStyle(QWidget *frm)
    {
        QString myStyle1="QPushButton {";
        myStyle1.append("border: 2px solid #8f8f91; ");
        myStyle1.append("border-radius: 6px;");
        myStyle1.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde); ");
        myStyle1.append("min-width: 80px;}");
        myStyle1.append("QPushButton:pressed {");
        myStyle1.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dadbde, stop: 1 #f6f7fa); }");
        myStyle1.append("QPushButton:default  {");
        myStyle1.append("border-color: navy; /* ʹ�ð�ť��ʾ��ø���ͻ�� */}");

        QString myStyle2="QSlider::groove:horizontal {";
        myStyle2.append("border: 1px solid #999999;");
        myStyle2.append("height: 8px;");
        myStyle2.append("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);");
        myStyle2.append("margin: 2px 0;}");
        myStyle2.append("QSlider::handle:horizontal {");
        myStyle2.append("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);");
        myStyle2.append("border: 1px solid #5c5c5c;");
        myStyle2.append("width: 18px; margin: -2px 0;");
        myStyle2.append("border-radius: 3px;}");

        QString myStyle3="QGroupBox {";
        myStyle3.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,  stop: 0 #E0E0E0, stop: 1 #FFFFFF);");
        myStyle3.append("border: 2px solid gray;");
        myStyle3.append("border-radius: 5px;");
        myStyle3.append("margin-top: 1ex;}");
        myStyle3.append("QGroupBox::title {");
        myStyle3.append("subcontrol-origin: margin; ");
        myStyle3.append("subcontrol-position: top center;");
        myStyle3.append("padding: 0 3px;}");

        frm->setStyleSheet(myStyle1+myStyle2+myStyle3);
    }

    //�ı���ʽ
    static bool InstallStyle(QWidget* window,QString styleFile)
    {
        QFile file(styleFile);
        QString strStyleSheet;
        if (!file.open(QIODevice::WriteOnly))
        {
            strStyleSheet= "";
        }
        else
        {
            strStyleSheet=file.readAll();
        }
        file.close();

        if(strStyleSheet.isEmpty())
        {
            return false;
        }
        window->setStyleSheet(strStyleSheet);
        return true;
    }
};


class FileHelper : public QObject
{
public:    

    //��ȡѡ���ļ�
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(NULL,tr("ѡ���ļ�"),QCoreApplication::applicationDirPath(),filter);
    }

    //��ȡѡ���ļ�����
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(NULL,tr("ѡ���ļ�"),QCoreApplication::applicationDirPath(),filter);
    }

    //��ȡѡ���ļ����е��ļ�
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0)
        {
            QDir myFolder(strFolder);

            if (myFolder.exists())
            {
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //�ļ��Ƿ����
    static bool FileExists(QString strFile)
    {
        QFile tempFile(strFile);
        if (tempFile.exists())
        {
            return true;
        }
        return false;
    }

    //�ļ����Ƿ����
    static bool FolderExists(QString strFolder)
    {
        QDir tempFolder(strFolder);
        if(tempFolder.exists())
        {
            return true;
        }
        return false;
    }

    //�Ƴ��ļ�
    static bool RemoveFile(QString strFile)
    {
        return QFile::remove(strFile);
    }

    //�������ļ�
    static bool RenameFile(QString oldName,QString newName)
    {
        return QFile::rename(oldName,newName);
    }

    //��ȡ�ļ�Ŀ¼
    static QString GetBasePath(QString strFullPath)
    {
        QFileInfo fileInfo(strFullPath);
        return fileInfo.path();
    }

    //��ȡ��չ��
    static QString GetExtension(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.suffix();
    }

    //��ȡ�ļ�������
    static QString GetFolderName(QString strFullPath)
    {
        QString strPath(strFullPath);
        strPath.replace('\\','/');
        int pos=strPath.lastIndexOf('/');
        return strPath.mid(pos+1);
    }

    //�����ļ�
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileExists(targetFile))
        {
            int ret=QMessageBox::information(NULL,tr("��ʾ"),tr("�ļ��Ѿ����ڣ��Ƿ��滻��"),QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes)
            {
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }

    //�ж��Ƿ����ļ�
    static bool IsFile(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.isFile();
    }

    //�ж��Ƿ����ļ���
    static bool IsFolder(QString strFolder)
    {
        QFileInfo fileInfo(strFolder);
        return fileInfo.isDir();
    }

    //׷���ı�
    static bool AppendText(QString strFile, QString strContent )
    {
        QFile inFile(strFile);
        if (!inFile.open(QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::information(NULL,tr("��ʾ"),tr("���ļ�����"));
            return false;
        }
        QTextStream inStream(&inFile);
        inStream<<strContent;
        inFile.close();
        return true;
    }

    //�����ļ���
    static bool CreateFolder(QString strFolder)
    {
        QDir dir;
        return dir.mkdir(strFolder);
    }

    //�����ļ�
    static bool CreateFile(QString strFile)
    {
        QFile file(strFile);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }
        file.close();
        return true;
    }

    //��ȡ�ļ�����
    static QString GetFileContent(QString strFile)
    {
        QFile file(strFile);
        QString txt;
        if (!file.open(QIODevice::WriteOnly))
        {
            txt= "";
        }
        else
        {
            txt=file.readAll();
        }
        file.close();
        return txt;
    }

};

#endif // MYHELPER_H
