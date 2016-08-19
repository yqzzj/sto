#include "getnettime.h"
#include <QMessageBox>
#include <QDateTime>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

GetNetTime::GetNetTime()
{

}

GetNetTime::~GetNetTime()
{

}

bool GetNetTime::isOutOfTime()
{
    QUrl url;
    url.setUrl("http://www.beijing-time.org/time.asp");
    QNetworkRequest request;
    request.setUrl(url);
    manager = new QNetworkAccessManager();
    manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void GetNetTime::replyFinished(QNetworkReply *reply)
{
    QMessageBox::information(NULL, "date", reply->readAll());
}
