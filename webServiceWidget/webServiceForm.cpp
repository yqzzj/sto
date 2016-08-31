#include "webServiceForm.h"
#include "globalDefine.h"
#include "QNetworkAccessManager"
#include <QUrl>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QDir>
#include <QTextCodec>
#include <QCoreApplication>
#include <QSsl>
#include <QProgressDialog>

#define eps 0.00001

webServiceForm::webServiceForm(QWidget *parent) :
    QWidget(parent)
{
    stockGroup = 0;
    isDownLoadFile = false;
    isGoing = false;
    upReply = NULL;
    proDlg = NULL;

    //打开软件时首先创建出今天的交易文件，昨日某涨幅的辅助文件，涨停文件，跌停文件
    //getFilePath(stocksBoughtFileDir);
    getFilePath(assistStocksFileDir);
    init();
}

webServiceForm::~webServiceForm()
{
    if(manager != NULL){
        delete manager;
    }
    if(ftpDownLoader != NULL){
        delete ftpDownLoader;
    }
    if(ftpUpLoader != NULL){
        delete ftpUpLoader;
    }
}

void webServiceForm::clearStoRecordIdList()
{
    stoRecordIdList.clear();
}

void webServiceForm::downloadFile()
{
    //上海证券交易所sh，深圳证券交易所sz
    //上证指数sh000001，深证指数sz399001
    QString urlStr = "http://hq.sinajs.cn/list=";
//    QString postUrlStr = "http://hq.sinajs.cn";
//    QByteArray postData;

    if(stockGroup == 0){
        urlStr += stockIds1;
//        postData.append("list=" + stockIds1);
        stockGroup++;
    }else if(stockGroup == 1){
        urlStr += stockIds2;
//        postData.append("list=" + stockIds2);
        stockGroup++;
    }else if(stockGroup == 2){
        urlStr += stockIds3;
//        postData.append("list=" + stockIds3);
        stockGroup++;
    }else if(stockGroup == 3){
        urlStr += stockIds4;
//        postData.append("list=" + stockIds4);
        stockGroup = 0;
    }else{
        return;
    }

    QUrl url(urlStr);
//    QSslConfiguration config;
//    config.setPeerVerifyMode(QSslSocket::VerifyNone);
//    config.setProtocol(QSsl::TlsV1_1);

    QNetworkRequest request(url);
//    QNetworkRequest request(postUrlStr);
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    manager->get(request);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

//    request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());
//    manager->post(request, postData);
}

void webServiceForm::getDataFromFtp()
{
    QUrl url;
    url.setScheme(scheme);
    url.setUserName(userName);
    url.setPassword(password);
    url.setHost(host);
    url.setPort(port);
    url.setPath(path);
    QNetworkRequest request(url);
    ftpDownLoader->get(request);
}

static int fileNo = -1;
void webServiceForm::putDataToFtp()
{
    QUrl url;
    url.setScheme(scheme);
    url.setUserName(userName);
    url.setPassword(password);
    url.setHost(host);
    url.setPort(port);

    QDir d(assistStocksFileDir);
    QFileInfoList infoList = d.entryInfoList(QDir::Files, QDir::Name);
    int count = infoList.count();
    qDebug() << "count: " << count;

    if(fileNo == -1){
        fileNo = count -2;
    }
    if(fileNo >= 0){
        QString fileName = infoList.at(fileNo).fileName();
        QString filePath = assistStocksFileDir +  fileName;
        QFile file(filePath);
        QByteArray data;
        if(file.exists()){
            qDebug() << "exist" << fileName;
            if(file.open(QIODevice::ReadOnly)){
                qDebug() << "open ok";
                data = file.readAll();
            }
            file.close();
        }
        url.setPath(pathup + fileName);
        qDebug() << "put path: " << pathup + fileName;
        QNetworkRequest request(url);
        upReply = ftpUpLoader->put(request, data);
        connect(upReply,SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(updateDataReadProgress(qint64,qint64)));
    }
//    QFile file("./assistStocks/2016-07-25");
//    QByteArray data;
//    if(file.exists()){
//        qDebug() << "exist";
//        if(file.open(QIODevice::ReadOnly)){
//            qDebug() << "open ok";
//            data = file.readAll();
//        }
//        file.close();
//    }
//    url.setPath(pathup);
//    qDebug() << "put path: " << pathup;
//    QNetworkRequest request(url);
//    ftpUpLoader->put(request, data);
}

