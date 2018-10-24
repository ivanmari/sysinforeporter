#include "biosinfo_linux.h"

#ifdef Q_OS_UNIX
#include "pstream.h"
#include <QFileInfo>
#include<QDebug>

BiosInfo_Linux::BiosInfo_Linux()
{

}

QString BiosInfo_Linux::getHwManufacturer() const
{
    QFileInfo dmitable("/sys/firmware/dmi/tables/smbios_entry_point");

    if(!dmitable.isReadable())
    {
        qInfo() << "Permission denied";
        return "Permission denied";
    }

    redi::pstream is("dmidecode --s baseboard-manufacturer");
    std::string res;

    is >> res;
    return QString::fromStdString(res);
}

QString BiosInfo_Linux::getHwModel() const
{
    QFileInfo dmitable("/sys/firmware/dmi/tables/smbios_entry_point");

    if(!dmitable.isReadable())
    {
        qInfo() << "Permission denied";
        return "Permission denied";
    }

    redi::pstream is("dmidecode --s baseboard-product-name");
    std::string res;

    is >> res;
    return QString::fromStdString(res);
}

#endif // Q_OS_UNIX
