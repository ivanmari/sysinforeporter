#ifndef RESTAPI_H
#define RESTAPI_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class RestApi
{
public:
    RestApi(const QUrl& url, const QString& basic_auth_hash = "");
    QJsonObject post(const QJsonDocument& body);
    QJsonObject get();

private:

    QUrl m_url;
    QString m_basic_auth;
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    QByteArray* m_buffer;

};

#endif // RESTAPI_H
