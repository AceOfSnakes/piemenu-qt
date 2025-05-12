QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    PieMenu.cpp

HEADERS += \
    MainWindow.h \
    PieMenu.h

FORMS += \
    MainWindow.ui

#message(inside target scope)
target.path = ./settings
target.files = *
INSTALLS += target
#INSTALLS += ./settings/*
#QMAKE_POST_LINK += "cp -f $$OUT_PWD/ settings/*"
#copyToDir($$PWD/settings, deployment/)

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
