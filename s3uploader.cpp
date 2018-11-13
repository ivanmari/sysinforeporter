#include "s3uploader.h"

#include <QtNetwork>
#include <QUrl>

const char S3_BUCKET_URL[] = "https://m3g1gv22f9.execute-api.us-east-2.amazonaws.com/dev/sysinforeport-dev";


void
S3Uploader::put(FilePath filepath, FileName filename)
{
    QNetworkAccessManager qnam;
    QNetworkReply* reply;
    QString url {S3_BUCKET_URL};

    url.append("/");
    url.append(filename);

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");

    QFile file {filepath};

    if(file.open(QIODevice::ReadOnly))
    {
        reply = qnam.put(request, &file);

        while(!reply->isFinished())
        {
            qApp->processEvents();
        }

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

