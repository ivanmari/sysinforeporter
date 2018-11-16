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
#include <QCommandLineParser>

#include <iostream>

#include "biosfactory.h"
#include "cpufactory.h"
#include "resourcesfactory.h"
#include "s3uploader.h"
#include "passwordreader.h"

#include "filepaths.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;

    QCoreApplication::setApplicationName("System Info Report");
    QCoreApplication::setApplicationVersion("0.2.2");

    const QCommandLineOption userOption("u", "The username", "username");
    parser.addOption(userOption);
    parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments()))
    {
        qInfo() << parser.errorText();
        return -1;
    }

    if (parser.isSet(versionOption))
    {
        std::cout << qPrintable(QCoreApplication::applicationName()) << " " <<
                     qPrintable(QCoreApplication::applicationVersion());
        return 0;
    }


    QString username;

    if (parser.isSet(userOption))
    {
        username = parser.value(userOption);
    }
    else
    {
        std::cout << parser.helpText().toStdString();
        return -1;
    }

    QString password = PasswordReader::askPassword();

    QString concatenated = username + ":" + password;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;

    password.clear();
    concatenated.clear();
    data.clear();

    QString report_name {QSysInfo::machineHostName() + ".txt"};
    QFile report{report_name};

    if(!report.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qInfo() << "Error: Cannot create report file";
        return -1;
    }

    QTextStream out(&report);

    qInfo() << "Processing files. This could take several minutes ...";

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

    const QString s3_filename = username + "%2F" + report_name;
    const QString local_path = "./" + report_name;

    qInfo() << "Uploading report";

    S3Uploader::put(headerData, local_path, s3_filename);

    qInfo() << "Done!";

    return 0;
}
