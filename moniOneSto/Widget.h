#ifndef WIDGET_H
#define WIDGET_H

#include "stockData.h"
#include <QWidget>
#include <QMap>
#include <QNetworkAccessManager>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString stoId, QString stoName, QWidget *parent = 0);
    ~Widget();
    void initStoId(const QString &id, const QString &name);

private slots:
    void onFinished(QNetworkReply* reply);

private:
    //初始化近期的所有收盘数据
    void initRecentStockRecord();
    //初始化以上股票id的股本列表
    void initGbMap();
    //历史收盘数据，总共记录id，name，...，日期，时间33个值
    void initStockRecordMapFromAssistFile(QMap<QString, stockData> &stoMap, const QString &filePath);
    void initStockRecordMap(QMap<QString, stockData> &stoMap, const QStringList &stockDataList);
    void clearRencentStockRecordMap();
    void initPalette();
    void moniSto();
    void updateStockData(QString str);
    void updateUi(stockData stock);
    //量比
    double getLb(const stockData &curStock);
    double getHsl(const stockData &curStock);
    //当前开市时长，单位分
    double getTime(const stockData &stock);

    Ui::Widget *ui;
    QPalette redPalette;
    QPalette greenPalette;
    QNetworkAccessManager *manager;
    QString stoId;
    QString stoName;
    bool isInit;
    //初始化时从文件中读出最近15天的数据
    QMap<QString, QMap<QString, stockData> > recentStockRecordMap;
    //以上股票id的股本列表
    QMap<QString, double> gbMap;
};

#endif // WIDGET_H
