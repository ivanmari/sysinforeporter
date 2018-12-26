QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    biosinfo_win32.cpp \
    cpuinfo_win32.cpp \
    biosfactory.cpp \
    biosinfo_linux.cpp \
    cpufactory.cpp \
    cpuinfo_linux.cpp \
    third_party/SimpleCrypt/simplecrypt.cpp \
    resourcesinfo_win.cpp \
    cpuload_win.cpp \
    resourcesinfo_linux.cpp \
    consolereader.cpp \
    s3uploader.cpp \
    passwordreader.cpp \
    restapi.cpp \
    filepaths.cpp \
    fileinfo.cpp \
    processinfo.cpp \
    third_party/QProcessInfo/qprocessinfo.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpuinfo.h \
    biosinfo.h \
    biosinfo_win32.h \
    cpuinfo_win32.h \
    biosfactory.h \
    biosinfo_linux.h \
    cpufactory.h \
    cpuinfo_linux.h \
    filepaths.h \
    third_party/SimpleCrypt/simplecrypt.h \
    resourcesinfo.h \
    resourcesfactory.h \
    resourcesinfo_win.h \
    cpuload_win.h \
    resourcesinfo_linux.h \
    consolereader.h \
    s3uploader.h \
    passwordreader.h \
    restapi.h \
    fileinfo.h \
    processinfo.h \
    third_party/QProcessInfo/qprocessinfo.h
