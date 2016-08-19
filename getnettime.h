#ifndef GETNETTIME_H
#define GETNETTIME_H

#include <QWidget>

class QNetworkReply;
class QNetworkAccessManager;

class GetNetTime : public QWidget
{
public:
    GetNetTime();
    ~GetNetTime();

    bool isOutOfTime();

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager *manager;
};

#endif // GETNETTIME_H
