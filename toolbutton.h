#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QWidget>

class ToolButton : public QWidget
{
    Q_OBJECT
public:
    explicit ToolButton(QPixmap pixmap, QString text, QWidget *parent = 0, bool isOnlyToolButton = false);
    void setChecked(bool isChecked);

signals:
    void toolClicked();

protected:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *e);

private:
    QPixmap m_pixmap;
    bool m_isChecked;
    bool m_isOnlyToolButton;

    void setBkPalette(int p);
};

#endif // TOOLBUTTON_H
