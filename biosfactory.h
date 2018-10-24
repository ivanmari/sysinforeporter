#ifndef BIOSFACTORY_H
#define BIOSFACTORY_H

#include "biosinfo_win32.h"
#include "biosinfo_linux.h"

#include "QtGlobal"

class BiosFactory
{
public:
    BiosFactory();
    static BiosFactory* Instance()
    {
        static BiosFactory instance;
        return &instance;
    }

    BiosInfo* getBiosInfo(){
#ifdef Q_OS_WIN
        return new BiosInfo_Win32();

#endif

#ifdef Q_OS_UNIX
        return new BiosInfo_Linux();
#endif
    }
};

#endif // BIOSFACTORY_H
