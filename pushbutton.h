#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
public:
    PushButton(QPixmap pixmap, QString tipText, QWidget *parent = 0);

protected:
    void enterEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *);

private:
    QPixmap m_pixmap;
    int iconWidth;
    int iconHeight;
    int btnWidth;
    int btnHeight;

};

#endif // PUSHBUTTON_H
