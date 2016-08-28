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

//#define SYS_LINUX           //适用于linux系统

#ifdef SYS_LINUX
#include <sys/statfs.h>     //查询文件系统相关的信息(linux)
#endif

//#define QT4_VERSION         //适用于Qt4.x,PC端

#ifdef QT4_VERSION
//QT4.x中模块
#include <QSound>
#else
//QT5.x中模块
#include <QtSerialPort/QSerialPortInfo>     //提供操作串口的各种接口
#include <QtSerialPort/QSerialPort>         //提供计算机中可用串口的各种信息
#endif

//显示警告信息,定时3s关闭信息
//box.setWindowFlags(Qt::FramelessWindowHint);
#define MESSAGEBOX(strshow) { QMessageBox box ;\
    QTimer::singleShot(3000, &box, SLOT(close()));\
    box.setWindowTitle(tr("警告"));\
    box.setIcon(QMessageBox::Warning);\
    box.setText(strshow);\
    box.exec();}

//类型重命名
typedef unsigned long long U64;

class myHelper:public QObject
{
public:

    #if 1       //DDU数据流处理函数
    //计算校验位
    static int calculateCheckBit(QByteArray temp)
    {
        //计算校验位,为帧头(和为FF)、界面号、控件号、数据共7字节累加和的最低八位
        //与: & 0xFF; 数组的前七个字节和
        int _checkBit = 0;   //累加之前先置为0

        for(int i = 0; i < 7; i++)
        {
            _checkBit += temp[i];
        }

        //最低八位
        _checkBit = _checkBit & 0xFF;

        //qDebug() << "_checkBit: " << _checkBit;
        //qDebug() << "_checkBit(16): 0x" << QString::number(_checkBit, 16);

        return _checkBit;
    }

    //初始化字节数组
    static QByteArray initByteArray(QByteArray temp, int dataType)
    {
        //10字节数据流
        temp[0] = 0x55;    //帧头
        temp[1] = 0xAA;
        temp[2] = dataType;

        for(int i = 3; i < 8; i++)
        {
            temp[i] = 0;
        }

        temp[8] = 0xEE;    //帧尾
        temp[9] = 0XFF;

        return temp;
    }
    #endif

    //设置全局为plastique样式
    static void SetPlastiqueStyle()
    {
        QApplication::setStyle(QStyleFactory::create("Plastique"));
        QApplication::setPalette(QApplication::style()->standardPalette());//替换应用程序的颜色组合
    }

#ifdef QT4_VERSION
    //设置合适的编码格式
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

    //设置编码为GB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    }

    //设置编码为UTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    }
#endif

    //设置定时关闭对话框(可设计成对话框显示倒计时)
    static void Show_Time_Message(QString strshow, int m_time)
    {
        QMessageBox box ;
        QTimer::singleShot(m_time, &box, SLOT(close()));
        box.setStyleSheet("font:12pt '宋体'");
        box.setWindowTitle(tr("温馨提示 (定时%1s,关闭对话框)")
                           .arg(m_time / 1000));
        box.setText(strshow);
        box.setIcon(QMessageBox::Information);
        box.addButton(tr("确定"),QMessageBox::ActionRole);
        box.exec();
    }

