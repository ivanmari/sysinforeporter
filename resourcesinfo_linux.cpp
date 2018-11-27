#include <QtGlobal>

#ifdef Q_OS_UNIX
#include <QDebug>
#include "resourcesinfo_linux.h"

#include "consolereader.h"

#include <QFile>
#include <QRegularExpression>
#include <QThread>

enum NetstatFields
{
    Proto,
    RecvQ,
    SendQ,
    LocalAddress,
    ForeignAddress,
    State,
    PidProgramName
};

ResourcesInfo_Linux::ResourcesInfo_Linux()
{

}

float
ResourcesInfo_Linux::getTotalMemoryUsed()
{
    size_t total_mem = 0;
    size_t available_mem = 0;

    QFile meminfo_file("/proc/meminfo");

    if(meminfo_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        QString meminfo_buf = meminfo_file.readAll();

        QRegExp rx("(\\r\\n |\\n)");
        QStringList output_as_list = meminfo_buf.split(rx);

        for(auto line : output_as_list)
        {
            QRegExp rx("(: |" ")");
            QStringList cols = line.split(":");

            if(cols.size()>=2)
            {
                if(cols.at(0).contains(("MemTotal")))
                {
                    QRegularExpression re("\\d+", QRegularExpression::CaseInsensitiveOption);
                    QRegularExpressionMatch match = re.match(cols.at(1));

                    if(match.hasMatch())
                    {
                        total_mem = match.captured(0).toULong();
                    }

                }
                else if(cols.at(0).contains(("MemAvailable")))
                {
                    QRegularExpression re("\\d+", QRegularExpression::CaseInsensitiveOption);
                    QRegularExpressionMatch match = re.match(cols.at(1));

                    if(match.hasMatch())
                    {
                        available_mem = match.captured(0).toULong();
                    }
                }

            }

        }
    }

    size_t used_mem = total_mem - available_mem;

    if(total_mem)
    {
        return ((float)used_mem/(float)total_mem) * 100.0;
    }
    else
    {
        return 0;
    }
}

int
ResourcesInfo_Linux::getCpuLoad()
{
    QStringList top_text = ConsoleReader::callProcess("top -b -n2");
    int cpu_load = -1;
    size_t row_count = 0;
    QRegExp number_rx( "(\\d*\\.\\d).*");
    for(int i = 0; i < top_text.size(); ++i)
    {
        QString line = top_text[i];

        QStringList attr = line.split(" ", QString::SkipEmptyParts);

        QString a,b;
        if(attr.size() >= 4 && attr.at(0).contains("Cpu(s)"))
        {   row_count++;
            if(row_count == 2)
            {
                if(attr.at(1).contains(number_rx))
                {
                    a = number_rx.cap(1);
                    if(attr.at(3).contains(number_rx))
                    {
                        b = number_rx.cap(1);
                        cpu_load = a.toFloat() + b.toFloat();
                    }
                }
            }
        }
    }

    return cpu_load;
}

QStringList
ResourcesInfo_Linux::getOpenPorts()
{
    QStringList ports_list = ConsoleReader::callProcess("netstat -nlpt");
    QStringList csv_ports_list;

    for(auto line : ports_list)
    {
        QRegExp rx("(\\ |\\t)");
        QStringList port_details = line.split(rx, QString::SkipEmptyParts);

        if(port_details.size() >= 6 && (port_details.at(0).contains("TCP", Qt::CaseInsensitive) || port_details.at(0).contains("UDP", Qt::CaseInsensitive)))
        {
            int fields_filter[] = {NetstatFields::Proto,NetstatFields::LocalAddress,NetstatFields::ForeignAddress,NetstatFields::State};

            QString csv_line;

            for(auto field_index : fields_filter)
            {
                csv_line.append(port_details.at(field_index));
                csv_line.append(";");
            }


            QStringList pid_name_lst = port_details.at(6).split("/");
            QString pid_name_csv = pid_name_lst.join(";");

            csv_line.append(pid_name_csv);

            csv_ports_list.append(csv_line);
        }
    }

    return csv_ports_list;
}

#endif // Q_OS_UNIX
