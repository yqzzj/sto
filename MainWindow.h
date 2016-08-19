#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Widget.h"
#include <QMainWindow>
#include <QPoint>
#include <QSystemTrayIcon>

class QLineEdit;
class QToolButton;
class QSystemTrayIcon;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    //工具栏按钮相关槽
    void on_actionRefresh_triggered();
    void on_actionStart_triggered();
    void on_actionDownload_triggered();
    void on_actionStop_triggered();
    void on_actionRecover_triggered();
    void on_actionExit_triggered();
    void lineEdit_returnPressed();
    void on_actionSetting_triggered(bool checked);

    //标题栏按钮相关槽
    void actionMinimize_triggered();
    void actionClose_triggered();
    void actionMaximize_triggered();

    //托盘图标
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleHidden();

private:
    Ui::MainWindow *ui;

    Widget *w;

    //窗体移动所用变量
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

    //标题栏按钮
    QToolButton *minButton;
    //帮助按钮
    QToolButton *helpButton;
    //关闭按钮
    QToolButton *closeButton;
    //最大按钮
    QToolButton *maxButton;
    //菜单按钮
    QToolButton *menuButton;
    QToolButton *normalButton;
    QToolButton *shadeButton;
    QToolButton *unshadeButton;

    QWidget *labelWidget;
    QLabel *label;
    QLineEdit *lineEdit;

    QSystemTrayIcon *trayIcon;

    QAction *toggleHideAction;

    QLabel *statusLabel;

    void initLineEdit();
    void initTitleBar();

    void createTrayIcon();
    void createActions();

    void initStatusBar();
};

#endif // MAINWINDOW_H
