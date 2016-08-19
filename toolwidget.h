#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>

class ToolButton;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;

class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWidget(QWidget *parent = 0);
    QString getLineEditText();

private:
    ToolButton *refreshButton;
    ToolButton *startButton;
    ToolButton *downLoadButton;
    ToolButton *stopButton;
    ToolButton *recoverButton;
    ToolButton *settingButton;
    ToolButton *saveModelButton;
    ToolButton *applyModelButton;
    ToolButton *quitButton;
    QLineEdit *lineEdit;
    QVBoxLayout *lineEditgLayout;
    QHBoxLayout *mainLayout;

    void createButtons();
    void createMainLayout();
    void createConnections();

private slots:
    void refresh();
    void start();
    void downLoad();
    void stop();
    void recover();
    void setting();
    void saveModel();
    void applyModel();
    void quit();
    void lineEditPress();

signals:
    void toRefresh();
    void toStart();
    void toDownLoad();
    void toStop();
    void toRecover();
    void toSetting();
    void toSaveModel();
    void toApplyModel();
    void toQuit();
    void toLineEdit();
};

#endif // TOOLWIDGET_H