    //显示信息框，仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        //msg.setStyleSheet("font:12pt '微软雅黑'");
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '微软雅黑'"));
        //msg.setWindowTitle(tr("提示"));
        msg.setWindowTitle(QString::fromLocal8Bit("提示"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        //msg.addButton(tr("确定"),QMessageBox::ActionRole);
        msg.addButton(QString::fromLocal8Bit("确定"),QMessageBox::ActionRole);
        msg.exec();
    }

    //显示错误框，仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '微软雅黑'"));
        msg.setWindowTitle(QString::fromLocal8Bit("错误"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.addButton(QString::fromLocal8Bit("确定"),QMessageBox::ActionRole);
        msg.exec();
    }

    //显示询问框，确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '微软雅黑'"));
        msg.setWindowTitle(QString::fromLocal8Bit("询问"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);        
        msg.addButton(QString::fromLocal8Bit("取消"),QMessageBox::NoRole);
        msg.addButton(QString::fromLocal8Bit("确定"),QMessageBox::YesRole);

        return msg.exec();
    }

    static int ShowMessageBoxQuesionStandard(QString info)
    {
        QMessageBox msg;
        msg.setStyleSheet(QString::fromLocal8Bit("font:12pt '微软雅黑'"));
        msg.setWindowTitle(QString::fromLocal8Bit("询问"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);        
        msg.addButton(QMessageBox::No);
        msg.addButton(QMessageBox::Yes);

        return msg.exec();
    }

    //16进制字符串转字节数组
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

    //字节数组转16进制字符串
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

    //字节数组转换为字符串
    static QString ByteArrayToStr(const QByteArray &data)
    {
        QString tempdata = data.toHex().data(); //字符串长度都是偶数

        return tempdata;
    }

    //16进制字符串转10进制
    static uchar StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //整型转换为字节,test
    static void intToBit(unsigned int a)
    {
        unsigned int b;

        qDebug() <<"请输入一个数(小于4294967296，整数是4字节)：";
        qDebug() <<"各字节的值为(10进制)：";

        b = a & 0x000000ff;
        qDebug() <<"低字节为：" << b;

        b = a & 0x0000ff00;
        b/=256;
        qDebug() <<"次低字节为：" << b;

        b = a & 0x00ff0000;
        b/=65536;
        qDebug() <<"次高字节为：" << b;

        b = a & 0xff000000;
        b/=65536*256;
        qDebug() <<"高字节为：" << b;
    }

    //10进制字符串转10进制
    static uchar StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //16进制字符串转2进制字符串
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

    //计算校验码
    static uchar GetCheckCode(uchar data[],uchar len)
    {
        uchar temp=0;

        for (uchar i=0;i<len;i++)
        {
            temp+=data[i];
        }

        return temp%256;
    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    //获取当前路径
    static QString GetCurrentPath()
    {
        return QString(QCoreApplication::applicationDirPath()+"/");
    }

#ifdef QT4_VERSION
    //播放声音
    static void PlaySound(QString soundName)
    {
        QSound::play(soundName);
    }
#endif

    //获取当前日期时间星期
    static QString GetLongDateTime()
    {
        QDateTime time=QDateTime::currentDateTime();
        return time.toString("yyyy年MM月dd日 hh时mm分ss秒 dddd");
    }

    //打开网址
    static void OpenUrlOrExe(QString url)
    {
        QDesktopServices::openUrl(QUrl(url));
    }

    //判断是否闰年
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
    //查看磁盘空间(linux下使用)
    static int GetStorageInfo(char *MountPoint,     //存储卡一个分区
                              int *Capacity,        //想要获取的空间大小
                              int type)             //获取什么类型的空间
    {
        struct statfs statFS;   //系统stat的结构体
        U64 usedBytes = 0;
        U64 freeBytes = 0;
        U64 totalBytes = 0;
        U64 endSpace = 0;       //要获取类型的空间大小

        if(statfs(MountPoint, &statFS) == -1)
        {//获取分区的状态
            printf("statfs failed for path->[%s]\n", MountPoint);
            return -1;
        }

        //分区总容量, 以字节为单位
        totalBytes = (U64)statFS.f_blocks * (U64)statFS.f_frsize;
        //剩余空间容量，以字节为单位
        freeBytes = (U64)statFS.f_bfree * (U64)statFS.f_frsize;
        //已用空间容量，以字节为单位
        usedBytes = (U64)(totalBytes - freeBytes);

        switch( type )
        {
            case 1:
            endSpace = totalBytes/1024;     //以KB为单位的总容量
            break;

            case 2:
            endSpace = usedBytes/1024;      //以KB为单位的已用空间
            break;

            case 3:
            endSpace = freeBytes/1024;      //以KB为单位的剩余空间
            break;

            case 4:
            endSpace = totalBytes >> 20;    //以MB为单位的总容量
            break;

            case 5:
            endSpace = usedBytes >> 20;     //以MB为单位的已用空间
            break;

            case 6:
            endSpace = freeBytes >> 20;     //以MB为单位的剩余空间
            break;

            case 7:
            endSpace = totalBytes >> 30;    //以GB为单位的总容量
            break;

            case 8:
            endSpace = usedBytes >> 30;     //以GB为单位的已用空间
            break;

            case 9:
            endSpace = freeBytes >> 30;     //以GB为单位的剩余空间
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

    //按钮填充图片
    static void SetButtonBackGround(QPushButton *button, QPixmap picturepath)
    {
        button->setFixedSize( picturepath.width(), picturepath.height());
        button->setIcon(QIcon(picturepath));
        button->setFlat(true);
        button->setIconSize(QSize(picturepath.width(),picturepath.height()));;
        button->setToolTip("");
    }

    //窗体居中显示
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

    //窗体没有最大化按钮
    static void FormNoMaxButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }

    //窗体没有最大化和最小化按钮
    static void FormOnlyCloseButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinMaxButtonsHint);
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //设置窗体透明度
    static void FormOpacity(QWidget *frm,double value)
    {
        frm->setWindowOpacity(value);
    }

    //窗体不能改变大小
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //窗体前端显示
    static void FormOnTop(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnTopHint);
    }

    //窗体后端显示
    static void FormOnBottom(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnBottomHint);
    }

    //获取桌面大小
    static QSize GetDesktopSize()
    {
        QDesktopWidget desktop;
        return QSize(desktop.availableGeometry().width(),desktop.availableGeometry().height());
    }

    //设置指定窗体为plastique样式
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
        myStyle1.append("border-color: navy; /* 使得按钮显示变得更加突出 */}");

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

    //改变样式
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

    //获取选择文件
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(NULL,tr("选择文件"),QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择文件集合
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(NULL,tr("选择文件"),QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择文件夹中的文件
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

    //文件是否存在
    static bool FileExists(QString strFile)
    {
        QFile tempFile(strFile);
        if (tempFile.exists())
        {
            return true;
        }
        return false;
    }

    //文件夹是否存在
    static bool FolderExists(QString strFolder)
    {
        QDir tempFolder(strFolder);
        if(tempFolder.exists())
        {
            return true;
        }
        return false;
    }

    //移除文件
    static bool RemoveFile(QString strFile)
    {
        return QFile::remove(strFile);
    }

    //重命名文件
    static bool RenameFile(QString oldName,QString newName)
    {
        return QFile::rename(oldName,newName);
    }

    //获取文件目录
    static QString GetBasePath(QString strFullPath)
    {
        QFileInfo fileInfo(strFullPath);
        return fileInfo.path();
    }

    //获取拓展名
    static QString GetExtension(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.suffix();
    }

    //获取文件夹名称
    static QString GetFolderName(QString strFullPath)
    {
        QString strPath(strFullPath);
        strPath.replace('\\','/');
        int pos=strPath.lastIndexOf('/');
        return strPath.mid(pos+1);
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileExists(targetFile))
        {
            int ret=QMessageBox::information(NULL,tr("提示"),tr("文件已经存在，是否替换？"),QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes)
            {
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }

    //判断是否是文件
    static bool IsFile(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.isFile();
    }

    //判断是否是文件夹
    static bool IsFolder(QString strFolder)
    {
        QFileInfo fileInfo(strFolder);
        return fileInfo.isDir();
    }

    //追加文本
    static bool AppendText(QString strFile, QString strContent )
    {
        QFile inFile(strFile);
        if (!inFile.open(QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::information(NULL,tr("提示"),tr("打开文件出错！"));
            return false;
        }
        QTextStream inStream(&inFile);
        inStream<<strContent;
        inFile.close();
        return true;
    }

    //创建文件夹
    static bool CreateFolder(QString strFolder)
    {
        QDir dir;
        return dir.mkdir(strFolder);
    }

    //创建文件
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

    //读取文件内容
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
