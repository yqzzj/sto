#ifndef SYSBUTTON_H
#define SYSBUTTON_H

#include <QPushButton>

class SysButton : public QPushButton
{
public:
    SysButton(QPixmap pixmap, QString tipText, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *);

private:
    enum buttonStatus{NORMAL,ENTER,PRESS,NOSTATUS};
    buttonStatus status;
    QPixmap m_pixmap;
    int btnWidth;
    int btnHeight;

};

#endif // SYSBUTTON_H
