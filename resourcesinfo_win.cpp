#include "resourcesinfo_win.h"

#include <QtDebug>
#include <QThread>
#include <QProcess>

#include <QtGlobal>

#ifdef Q_OS_WIN

#include "Windows.h"

#include "cpuload_win.h"

ResourcesInfo_Win::ResourcesInfo_Win()
{}

float
ResourcesInfo_Win::getTotalMemoryUsed()
{

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

    return ((float)physMemUsed/(float)totalPhysMem) * 100.0;
}

int
ResourcesInfo_Win::getCpuLoad()
{
    CpuLoad_Win cpu_load;

    return cpu_load.get_load();
}

QStringList
ResourcesInfo_Win::getOpenPorts()
{

    std::map<int, QString> pid_name_map = getProcessNames();

    QStringList enriched_list;

    QStringList ports_list = callProcess("netstat -ano -p tcp");

    for(auto line : ports_list)
    {
        int pid;

        QRegExp rx("(\\ |\\t)");
        QStringList port_details = line.split(rx, QString::SkipEmptyParts);

        if(port_details.size() >= 4 && (port_details.at(0) == "TCP" || port_details.at(0) == "UDP"))
        {
            bool ok = false;
            pid = port_details.at(4).toInt(&ok);
            QString csv_line;

            if(ok)
            {
                for(auto detail : port_details)
                {
                    csv_line.append(detail);
                    csv_line.append(";");
                }

                csv_line.append(pid_name_map[pid]);
            }
            enriched_list.append(csv_line);
        }
    }

    return enriched_list;
}


QStringList
ResourcesInfo_Win::callProcess(QString exename)
{
    QProcess process;

    process.start(exename);
    process.waitForFinished();
    QString output (process.readAllStandardOutput());

    QStringList output_as_list = output.split("\r\n");

    return output_as_list;
}

std::map<int, QString>
ResourcesInfo_Win::getProcessNames()
{
    std::map<int, QString> pid_name_map;

    QStringList tasks_list = callProcess("tasklist /fo csv /nh");


    for(auto line : tasks_list)
    {
        QString name;
        int pid;
        line.remove("\"");

        QRegExp rx("\\,");
        QStringList task_details = line.split(rx, QString::SkipEmptyParts);

        if(task_details.size() >= 2)
        {
            name = task_details.at(0);
            bool ok = false;
            pid = task_details.at(1).toInt(&ok);

            if(ok)
            {
                pid_name_map[pid] = name;
            }
            else
            {
                pid_name_map[pid] = "Error-ProcessList: could not convert string to int";
            }
        }
    }

    return pid_name_map;
}
#endif //Q_OS_WIN
