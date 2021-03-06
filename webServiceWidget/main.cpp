﻿#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QFontDatabase>
#include <QTranslator>
#include <QSplashScreen>
#include <QDebug>
#include <QTcpSocket>
#include <QDateTime>
#include <QProcess>

#if QT_VERSION < 0x050000
void qDebugMsgHandler(QtMsgType type, const char *msg)
{
    QString typeStr = "";
    switch (type)
    {
        case QtDebugMsg:
        {
            typeStr = "[debug]";
            break;
        }
        case QtWarningMsg:
        {
            typeStr = "[warning]";
            break;
        }
        case QtCriticalMsg:
        {
            typeStr = "[critical]";
            break;
        }
        case QtFatalMsg:
        {
            typeStr = "[fatal]";
            break;
        }
    }

    QFile appLogFile("stock.log");
    appLogFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&appLogFile);
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")
       << " " << typeStr << " " << QString(msg) << endl;
   // logFile.flush();
   // logFile.close();
}
#else
void qDebugMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString typeStr = "";
    switch (type)
    {
        case QtDebugMsg:
        {
            typeStr = "[debug]";
            break;
        }
        case QtWarningMsg:
        {
            typeStr = "[warning]";
            break;
        }
        case QtCriticalMsg:
        {
            typeStr = "[critical]";
            break;
        }
        case QtFatalMsg:
        {
            typeStr = "[fatal]";
            break;
        }
    }

    QFile appLogFile("stock.log");
    appLogFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&appLogFile);
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")
       << " " << typeStr << " " << msg << endl << endl;
   // logFile.flush();
   // logFile.close();
}
#endif

int stoLogin()
{
    QProcess *pro = new QProcess;
    QStringList arg;
    arg.append("stoLogging");
    pro->start("./STO_login.exe", arg);

    if(!pro->waitForStarted()){
        qDebug() << "stoLoginPro started failed.";
    }else{
        pro->waitForFinished(3000000);
        return pro->exitCode();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //vc 编译时选择GB2312，若用mingw4，选择UTF-8，并且setCodecForCStrings()参数为GB2312
//    #ifdef _MSC_VER
//    QTextCodec *codec = QTextCodec::codecForName("GB2312");
//    #else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec1 = QTextCodec::codecForName("GB2312");
    //#endif
    if (codec==NULL) codec = QTextCodec::codecForLocale();//有些计算机GBK为空会乱码
    QTextCodec::setCodecForLocale(codec1);
#if QT_VERSION < 0x050000
    // Internal string conversion is all UTF-8
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec1);
#endif

    QApplication::addLibraryPath("./plugins");
    QTranslator appTranslator;
    appTranslator.load("./language/qt_zh_CN.qm");
    a.installTranslator(&appTranslator);

#if QT_VERSION < 0x050000
    qInstallMsgHandler(qDebugMsgHandler);
#else
    qInstallMessageHandler(qDebugMsgHandler);
#endif

    a.setWindowIcon(QIcon(":icons/sto"));

//    QSplashScreen splash(QPixmap(":/images/sto.png"), 0);
//    splash.show();
//    splash.setAutoFillBackground(true);

//    QString qss;
//    QFile qssFile("./qss/niceui.qss");
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen()){
//        qss = QLatin1String(qssFile.readAll());
//        a.setStyleSheet(qss);
//        qssFile.close();
//    }

    int loginRet = stoLogin();
    if(!loginRet){
        qDebug() << "login failed.";
        return -1;
    }

    MainWindow m;
//    splash.finish(&m);
    m.show();

    return a.exec();
}
