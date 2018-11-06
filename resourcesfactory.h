#ifndef RESOURCESFACTORY_H
#define RESOURCESFACTORY_H

#include "QtGlobal"

#include "resourcesinfo_win.h"
#include "resourcesinfo_linux.h"

class ResourcesFactory
{
public:
    static ResourcesFactory* Instance()
    {
        static ResourcesFactory instance;
        return &instance;
    }

    ResourcesInfo* getResourcesInfo()
    {
#ifdef Q_OS_WIN
        return new ResourcesInfo_Win();
#endif //Q_OS_WIN

#ifdef Q_OS_UNIX
        return new ResourcesInfo_Linux();
#endif //Q_OS_UNIX

    }

private:
    ResourcesFactory(){}
};

#endif // RESOURCESFACTORY_H
