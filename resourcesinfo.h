#ifndef RESOURCESINFO_H
#define RESOURCESINFO_H

#include <QStringList>

class ResourcesInfo
{
public:
    virtual float getTotalMemoryUsed() = 0;
    virtual int getCpuLoad() = 0;
    virtual QStringList getOpenPorts() = 0;

    virtual ~ResourcesInfo(){}
};

#endif // RESOURCESINFO_H
