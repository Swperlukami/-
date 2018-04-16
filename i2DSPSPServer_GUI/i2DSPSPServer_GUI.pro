TEMPLATE = app

QT += core
QT += qml quick
QT += network
QT += widgets
QT += multimedia
QT += serialport
QT += sql

CONFIG += c++11

SOURCES += main.cpp \
    APServer/processaddata.cpp \
    APServer/aptcpserver.cpp \
    APServer/tcpsocket.cpp \
    USC4000/rs485.cpp \
    USC4000/serialport.cpp \
    PeripheryDevices/powermodeserver.cpp \
    UDPRecordFileClient/udpboradcastclient.cpp \
    SysDataBase/database.cpp \
    APController/apcontroller.cpp \
    SPController/spcontroller.cpp \
    Broadcast/alsasoundobject.cpp \
    Broadcast/audiopcmoutput.cpp \
    uicontroller.cpp \
    ShareMemory/queuesharememory.cpp \
    SysConfig/getsysconfig.cpp \
    APServer/processuievent.cpp \
    SysRecord/sysrecord.cpp \
    PeripheryDevices/tcpsocketbasecontroller.cpp \
    PeripheryDevices/matrixcontroller.cpp \
    SysDataBase/emgbroadcastdatabase.cpp \
    EventControllers/channelmusicevent.cpp \
    EventControllers/audiofilesmanagecontroller.cpp \
    PeripheryDevices/powermodelcontroller.cpp \
    PeripheryDevices/netmiccontroller.cpp \
    PeripheryDevices/lisenlingspkcontroller.cpp \
    EventControllers/systemeventscontroller.cpp \
    Broadcast/sysbroadcastcontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    APServer/ap_protocol.h \
    APServer/aptcpserver.h \
    APServer/tcpsocket.h \
    USC4000/rs485.h \
    USC4000/serialport.h \
    USC4000/usc4000_protocol.h \
    PeripheryDevices/periphery_protocol.h \
    PeripheryDevices/powermodeserver.h \
    UDPRecordFileClient/udpboradcastclient.h \
    SysDataBase/database.h \
    SysProrocol/mc_uiprotocol.h \
    SysProrocol/server_ui_protocol.h \
    APController/apcontroller.h \
    SPController/spcontroller.h \
    Broadcast/alsasoundobject.h \
    Broadcast/audiopcmoutput.h \
    uicontroller.h \
    ShareMemory/queuesharememory.h \
    SysConfig/getsysconfig.h \
    SysRecord/sysrecord.h \
    SysProrocol/mc_protocol.h \
    PeripheryDevices/matrix_protocol.h \
    PeripheryDevices/tcpsocketbasecontroller.h \
    PeripheryDevices/matrixcontroller.h \
    SysDataBase/emgbroadcastdatabase.h \
    EventControllers/channelmusicevent.h \
    EventControllers/audiofilesmanagecontroller.h \
    PeripheryDevices/powermodelcontroller.h \
    PeripheryDevices/netmiccontroller.h \
    PeripheryDevices/lisenlingspkcontroller.h \
    EventControllers/systemeventscontroller.h \
    Broadcast/sysbroadcastcontroller.h

LIBS += -lasound

DISTFILES +=

FORMS += \
    matrixtest.ui
