#ifndef ORACLEINSTANCEDETECTOR_H
#define ORACLEINSTANCEDETECTOR_H

#include <QJsonArray>

class OracleInstanceDetector
{
public:
    virtual QJsonArray getOracleInstances() const = 0;

    virtual ~OracleInstanceDetector(){}
};

#endif // ORACLEINSTANCEDETECTOR_H
