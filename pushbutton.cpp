#include "pushbutton.h"

PushButton::PushButton(QPixmap pixmap, QString tipText, QWidget *parent) : QPushButton(parent)
{
    m_pixmap = pixmap;
    setToolTip(tipText);
    setFlat(true);
    setWindowOpacity(0);

    iconWidth = m_pixmap.width() / 3;
    iconHeight = m_pixmap.height();

    setIcon(QIcon(m_pixmap.copy(0, 0, iconWidth, iconHeight)));
    setIconSize(QSize(iconWidth, iconHeight));
}

void PushButton::enterEvent(QEvent *)
{
    setWindowOpacity(0.1);
    setIcon(QIcon(m_pixmap.copy(iconWidth, 0, iconWidth, iconHeight)));
}

void PushButton::mousePressEvent(QMouseEvent *)
{
    setWindowOpacity(0.3);
    setIcon(QIcon(m_pixmap.copy(iconWidth*2, 0, iconWidth, iconHeight)));
}

void PushButton::mouseReleaseEvent(QMouseEvent *)
{
    setWindowOpacity(0.1);
    setIcon(QIcon(m_pixmap.copy(iconWidth, 0, iconWidth, iconHeight)));
    emit clicked();
}

void PushButton::leaveEvent(QEvent *)
{
    setWindowOpacity(0);
    setIcon(QIcon(m_pixmap.copy(0, 0, iconWidth, iconHeight)));
}
