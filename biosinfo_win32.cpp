#include "biosinfo_win32.h"
#include "QtGlobal"
#include "QSettings"

#ifdef Q_OS_WIN

BiosInfo_Win32::BiosInfo_Win32()
{

}

QString
BiosInfo_Win32::getHwManufacturer() const
{
    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", QSettings::NativeFormat);

    return settings.value("SystemManufacturer").toString();
}

QString
BiosInfo_Win32::getHwModel() const
{
    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", QSettings::NativeFormat);

    return settings.value("SystemProductName").toString();
}
#endif //Q_OS_WIN
