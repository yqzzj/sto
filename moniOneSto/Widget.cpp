#include "Widget.h"
#include "ui_Widget.h"

#include <QNetworkReply>
#include <QDebug>
#include <QDir>
#include <QTextCodec>

//历史收盘数据
const QString assistStocksFileDir = "./assistStocks/";
//股本
const QString gbFilePath = "./guben.txt";

Widget::Widget(QString stoId, QString stoName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    manager = new QNetworkAccessManager(this);
    this->stoId = stoId;
    this->stoName = stoName;
    isInit = true;

    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    this->setWindowTitle(stoId.right(6));

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply *)));

    initPalette();
    initRecentStockRecord();
    initGbMap();
    moniSto();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initStoId(const QString &id, const QString &name)
{
    stoId = id;
    stoName = name;
}

void Widget::onFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QString stoData = QString(codec->toUnicode(reply->readAll()));
    updateStockData(stoData);
    moniSto();
}

void Widget::initRecentStockRecord()
{
    QDir d(assistStocksFileDir);
    QFileInfoList infoList = d.entryInfoList(QDir::Files, QDir::Name);
    int count = infoList.count();

    clearRencentStockRecordMap();
    if(count > 1){
        for(int i = count - 2; i >= 0; --i){
            if(count - i < 17){
                QString fileName = infoList.at(i).fileName();
                QString filePath = assistStocksFileDir +  fileName;
                QMap<QString, stockData> stockRecordMap;
                initStockRecordMapFromAssistFile(stockRecordMap, filePath);
                if(!recentStockRecordMap.contains(fileName)){
                    recentStockRecordMap.insert(fileName, stockRecordMap);
                }
            }else{
                break;
            }
        }
    }
}

void Widget::initGbMap()
{
    QFile file(gbFilePath);
    QTextStream in(&file);

    if(file.isOpen() || file.open(QIODevice::ReadOnly)){
        while(!in.atEnd()){
            QString str = in.readLine();
            //去掉line两端空白
            str = str.trimmed();
            QStringList gbInfos = str.split("\t");
            if(gbInfos.count() == 2){
                QString id = gbInfos.at(0);
                QString gb = gbInfos.at(1);
                if(gb.isEmpty() || gb.contains("-")) continue;
                double gbdouble = gb.toDouble();
                gbMap.insert(id, gbdouble);
            }
        }
    }
    file.close();
}

void Widget::initStockRecordMapFromAssistFile(QMap<QString, stockData> &stoMap, const QString &filePath)
{
    QFile file(filePath);
    QStringList stockDataList;
    if(!file.isOpen() && !file.open(QIODevice::ReadOnly)){
        qDebug()<<"Can't open the file!"<<endl;
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd()){
        QString lineStr = in.readLine();
        //去掉line两端空白
        lineStr = lineStr.trimmed();
        stockDataList.append(lineStr);
    }
    file.close();
    initStockRecordMap(stoMap, stockDataList);
}

void Widget::initStockRecordMap(QMap<QString, stockData> &stoMap, const QStringList &stockDataList)
{
    for(int i = 0; i < stockDataList.count(); ++i){
        //拆分为两个子串，第一个为var hq_str_sh601006，第二个为逗号分开的多个value
        QStringList stockIdAndValues = stockDataList.at(i).split("=");
        if(stockIdAndValues.count() != 2) continue;
        //将var hq_str_sh601006按"_"拆为3个子串
        QStringList stockIdInfo = stockIdAndValues.at(0).split("_");
        if(stockIdInfo.count() != 3) continue;
        //取value的列表
        QStringList stockValueList = stockIdAndValues.at(1).split(",");
        if(stockValueList.count() < 32) continue;
        //取Id：sh601006
        QString stockId = stockIdInfo.last();
        stockData stock(stockId, stockValueList);
        if(!stoMap.contains(stockId)){
            stoMap.insert(stockId, stock);
        }
    }
}

void Widget::clearRencentStockRecordMap()
{
    foreach(QString key, recentStockRecordMap.keys()){
        recentStockRecordMap[key].clear();
    }
}

void Widget::initPalette()
{
    redPalette.setColor(QPalette::Text,Qt::red);
    greenPalette.setColor(QPalette::Text,Qt::green);
}

void Widget::moniSto()
{
    QString urlStr = "http://hq.sinajs.cn/list=" + stoId;
    QUrl url(urlStr);
    QNetworkRequest request(url);
    manager->get(request);
}

void Widget::updateStockData(QString str)
{
    //拆分为两个子串，第一个为var hq_str_sh601006，第二个为逗号分开的多个value
    QStringList stockIdAndValues = str.split("=");
    if(stockIdAndValues.count() != 2) return;
    //将var hq_str_sh601006按"_"拆为3个子串
    QStringList stockIdInfo = stockIdAndValues.at(0).split("_");
    if(stockIdInfo.count() != 3) return;
    //取value的列表
    QStringList stockValueList = stockIdAndValues.at(1).split(",");
    if(stockValueList.count() < 32) return;
    //取Id：sh601006
    QString stockId = stockIdInfo.last();
    stockData stock(stockId, stockValueList);
    updateUi(stock);
}

