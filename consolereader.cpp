#include "consolereader.h"

#include <QProcess>
#include <QDebug>

ConsoleReader::ConsoleReader()
{

}

QStringList
ConsoleReader::callProcess(QString exename)
{
    QProcess process;

    process.start(exename);
    process.waitForFinished();
    QString output (process.readAllStandardOutput());

    QRegExp rx("(\\r\\n |\\n)");
    QStringList output_as_list = output.split(rx);

    return output_as_list;
}
