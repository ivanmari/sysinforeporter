#ifndef CPUINFO_WIN32_H
#define CPUINFO_WIN32_H
#include "cpuinfo.h"
#include "QtGlobal"

#ifdef Q_OS_WIN

class CpuInfo_Win32 : public CpuInfo
{
public:
    CpuInfo_Win32();

    QString getCpuManufacturer() const final;
    QString getCpuModel() const final;
    QString getCpuCount() const final;

    virtual ~CpuInfo_Win32(){}
};

#endif // Q_OS_WIN

#endif // CPUINFO_WIN32_H
