#include "sysbutton.h"

#include <QPainter>

SysButton::SysButton(QPixmap pixmap, QString tipText, QWidget *parent) : QPushButton(parent)
{
    m_pixmap = pixmap;
    setWindowOpacity(0);
    setFlat(true);

    btnWidth = m_pixmap.width()/4;
    btnHeight = m_pixmap.height();

    setFixedSize(btnWidth,btnHeight);
    setToolTip(tipText);
    status = NORMAL;
}

void SysButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}

void SysButton::mousePressEvent(QMouseEvent *)
{
    status = PRESS;
    update();
}

void SysButton::mouseReleaseEvent(QMouseEvent *)
{
    status = ENTER;
    update();
    emit clicked();
}
void SysButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void SysButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), m_pixmap.copy(btnWidth*status, 0, btnWidth, btnHeight));
}
