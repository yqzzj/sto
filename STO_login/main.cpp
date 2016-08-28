#include "widget.h"
#include <QApplication>

volatile bool dbConnectFlag = false;            //mysql数据库连接状态
volatile bool NetConnectFlag = false;           //网络连接状态
volatile int currentLoginID = -1;               //当前用户登陆标示

int main(int argc, char *argv[])
{
    QString flag = QString::fromLocal8Bit(argv[1]);
    if(flag != "stoLogging") return 0;

    qDebug() << "addLibraryPath: ./plugins ";
    QApplication::addLibraryPath("./plugins");      //添加控件库路径，类似环境变量

    QApplication a(argc, argv);

    // 连接数据库
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

    a.exec();         //程序开始运行
    db.closeDBConnection();     //关闭数据库连接
    int ret = w.isOk ? 1 : 0;
    qDebug() << "ret: " << ret;
    return ret;                 //程序运行结束
}
