#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H
#include <QString>

//股票代码
const QString stockIdsFilePath = "./stockIds_sh_sz.txt";
//股本
const QString gbFilePath = "./guben.txt";
//实时股票数据文件路径
const QString stockDataFilePath = "./stockData";
//已买的股票
const QString stocksBoughtFileDir = "./boughtStocks/";
//历史收盘数据
const QString assistStocksFileDir = "./assistStocks/";

//ftp参数
const QString scheme("ftp");
const QString userName("StoFTPData");
const QString password("Sto123456");
const QString host("139.196.47.109");
const int port(21);
const QString path("/home/StoFTPData/test");
const QString pathup("/home/StoFTPData/assistStocks/");

enum stockFindStation
{
    GREATERTHAN = 0,
    LESSTHAN = 1
};

enum date
{
    TODAY = 0,
    YESTERDAY = 1
};

enum dailyLimitType
{
    DAILYLIMITUP = 0,
    DAILYLIMITDOWN = 1
};

class globalDefine
{
public:
    globalDefine();
};

#endif // GLOBALDEFINE_H
