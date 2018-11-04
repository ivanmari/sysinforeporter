#ifndef RESOURCESINFO_LINUX_H
#define RESOURCESINFO_LINUX_H

#include <QtGlobal>

#ifdef Q_OS_UNIX

#include "resourcesinfo.h"

class ResourcesInfo_Linux : public ResourcesInfo
{
public:
    ResourcesInfo_Linux();
public:
    ResourcesInfo_Win();
    float getTotalMemoryUsed();
    int getCpuLoad();
    QStringList getOpenPorts();
};

#endif //Q_OS_UNIX

#endif // RESOURCESINFO_LINUX_H
