#ifndef BIOSINFO_LINUX_H
#define BIOSINFO_LINUX_H
#include "biosinfo.h"

#ifdef Q_OS_UNIX
class BiosInfo_Linux : public BiosInfo
{
public:
    BiosInfo_Linux();

    QString getHwManufacturer() const final;
    QString getHwModel() const final;

    virtual ~BiosInfo_Linux(){}
};
#endif // Q_OS_UNIX
#endif // BIOSINFO_LINUX_H
