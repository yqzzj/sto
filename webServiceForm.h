#ifndef WEBSERVICEFORM_H
#define WEBSERVICEFORM_H

#include "stockData.h"
#include "globalDefine.h"
#include <QWidget>
#include <QMap>
#include <QList>
#include <QMessageBox>

class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class webServiceForm;
}

class webServiceForm : public QWidget
{
    Q_OBJECT

public:
    explicit webServiceForm(QWidget *parent = 0);
    ~webServiceForm();
    void clearStoRecordIdList();

    void downloadFile();
    void getDataFromFtp();

    void getFindStockStation1(stockFindStation &station, double value);

    void setFindStockStation1(stockFindStation station);
    stockFindStation getFindStockStation1();

    void setStationValue1(double value);
    double getStationValue1();

    void setStationValue2(double value);
    void setYestodayUpValue(double value);
    double getStationValue2();
    double getYestodayUpValue();

    //前M个交易日不出现连续N天上涨
    bool meetCondition1(const stockData &curStock);
    //前M个交易日不出现连续N天上涨均超过X
    bool meetCondition2(const stockData &curStock);
    //前M个交易日整体涨幅小于X,若数据不足，则返回false
    bool meetCondition3(const stockData &curStock);
    //当日振幅小于X
    bool meetCondition4(const stockData &curStock);
    //量比=现成交总手/(过去5日平均每分钟成交量×当日累计开市时间(分))
    bool meetCondition6(const stockData &curStock);
    //实时换手率大于
    bool meetCondition7(const stockData &curStock);
    //小于前第M日收盘价
    bool meetCondition9(const stockData &curStock);
    //换手率连续n日大于x
    bool meetCondition10(const stockData &curStock);
    //连续n日大于前5日平均值
    bool meetCondition11(const stockData &curStock);

    bool fun_mnx(const stockData &curStock, int m, int n, double x);
    //量比
    double getLb(const stockData &curStock);
    //换手率
    double getHsl(const stockData &curStock);
    //当前开市时长，单位分
    double getTime(const stockData &stock);

    void deleteTodayAssistFile();

    //前M个交易日不出现连续N天上涨
    bool box1Checked;
    int box1_M;
    int box1_N;

    //前M个交易日不出现连续N天上涨均超过X
    bool box2Checked;
    int box2_M;
    int box2_N;
    double box2_X;

    //前M个交易日整体涨幅小于X
    bool box3Checked;
    int box3_M;
    double box3_X;

    //当日振幅小于
    bool box4Checked;
    double box4_swingValue;

    //不重复记录已出现在结果中的股票
    bool box5Checked;

    bool box6Checked;
    //量比=现成交总手/(过去5日平均每分钟成交量×当日累计开市时间(分))
    double box6_lbValue;

    double box7Checked;
    //换手率=成交量/发行(/流通)总股数×100%
    double box7_hslValue;

    //实时涨幅小于
    bool box8Checked;
    double box8_value;

    //实时价小于第前M日收盘价
    bool box9Checked;
    int box9_M;

    //换手率连续n日大于x
    bool box10Checked;
    int box10_n;
    double box10_x;

    //换手率连续n日大于前5日平均值
    bool box11Checked;
    int box11_n;
    double box11_x;

    bool isDownLoadFile;
    bool isGoing;

signals:
    void toUpdateData();
    void toUpdateData(QList<stockData> stoList);
    void downLoadFileOver();

private slots:
    void onFinished(QNetworkReply *reply);
    void ftpDownLoad(QNetworkReply *reply);

private:
    void init();
    //初始化股票id列表
    void initStockIdList();
    //初始化以上股票id的股本列表
    void initGbMap();
    //初始化近期的所有收盘数据
    void initRecentStockRecord();
    void clearRencentStockRecordMap();

    void initStockRecordMap(QMap<QString, stockData> &stoMap, const QStringList &stockDataList);
    //历史收盘数据，总共记录id，name，...，日期，时间33个值
    void initStockRecordMapFromAssistFile(QMap<QString, stockData> &stoMap, const QString &filePath);

    //从一次查询后的股票数据中找到适合购买的股票
    void findBestStocks();
    //从一次查询后的股票数据中找到辅助股票
    void findAssistStocks(QStringList stockDataList);
    //从一次查询后的股票数据中找到涨停跌停的股票
    void findDailyLimitStocks();
    //购买股票
    void buyBestStocks();
    //记录辅助股票
    void recordAssistStocks();
    //记录涨停股票
    void recordDailyLimitStock(const QString &dir);
    //记录跌停股票
    void recordDailyLimitDownStock();
    //卖掉股票
    void saleBestStocks();
    //某只股票是否已经购买
    bool isStockBought(stockData stock);
    //某只股票是否已经记录到当日的辅助股票文件中
    bool isStockAssisted(stockData stock);
    //昨日辅助文件中是否记录了这只股票
    bool isStockInYestodayAssistFile(stockData stock);
    //判断股票在某天是否涨跌停
    bool isDailyLimit(stockData stock, date day, dailyLimitType type);
    //判断传入的路径文件中是否含有某只股票信息
    bool isHasRecord(const QString &path, stockData stock);

    //得到交易股票记录文件，涨停股记录文件，跌停股记录文件等的路径
    //dir为所在目录，返回该目录下的以日期命名的文件路径
    QString getFilePath(const QString &dir, date day = TODAY);

    //对昨日不开盘的没做处理
    //当前价较昨日收盘价涨幅大于
    bool currentValueGreaterThan(stockData stock);
    bool currentValueLessThan(stockData stock);
    //该股票昨日涨幅小于
    //注意，昨日停盘或者没有昨天的数据文件，都返回false
    bool yesterdayValueLessThan(const stockData &stock);

    QNetworkAccessManager *manager;
    QNetworkAccessManager *ftpDownLoader;

    //股票实时数据
    QMap<QString, stockData> stockDataMap;
    //初始化时从文件中读出最近15天的数据
    QMap<QString, QMap<QString, stockData> > recentStockRecordMap;
    QList<stockData> bestStockList;
    QStringList assistStockList;
    QList<stockData> dailyLimitUpStockList;
    QList<stockData> dailyLimitDownStockList;
    QStringList oldDailyLimitUpStockList;
    QStringList oldDailyLimitDownStockList;

    //股票查询条件
    stockFindStation m_station1;
    stockFindStation m_station2;
    //股票查询参数,当日涨幅大于
    double m_stationValue1;
    //股票查询参数,昨日涨幅小于
    double m_stationValue2;
    //昨日涨幅大于等于
    double m_yestodayUp;

    //股票id列表
    QStringList stockIdList;
    //以上股票id的股本列表
    QMap<QString, double> gbMap;
    QString stockIds1;
    QString stockIds2;
    QString stockIds3;
    QString stockIds4;

    //已经记录的股票代码，为了不重复记录时做计算
    QStringList stoRecordIdList;

    void sleep(int msec);

public:
    int stockGroup;
};

#endif // WEBSERVICEFORM_H
