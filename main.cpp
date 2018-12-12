#include <QCoreApplication>
#include <QDebug>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QSysInfo>
#include <QThread>
#include <QNetworkInterface>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

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
    const QCommandLineOption disableSearchOption(QStringList() << "n" << "nosearch", "Disables file search");
    parser.addOption(disableSearchOption);
    parser.addHelpOption();
    //parser.setApplicationDescription("");
    const QCommandLineOption versionOption = parser.addVersionOption();

    parser.process(QCoreApplication::arguments());

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

    QJsonObject js_collected_data;
    QJsonObject js_system_info;
    QJsonObject js_os_info;
    QJsonObject js_hw_info;
    QJsonObject js_cpu_info;
    QJsonObject js_res_info;
    QJsonArray js_ports;
    QJsonArray js_nets;
    QJsonArray js_files;

    js_collected_data.insert("timestamp", QJsonValue::fromVariant(QDateTime::currentDateTimeUtc()));

    qInfo() << "Processing files. This could take several minutes ...";

    std::vector<QRegularExpression> regexes;

    for(auto path : paths)
    {
        regexes.push_back(QRegularExpression(path, QRegularExpression::CaseInsensitiveOption));
    }

#ifdef  QT_NO_DEBUG
    if(!parser.isSet(disableSearchOption))
    {
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
                        js_files.push_back(QJsonValue::fromVariant(file_info.absoluteFilePath()));
                    }
                }
                //QThread::msleep(1);
            }
        }
    }

#endif  //QT_NO_DEBUG
    js_system_info.insert("files", js_files);
    js_system_info.insert("server_name", QJsonValue::fromVariant(QSysInfo::machineHostName()));

    js_os_info.insert("kernel_type", QJsonValue::fromVariant(QSysInfo::kernelType()));
    js_os_info.insert("kernel_version", QJsonValue::fromVariant(QSysInfo::kernelVersion()));
    js_os_info.insert("pretty_name", QJsonValue::fromVariant(QSysInfo::prettyProductName()));
    js_system_info.insert("os_info", js_os_info);

    BiosInfo* bios_info = BiosFactory::Instance()->getBiosInfo();
    CpuInfo* cpu_info = CpuFactory::Instance()->getCpuInfo();
    ResourcesInfo* res_info = ResourcesFactory::Instance()->getResourcesInfo();

    js_hw_info.insert("sys_manufacturer", QJsonValue::fromVariant(bios_info->getHwManufacturer()));
    js_hw_info.insert("sys_model", QJsonValue::fromVariant(bios_info->getHwModel()));

    js_cpu_info.insert("cpu_model", QJsonValue::fromVariant(cpu_info->getCpuModel()));
    js_cpu_info.insert("cpu_manuf", QJsonValue::fromVariant(cpu_info->getCpuManufacturer()));
    js_cpu_info.insert("cpu_arch", QJsonValue::fromVariant(QSysInfo::currentCpuArchitecture()));
    js_cpu_info.insert("logical_cpus", QJsonValue::fromVariant(QThread::idealThreadCount()));
    js_cpu_info.insert("phys_cpus",  QJsonValue::fromVariant(cpu_info->getCpuCount()));
    js_hw_info.insert("cpu_info", js_cpu_info);
    js_system_info.insert("hw_info", js_hw_info);

    js_res_info.insert("mem_use", QJsonValue::fromVariant(res_info->getTotalMemoryUsed()));
    js_res_info.insert("cpu_use", QJsonValue::fromVariant(res_info->getCpuLoad()));
    js_system_info.insert("res_info", js_res_info);

    QStringList ports = res_info->getOpenPorts();

    for(auto port : ports)
    {
        js_ports.push_back(QJsonValue::fromVariant(port));
    }

    js_system_info.insert("ports_info", js_ports);

    for(auto interface : QNetworkInterface::allInterfaces())
    {
        QJsonObject net_int;
        net_int.insert("if", QJsonValue::fromVariant(interface.humanReadableName()));
        net_int.insert("mac_addr", QJsonValue::fromVariant(interface.hardwareAddress()));
        js_nets.push_back(net_int);
    }

    js_system_info.insert("net_info", js_nets);

    js_collected_data.insert("system_info", js_system_info);

    QJsonDocument js_report(js_collected_data);

    std::cout << js_report.toJson(QJsonDocument::JsonFormat::Indented).toStdString();

    QString report_name {QSysInfo::machineHostName() + ".txt"};
    QFile report{report_name};

    if(!report.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qInfo() << "Error: Cannot create report file";
        return -1;
    }

    QTextStream out(&report);

    out << js_report.toJson(QJsonDocument::JsonFormat::Indented);

    const QString s3_filename = username + "%2F" + report_name;
    const QString local_path = "./" + report_name;

    qInfo() << "Uploading report";

    S3Uploader::put(headerData, local_path, s3_filename);

    qInfo() << "Done!";

    return 0;
}
