#include "Widget.h"
#include "ui_Widget.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QPalette>
#include <QProcess>

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget)
{
    isGoing = false;
    currentRow = 0;
    m_webServiceForm = new webServiceForm(this);

    showDataFromNumber = 1;
    strList_result.append(tr("序号 记录时间 股票代码 股票名称 昨日收盘价 今日开盘价 当前价 涨幅 量比"));

    strListModel_result = new QStringListModel(this);
    strListModel_result->setStringList(strList_result);

    connect(m_webServiceForm, SIGNAL(toUpdateData(QList<stockData>)),
            this, SLOT(resultDataChanged(QList<stockData>)));
    connect(m_webServiceForm, SIGNAL(downLoadFileOver()),
            this, SLOT(downLoadFileOver()));

    ui->setupUi(this);
    initUi();
}

Widget::~Widget()
{
    delete ui;
    if(m_webServiceForm != NULL){
        delete m_webServiceForm;
    }
}

void Widget::initUi()
{
    this->setLayout(ui->verticalLayout);

    ui->listView_result->setModel(strListModel_result);
    ui->listView_result->setEditTriggers(QAbstractItemView::NoEditTriggers);

    recorveValues();

    hideSetting(true);

    ui->pushButton_boughtRecord->hide();
    ui->pushButton_refresh->hide();
    ui->pushButton_start->hide();
    ui->pushButton_downLoad->hide();
    ui->pushButton_stop->hide();
    ui->pushButton_recorve->hide();
    //ui->listView_result->
}

void Widget::newProMoniOne(const QString &stockId, const QString &stockName)
{
    QProcess *pro = new QProcess(this);
    QStringList arg;
    arg.append(stockId);
    arg.append(stockName);
    pro->start("./moniOneSto.exe", arg);
}

void Widget::hideSetting(bool is)
{
    if(is){
        ui->groupBox_strategy->hide();
    }else{
        ui->groupBox_strategy->show();
    }
}

void Widget::refreshFtpData()
{
    m_webServiceForm->getDataFromFtp();
}

void Widget::uploadFtpData()
{
    m_webServiceForm->putDataToFtp();
}

void Widget::closeEvent(QCloseEvent *event)
{
    (void)event;
}

void Widget::on_pushButton_start_clicked()
{
    if(!isGoing){
        isGoing = true;
        m_webServiceForm->isGoing = true;
        setItemEnabled(false);
        on_pushButton_refresh_clicked();
        m_webServiceForm->downloadFile();
    }
}

void Widget::on_pushButton_stop_clicked()
{
    if(isGoing){
        isGoing = false;
        m_webServiceForm->isGoing = false;
        setItemEnabled(true);
    }
}

void Widget::on_doubleSpinBox_todayUp_valueChanged(double arg1)
{
    m_webServiceForm->setStationValue1(arg1);
}

void Widget::setItemEnabled(bool en)
{
    ui->checkBox_1->setEnabled(en);
    ui->checkBox1_M->setEnabled(en);
    ui->checkBox1_N->setEnabled(en);

    ui->checkBox_2->setEnabled(en);
    ui->checkBox2_M->setEnabled(en);
    ui->checkBox2_N->setEnabled(en);
    ui->checkBox2_X->setEnabled(en);

    ui->checkBox_3->setEnabled(en);
    ui->checkBox3_M->setEnabled(en);
    ui->checkBox3_X->setEnabled(en);

    ui->checkBox_4->setEnabled(en);
    ui->doubleSpinBox_swing->setEnabled(en);

    ui->checkBox_5->setEnabled(en);

    ui->checkBox_6->setEnabled(en);
    ui->lbValue->setEnabled(en);

    ui->checkBox_9->setEnabled(en);
    ui->checkBox9_M->setEnabled(en);

    ui->doubleSpinBox_todayDown->setEnabled(en);
    ui->doubleSpinBox_todayUp->setEnabled(en);
    ui->doubleSpinBox_yestodayDown->setEnabled(en);
    ui->doubleSpinBox_yestodayUp->setEnabled(en);

    //换手率
    ui->checkBox_7->setEnabled(en);
    ui->hslValue->setEnabled(en);
    ui->checkBox_10->setEnabled(en);
    ui->hsl10n->setEnabled(en);
    ui->hsl10x->setEnabled(en);
    ui->checkBox_11->setEnabled(en);
    ui->hsl11n->setEnabled(en);
    ui->hsl11x->setEnabled(en);
}

