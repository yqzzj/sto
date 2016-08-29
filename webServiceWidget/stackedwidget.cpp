#include "stackedwidget.h"
#include <QPalette>

StackedWidget::StackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QColor(250,255,240,150)));
    setPalette(palette);
    setAutoFillBackground(true);
    setMinimumHeight(490);
}
