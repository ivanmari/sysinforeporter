#ifndef ORACLEINSTANCEDETECTOR_WIN_H
#define ORACLEINSTANCEDETECTOR_WIN_H

#include "oracleinstancedetector.h"

#include <QtGlobal>

#ifdef Q_OS_WIN

class OracleInstanceDetector_Win : public OracleInstanceDetector
{
public:
    OracleInstanceDetector_Win();

    QJsonArray getOracleInstances() const override;
};

#endif //Q_OS_WIN
#endif // ORACLEINSTANCEDETECTOR_WIN_H
