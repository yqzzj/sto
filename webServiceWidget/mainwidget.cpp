#include "mainwidget.h"
#include "Widget.h"
#include "stackedwidget.h"
#include "titlewidget.h"
#include "toolwidget.h"

#include <QWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QMenu>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent), titleWidget(0), toolWidget(0), stackWidget(0), statusBar(0), mainLayout(0)
{
    resize(850,600);
    setWindowFlags(Qt::FramelessWindowHint);

    createTitleWidget();
    createToolWidget();
    createStackWidget();
    createStatusBar();
    createMainLayout();
    createActions();
    createTrayIcon();
    setLayout(mainLayout);
}

void MainWidget::createTitleWidget()
{
    titleWidget = new TitleWidget(this);
    connect(titleWidget, SIGNAL(showMax()), this, SLOT(showMax()));
    connect(titleWidget, SIGNAL(showMin()), this, SLOT(showMin()));
    connect(titleWidget, SIGNAL(showSkin()), this, SLOT(showSkinWidget()));
}

void MainWidget::createToolWidget()
{
    toolWidget = new ToolWidget(this);
    toolWidget->setFocus();
    createToolWidgetConnections();
}

void MainWidget::createStackWidget()
{
    stackWidget = new StackedWidget(this);
    w = new Widget(this);
    stackWidget->addWidget(w);
    stackWidget->setCurrentWidget(w);
}

void MainWidget::createStatusBar()
{
    statusBar = new QStatusBar;
    statusBar->setMinimumWidth(600);
    statusLabel = new QLabel(this);
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusBar->addWidget(statusLabel);
    statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

void MainWidget::createMainLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(titleWidget);
    mainLayout->addWidget(toolWidget);
    mainLayout->addWidget(stackWidget);
    stackWidget->setContentsMargins(1,5,1,0);
    mainLayout->addWidget(statusBar);
}

void MainWidget::createToolWidgetConnections()
{
    connect(toolWidget, SIGNAL(toRefresh()), this, SLOT(actionRefresh_triggered()));
    connect(toolWidget, SIGNAL(toStart()), this, SLOT(actionStart_triggered()));
    connect(toolWidget, SIGNAL(toDownLoad()), this, SLOT(actionDownload_triggered()));
    connect(toolWidget, SIGNAL(toStop()), this, SLOT(actionStop_triggered()));
    connect(toolWidget, SIGNAL(toRecover()), this, SLOT(actionRecover_triggered()));
    connect(toolWidget, SIGNAL(toQuit()), this, SLOT(actionExit_triggered()));
    connect(toolWidget, SIGNAL(toLineEdit()), this, SLOT(lineEdit_returnPressed()));
    connect(toolWidget, SIGNAL(toSetting()), this, SLOT(actionSetting_triggered()));
}

void MainWidget::createTrayIcon()
{
    QMenu *trayIconMenu;

    trayIcon = new QSystemTrayIcon(this);
    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("sto"));
    trayIcon->setIcon(QIcon("./images/sto.png"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionExit);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(actionExit_triggered()));
//    trayIconMenu->addSeparator();
    //    trayIconMenu->addAction(messageAction);
}

void MainWidget::createActions()
{
    toggleHideAction = new QAction(tr("显示/隐藏sto"), this);
    toggleHideAction->setToolTip(tr("显示或者隐藏主面板"));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    actionExit = new QAction(QIcon(":/icons/exit.ico"), tr("退出"), this);
}

void MainWidget::actionRefresh_triggered()
{
    w->on_pushButton_refresh_clicked();
}

void MainWidget::actionStart_triggered()
{
    w->on_pushButton_start_clicked();
    statusLabel->setText(tr("正在监控股票实时数据···"));
}

void MainWidget::actionDownload_triggered()
{
    w->on_pushButton_downLoad_clicked();
}

void MainWidget::actionStop_triggered()
{
    w->on_pushButton_stop_clicked();
    statusLabel->clear();
}

void MainWidget::actionRecover_triggered()
{
    w->on_pushButton_recorve_clicked();
}

void MainWidget::actionExit_triggered()
{
    qApp->quit();
}

void MainWidget::lineEdit_returnPressed()
{
    QString text = toolWidget->getLineEditText();
    QString firstStr = text.left(1);
    QString stockId;
    QString stockName;
    if(text.count() == 6){
        if(firstStr == "0" || firstStr == "3"){
            stockId = "sz" + text;
        }else if(firstStr == "6"){
            stockId = "sh" + text;
        }
    }
    if(!stockId.isEmpty()){
        w->newProMoniOne(stockId, stockName);
    }
}

void MainWidget::actionSetting_triggered(bool checked)
{
    w->hideSetting(!checked);
}

void MainWidget::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        toggleHideAction->trigger();
    }
}

void MainWidget::toggleHidden()
{
    if(isHidden()){
        show();
        activateWindow();
    }else if(isMinimized()){
        showNormal();
        activateWindow();
    }else{
        hide();
    }
}

void MainWidget::showMax()
{
    static bool isMax = false;
    if (isMax)
    {
        move(normalTopLeft);
        resize(850, 600);
        isMax = false;
    }
    else
    {
        normalTopLeft = this->pos();
        setGeometry(QApplication::desktop()->availableGeometry());
        isMax = true;
    }
}

void MainWidget::showMin()
{
    showMinimized();
}

void MainWidget::showSkinWidget()
{

}