void webServiceForm::setFindStockStation1(stockFindStation station)
{
    m_station1 = station;
}

stockFindStation webServiceForm::getFindStockStation1()
{
    return m_station1;
}

void webServiceForm::setStationValue1(double value)
{
    m_stationValue1 = value;
}

double webServiceForm::getStationValue1()
{
    return m_stationValue1;
}

void webServiceForm::setStationValue2(double value)
{
    m_stationValue2 = value;
}

void webServiceForm::setYestodayUpValue(double value)
{
    m_yestodayUp = value;
}

double webServiceForm::getStationValue2()
{
    return m_stationValue2;
}

double webServiceForm::getYestodayUpValue()
{
    return m_yestodayUp;
}

bool webServiceForm::meetCondition1(const stockData &curStock)
{
    bool ret = true;

    if(box1Checked){
        ret = fun_mnx(curStock, box1_M, box1_N, 0);
    }
    return ret;
}

bool webServiceForm::meetCondition2(const stockData &curStock)
{
    bool ret = true;

    if(box2Checked){
        ret = fun_mnx(curStock, box2_M, box2_N, box2_X);
    }
    return ret;
}

bool webServiceForm::meetCondition3(const stockData &curStock)
{
    bool ret = false;
    int recStoRecCount = recentStockRecordMap.count();
    QStringList keyList = recentStockRecordMap.keys();
    int keysCount = keyList.count();
    QString curStockId = curStock.stockId;

    if(box3Checked){
        if(recStoRecCount >= box3_M){
            QString key = keyList.at(keysCount - box3_M);
            stockData stock = recentStockRecordMap.value(key).value(curStockId);
            if((box3_X - ((curStock.zrspj - stock.zrspj) / stock.zrspj) > 0) && stock.zrspj != 0){
                ret = true;
            }
        }
    }else{
        ret = true;
    }
    return ret;
}

bool webServiceForm::meetCondition4(const stockData &curStock)
{
    bool ret = false;
    if(box4Checked){
        if((box4_swingValue - ((curStock.jrzgj - curStock.jrzdj) / curStock.zrspj) > 0) && curStock.zrspj != 0){
            ret = true;
        }
    }else{
        ret = true;
    }
    return ret;
}

bool webServiceForm::meetCondition6(const stockData &curStock)
{
    bool ret = true;
    if(box6Checked){
        double curLb = getLb(curStock);
        if(box6_lbValue - curLb > 0){
            ret = false;
        }
    }
    return ret;
}

bool webServiceForm::meetCondition7(const stockData &curStock)
{
    bool ret = false;
    if(box7Checked){
        if((getHsl(curStock) - box7_hslValue) > 0 && gbMap.value(curStock.stockId) != 0){
            ret = true;
        }
    }else{
        ret = true;
    }
    return ret;
}

bool webServiceForm::meetCondition9(const stockData &curStock)
{
    bool ret = true;
    if(box9Checked){
        QStringList keyList = recentStockRecordMap.keys();
        int keyCount = keyList.count();
        stockData stock = recentStockRecordMap.value(keyList.at(keyCount - box9_M)).value(curStock.stockId);
        if(stock.dqj - curStock.dqj <= 0){
            ret = false;
        }
    }
    return ret;
}

bool webServiceForm::meetCondition10(const stockData &curStock)
{
    bool ret = false;
    int recStoRecCount = recentStockRecordMap.count();
    QStringList keyList = recentStockRecordMap.keys();
    int keysCount = keyList.count();
    QString curStockId = curStock.stockId;

    if(box10Checked){
        if(recStoRecCount >= box10_n){
            ret = true;
            for(int i = 1; i <= box10_n; i++){
                int k = keysCount - 1;
                if(getHsl(recentStockRecordMap.value(keyList.at(k)).value(curStockId)) - box10_x <= 0){
                    ret = false;
                }
            }
        }
    }else{
        ret = true;
    }
    return ret;
}

bool webServiceForm::meetCondition11(const stockData &curStock)
{
    bool ret = false;
    int recStoRecCount = recentStockRecordMap.count();
    QStringList keyList = recentStockRecordMap.keys();
    int keysCount = keyList.count();
    QString curStockId = curStock.stockId;

    if(box11Checked){
        if(recStoRecCount >= box11_n + 5){
            ret = true;
            for(int i = 1; i <= box11_n; i++){
                double totalValue = 0;
                QString key = keyList.at(keysCount - i);
                double value = getHsl(recentStockRecordMap.value(key).value(curStockId));
                for(int j = 1; j <= 5; j++){
                    QString keyj = keyList.at(keysCount - i - j);
                    totalValue += getHsl(recentStockRecordMap.value(keyj).value(curStockId));
                }
                double pjValue = totalValue / 5;
                if(value <= (pjValue * box11_x)){
                    ret = false;
                    break;
                }
            }
        }
    }else{
        ret = true;
    }
    return ret;
}

