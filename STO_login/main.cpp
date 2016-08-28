#include "widget.h"
#include <QApplication>

volatile bool dbConnectFlag = false;            //mysql���ݿ�����״̬
volatile bool NetConnectFlag = false;           //��������״̬
volatile int currentLoginID = -1;               //��ǰ�û���½��ʾ

int main(int argc, char *argv[])
{
    QString flag = QString::fromLocal8Bit(argv[1]);
    if(flag != "stoLogging") return 0;

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
