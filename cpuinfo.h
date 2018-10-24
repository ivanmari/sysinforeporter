#ifndef CPUINFO_H
#define CPUINFO_H
#include "QString"

class CpuInfo
{
public:
    virtual QString getCpuManufacturer() const = 0;
    virtual QString getCpuModel() const = 0;
    virtual QString getCpuCount() const = 0;
    virtual ~CpuInfo(){}

};

#endif // CPUINFO_H