bool webServiceForm::fun_mnx(const stockData &curStock, int m, int n, double x)
{
    bool ret = true;
    int recStoRecCount = recentStockRecordMap.count();
    QStringList keyList = recentStockRecordMap.keys();
    int keysCount = keyList.count();
    //M个交易日中实际连续上涨的天数
    int upDayCount = 0;
    QString curStockId = curStock.stockId;

    if(m >= n){
        //已记录的收盘数据天数大于等于M
        if(recStoRecCount >= m){
            //前m个交易日数据
            for(int i = keysCount - 1; i >= keysCount - m; --i){
                //key为交易日文件名
                QString key = keyList.at(i);
                //key交易日的curStockId数据
                stockData stock = recentStockRecordMap.value(key).value(curStockId);
                //上涨且前一日不停盘
                bool isUp = ((stock.getStockIncreaseAmount() - x > 0) ? true : false) && (stock.zrspj != 0);
                if(isUp){
                    ++upDayCount;
                }else{
                    upDayCount = 0;
                }
                if(upDayCount == n){
                    ret = false;
                    break;
                }
            }
        }else{
            //若条件为前六个交易日不出现连续三天上涨，那么当记录只有5个时，返回false
            ret = false;
//            for(int i = 0; i < recStoRecCount; ++i){
//                //key为交易日文件名
//                QString key = keyList.at(i);
//                //key交易日的curStockId数据
//                stockData stock = recentStockRecordMap.value(key).value(curStockId);
//                //上涨且前一日不停盘
//                bool isUp = ((stock.getStockIncreaseAmount() > x) ? true : false) && (stock.zrspj != 0);
//                if(isUp){
//                    ++upDayCount;
//                }else{
//                    upDayCount = 0;
//                }
//                if(upDayCount == n){
//                    ret = false;
//                    break;
//                }
//            }
        }
    }
    return ret;
}

double webServiceForm::getLb(const stockData &curStock)
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

double webServiceForm::getHsl(const stockData &curStock)
{
    return curStock.cjgps / gbMap.value(curStock.stockId);
}

double webServiceForm::getTime(const stockData &stock)
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

void webServiceForm::deleteTodayAssistFile()
{
    QString path = getFilePath(assistStocksFileDir);
    QFile file(path);
    if(file.isOpen() || file.open(QIODevice::ReadWrite)){
        file.resize(0);
    }
    file.close();
}

void webServiceForm::onFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QStringList stockDataList;
    QNetworkReply::NetworkError error = reply->error();

    if(error != QNetworkReply::NoError){
        if(error != QNetworkReply::ProtocolInvalidOperationError){
            QMessageBox::information(NULL,tr("错误提示"),tr("从服务器获取数据错误。"));
            qDebug() << tr("错误代码：") << error;
            return;
        }
    }

    while(!reply->atEnd()){
        stockDataList.append(codec->toUnicode(reply->readLine()));
    }

    //findDailyLimitStocks();
    //交易日15点5分0秒到10秒生成辅助文件
    if(isDownLoadFile){
        findAssistStocks(stockDataList);
        if(stockGroup != 0){
            downloadFile();
        }else{
            emit downLoadFileOver();
        }
    }else{
        stockDataMap.clear();
        initStockRecordMap(stockDataMap, stockDataList);
        findBestStocks();
        if(isGoing){
            downloadFile();
        }
    }
    reply->deleteLater();
}

void webServiceForm::ftpDownLoad(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QStringList stockDataList;
    QNetworkReply::NetworkError error = reply->error();

    if(error != QNetworkReply::NoError){
        QMessageBox::information(NULL,tr("错误提示"),tr("从服务器获取数据错误。"));
        qDebug() << tr("错误代码：") << error;
        reply->deleteLater();
        return;
    }

    while(!reply->atEnd()){
        stockDataList.append(codec->toUnicode(reply->readLine()));
    }

    qDebug() << stockDataList;
    reply->deleteLater();
}

