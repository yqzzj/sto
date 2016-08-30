#include "widget.h"
#include <QApplication>

volatile bool dbConnectFlag = false;            //mysql���ݿ�����״̬
volatile bool NetConnectFlag = false;           //��������״̬
volatile int currentLoginID = -1;               //��ǰ�û���½��ʾ

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
    QString flag = QString::fromLocal8Bit(argv[1]);
    if(flag != "stoLogging") return 0;

#if QT_VERSION < 0x050000
    qInstallMsgHandler(qDebugMsgHandler);
#else
    qInstallMessageHandler(qDebugMsgHandler);
#endif

    qDebug() << "addLibraryPath: ./plugins ";
    QApplication::addLibraryPath("./plugins");      //��ӿؼ���·�������ƻ�������

    QApplication a(argc, argv);

    // �������ݿ�
    DatabaseHandle db;
    if(!db.createDBConnection())
    {
        qDebug() << "connect mysql fail!";
    }
    else
    {
        qDebug() << "connect mysql OK!";
    }

    Widget w;
    w.show();

    a.exec();         //����ʼ����
    db.closeDBConnection();     //�ر����ݿ�����
    int ret = w.isOk ? 1 : 0;
    qDebug() << "ret: " << ret;
    return ret;                 //�������н���
}
