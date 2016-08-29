#ifndef WIDGET_H
#define WIDGET_H

#include "webServiceForm.h"
#include <QWidget>
#include <QStringListModel>
#include <QStack>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initUi();

    void newProMoniOne(const QString &stockId, const QString &stockName);

    void hideSetting(bool is);

    //ftp操作
    void refreshFtpData();
    void uploadFtpData();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_doubleSpinBox_todayUp_valueChanged(double arg1);
    void on_doubleSpinBox_yestodayDown_valueChanged(double arg1);

    //void resultDataChanged();
    void resultDataChanged(QList<stockData> stoList);
    void downLoadFileOver();

    void on_listView_result_doubleClicked(const QModelIndex &index);

    void on_checkBox_1_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox1_M_valueChanged(int arg1);

    void on_checkBox1_N_valueChanged(int arg1);

    void on_checkBox2_M_valueChanged(int arg1);

    void on_checkBox2_N_valueChanged(int arg1);

    void on_checkBox2_X_valueChanged(double arg1);

    void on_checkBox3_M_valueChanged(int arg1);

    void on_checkBox3_X_valueChanged(double arg1);

    void on_doubleSpinBox_swing_valueChanged(double arg1);

    void on_pushButton_downLoad_clicked();

    void on_pushButton_refresh_clicked();

    void on_listView_result_clicked(const QModelIndex &index);

    void on_pushButton_boughtRecord_clicked();

    void on_checkBox_6_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_lbValue_valueChanged(double arg1);

    void on_hslValue_valueChanged(double arg1);

    void on_doubleSpinBox_todayDown_valueChanged(double arg1);

    void on_pushButton_recorve_clicked();

    void on_checkBox_9_clicked(bool checked);

    void on_checkBox9_M_valueChanged(int arg1);

private slots:
    void on_checkBox_10_clicked(bool checked);

    void on_checkBox_11_clicked(bool checked);

    void on_hsl10n_valueChanged(int arg1);

    void on_hsl10x_valueChanged(double arg1);

    void on_hsl11n_valueChanged(int arg1);

    void on_hsl11x_valueChanged(double arg1);

    void on_doubleSpinBox_yestodayUp_valueChanged(double arg1);

private:
    webServiceForm *m_webServiceForm;
    Ui::Widget *ui;

    bool isGoing;
    QStringList strList_result;
    QStringListModel *strListModel_result;
    int showDataFromNumber;
    int currentRow;

    void setItemEnabled(bool en);

    void recorveValues();
};

#endif // WIDGET_H