void webServiceForm::ftpUpLoad(QNetworkReply *reply)
{
    qDebug() << "ftpUplod over";
    QNetworkReply::NetworkError error = reply->error();

    if(error != QNetworkReply::NoError){
        QMessageBox::information(NULL,tr("错误提示"),tr("从服务器获取数据错误。"));
        qDebug() << tr("错误代码：") << error;
        reply->deleteLater();
        return;
    }
    fileNo -= 1;
    putDataToFtp();
    reply->deleteLater();
}

webServiceForm::updateDataReadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "progress";
    if(proDlg == NULL){
        proDlg = new QProgressDialog(this);
        proDlg->setRange(0, bytesTotal);
    }else{
        proDlg->setValue(bytesReceived);
        if(bytesReceived == bytesTotal){
            delete proDlg;
            proDlg = NULL;
        }
    }
}

void webServiceForm::init()
{
    m_station1 = GREATERTHAN;
    m_station2 = LESSTHAN;
    m_stationValue1 = 0.08;
    m_stationValue2 = 0.05;
    m_yestodayUp = 0;
    box1Checked = true;
    box2Checked = true;
    box3Checked = true;
    box4Checked = true;
    box5Checked = true;
    box6Checked = true;
    box7Checked = true;
    box9Checked = false;
    box10Checked = true;
    box11Checked = true;
    box1_M = 6;
    box1_N = 3;
    box2_M = 6;
    box2_N = 2;
    box2_X = 0.04;
    box3_M = 6;
    box3_X = 0.1;
    box4_swingValue = 0.125;
    box6_lbValue = 2;
    box8_value = 0.1;

    box7_hslValue = 0.04;
    box10_n = 3;
    box10_x = 0.04;
    box11_n = 3;
    box11_x = 20;

    initStockIdList();
    initGbMap();
    initRecentStockRecord();
    this->manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onFinished(QNetworkReply *)));

    //ftp操作
    this->ftpDownLoader = new QNetworkAccessManager;
    connect(ftpDownLoader, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(ftpDownLoad(QNetworkReply *)));
    this->ftpUpLoader = new QNetworkAccessManager;
    connect(ftpUpLoader, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(ftpUpLoad(QNetworkReply *)));
}

void webServiceForm::initStockIdList()
{
    QFile file(stockIdsFilePath);
    QTextStream in(&file);

    if(file.isOpen() || file.open(QIODevice::ReadOnly)){
        //上证代码为1A0001，但是新浪数据为sh000001,sz000001是平安银行
        stockIdList.append("sh000001,");
        while(!in.atEnd()){
            QString str = in.readLine();
            //去掉line两端空白
            str = str.trimmed();
            QStringList stockInfos = str.split(" ");
            if(stockInfos.count() == 2){
                QString stockId = stockInfos.at(1);
                QString id;
                if(stockId.left(1) == "6"){//上海
                    id = "sh" + stockId + ",";
                }else if(stockId.left(1) == "0"){//深圳
                    id = "sz" + stockId + ",";
                }else if(stockId.left(1) == "3"){
                    id = "sz" + stockId + ",";
                }else{
                    continue;
                }
                stockIdList.append(id);
            }
        }
    }
    file.close();

    for(int i = 0; i < stockIdList.count(); i++){
        if(i < 850){
            stockIds1 += stockIdList.at(i);
        }else if(i < 1700){
            stockIds2 += stockIdList.at(i);
        }else if(i < 2550){
            stockIds3 += stockIdList.at(i);
        }else{
            stockIds4 += stockIdList.at(i);
        }
    }
}

void webServiceForm::initGbMap()
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

void webServiceForm::initRecentStockRecord()
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

void webServiceForm::clearRencentStockRecordMap()
{
    foreach(QString key, recentStockRecordMap.keys()){
        recentStockRecordMap[key].clear();
    }
}

