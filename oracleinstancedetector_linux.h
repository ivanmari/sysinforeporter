#ifndef ORACLEINSTANCEDETECTOR_LINUX_H
#define ORACLEINSTANCEDETECTOR_LINUX_H

#include "oracleinstancedetector.h"

#include <QtGlobal>

#ifdef Q_OS_UNIX

class OracleInstanceDetector_Linux : public OracleInstanceDetector
{
public:
    OracleInstanceDetector_Linux();

        QJsonArray getOracleInstances() const override;
};

#endif //Q_OS_UNIX
#endif // ORACLEINSTANCEDETECTOR_LINUX_H
