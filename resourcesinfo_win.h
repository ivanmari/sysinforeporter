#ifndef RESOURCESINFO_WIN_H
#define RESOURCESINFO_WIN_H

#include "QtGlobal"

#ifdef Q_OS_WIN

#include "resourcesinfo.h"

#include <QStringList>
#include <map>

class ResourcesInfo_Win : public ResourcesInfo
{
public:
    ResourcesInfo_Win();
    float getTotalMemoryUsed();
    int getCpuLoad();
    QStringList getOpenPorts();
private:

    static QStringList callProcess(QString process);
    static std::map<int, QString> getProcessNames();
};

#endif
#endif // RESOURCESINFO_WIN_H
