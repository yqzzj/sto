#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class PushButton;
class SysButton;
class QHBoxLayout;
class QLabel;


class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);

signals:
    void showMin();
    void showMax();
    void showSkin();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    QPoint pressedPoint;
    bool isMove;
    PushButton *btnSkin;
    SysButton *btnMenuBar;
    SysButton *btnMin;
    SysButton *btnMax;
    SysButton *btnClose;
    QHBoxLayout *mainLayout;
    QLabel *versionText;

    void createButtons();
    void createMainLayout();
};

#endif // TITLEWIDGET_H
