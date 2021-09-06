#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T18:38:15
#
#-------------------------------------------------

QT       += core gui network sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = partrolSystem
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../partrol

CONFIG      += warn_off
HEADERS     += head.h \
    mount/cmountstate.h \
    mount/commonconfig.h \
    mount/mountnet.h \
    mount/steeringnet.h \
    mount/tempmanager.h \
    mount/xsocketclient.h \
    sql/sqlmanager.h \
    task/periodtask.h \
    task/taskmanager.h \
    task/taskresult.h \
    mount/smtstyle.h \
    mount/mountfaultexec.h \
    mount/mountalarm.h
SOURCES     += main.cpp \
    mount/cmountstate.cpp \
    mount/commonconfig.cpp \
    mount/mountnet.cpp \
    mount/steeringnet.cpp \
    mount/tempmanager.cpp \
    mount/xsocketclient.cpp \
    sql/sqlmanager.cpp \
    task/periodtask.cpp \
    task/taskmanager.cpp \
    task/taskresult.cpp \
    mount/mountfaultexec.cpp \
    mount/mountalarm.cpp
RESOURCES   += other/main.qrc
RESOURCES   += other/qss.qrc

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/api
INCLUDEPATH += $$PWD/form

include ($$PWD/api/api.pri)
include ($$PWD/form/form.pri)
include ($$PWD/QsLog/QsLog.pri)
include ($$PWD/json/json.pri)
include ($$PWD/zip/zip.pri)
include ($$PWD/wait/wait.pri)
include ($$PWD/xlsx/qtxlsx.pri)

LIBS += -L$$PWD/api/camera/magsdk/ -lThermoGroupSDKLib_x64 Ws2_32.lib

RC_ICONS = AppT.ico
