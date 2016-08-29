#ifndef STOCKDATA_H
#define STOCKDATA_H

#include "globalDefine.h"
#include <QString>
#include <QDateTime>

class stockData
{
public:
    stockData(){}
    stockData(const QString &stockId, const QStringList &valueList);
    stockData(const QStringList &valueList);
    stockData(const stockData &stock);
    stockData& operator=(const stockData& stock){
        this->stockId = stock.stockId;
        stockName = stock.stockName;

        jrkpj = stock.jrkpj;
        zrspj = stock.zrspj;
        dqj = stock.dqj;
        jrzgj = stock.jrzgj;
        jrzdj = stock.jrzdj;
        jmj_buy = stock.jmj_buy;
        jmj_sale = stock.jmj_sale;
        cjgps = stock.cjgps;
        cjje = stock.cjje;

        gs_buy1 = stock.gs_buy1;
        bj_buy1 = stock.bj_buy1;
        gs_buy2 = stock.gs_buy2;
        bj_buy2 = stock.bj_buy2;
        gs_buy3 = stock.gs_buy3;
        bj_buy3 = stock.bj_buy3;
        gs_buy4 = stock.gs_buy4;
        bj_buy4 = stock.bj_buy4;
        gs_buy5 = stock.gs_buy5;
        bj_buy5 = stock.bj_buy5;

        gs_sale1 = stock.gs_sale1;
        bj_sale1 = stock.bj_sale1;
        gs_sale2 = stock.gs_sale2;
        bj_sale2 = stock.bj_sale2;
        gs_sale3 = stock.gs_sale3;
        bj_sale3 = stock.bj_sale3;
        gs_sale4 = stock.gs_sale4;
        bj_sale4 = stock.bj_sale4;
        gs_sale5 = stock.gs_sale5;
        bj_sale5 = stock.bj_sale5;

        date = stock.date;
        time = stock.time;
        return *this;
    }

    QString getStockId();
    QString getStockName();
    double getZrspj();
    double getJrkpj();
    double getDqj();

    //根据不同算法判断是否购买该只股票
    bool isToBuy(stockFindStation station, double value);

    //取当前振幅
    double getCurSwing();

    //是否涨停
    bool isDailyLimitUp();
    //是否跌停
    bool isDailyLimitDown();
    //涨跌停的涨幅，与昨日收盘价比较
    double getStockDailyLimitAmount();
    //获取股票涨幅
    double getStockIncreaseAmount();
    //获取股票跌幅
    double getStockReduceAmount();

private:
    void init(const QString &stockId, const QStringList &valueList);

public:
    //股票代码
    QString stockId;
    //股票名字
    QString stockName;

    //今日开盘价
    double jrkpj;
    //昨日收盘价
    double zrspj;
    //当前价
    double dqj;
    //今日最高价
    double jrzgj;
    //今日最低价
    double jrzdj;
    //竞买价，即“买一”报价
    double jmj_buy;
    //竞卖价，即“卖一”报价
    double jmj_sale;
    //成交的股票数，由于股票交易以一百股为基本单位，所以在使用时，通常把该值除以一百
    double cjgps;
    //成交金额，单位为“元”，为了一目了然，通常以“万元”为成交金额的单位，所以通常把该值除以一万
    double cjje;

    //“买一”股数，例：申请4695股，即47手
    double gs_buy1;
    //“买一”报价
    double bj_buy1;
    double gs_buy2;
    double bj_buy2;
    double gs_buy3;
    double bj_buy3;
    double gs_buy4;
    double bj_buy4;
    double gs_buy5;
    double bj_buy5;

    //“卖一”股数，例：申请4695股，即47手
    double gs_sale1;
    //“卖一”报价
    double bj_sale1;
    double gs_sale2;
    double bj_sale2;
    double gs_sale3;
    double bj_sale3;
    double gs_sale4;
    double bj_sale4;
    double gs_sale5;
    double bj_sale5;

    //日期
    QDate date;
    //时间
    QTime time;
};

#endif // STOCKDATA_H
