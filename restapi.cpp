#include "restapi.h"

#include <QtNetwork>
#include <QUrl>
#include <QObject>

RestApi::RestApi(const QUrl& url, const QString& basic_auth_hash): m_url(url), m_basic_auth(basic_auth_hash)
{

}

QJsonObject
RestApi::post(const QJsonDocument& body)
{
    QJsonObject ret;
    QNetworkRequest request(m_url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if(!m_basic_auth.isEmpty())
    {
        request.setRawHeader("Authorization", m_basic_auth.toLocal8Bit());
    }

    if(!body.isEmpty())
    {
        m_reply = m_qnam.post(request, body.toBinaryData());

        while(!m_reply->isFinished())
        {
            qApp->processEvents();
        }

        if(QNetworkReply::NetworkError::NoError != m_reply->error())
        {
            qInfo() << "Error sending post: " << m_reply->error();
        }
    }
    else
    {
        qInfo() << "Empty body";
    }

    return ret;
}

QJsonObject
RestApi::get()
{
    QNetworkRequest request(m_url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if(!m_basic_auth.isEmpty())
    {
        request.setRawHeader("Authorization", m_basic_auth.toLocal8Bit());
    }
    m_reply = m_qnam.get(request);

    QEventLoop loop;
    QObject::connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(QNetworkReply::NetworkError::NoError != m_reply->error())
    {
        qInfo() << "Error sending get: " << m_reply->error();
    }

    return QJsonDocument::fromJson(m_reply->readAll()).object();
}
