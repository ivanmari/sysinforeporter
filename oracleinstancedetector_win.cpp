#include "oracleinstancedetector_win.h"
#include "consolereader.h"

#include <iostream>

#include <QtGlobal>

#ifdef Q_OS_WIN

OracleInstanceDetector_Win::OracleInstanceDetector_Win()
{


}

QJsonArray
OracleInstanceDetector_Win::getOracleInstances() const
{
    QStringList services = ConsoleReader::callProcess("sc.exe queryex type= service state= all");
    QStringList filtered_services;

    std::copy_if(services.begin(), services.end(), std::back_inserter(filtered_services), [](const QString service ){return service.contains("SERVICE_NAME: OracleService", Qt::CaseInsensitive);});

    return QJsonArray::fromStringList(filtered_services);
}
#endif //Q_OS_WIN