void webServiceForm::initStockRecordMap(QMap<QString, stockData> &stoMap, const QStringList &stockDataList)
{
    for(int i = 0; i < stockDataList.count(); ++i){
        //拆分为两个子串，第一个为var hq_str_sh601006，第二个为逗号分开的多个value
        QStringList stockIdAndValues = stockDataList.at(i).split("=");
        if(stockIdAndValues.count() != 2){
            //qDebug() << tr("拆分为两个子串失败：") << stockIdAndValues;
            continue;
        }
        //将var hq_str_sh601006按"_"拆为3个子串
        QStringList stockIdInfo = stockIdAndValues.at(0).split("_");
        if(stockIdInfo.count() != 3){
            //qDebug() << tr("拆分代码部分失败：") << stockIdAndValues;
            continue;
        }
        //取value的列表
        QStringList stockValueList = stockIdAndValues.at(1).split(",");
        if(stockValueList.count() < 32){
            //qDebug() << tr("value列表数据不对：") << stockIdAndValues;
            continue;
        }
        //取Id：sh601006
        QString stockId = stockIdInfo.last();
        stockData stock(stockId, stockValueList);
        if(!stoMap.contains(stockId)){
            stoMap.insert(stockId, stock);
        }
    }
}

void webServiceForm::initStockRecordMapFromAssistFile(QMap<QString, stockData> &stoMap, const QString &filePath)
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

void webServiceForm::findBestStocks()
{
    bestStockList.clear();

    foreach(const QString &key, stockDataMap.keys()){
        stockData curStockData = stockDataMap.value(key);
        if(currentValueGreaterThan(curStockData) && !isStockBought(curStockData)
                && yesterdayValueLessThan(curStockData) && meetCondition1(curStockData)
                && meetCondition2(curStockData) && meetCondition3(curStockData)
                && meetCondition4(curStockData) && currentValueLessThan(curStockData)
                && meetCondition6(curStockData) && meetCondition9(curStockData)
                && meetCondition7(curStockData) && meetCondition10(curStockData)
                && meetCondition11(curStockData)){
            bestStockList.append(curStockData);
            if(!stoRecordIdList.contains(curStockData.stockId)){
                stoRecordIdList.append(curStockData.stockId);
            }
        }
    }

    if(!bestStockList.isEmpty()){
//        QString tips = "找到以下股票符合条件：";
//        if(bestStockList.count() > 3){
//            tips += bestStockList[0].getStockName() + ","
//                    + bestStockList[1].getStockName()
//                    + "," + bestStockList[2].getStockName() + "...";
//        }else{
//            for(int i = 0; i < bestStockList.count(); ++i){
//                if(i != bestStockList.count() - 1){
//                    tips += bestStockList[i].getStockName() + ",";
//                }else{
//                    tips += bestStockList[i].getStockName();
//                }
//            }
//        }
//        QMessageBox *box = new QMessageBox(this);
//        box->setWindowTitle(tr("查询提示"));
//        box->setText(tips);
//        messageBoxList.append(box);
//        box->show();
        //QMessageBox::information(NULL,tr("查询提示"), tips);
        //buyBestStocks();
        //emit toUpdateData();
        emit toUpdateData(bestStockList);
    }
}

void webServiceForm::findAssistStocks(QStringList stockDataList)
{
    assistStockList.clear();
    for(int i = 0; i < stockDataList.count(); ++i){
        QString curStockData = stockDataList.at(i);
        //拆分为两个子串，第一个为var hq_str_sh601006，第二个为逗号分开的多个value
        QStringList stockIdAndValues = curStockData.split("=");
        if(stockIdAndValues.count() != 2) continue;
        //将var hq_str_sh601006按"_"拆为3个子串
        QStringList stockIdInfo = stockIdAndValues.at(0).split("_");
        if(stockIdInfo.count() != 3) continue;
        //取value的列表
        QStringList stockValueList = stockIdAndValues.at(1).split(",");
        if(stockValueList.count() < 32) continue;
        assistStockList.append(curStockData);
    }
    if(!assistStockList.isEmpty()){
        recordAssistStocks();
    }
}

void webServiceForm::buyBestStocks()
{
    QString path = getFilePath(stocksBoughtFileDir);
    QFile file(path);
    QTextStream out(&file);

    if(file.isOpen() || file.open(QIODevice::WriteOnly | QIODevice::Append)){
        //筛选出的股票输出昨日收盘价，今日开盘价，当前价
        foreach(stockData stock, bestStockList){
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz ")
                << stock.getStockId() << " " << stock.getStockName().remove("\"") << " "
                << stock.getZrspj() << " " << stock.getJrkpj() << " " << stock.getDqj() << " "
                <<  stock.getStockIncreaseAmount() << endl;
        }
    }else{
        qDebug() << "stocksTransaction文件打开失败。";
        return;
    }
    file.close();
}

