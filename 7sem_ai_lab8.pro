#-------------------------------------------------
#
# Project created by QtCreator 2018-09-29T15:26:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 7sem_ai_lab8
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++1z
QMAKE_CXXFLAGS += -std=c++1z

SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
        ui/hebbianneuralnetworkwidget.cpp \
        ui/GridDrawer/griddrawer.cpp \
        ui/GridDrawer/markeddrawer.cpp

HEADERS += \
        ui/mainwindow.hpp \
        ui/hebbianneuralnetworkwidget.hpp \
        ui/GridDrawer/griddrawer.hpp \
        ui/GridDrawer/markeddrawer.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
