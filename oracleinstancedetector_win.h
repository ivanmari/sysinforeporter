#ifndef ORACLEINSTANCEDETECTOR_WIN_H
#define ORACLEINSTANCEDETECTOR_WIN_H

#include "oracleinstancedetector.h"

class OracleInstanceDetector_Win : public OracleInstanceDetector
{
public:
    OracleInstanceDetector_Win();

    QJsonArray getOracleInstances() const;
};

#endif // ORACLEINSTANCEDETECTOR_WIN_H
