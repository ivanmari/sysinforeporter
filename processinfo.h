#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QJsonArray>
#include <vector>

const std::vector<QString> process_patterns {"pmon", "regedit"};

class ProcessInfo
{
public:
    ProcessInfo();
    static QJsonArray findProcesses(const std::vector<QString>& patterns);
};

#endif // PROCESSINFO_H
