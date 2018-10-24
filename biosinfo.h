#ifndef BIOSINFO_H
#define BIOSINFO_H
#include "QString"

class BiosInfo
{
public:

    //Computer\HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\BIOS
    //SystemManufacturer
    //SystemProductName
    virtual QString getHwManufacturer() const = 0;
    virtual QString getHwModel() const = 0;
    ~BiosInfo(){}

};

#endif // BIOSINFO_H
