#include <QtGlobal>

#ifdef Q_OS_UNIX

#include "oracleinstancedetector_linux.h"
#include "processinfo.h"

OracleInstanceDetector_Linux::OracleInstanceDetector_Linux()
{

}

QJsonArray
OracleInstanceDetector_Linux::getOracleInstances() const
{
    std::vector<QString> patterns {"ora_pmon"};

    return ProcessInfo::findProcesses(patterns);
}

QStringList
OracleInstanceDetector_Linux::extractSqlInfo(QString sid) const
{
   QString exec = "/bin/sh \"export ORACLE_SID=" + sid + " ; sqlplus / as sysdba < extract.sql\"";
   QStringList entries = ConsoleReader::callProcess(exec);

   return entries.
}

#endif //Q_OS_UNIX
