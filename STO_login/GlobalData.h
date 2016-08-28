#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QString>
#include "userdefine/myhelper.h"

//Debug��־�ļ�
#define DEBUGLOG_FILE_NAME      "debuglog.txt"

//��ɫ
#define RED_COLOR     255,0,0

//!ȫ�ֱ���,volatile���������ǲ��ȶ���,�����ڴ�ʱ��֤����̷߳��ֵı�������ͬһ��ֵ!
extern volatile bool dbConnectFlag;          //mysql���ݿ�����״̬

extern volatile bool NetConnectFlag;         //��������״̬

extern volatile int currentLoginID;          //��ǰ�û���½��ʾ

//�û���½��ʾ������1,����0
enum UserLoginFlag{
            GoOnlineFlag = 1,               //�û�����
            OfflineFlag = 0,                //�û�����
};

//�������ĳ���(����4)
#define SERIAL_DATALENGHT       10              //�ֽ�����ʹ��
//֡ͷ
#define SERIALDATA_HEAD_1       0x55            //������֡ͷ1
#define SERIALDATA_HEAD_2       0xAA            //������֡ͷ2
//֡β
#define SERIALDATA_TAIL_1       0xEE            //������֡β1
#define SERIALDATA_TAIL_2       0xFF            //������֡β2

//������������(�����)
enum DataTypeFlag{
            MainDataType = 1,               //��������������
            InsulateOrderType = 2,          //�����������
            FaultInfoType = 3,              //���Ͻ�������
            OnlineStatusType = 4,           //����״̬����
};

//�ض�ͼ�갴ť��ʾ
enum PushButtonFlag{
            PushButton11 = 0x11,            //11�Ű�ť,���ش�
            PushButton12 = 0x12,
            PushButton13 = 0x13,
            PushButton14 = 0x14,
            PushButton15 = 0x15,
            PushButton16 = 0x16,
            PushButton17 = 0x17,
            PushButton18 = 0x18,
            PushButton19 = 0x19,
            PushButton20 = 0x20,
            PushButton10 = 0x10,
            PushButton9 = 0x09,
            PushButton8 = 0x08,
            PushButton7 = 0x07,

            PushButton1 = 0x1,
            PushButton2 = 0x2,
            PushButton3 = 0x3,
            PushButton4 = 0x4,
            PushButton5 = 0x5,
            PushButton6 = 0x6,
            PushButton21 = 0x21,
            PushButton22 = 0x22,
};

//LineEdit��ֵ������ʾ
enum LineEditValueFlag{
            NetVoltageFlag = 0x21,          //��ѹ
            PrimaryCurrentFlag = 0x22,      //ԭ�ߵ���
            BatteryVoltageFlag = 0x23,      //���ص�ѹ
            BrakeFlag = 0x24,               //�ƶ�
};

#define DATABASE_WRITE_OK       "DBWO"      //����д�����ݿ�ɹ�
#define DATABASE_WRITE_NO       "DBWN"      //����д�����ݿ�ʧ��

#define UNKNOWN_DATA            "unknown"   //δ֪����

//!���洰���С
#define WINDOW_HIGH     630                 //����ĸ�
#define WINDOW_WEIGHT   800                 //����Ŀ�

#define MASTER_MENUBAR_HIGH     40          //���˵�����
#define MASTER_MENUBAR_WEIGHT   800         //���˵�����

#define STATUSBAR_HIGH      40              //״̬����
#define STATUSBAR_WEIGHT    800             //״̬����

#define DATAWIDGET_HIGH      585            //���ݽ����
#define DATAWIDGET_WEIGHT    800            //���ݽ����

//!������Ϣ
//x.x.5�ǵ��԰�, x.x.0�Ƿ�����
#define M_WINDOWTITLE       "StoGod��Ʊ����ϵͳV1.0.5.20160822"
#define SoftwareVersion     "V1.0.5.20160822"
#define CopyrightCompany    "StoGod"
#define WEB_URL             "www.stogod.com"

#endif // GLOBALDATA_H
