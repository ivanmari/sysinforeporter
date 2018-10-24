#ifndef CPUFACTORY_H
#define CPUFACTORY_H

#include "cpuinfo_win32.h"
#include "cpuinfo_linux.h"

#include "QtGlobal"

class CpuFactory
{
public:
    CpuFactory();
    static CpuFactory* Instance()
    {
        static CpuFactory instance;
        return &instance;
    }

    CpuInfo* getCpuInfo(){
#ifdef Q_OS_WIN
        return new CpuInfo_Win32();

#endif

#ifdef Q_OS_UNIX
        return new CpuInfo_Linux();
#endif
    }
};
#endif // CPUFACTORY_H