void Widget::recorveValues()
{
    ui->doubleSpinBox_todayUp->setValue(0.08);
    on_doubleSpinBox_todayUp_valueChanged(0.08);
    ui->doubleSpinBox_yestodayDown->setValue(0.05);
    on_doubleSpinBox_yestodayDown_valueChanged(0.05);
    ui->doubleSpinBox_yestodayUp->setValue(0.00);
    on_doubleSpinBox_yestodayUp_valueChanged(0.00);

    ui->checkBox_1->setChecked(true);
    on_checkBox_1_clicked(true);
    ui->checkBox1_M->setValue(6);
    on_checkBox1_M_valueChanged(6);
    ui->checkBox1_N->setValue(3);
    on_checkBox1_N_valueChanged(3);

    ui->checkBox_2->setChecked(true);
    on_checkBox_2_clicked(true);
    ui->checkBox2_M->setValue(6);
    on_checkBox2_M_valueChanged(6);
    ui->checkBox2_N->setValue(2);
    on_checkBox2_N_valueChanged(2);
    ui->checkBox2_X->setValue(0.04);
    on_checkBox2_X_valueChanged(0.04);

    ui->checkBox_3->setChecked(true);
    on_checkBox_3_clicked(true);
    ui->checkBox3_M->setValue(6);
    on_checkBox3_M_valueChanged(6);
    ui->checkBox3_X->setValue(0.1);
    on_checkBox3_X_valueChanged(0.1);

    ui->checkBox_4->setChecked(true);
    on_checkBox_4_clicked(true);
    ui->doubleSpinBox_swing->setValue(0.125);
    on_doubleSpinBox_swing_valueChanged(0.125);

    ui->checkBox_5->setChecked(true);
    on_checkBox_5_clicked(true);

    ui->checkBox_6->setChecked(true);
    on_checkBox_6_clicked(true);
    ui->lbValue->setValue(2);
    on_lbValue_valueChanged(2);

    ui->doubleSpinBox_todayDown->setValue(0.1);
    on_doubleSpinBox_todayDown_valueChanged(0.1);

    ui->checkBox_9->setChecked(false);
    on_checkBox_9_clicked(false);
    ui->checkBox9_M->setValue(10);
    on_checkBox9_M_valueChanged(10);

    ui->checkBox_7->setChecked(true);
    on_checkBox_7_clicked(true);
    ui->hslValue->setValue(0.04);
    on_hslValue_valueChanged(0.04);

    ui->checkBox_10->setChecked(true);
    on_checkBox_10_clicked(true);
    ui->hsl10n->setValue(3);
    on_hsl10n_valueChanged(3);
    ui->hsl10x->setValue(0.04);
    on_hsl10x_valueChanged(0.04);

    ui->checkBox_11->setChecked(true);
    on_checkBox_11_clicked(true);
    ui->hsl11n->setValue(3);
    on_hsl11n_valueChanged(3);
    ui->hsl11x->setValue(20);
    on_hsl11x_valueChanged(20);
}

void Widget::on_doubleSpinBox_yestodayDown_valueChanged(double arg1)
{
    m_webServiceForm->setStationValue2(arg1);
}

//void Widget::resultDataChanged()
//{
//    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
//    QString curPath = "./buyStocks/" + date;
//    QFile file(curPath);
//    QTextStream in(&file);
//    strList_result.clear();
//    strList_result.append("行号 记录时间 股票代码 股票名称 昨日收盘价 今日开盘价 当前价 涨幅");

//    if(file.exists()){
//        if(file.open(QIODevice::ReadOnly)){
//            int i = 1;
//            while(!in.atEnd()){
//                QString str = QString::number(i) + " ";
//                str += in.readLine();
//                if(i >= showDataFromNumber){
//                    strList_result.append(str);
//                }
//                ++i;
//            }
//        }else{
//            QMessageBox::information(NULL,tr("错误提示"),tr("文件打开失败。"));
//        }
//    }
//    strListModel_result->setStringList(strList_result);
//    file.close();
//    m_webServiceForm->initStoRecordIdList(strList_result);
//}

void Widget::resultDataChanged(QList<stockData> stoList)
{
    foreach(stockData stock, stoList){
        QString str = QString::number(strList_result.count()) + " "
                        + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz ")
                        + stock.getStockId() + " " + stock.getStockName().remove("\"") + " "
                        + QString::number(stock.getZrspj()) + " " + QString::number(stock.getJrkpj()) + " "
                        + QString::number(stock.getDqj()) + " " + QString::number(stock.getStockIncreaseAmount())
                + " " + QString::number(m_webServiceForm->getLb(stock));
        strList_result.append(str);
    }
    strListModel_result->setStringList(strList_result);
}

void Widget::downLoadFileOver()
{
    m_webServiceForm->isDownLoadFile = false;
    QMessageBox::information(NULL,tr("提示"),tr("下载实时数据成功。"));
}

