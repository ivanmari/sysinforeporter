#include "fileinfo.h"
#include "filepaths.h"

#include <QDirIterator>
#include <QRegularExpression>

FileInfo::FileInfo()
{

}

QJsonArray
FileInfo::findFiles(const std::vector<QString> paths)
{
    QJsonArray js_files;
    std::vector<QRegularExpression> regexes;
    for(auto path : paths)
    {
        regexes.push_back(QRegularExpression(path, QRegularExpression::CaseInsensitiveOption));
    }

#ifdef  QT_NO_DEBUG

    for(auto drive: QDir::drives())
    {
        QDirIterator it(drive.path(), QDir::AllEntries | QDir::System | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString path = it.next();

            for(auto re : regexes)
            {
                QRegularExpressionMatch match = re.match(path);
                if(match.hasMatch())
                {
                    QFileInfo file_info(match.captured(0));
                    js_files.push_back(QJsonValue::fromVariant(file_info.absoluteFilePath()));
                }
            }
            //QThread::msleep(1);
        }
    }

#endif  //QT_NO_DEBUG

    return js_files;
}
