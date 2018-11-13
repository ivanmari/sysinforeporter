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
#include <QNetworkInterface>

#include "biosfactory.h"
#include "cpufactory.h"
#include "resourcesfactory.h"
#include "s3uploader.h"

#include "filepaths.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString report_name {QSysInfo::machineHostName() + ".txt"};
    QFile report{report_name};

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

#ifdef  QT_NO_DEBUG
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

#endif  //QT_NO_DEBUG

    out << "\n*** System Report ***" << endl;
    out <<"Server name :" << QSysInfo::machineHostName() << endl;
    out << "Kernel Type: " << QSysInfo::kernelType() << endl;
    out << "Kernel Version: " << QSysInfo::kernelVersion() << endl;
    out << "Pretty Product Name: "  << QSysInfo::prettyProductName() << endl;
    out << "\n*** HW Report ***" << endl;

    BiosInfo* bios_info = BiosFactory::Instance()->getBiosInfo();
    CpuInfo* cpu_info = CpuFactory::Instance()->getCpuInfo();
    ResourcesInfo* res_info = ResourcesFactory::Instance()->getResourcesInfo();

    out << "System HW Manufacturer: " << bios_info->getHwManufacturer() << endl;
    out << "System HW Model: " << bios_info->getHwModel() << endl;

    out << "Cpu Model: " << cpu_info->getCpuModel() << endl;
    out << "Cpu Manufacturer: " << cpu_info->getCpuManufacturer() << endl;
    out << "Cpu Arch: "  << QSysInfo::currentCpuArchitecture() << endl;
    out << "Logical Processors: " << QThread::idealThreadCount() << endl;
    out << "Physical Processors: " << cpu_info->getCpuCount() << endl;

    out << "Total Memory Used: " << res_info->getTotalMemoryUsed() << endl;

    out << "Cpu Load: " << res_info->getCpuLoad() << endl;

    QStringList ports = res_info->getOpenPorts();

    out << "\n*** Network connections ***\n";

    out << "Proto;Local Address;Foreign Address;State;PID;Process Name" << endl;

    for(auto port : ports)
    {
        out << port << endl;
    }

    out << "\n*** Network interfaces ***\n";

    for(auto interface : QNetworkInterface::allInterfaces())
    {
        out << "IF: " << interface.humanReadableName();
        out << " MAC Address# " << " : " << interface.hardwareAddress() << endl;
    }

    S3Uploader::put("./" + report_name, report_name);

    qInfo() << "Done!";

    return 0;
}