void webServiceForm::recordAssistStocks()
{
    QString path = getFilePath(assistStocksFileDir);
    QFile file(path);
    QTextStream out(&file);

    if(file.isOpen() || file.open(QIODevice::WriteOnly | QIODevice::Append)){
        //筛选出的股票输出昨日收盘价，今日开盘价，当前价
        foreach(QString stockStr, assistStockList){
            out << stockStr;
        }
    }else{
        qDebug() << "Assist文件打开失败。";
        return;
    }
    file.close();
}

void webServiceForm::saleBestStocks()
{

}

bool webServiceForm::isStockBought(stockData stock)
{
    bool is = false;
    //不重复记录时查找是否记录了该股票
    if(box5Checked){
        if(stoRecordIdList.contains(stock.stockId)){
            is = true;
        }
    }
    return is;
}

bool webServiceForm::isStockAssisted(stockData stock)
{
    QString path = getFilePath(assistStocksFileDir);
    return isHasRecord(path, stock);
}

bool webServiceForm::isStockInYestodayAssistFile(stockData stock)
{
    QString path = getFilePath(assistStocksFileDir, YESTERDAY);
    return isHasRecord(path, stock);
}

bool webServiceForm::isHasRecord(const QString &path, stockData stock)
{
    bool is = false;
    QFile file(path);
    QTextStream in(&file);
    QStringList stockList;

    if(file.isOpen() || file.open(QIODevice::ReadOnly)){
        while(!in.atEnd()){
            QString lineStr = in.readLine();
            //去掉line两端空白
            lineStr = lineStr.trimmed();
            stockList.append(lineStr);
        }
    }else{
        qDebug() << "isHasRecord时文件打开失败。" << path;
    }
    file.close();
    foreach(QString findStock, stockList){
        QStringList stockInfo = findStock.split(" ");
        if(stockInfo.contains(stock.getStockId())){
            is = true;
            break;
        }
    }
    return is;
}

QString webServiceForm::getFilePath(const QString &dir, date day)
{
    QString fileName;
    QDir tempDir;
    QString path;
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    if(!tempDir.exists(dir)){
        tempDir.mkdir(dir);
    }

    if(day == TODAY){
        fileName = date;
        QFile todayFile(dir + date);
        //当日的该文件不存在，则创建，一般会在初次打开文件时创建该文件
        //这里是为了防止第一天打开软件后不关闭，到第二天0点之后的情况
        if(!todayFile.exists()){
            todayFile.open(QIODevice::WriteOnly);
            todayFile.close();
        }
    }else if(day == YESTERDAY){
        QDir d(dir);
        QFileInfoList infoList = d.entryInfoList(QDir::Files, QDir::Name);
        int count = infoList.count();

        if(count >= 2){
            fileName = infoList.at(count - 2).fileName();
        }else{//只有在第一天使用软件时，才会获取不到昨天的相关文件
            fileName = date;
        }
    }
    path = dir + fileName;

    return path;
}

bool webServiceForm::currentValueGreaterThan(stockData stock)
{
    bool ret = ((stock.getStockIncreaseAmount() - m_stationValue1 >= 0) ? true : false)
            && (stock.zrspj != 0);
    return ret;
}

bool webServiceForm::currentValueLessThan(stockData stock)
{
    bool ret = true;
    ret = ((box8_value - stock.getStockIncreaseAmount() > 0) ? true : false)
            && (stock.zrspj != 0);
    return ret;
}

bool webServiceForm::yesterdayValueLessThan(const stockData &stock)
{
    bool ret = false;
    QString stockId = stock.stockId;

    if(!recentStockRecordMap.isEmpty()){
        QString yesterdatKey = recentStockRecordMap.keys().last();
        QStringList yesterdayStockIds = recentStockRecordMap.value(yesterdatKey).keys();
        if(yesterdayStockIds.contains(stockId)){
            stockData yesterdayData = recentStockRecordMap.value(yesterdatKey).value(stockId);
            bool ret1 = ((m_stationValue2 - yesterdayData.getStockIncreaseAmount() > 0) ? true : false)
                    && (yesterdayData.zrspj != 0);
            bool ret2 = ((m_yestodayUp - yesterdayData.getStockIncreaseAmount() <= 0) ? true : false)
                    && (yesterdayData.zrspj != 0);
            ret = ret1 && ret2;
        }else{//昨日该股停盘返回false
            ret = false;
        }
    }
    return ret;
}

void webServiceForm::sleep(int msec)
{
    QTime reachTime = QTime::currentTime().addMSecs(msec);

    while(QTime::currentTime() < reachTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
