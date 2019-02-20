#ifndef ORACLEINSTANCEDETECTOR_LINUX_H
#define ORACLEINSTANCEDETECTOR_LINUX_H

#include <QtGlobal>

#ifdef Q_OS_UNIX
#include "oracleinstancedetector.h"

class OracleInstanceDetector_Linux : public OracleInstanceDetector
{
public:
    OracleInstanceDetector_Linux();

        QJsonArray getOracleInstances() const override;
        QStringList extractSqlInfo(QString sid) const override;
};

#endif //Q_OS_UNIX
#endif // ORACLEINSTANCEDETECTOR_LINUX_H
