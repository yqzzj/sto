#include "titlewidget.h"
#include "pushbutton.h"
#include "sysbutton.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStyle>

TitleWidget::TitleWidget(QWidget *parent) :
    QWidget(parent)
{
    versionText = new QLabel(tr("sto"));

    createButtons();
    createMainLayout();

    setLayout(mainLayout);
    setFixedHeight(25);

    isMove = false;
}

void TitleWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->x() + 170 >= this->width())
        return;
    pressedPoint = e->pos();
    isMove = true;
}

void TitleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if( (e->buttons() & Qt::LeftButton) && isMove)
    {
        static QWidget* parWidget = this->parentWidget();
        QPoint nowParPoint = parWidget->pos();
        nowParPoint.setX(nowParPoint.x() + e->x() - pressedPoint.x());
        nowParPoint.setY(nowParPoint.y() + e->y() - pressedPoint.y());
        parWidget->move(nowParPoint);
    }
}

void TitleWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(isMove)
        isMove = false;
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->x() + 170 >= this->width())
        return;
    emit showMax();
}

void TitleWidget::createButtons()
{
    QPixmap skinPix(":icons/skin");
    QPixmap minPix(":icons/min");
    QPixmap closePix(":icons/close");
    QPixmap maxPix(":icons/max");
    QPixmap menuPix(":icons/menu");

    btnSkin = new PushButton(skinPix, tr("Skin peeler"), this);
    btnMenuBar = new SysButton(menuPix, tr("Main menu"), this);
    btnMin = new SysButton(minPix, tr("Minimize"), this);
    btnMax = new SysButton(maxPix, tr("Maximize"), this);
    btnClose = new SysButton(closePix, tr("Close"), this);

    connect(btnSkin, SIGNAL(clicked()), this, SIGNAL(showSkin()));
    connect(btnMenuBar, SIGNAL(clicked()), this, SLOT(showMenu()));
    connect(btnMin, SIGNAL(clicked()), this, SIGNAL(showMin()));
    connect(btnMax, SIGNAL(clicked()), this, SIGNAL(showMax()));
    connect(btnClose, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void TitleWidget::createMainLayout()
{
    mainLayout = new QHBoxLayout(this);
//    QLabel *picLabel = new QLabel;
//    picLabel->setPixmap(pixmap);
//    picLabel->setFixedSize(pixmap.size());
//    mainLayout->addWidget(picLabel, 0, Qt::AlignVCenter);
    mainLayout->addWidget(versionText, 0, Qt::AlignVCenter);
    versionText->setContentsMargins(5, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(btnSkin, 0, Qt::AlignVCenter);
    btnSkin->setContentsMargins(0, 0, 5, 0);
    mainLayout->addWidget(btnMenuBar);
    mainLayout->addWidget(btnMin);
    mainLayout->addWidget(btnMax);
    mainLayout->addWidget(btnClose);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}
