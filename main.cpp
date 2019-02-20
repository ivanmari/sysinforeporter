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
#include <QList>
#include <QUuid>
#include <QSettings>

#include <iostream>

#include "biosfactory.h"
#include "cpufactory.h"
#include "resourcesfactory.h"
#include "s3uploader.h"
#include "passwordreader.h"
#include "restapi.h"
#include "filepaths.h"
#include "fileinfo.h"
#include "processinfo.h"
#include "orainstancedetectorfactory.h"

const QString AGENT_ID_KEY = "agent_id";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;

    QCoreApplication::setApplicationName("SystemInfoReport");
    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setOrganizationDomain("mycompany.com");
    QCoreApplication::setApplicationVersion("0.3.4");

    QSettings app_settings;

    if(!app_settings.contains(AGENT_ID_KEY))
    {
        app_settings.setValue(AGENT_ID_KEY, QUuid::createUuid());
    }

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

    QString config_query_str = "user=" + username;
    QUrl CONFIG_URL{"https://8q175xox23.execute-api.us-east-2.amazonaws.com/dev/filepatterns"};
    CONFIG_URL.setQuery(config_query_str);

    RestApi restClient(CONFIG_URL, headerData);

    QJsonObject config = restClient.get();

    std::vector<std::pair<QString, bool>> filepaths;

    //Ads the default paths patterns
    std::copy(paths.begin(), paths.end(),std::back_inserter(filepaths));

    QString PATTERN {"pattern"};
    QString UPLOAD {"upload"};

    if(config["data"].isArray())
    {
        for(auto elem : config["data"].toArray())
        {
            filepaths.push_back(std::make_pair(elem.toObject()[PATTERN].toString(), elem.toObject()[UPLOAD].toBool()));
        }
    }
    else
    {
        qInfo() << "Client could not be configured";
    }

    QJsonObject js_collected_data;
    QJsonObject js_system_info;
    QJsonObject js_os_info;
    QJsonObject js_hw_info;
    QJsonObject js_cpu_info;
    QJsonObject js_res_info;
    QJsonArray js_ports;
    QJsonArray js_nets;
    QJsonArray js_processes;
    QJsonArray js_java_processes;
    QJsonArray js_files;
    QJsonArray js_ora_instances;

    js_collected_data.insert("timestamp", QJsonValue::fromVariant(QDateTime::currentDateTimeUtc()));

    js_collected_data.insert("agent_id", QJsonValue::fromVariant(app_settings.value(AGENT_ID_KEY)));

    if(!parser.isSet(disableSearchOption))
    {
        qInfo() << "Processing files. This could take several minutes ...";
        js_files = FileInfo::findFiles(filepaths);
        js_system_info.insert("files", js_files);
    }

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

    ProcessInfo proc_info;

    js_processes = proc_info.findProcesses(process_patterns);

    js_java_processes = proc_info.findJavaProcesses();

    js_ora_instances = OraInstanceDetectorFactory::Instance()->getOracleInstanceDetector()->getOracleInstances();

    js_system_info.insert("ora_instances", js_ora_instances);

    js_system_info.insert("processes", js_processes);

    js_system_info.insert("java_processes", js_java_processes);

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

    out.flush();

    const QString s3_base_path = username + "%2F" + app_settings.value(AGENT_ID_KEY).toString();
    const QString s3_filename = s3_base_path + "%2F" + report_name;
    const QString local_path = "./" + report_name;

    qInfo() << "Uploading report";

    S3Uploader::put(headerData, local_path, s3_filename);

    std::vector<QString> files_to_upload = FileInfo::getFilesToUpload(js_files, paths);

    for(auto filename_to_upload : files_to_upload)
    {
        QList<QString> splitted_path = filename_to_upload.split("/");
        S3Uploader::put(headerData, filename_to_upload, s3_base_path + "%2F" + splitted_path.back());
    }

    for(auto instance : js_ora_instances)
    {
        QString sid = instance.toString();

        qInfo() << OraInstanceDetectorFactory::Instance()->getOracleInstanceDetector()->extractSqlInfo(sid);

    }

    qInfo() << "Done!";

    return 0;
}
