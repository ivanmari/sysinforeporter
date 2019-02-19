#ifndef ORAINSTANCEDETECTORFACTORY_H
#define ORAINSTANCEDETECTORFACTORY_H

#include "QtGlobal"

#include "oracleinstancedetector_win.h"
#include "oracleinstancedetector_linux.h"

class OraInstanceDetectorFactory
{
public:

    static OraInstanceDetectorFactory* Instance()
    {
        static OraInstanceDetectorFactory instance;
        return &instance;
    }

    OracleInstanceDetector* getOracleInstanceDetector()
    {
#ifdef Q_OS_WIN
        return new OracleInstanceDetector_Win();
#endif //Q_OS_WIN

#ifdef Q_OS_UNIX
        return new OracleInstanceDetector_Linux();
#endif //Q_OS_UNIX

    }

private:
    OraInstanceDetectorFactory();
};

#endif // ORAINSTANCEDETECTORFACTORY_H
