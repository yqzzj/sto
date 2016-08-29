#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMouseEvent>
#include <QLineEdit>
#include <QLabel>
#include <QProcess>
#include <QToolButton>
#include <QSystemTrayIcon>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    w = new Widget;
    ui->setupUi(this);

    createActions();
    createTrayIcon();
    //initTitleBar();
    initLineEdit();

    this->setCentralWidget(w);
    this->setWindowTitle("sto_2.4");

    initStatusBar();
    ui->actionSaveModel->setVisible(false);
    ui->actionLoadModel->setVisible(false);
    ui->ftpDown->setVisible(false);
    ui->ftpUp->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //获得部件当前位置
    this->windowPos = this->pos();
    //获得鼠标位置
    this->mousePos = event->globalPos();
    //移动后部件所在的位置
    this->dPos = mousePos - windowPos;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void MainWindow::on_actionRefresh_triggered()
{
    w->on_pushButton_refresh_clicked();
}

void MainWindow::on_actionStart_triggered()
{
    statusLabel->setText(tr("正在监控股票实时数据···"));
    w->on_pushButton_start_clicked();
}

void MainWindow::on_actionDownload_triggered()
{
    w->on_pushButton_downLoad_clicked();
}

void MainWindow::on_actionStop_triggered()
{
    w->on_pushButton_stop_clicked();
    statusLabel->clear();
}

void MainWindow::on_actionRecover_triggered()
{
    w->on_pushButton_recorve_clicked();
}

void MainWindow::on_actionExit_triggered()
{
//    int ret = QMessageBox::question(this, tr("提示"),
//                                       tr("确定要退出程序吗？"),
//                                       QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
//    if(ret == QMessageBox::Yes){
//        close();
//    }
    qApp->quit();
}

void MainWindow::lineEdit_returnPressed()
{
    QString text = lineEdit->text();
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

void MainWindow::initLineEdit()
{
    QString str = QString(tr("股票查询"));
    lineEdit = new QLineEdit(this);
    labelWidget = new QWidget(this);
    label = new QLabel(labelWidget);
    label->setText(str);

    ui->toolBar->addWidget(labelWidget);
    ui->toolBar->addWidget(lineEdit);

    QValidator *validator=new QIntValidator(0, 999999, this);
    lineEdit->setValidator(validator);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(lineEdit_returnPressed()));
}

void MainWindow::initTitleBar()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    //获取界面的宽度
    int width = this->width();
    //int height = this->height();
    //最小按钮
    minButton = new QToolButton(this);
    //帮助按钮
    helpButton = new QToolButton(this);
    //关闭按钮
    closeButton= new QToolButton(this);
    //最大按钮
    maxButton = new QToolButton(this);
    //菜单按钮
    menuButton = new QToolButton(this);
    normalButton = new QToolButton(this);
    shadeButton = new QToolButton(this);
    unshadeButton = new QToolButton(this);

    connect(minButton, SIGNAL(clicked()), this, SLOT(actionMinimize_triggered()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(actionClose_triggered()));
    connect(maxButton, SIGNAL(clicked()), this, SLOT(actionMaximize_triggered()));

    //获取最小化、关闭按钮图标
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap helpPix = style()->standardPixmap(QStyle::SP_TitleBarContextHelpButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    QPixmap shadePix = style()->standardPixmap(QStyle::SP_TitleBarShadeButton);
    QPixmap unshadePix = style()->standardPixmap(QStyle::SP_TitleBarUnshadeButton);
    QPixmap normalPix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);
    QPixmap menuPix = style()->standardPixmap(QStyle::SP_TitleBarMenuButton);

    //设置最小化、关闭按钮图标
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);
    helpButton->setIcon(helpPix);
    maxButton->setIcon(maxPix);
    shadeButton->setIcon(shadePix);
    unshadeButton->setIcon(unshadePix);
    normalButton->setIcon(normalPix);
    menuButton->setIcon(menuPix);

    //设置最小化、关闭按钮在界面的位置
    normalButton->setGeometry(width-160, 0, 20, 20);
    menuButton->setGeometry(width-140, 0, 20, 20);
    minButton->setGeometry(width-120,0,20,20);
    closeButton->setGeometry(width-100,0,20,20);
    helpButton->setGeometry(width-80,0,20,20);
    maxButton->setGeometry(width-60, 0, 20, 20);
    shadeButton->setGeometry(width-40,0, 20, 20);
    unshadeButton->setGeometry(width-20, 0, 20, 20);

    //设置鼠标移至按钮上的提示信息
//    minButton->setToolTip(tr("最小化"));
//    closeButton->setToolTip(tr("关闭"));
//    maxButton->setToolTip(tr("最大化"));

    //设置最小化、关闭等按钮的样式
    minButton->setStyleSheet("background-color:transparent;");
    closeButton->setStyleSheet("background-color:transparent;");
    normalButton->setStyleSheet("background-color:transparent;");
    menuButton->setStyleSheet("background-color:transparent;");
    helpButton->setStyleSheet("background-color:transparent;");
    maxButton->setStyleSheet("background-color:transparent;");
    shadeButton->setStyleSheet("background-color:transparent;");
    unshadeButton->setStyleSheet("background-color:transparent;");
}

void MainWindow::createTrayIcon()
{
    QMenu *trayIconMenu;

    trayIcon = new QSystemTrayIcon(this);
    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("sto"));
    trayIcon->setIcon(QIcon(":icons/sto"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui->actionExit);
//    trayIconMenu->addSeparator();
    //    trayIconMenu->addAction(messageAction);
}

void MainWindow::createActions()
{
    toggleHideAction = new QAction(tr("显示/隐藏sto"), this);
    toggleHideAction->setToolTip(tr("显示或者隐藏主面板"));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
}

void MainWindow::initStatusBar()
{
    statusLabel = new QLabel(this);
    statusLabel->setMinimumSize(statusLabel->sizeHint());
    statusLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(statusLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

void MainWindow::on_actionSetting_triggered(bool checked)
{
    w->hideSetting(!checked);
}

void MainWindow::actionMinimize_triggered()
{
    showMinimized();
}

void MainWindow::actionClose_triggered()
{
    on_actionExit_triggered();
}

void MainWindow::actionMaximize_triggered()
{
    showMaximized();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger){
        toggleHideAction->trigger();
    }
}

void MainWindow::toggleHidden()
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

void MainWindow::on_ftpDown_triggered()
{
    w->refreshFtpData();
}

void MainWindow::on_ftpUp_triggered()
{
    w->uploadFtpData();
}
