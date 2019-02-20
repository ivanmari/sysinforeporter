#include <QtGlobal>

#ifdef Q_OS_WIN

#include "oracleinstancedetector_win.h"
#include "consolereader.h"

#include <iostream>

#include <QRegularExpression>

const QString ORACLE_INSTANCE_PREFIX = "SERVICE_NAME: OracleService";

OracleInstanceDetector_Win::OracleInstanceDetector_Win()
{
}

QJsonArray
OracleInstanceDetector_Win::getOracleInstances() const
{
    QStringList services = ConsoleReader::callProcess("sc.exe queryex type= service state= all");
    QStringList filtered_services;

    filtered_services.reserve(services.size());

    for(auto const & service : services)
    {
        QRegularExpression regex{ORACLE_INSTANCE_PREFIX + "([a-zA-Z0-9]+)",  QRegularExpression::CaseInsensitiveOption};

        QRegularExpressionMatch match = regex.match(service);

        if(match.hasMatch())
        {
            filtered_services.push_back(match.captured(1));
        }
    }

    return QJsonArray::fromStringList(filtered_services);
}

QStringList
OracleInstanceDetector_Win::extractSqlInfo(QString sid) const
{
   QString exec = "cmd.exe /c \"set ORACLE_SID=" + sid + " & sqlplus / as sysdba < extract.sql\"";

   std::cout << exec.toStdString() << std::endl;

   QStringList entries = ConsoleReader::callProcess(exec);

   return entries;
}

#endif //Q_OS_WIN
