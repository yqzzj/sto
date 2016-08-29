#include "stockData.h"
#include <QDateTime>
#include <QStringList>

stockData::stockData(const QString &stockId, const QStringList &valueList)
{
    init(stockId, valueList);
}

stockData::stockData(const QStringList &valueList)
{
    QStringList list = valueList;
    list.removeFirst();
    init(valueList.first(), list);
}

stockData::stockData(const stockData &stock)
{
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
}

QString stockData::getStockId()
{
    return stockId;
}

QString stockData::getStockName()
{
    return stockName.remove("\"");
}

double stockData::getZrspj()
{
    return zrspj;
}

double stockData::getJrkpj()
{
    return jrkpj;
}

double stockData::getDqj()
{
    return dqj;
}

bool stockData::isToBuy(stockFindStation station, double value)
{
    bool is = false;

    switch(station){
    case GREATERTHAN:
        is = ((getStockIncreaseAmount() >= value) ? true : false) && (zrspj != 0);
        break;
    case LESSTHAN:
        is = ((getStockIncreaseAmount() <= value) ? true : false) && (zrspj != 0);
    default:
        break;
    }
    return is;
}

double stockData::getCurSwing()
{
    return (jrzgj - jrzdj) / zrspj;
}

bool stockData::isDailyLimitUp()
{
    return ((dqj - zrspj) / zrspj >= 0.1) ? true : false;
}

bool stockData::isDailyLimitDown()
{
    return ((dqj - zrspj) / zrspj <= -0.1) ? true : false;
}

double stockData::getStockDailyLimitAmount()
{
    return (dqj - zrspj) / zrspj;
}

double stockData::getStockIncreaseAmount()
{
    return (dqj - zrspj) / zrspj;
}

double stockData::getStockReduceAmount()
{
    return (jrkpj - dqj) / jrkpj;
}

void stockData::init(const QString &stockId, const QStringList &valueList)
{
    this->stockId = stockId;
    stockName = valueList.at(0);

    jrkpj = valueList.at(1).toDouble();
    zrspj = valueList.at(2).toDouble();
    dqj = valueList.at(3).toDouble();
    jrzgj = valueList.at(4).toDouble();
    jrzdj = valueList.at(5).toDouble();
    jmj_buy = valueList.at(6).toDouble();
    jmj_sale = valueList.at(7).toDouble();
    cjgps = valueList.at(8).toDouble();
    cjje = valueList.at(9).toDouble();

    gs_buy1 = valueList.at(10).toDouble();
    bj_buy1 = valueList.at(11).toDouble();
    gs_buy2 = valueList.at(12).toDouble();
    bj_buy2 = valueList.at(13).toDouble();
    gs_buy3 = valueList.at(14).toDouble();
    bj_buy3 = valueList.at(15).toDouble();
    gs_buy4 = valueList.at(16).toDouble();
    bj_buy4 = valueList.at(17).toDouble();
    gs_buy5 = valueList.at(18).toDouble();
    bj_buy5 = valueList.at(19).toDouble();

    gs_sale1 = valueList.at(20).toDouble();
    bj_sale1 = valueList.at(21).toDouble();
    gs_sale2 = valueList.at(22).toDouble();
    bj_sale2 = valueList.at(23).toDouble();
    gs_sale3 = valueList.at(24).toDouble();
    bj_sale3 = valueList.at(25).toDouble();
    gs_sale4 = valueList.at(26).toDouble();
    bj_sale4 = valueList.at(27).toDouble();
    gs_sale5 = valueList.at(28).toDouble();
    bj_sale5 = valueList.at(29).toDouble();

    QStringList ymdList = valueList.at(30).split("-");
    QStringList hmsList = valueList.at(31).split(":");
    date.setDate(ymdList.at(0).toInt(), ymdList.at(1).toInt(), ymdList.at(2).toInt());
    time.setHMS(hmsList.at(0).toInt(), hmsList.at(1).toInt(), hmsList.at(2).toInt());
}
