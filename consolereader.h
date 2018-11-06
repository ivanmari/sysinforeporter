#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#include <QStringList>

class ConsoleReader
{
public:
    ConsoleReader();

    static QStringList callProcess(QString exename);
};

#endif // CONSOLEREADER_H
