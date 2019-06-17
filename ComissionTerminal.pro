QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComissionTerminal
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        MySQLSimpleDriver.cpp \
        advancedpupilsearch.cpp \
        advancedsearch.cpp \
        csvimporter.cpp \
        examprop.cpp \
        exportxlsx.cpp \
        main.cpp \
        mainwindow.cpp \
        permissions.cpp \
        selectexam.cpp \
        team.cpp

HEADERS += \
        MySQLSimpleDriver.hpp \
        advancedpupilsearch.h \
        advancedsearch.h \
        csvimporter.h \
        examprop.h \
        exportxlsx.h \
        mainwindow.h \
        permissions.h \
        selectexam.h \
        team.h

FORMS += \
        advancedpupilsearch.ui \
        advancedsearch.ui \
        examprop.ui \
        mainwindow.ui \
        permissions.ui \
        selectexam.ui \
        team.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(3rdparty/src/xlsx/qtxlsx.pri)

DISTFILES +=

LIBS += -L/usr/lib/mysql -lmysqlclient
#QMAKE_CFLAGS += $$system(mysql_config --cflags -libs)
#QMAKE_CXXFLAGS += $$system(mysql_config --cflags -libs)
#LIBS += $$system(mysql_config --libs)
