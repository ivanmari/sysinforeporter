#ifndef S3UPLOADER_H
#define S3UPLOADER_H

#include <QString>

typedef QString FilePath;
typedef QString FileName;

class S3Uploader
{
public:

    static void put(QString basic_auth, FilePath filepath, FileName filename);
};

#endif // S3UPLOADER_H
