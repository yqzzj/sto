#include "./Widget.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>

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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec1 = QTextCodec::codecForName("GB2312");
    if (codec==NULL) codec = QTextCodec::codecForLocale();//有些计算机GBK为空会乱码
    QTextCodec::setCodecForLocale(codec1);
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    QApplication::addLibraryPath("./plugins");
    QTranslator appTranslator;
    appTranslator.load("./language/qt_zh_CN.qm");
    a.installTranslator(&appTranslator);
    qInstallMessageHandler(qDebugMsgHandler);
    QString id = QString::fromLocal8Bit(argv[1]);
    QString name = QString::fromLocal8Bit(argv[2]);

    Widget w(id, name);
    w.show();

    return a.exec();
}
