#include <QCoreApplication>
#include<QDebug>

#include <QRegularExpressionMatch>
#include <QRegularExpression>

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>

#include <QSysInfo>
#include<QThread>

#include "biosfactory.h"
#include "cpufactory.h"

#include <QNetworkInterface>

#include "filepaths.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile report(QSysInfo::machineHostName() + ".txt");

    if(!report.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;

    QTextStream out(&report);


    qInfo() << "Processing...";

    out << "*** Files Report ***" << endl;

    std::vector<QRegularExpression> regexes;

    for(auto path : paths)
    {
        regexes.push_back(QRegularExpression(path, QRegularExpression::CaseInsensitiveOption));
    }


    for(auto drive: QDir::drives())
    {
        QDirIterator it(drive.path(), QDir::AllEntries | QDir::System | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString path = it.next();

            for(auto re : regexes)
            {
                QRegularExpressionMatch match = re.match(path);
                if(match.hasMatch())
                {
                    QFileInfo file_info(match.captured(0));
                    out << "File : " << file_info.absoluteFilePath() << " " << file_info.size() << " bytes" << endl;
                }
            }
            //QThread::msleep(1);
        }
    }

    out << "\n*** System Report ***" << endl;
    out <<"Server name :" << QSysInfo::machineHostName() << endl;
    out << "Kernel Type: " << QSysInfo::kernelType() << endl;
    out << "Kernel Version: " << QSysInfo::kernelVersion() << endl;
    out << "Pretty Product Name: "  << QSysInfo::prettyProductName() << endl;
    out << "\n*** HW Report ***" << endl;

    BiosInfo* bios_info = BiosFactory::Instance()->getBiosInfo();
    CpuInfo* cpu_info = CpuFactory::Instance()->getCpuInfo();

    out << "System HW Manufacturer: " << bios_info->getHwManufacturer() << endl;
    out << "System HW Model: " << bios_info->getHwModel() << endl;

    out << "Cpu Model: " << cpu_info->getCpuModel() << endl;
    out << "Cpu Manufacturer: " << cpu_info->getCpuManufacturer() << endl;
    out << "Cpu Arch: "  << QSysInfo::currentCpuArchitecture() << endl;
    out << "Logical Processors: " << QThread::idealThreadCount() << endl;
    out << "Physical Processors: " << cpu_info->getCpuCount() << endl;

    for(auto interface : QNetworkInterface::allInterfaces())
    {
        out << "IF: " << interface.humanReadableName();
        out << " MAC Address# " << " : " << interface.hardwareAddress() << endl;
    }

    qInfo() << "Done!";

    return 0;
}
