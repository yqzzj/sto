#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>

class TitleWidget;
class ToolWidget;
class StackedWidget;
class QStatusBar;
class QVBoxLayout;
class Widget;
class QLabel;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

private:
    Widget *w;
    TitleWidget *titleWidget;
    ToolWidget *toolWidget;
    StackedWidget *stackWidget;
    QStatusBar *statusBar;
    QVBoxLayout *mainLayout;

    QPoint normalTopLeft;
    QLabel *statusLabel;

    QSystemTrayIcon *trayIcon;
    QAction *toggleHideAction;
    QAction *actionExit;

    void createTitleWidget();
    void createToolWidget();
    void createStackWidget();
    void createStatusBar();
    void createMainLayout();
    void createToolWidgetConnections();
    void createTrayIcon();
    void createActions();

private slots:
    //工具栏按钮相关槽
    void actionRefresh_triggered();
    void actionStart_triggered();
    void actionDownload_triggered();
    void actionStop_triggered();
    void actionRecover_triggered();
    void actionExit_triggered();
    void lineEdit_returnPressed();
    void actionSetting_triggered(bool checked);

    //托盘图标
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleHidden();

    void showMax();
    void showMin();
    void showSkinWidget();
};

#endif // MAINWIDGET_H
