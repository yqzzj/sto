#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QString>
#include "userdefine/myhelper.h"

//Debug日志文件
#define DEBUGLOG_FILE_NAME      "debuglog.txt"

//红色
#define RED_COLOR     255,0,0

//!全局变量,volatile声明变量是不稳定的,共享内存时保证多个线程发现的变量都是同一个值!
extern volatile bool dbConnectFlag;          //mysql数据库连接状态

extern volatile bool NetConnectFlag;         //网络连接状态

extern volatile int currentLoginID;          //当前用户登陆标示

//用户登陆标示，上线1,下线0
enum UserLoginFlag{
            GoOnlineFlag = 1,               //用户上线
            OfflineFlag = 0,                //用户下线
};

//数据流的长度(大于4)
#define SERIAL_DATALENGHT       10              //字节数组使用
//帧头
#define SERIALDATA_HEAD_1       0x55            //数据流帧头1
#define SERIALDATA_HEAD_2       0xAA            //数据流帧头2
//帧尾
#define SERIALDATA_TAIL_1       0xEE            //数据流帧尾1
#define SERIALDATA_TAIL_2       0xFF            //数据流帧尾2

//接受数据类型(界面号)
enum DataTypeFlag{
            MainDataType = 1,               //主界面数据类型
            InsulateOrderType = 2,          //隔离界面数据
            FaultInfoType = 3,              //故障界面数据
            OnlineStatusType = 4,           //在线状态数据
};

//特定图标按钮标示
enum PushButtonFlag{
            PushButton11 = 0x11,            //11号按钮,开关打开
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

//LineEdit数值变量标示
enum LineEditValueFlag{
            NetVoltageFlag = 0x21,          //网压
            PrimaryCurrentFlag = 0x22,      //原边电流
            BatteryVoltageFlag = 0x23,      //蓄电池电压
            BrakeFlag = 0x24,               //制动
};

#define DATABASE_WRITE_OK       "DBWO"      //数据写入数据库成功
#define DATABASE_WRITE_NO       "DBWN"      //数据写入数据库失败

#define UNKNOWN_DATA            "unknown"   //未知数据

//!界面窗体大小
#define WINDOW_HIGH     630                 //窗体的高
#define WINDOW_WEIGHT   800                 //窗体的宽

#define MASTER_MENUBAR_HIGH     40          //主菜单栏高
#define MASTER_MENUBAR_WEIGHT   800         //主菜单栏宽

#define STATUSBAR_HIGH      40              //状态栏高
#define STATUSBAR_WEIGHT    800             //状态栏宽

#define DATAWIDGET_HIGH      585            //数据界面高
#define DATAWIDGET_WEIGHT    800            //数据界面宽

//!界面信息
//x.x.5是调试版, x.x.0是发布版
#define M_WINDOWTITLE       "StoGod股票分析系统V1.0.5.20160822"
#define SoftwareVersion     "V1.0.5.20160822"
#define CopyrightCompany    "StoGod"
#define WEB_URL             "www.stogod.com"

#endif // GLOBALDATA_H
