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
    static QJsonArray findJavaProcesses();
};

#endif // PROCESSINFO_H