void Widget::updateUi(stockData stock)
{
    double up = stock.getStockIncreaseAmount();
    double lb = getLb(stock);
    double hsl = getHsl(stock);
    if(isInit){
        isInit = false;
        ui->lineEdit_id->setText(stoId.right(6));
        ui->lineEdit_name->setText(stoName);
        ui->lineEdit_zrspj->setText(QString::number(stock.zrspj));
        ui->lineEdit_jrkpj->setText(QString::number(stock.jrkpj));
    }
    if(up > 0 && stock.zrspj > 0){
        ui->lineEdit_up->setPalette(redPalette);
        ui->lineEdit_dqj->setPalette(redPalette);
        ui->lineEdit_b1->setPalette(redPalette);
        ui->lineEdit_s1->setPalette(redPalette);
        ui->lineEdit_b2->setPalette(redPalette);
        ui->lineEdit_s2->setPalette(redPalette);
        ui->lineEdit_b3->setPalette(redPalette);
        ui->lineEdit_s3->setPalette(redPalette);
        ui->lineEdit_b4->setPalette(redPalette);
        ui->lineEdit_s4->setPalette(redPalette);
        ui->lineEdit_b5->setPalette(redPalette);
        ui->lineEdit_s5->setPalette(redPalette);
    }else{
        ui->lineEdit_up->setPalette(greenPalette);
        ui->lineEdit_dqj->setPalette(greenPalette);
        ui->lineEdit_b1->setPalette(greenPalette);
        ui->lineEdit_s1->setPalette(greenPalette);
        ui->lineEdit_b2->setPalette(greenPalette);
        ui->lineEdit_s2->setPalette(greenPalette);
        ui->lineEdit_b3->setPalette(greenPalette);
        ui->lineEdit_s3->setPalette(greenPalette);
        ui->lineEdit_b4->setPalette(greenPalette);
        ui->lineEdit_s4->setPalette(greenPalette);
        ui->lineEdit_b5->setPalette(greenPalette);
        ui->lineEdit_s5->setPalette(greenPalette);
    }
    if(ui->lineEdit_name->text().isEmpty()){
        ui->lineEdit_name->setText(stock.stockName.remove("\""));
    }
    ui->lineEdit_up->setText(QString::number(up * 100, 'f', 2) + "%");
    ui->lineEdit_dqj->setText(QString::number(stock.dqj));
    ui->lineEdit_zgj->setText(QString::number(stock.jrzgj));
    ui->lineEdit_zdj->setText(QString::number(stock.jrzdj));
    ui->lineEdit_cjs->setText(QString::number(stock.cjgps / 100));
    ui->lineEdit_cje->setText(QString::number(stock.cjje / 10000));
    ui->lineEdit_b1->setText(QString::number(stock.bj_buy1));
    ui->lineEdit_s1->setText(QString::number(stock.bj_sale1));
    ui->lineEdit_b2->setText(QString::number(stock.bj_buy2));
    ui->lineEdit_s2->setText(QString::number(stock.bj_sale2));
    ui->lineEdit_b3->setText(QString::number(stock.bj_buy3));
    ui->lineEdit_s3->setText(QString::number(stock.bj_sale3));
    ui->lineEdit_b4->setText(QString::number(stock.bj_buy4));
    ui->lineEdit_s4->setText(QString::number(stock.bj_sale4));
    ui->lineEdit_b5->setText(QString::number(stock.bj_buy5));
    ui->lineEdit_s5->setText(QString::number(stock.bj_sale5));
    ui->lineEdit_lb->setText(QString::number(lb, 'f', 2));
    ui->lineEdit_hsl->setText(QString::number(hsl, 'f', 4));
}

double Widget::getLb(const stockData &curStock)
{
    double lbValue = -99;
    QString curStoId = curStock.stockId;
    int recStoRecCount = recentStockRecordMap.count();
    QList<stockData> fiveDayData;
    QStringList keyList = recentStockRecordMap.keys();

    if(recStoRecCount >= 5){
        for(int i = recStoRecCount - 1; i >= 0; --i){
            QString key = keyList.at(i);
            if(recentStockRecordMap.value(key).value(curStoId).jrkpj > 0){
                fiveDayData.append(recentStockRecordMap.value(key).value(curStoId));
                if(fiveDayData.count() == 5){
                    break;
                }
            }
        }
        if(fiveDayData.count() == 5){
            //当日累计开市时间，单位分
            double time = getTime(curStock);
            //过去5日总成交量
            double value = 0;
            //过去5日平均每分钟成交量
            double valueMnt = 0;
            foreach(const stockData &stock, fiveDayData){
                value += stock.cjgps;
            }
            valueMnt = value / ((4 * 60) * 5);
            //量比=现成交总手/(过去5日平均每分钟成交量×当日累计开市时间(分))
            lbValue = curStock.cjgps / (valueMnt * time);
        }
    }
    return lbValue;
}

double Widget::getHsl(const stockData &curStock)
{
    return curStock.cjgps / gbMap.value(curStock.stockId);
}

double Widget::getTime(const stockData &stock)
{
    double retValue = 1;
    QTime curTime = stock.time;
    int curHour = curTime.hour();
    int curMin = curTime.minute();
    int curSec = curTime.second();
    double curTotleMits = curHour * 60 + curMin + curSec / 60;
    //curTotleMits应该是从九点半到11点半,1点到3点
    if(curTotleMits <= 690){//11点半以前
        retValue = curTotleMits - 570;
    }else{//11点半以后
        if(curHour == 11){
            retValue = 120;
        }else{
            retValue = curTotleMits - 570 - 90;
        }
    }
//    if(retValue > 240){
//        retValue = 240;
//    }
    return retValue;
}
