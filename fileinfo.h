#ifndef FILEINFO_H
#define FILEINFO_H

#include <QJsonArray>
#include <QString>
#include <vector>

class FileInfo
{
public:
    FileInfo();
    static QJsonArray findFiles(const std::vector<std::pair<QString, bool>> paths);
    static std::vector<QString> getFilesToUpload(QJsonArray filtered_files, const std::vector<std::pair<QString, bool>> paths);
};

#endif // FILEINFO_H
