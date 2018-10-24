#ifndef CPUINFO_LINUX_H
#define CPUINFO_LINUX_H
#include "cpuinfo.h"
#include "QtGlobal"

#ifdef Q_OS_UNIX
#include "QMap"

class CpuInfo_Linux : public CpuInfo
{
public:
    CpuInfo_Linux();

    // CpuInfo interface
public:
    QString getCpuManufacturer() const final;
    QString getCpuModel() const final;
    QString getCpuCount() const final;

private:
    QMap<QString, QString> m_cpuinfo;
};
#endif // Q_OS_UNIX
#endif // CPUINFO_LINUX_H
