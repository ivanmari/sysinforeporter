#ifdef Q_OS_UNIX

#include "resourcesinfo_linux.h"

ResourcesInfo_Linux::ResourcesInfo_Linux()
{

}

float
ResourcesInfo_Linux::getTotalMemoryUsed()
{
    return 0.0;
}

int
ResourcesInfo_Linux::getCpuLoad()
{
    return 0;
}

QStringList
ResourcesInfo_Linux::getOpenPorts()
{
    QStringList ret;
    return ret;
}

#endif // Q_OS_UNIX
