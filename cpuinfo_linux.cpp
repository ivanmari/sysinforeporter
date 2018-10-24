#include "cpuinfo_linux.h"

#ifdef Q_OS_UNIX
#include "pstream.h"
#include "QMap"

CpuInfo_Linux::CpuInfo_Linux()
{
    redi::pstream is("cat /proc/cpuinfo");
    std::string cpuinfo_line;
    QMap<QString, QString> cpuinfo;

    while(std::getline(is, cpuinfo_line))
    {
        QString line(cpuinfo_line.c_str());
        QStringList line_values = line.split(":");
        m_cpuinfo[line_values.first().trimmed()] = line_values.last().trimmed();
    }
}

QString CpuInfo_Linux::getCpuManufacturer() const
{
    return (m_cpuinfo["vendor_id"]);
}

QString CpuInfo_Linux::getCpuModel() const
{
     return m_cpuinfo["model name"];
}

QString CpuInfo_Linux::getCpuCount() const
{
     return m_cpuinfo["cpu cores"];
}
#endif // Q_OS_UNIX
