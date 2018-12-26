#include "s3uploader.h"

#include <QtNetwork>
#include <QUrl>

const char S3_BUCKET_URL[] = "https://m3g1gv22f9.execute-api.us-east-2.amazonaws.com/int/sysinforeport-dev";


void
S3Uploader::put(QString basic_auth, FilePath filepath, FileName filename)
{
    QNetworkAccessManager qnam;
    QNetworkReply* reply;
    QString url {S3_BUCKET_URL};

    url.append("/");
    url.append(filename);

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
    request.setRawHeader("Authorization", basic_auth.toLocal8Bit());

    QFile file {filepath};

    if(file.open(QIODevice::ReadOnly))
    {
        reply = qnam.put(request, &file);

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        file.close();

        if(QNetworkReply::NetworkError::NoError != reply->error())
        {
            qInfo() << "Error uploading file: " << reply->error();
        }
    }
    else
    {
        qInfo() << "Cannot open file for reading";
    }
}