void Widget::on_listView_result_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(row){
        QString result = strList_result.at(row);
        QStringList boughtStrList = result.split(" ");
        QString stockId = boughtStrList.at(3);
        QString stockName = boughtStrList.at(4);
        newProMoniOne(stockId, stockName);
    }
}

void Widget::on_checkBox_1_clicked(bool checked)
{
    m_webServiceForm->box1Checked = checked;
}

void Widget::on_checkBox_2_clicked(bool checked)
{
    m_webServiceForm->box2Checked = checked;
}

void Widget::on_checkBox_3_clicked(bool checked)
{
    m_webServiceForm->box3Checked = checked;
}

void Widget::on_checkBox_4_clicked(bool checked)
{
    m_webServiceForm->box4Checked = checked;
}

void Widget::on_checkBox_5_clicked(bool checked)
{
    m_webServiceForm->box5Checked = checked;
}

void Widget::on_checkBox1_M_valueChanged(int arg1)
{
    m_webServiceForm->box1_M = arg1;
}

void Widget::on_checkBox1_N_valueChanged(int arg1)
{
    m_webServiceForm->box1_N = arg1;
}

void Widget::on_checkBox2_M_valueChanged(int arg1)
{
    m_webServiceForm->box2_M = arg1;
}

void Widget::on_checkBox2_N_valueChanged(int arg1)
{
    m_webServiceForm->box2_N = arg1;
}

void Widget::on_checkBox2_X_valueChanged(double arg1)
{
    m_webServiceForm->box2_X = arg1;
}

void Widget::on_checkBox3_M_valueChanged(int arg1)
{
    m_webServiceForm->box3_M = arg1;
}

void Widget::on_checkBox3_X_valueChanged(double arg1)
{
    m_webServiceForm->box3_X = arg1;
}

void Widget::on_doubleSpinBox_swing_valueChanged(double arg1)
{
    m_webServiceForm->box4_swingValue = arg1;
}

void Widget::on_pushButton_downLoad_clicked()
{
    if(!isGoing){
        m_webServiceForm->stockGroup = 0;
        m_webServiceForm->isDownLoadFile = true;
        m_webServiceForm->deleteTodayAssistFile();
        m_webServiceForm->downloadFile();
    }else{
        QMessageBox::information(NULL,tr("提示"),tr("请在停止状态下下载数据。"));
    }
}

void Widget::on_pushButton_refresh_clicked()
{
    strList_result.clear();
    strList_result.append(tr("序号 记录时间 股票代码 股票名称 昨日收盘价 今日开盘价 当前价 涨幅 量比"));
    strListModel_result->setStringList(strList_result);
    m_webServiceForm->clearStoRecordIdList();
}

void Widget::on_listView_result_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}

void Widget::on_pushButton_boughtRecord_clicked()
{
    QProcess *pro = new QProcess(this);
    pro->start("./dataView.exe");
}

void Widget::on_checkBox_6_clicked(bool checked)
{
    m_webServiceForm->box6Checked = checked;
}

void Widget::on_checkBox_7_clicked(bool checked)
{
    m_webServiceForm->box7Checked = checked;
}

void Widget::on_lbValue_valueChanged(double arg1)
{
    m_webServiceForm->box6_lbValue = arg1;
}

void Widget::on_hslValue_valueChanged(double arg1)
{
    m_webServiceForm->box7_hslValue = arg1;
}

void Widget::on_doubleSpinBox_todayDown_valueChanged(double arg1)
{
    m_webServiceForm->box8_value = arg1;
}

void Widget::on_pushButton_recorve_clicked()
{
    if(!isGoing){
        recorveValues();
    }
}

void Widget::on_checkBox_9_clicked(bool checked)
{
    m_webServiceForm->box9Checked = checked;
}

void Widget::on_checkBox9_M_valueChanged(int arg1)
{
    m_webServiceForm->box9_M = arg1;
}

void Widget::on_checkBox_10_clicked(bool checked)
{
    m_webServiceForm->box10Checked = checked;
}

void Widget::on_checkBox_11_clicked(bool checked)
{
    m_webServiceForm->box11Checked = checked;
}

void Widget::on_hsl10n_valueChanged(int arg1)
{
    m_webServiceForm->box10_n = arg1;
}

void Widget::on_hsl10x_valueChanged(double arg1)
{
    m_webServiceForm->box10_x = arg1;
}

void Widget::on_hsl11n_valueChanged(int arg1)
{
    m_webServiceForm->box11_n = arg1;
}

void Widget::on_hsl11x_valueChanged(double arg1)
{
    m_webServiceForm->box11_x = arg1;
}

void Widget::on_doubleSpinBox_yestodayUp_valueChanged(double arg1)
{
    m_webServiceForm->setYestodayUpValue(arg1);
}
