#include "fileinfo.h"
#include "filepaths.h"

#include <QDirIterator>
#include <QRegularExpression>

FileInfo::FileInfo()
{

}

std::vector<QString>
FileInfo::
getFilesToUpload(QJsonArray filtered_files, const std::vector<std::pair<QString, bool>> paths)
{
    std::vector<QString> files_to_upload;

    std::vector<std::pair<QRegularExpression, bool>> regexes;

    for(auto path : paths)
    {
        QRegularExpression regex = QRegularExpression(path.first, QRegularExpression::CaseInsensitiveOption);

        for(auto elem : filtered_files)
        {
            QRegularExpressionMatch match = regex.match(elem.toString());
            if(match.hasMatch())
            {
                if(path.second)
                {
                    files_to_upload.push_back(match.captured(0));
                }
            }
        }
    }

    return files_to_upload;
}

QJsonArray
FileInfo::findFiles(const std::vector<std::pair<QString, bool>> paths)
{
    QJsonArray js_files;
    std::vector<QRegularExpression> regexes;
    for(auto path : paths)
    {
        regexes.push_back(QRegularExpression(path.first, QRegularExpression::CaseInsensitiveOption));
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
