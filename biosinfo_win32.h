#ifndef BIOSINFO_WIN32_H
#define BIOSINFO_WIN32_H
#include "biosinfo.h"
#include "QtGlobal"

#ifdef Q_OS_WIN

class BiosInfo_Win32 : public BiosInfo
{
public:
    BiosInfo_Win32();

     QString getHwManufacturer() const final;
     QString getHwModel() const final;

     virtual ~BiosInfo_Win32(){}
};

#endif //Q_OS_WIN

#endif // BIOSINFO_WIN32_H
