#include "processinfo.h"
#include "third_party/QProcessInfo/qprocessinfo.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>

const bool BACKGROUND_ONLY = false;

ProcessInfo::ProcessInfo()
{

}

QJsonArray
ProcessInfo::findProcesses(const std::vector<QString>& patterns)
{
    QJsonArray processes;
    std::vector<QProcessInfo> filtered_processes;

    QProcessList process_list = QProcessInfo::enumerate(BACKGROUND_ONLY);

    for(auto pattern : patterns)
    {
        std::copy_if(process_list.begin(), process_list.end(), std::back_inserter(filtered_processes), [pattern](const QProcessInfo proc_info ){return proc_info.name().contains(pattern, Qt::CaseInsensitive);});
    }

    for(const auto& filtered_process : filtered_processes)
    {
        processes.push_back(QJsonValue::fromVariant(filtered_process.name() ));
    }

    return processes;
}
