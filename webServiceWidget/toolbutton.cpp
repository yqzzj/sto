#include "toolbutton.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPalette>
#include <QPainter>
#include <QBitmap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFont>

ToolButton::ToolButton(QPixmap pixmap, QString text, QWidget *parent, bool isOnlyToolButton) : QWidget(parent)
{
    m_isChecked = false;
    m_isOnlyToolButton = isOnlyToolButton;
    m_pixmap = pixmap;
    QLabel *picLabel = new QLabel;
    picLabel->setPixmap(m_pixmap);
    picLabel->setFixedSize(35, 35);

    QFont font;
    font.setBold(true);
    font.setWeight(20);
    QLabel *textLabel = new QLabel;
    textLabel->setFont(font);
    textLabel->setText(text);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(picLabel,0,Qt::AlignHCenter);
    mainLayout->addWidget(textLabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    setLayout(mainLayout);

    setBkPalette(0);
    setAutoFillBackground(true);
    setFixedHeight(40);
    setFixedWidth(100);
}

void ToolButton::setChecked(bool isChecked)
{
    m_isChecked = isChecked;

    if(isEnabled()){
        if(m_isChecked){
            setBkPalette(120);
        }else{
            setBkPalette(0);
        }
    }
}

void ToolButton::paintEvent(QPaintEvent *e)
{
    QBitmap bitmap(this->size());
    bitmap.fill();
    QPainter painter(&bitmap);
    painter.setRenderHints(QPainter::Antialiasing,true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRoundedRect(bitmap.rect(),5,5);
    setMask(bitmap);
}

void ToolButton::enterEvent(QEvent *e)
{
    if(!m_isChecked && isEnabled()){
        setBkPalette(140);
    }
}

void ToolButton::mousePressEvent(QMouseEvent *e)
{
    if(!m_isChecked && isEnabled()){
        setBkPalette(190);
    }
}

void ToolButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->rect().contains(e->pos())){
        if(!m_isChecked && isEnabled()){
            if(!m_isOnlyToolButton){
                m_isChecked = true;
                setBkPalette(140);
            }else{
                setBkPalette(0);
            }
            emit toolClicked();
        }
    }
}

void ToolButton::leaveEvent(QEvent *e)
{
    if(!m_isOnlyToolButton){
        if(!m_isChecked && isEnabled()){
            setBkPalette(0);
        }
    }else{
        setBkPalette(0);
    }
}

void ToolButton::setBkPalette(int p)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QColor(255,255,255,p)));
    setPalette(palette);
}
