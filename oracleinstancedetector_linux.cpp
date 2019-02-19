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
