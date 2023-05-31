#-------------------------------------------------
#
# Project created by QtCreator 2023-03-22T16:12:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = servoControlDesktopApplication
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

INCLUDEPATH += C:/Users/hmcul/Documents/DynamixelSDK/c++/include/dynamixel_sdk

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        C:/Users/hmcul/Documents/DynamixelSDK/c++/src/dynamixel_sdk/*.cpp \
    ServoUtility.cpp \
    skinslipsubwindow.cpp \
    skinslipdelaysubwindow.cpp \
    squeezesubwindow.cpp \
    pushsubwindow.cpp \
    continuousskinslipsubwindow.cpp \
    continuousskinslipdelaysubwindow.cpp

HEADERS += \
        mainwindow.h \
    ServoUtility.h \
    skinslipsubwindow.h \
    skinslipdelaysubwindow.h \
    squeezesubwindow.h \
    pushsubwindow.h \
    continuousskinslipsubwindow.h \
    continuousskinslipdelaysubwindow.h

FORMS += \
        mainwindow.ui \
    skinslipsubwindow.ui \
    skinslipdelaysubwindow.ui \
    squeezesubwindow.ui \
    pushsubwindow.ui \
    continuousskinslipsubwindow.ui \
    continuousskinslipdelaysubwindow.ui
