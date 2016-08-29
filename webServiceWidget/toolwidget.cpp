#include "toolwidget.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <toolbutton.h>
#include <QSpacerItem>
#include <QLabel>
#include <QValidator>

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent)
{
    createButtons();
    createConnections();
    createMainLayout();
    setLayout(mainLayout);
    setFixedHeight(60);
}

QString ToolWidget::getLineEditText()
{
    return lineEdit->text();
}

void ToolWidget::createButtons()
{
    QPixmap refreshPix(":icons/refresh");
    refreshButton = new ToolButton(refreshPix, tr("刷新"));
    refreshButton->setToolTip(tr("刷新当前监控结果"));

    QPixmap startPix(":icons/start");
    startButton = new ToolButton(startPix, tr("开始"));
    startButton->setToolTip(tr("开始监控股票实时数据"));

    QPixmap downLoadPix(":icons/download");
    downLoadButton = new ToolButton(downLoadPix, tr("下载"));
    downLoadButton->setToolTip(tr("下载股票实时数据"));

    QPixmap stopPix(":icons/stop");
    stopButton = new ToolButton(stopPix, tr("停止"));
    stopButton->setToolTip(tr("停止监控"));

    QPixmap recoverPix(":icons/recover");
    recoverButton = new ToolButton(recoverPix, tr("恢复"));
    recoverButton->setToolTip(tr("恢复默认设置"));

    QPixmap settingPix(":icons/setting");
    settingButton = new ToolButton(settingPix, tr("设置"));
    settingButton->setToolTip(tr("设置监控条件"));

    QPixmap saveModelPix(":icons/bookmark_folder.ico");
    saveModelButton = new ToolButton(saveModelPix, tr("保存模板"));
    saveModelButton->setToolTip(tr("保存当前监控条件为模板"));

    QPixmap applyModelPix(":icons/editpaste.ico");
    applyModelButton = new ToolButton(applyModelPix, tr("应用模板"));
    applyModelButton->setToolTip(tr("应用某模板为监控条件"));

    QPixmap quitPix(":icons/exit");
    quitButton = new ToolButton(quitPix, tr("退出"));
    quitButton->setToolTip(tr("退出程序"));

    lineEdit = new QLineEdit();
    QValidator *validator=new QIntValidator(0, 999999, this);
    lineEdit->setValidator(validator);
}

void ToolWidget::createMainLayout()
{
    mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(5,0,5,0);
    mainLayout->addWidget(refreshButton);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(downLoadButton);
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(recoverButton);
    mainLayout->addWidget(settingButton);
//    mainLayout->addWidget(saveModelButton);
//    mainLayout->addWidget(applyModelButton);
    mainLayout->addWidget(quitButton);
    lineEditgLayout = new QVBoxLayout;
    lineEditgLayout->addWidget(lineEdit);
    QLabel *label = new QLabel(tr("股票查询"));
    lineEditgLayout->addWidget(label);
    mainLayout->addLayout(lineEditgLayout);
    QSpacerItem* hSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addSpacerItem(hSpacer);

}

void ToolWidget::createConnections()
{
    connect(refreshButton, SIGNAL(toolClicked()), this, SLOT(refresh()));
    connect(startButton, SIGNAL(toolClicked()), this, SLOT(start()));
    connect(downLoadButton, SIGNAL(toolClicked()), this, SLOT(downLoad()));
    connect(stopButton, SIGNAL(toolClicked()), this, SLOT(stop()));
    connect(recoverButton, SIGNAL(toolClicked()), this, SLOT(recover()));
    connect(settingButton, SIGNAL(toolClicked()), this, SLOT(setting()));
    connect(saveModelButton, SIGNAL(toolClicked()), this, SLOT(saveModel()));
    connect(applyModelButton, SIGNAL(toolClicked()), this, SLOT(applyModel()));
    connect(quitButton, SIGNAL(toolClicked()), this, SLOT(quit()));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(lineEditPress()));
}

void ToolWidget::refresh()
{
    emit toRefresh();
}

void ToolWidget::start()
{
    emit toStart();
}

void ToolWidget::downLoad()
{
    emit toDownLoad();
}

void ToolWidget::stop()
{
    emit toStop();
}

void ToolWidget::recover()
{
    emit toRecover();
}

void ToolWidget::setting()
{
    emit toSetting();
}

void ToolWidget::saveModel()
{
    emit toSaveModel();
}

void ToolWidget::applyModel()
{
    emit toApplyModel();
}

void ToolWidget::quit()
{
    emit toQuit();
}

void ToolWidget::lineEditPress()
{
    emit toLineEdit();
}
