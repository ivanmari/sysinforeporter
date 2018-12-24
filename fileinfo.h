#ifndef FILEINFO_H
#define FILEINFO_H

#include <QJsonArray>
#include <QString>
#include <vector>

class FileInfo
{
public:
    FileInfo();
    static QJsonArray findFiles(const std::vector<QString> paths);
};

#endif // FILEINFO_H
