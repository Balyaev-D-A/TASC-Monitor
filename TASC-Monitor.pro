QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport serialport multimedia

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    commondialog.cpp \
    connectiondialog.cpp \
    errordialog.cpp \
    flowmeter.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    settings.cpp

HEADERS += \
    aboutdialog.h \
    commondialog.h \
    connectiondialog.h \
    errordialog.h \
    flowmeter.h \
    mainwindow.h \
    qcustomplot.h \
    settings.h

FORMS += \
    aboutdialog.ui \
    commondialog.ui \
    connectiondialog.ui \
    errordialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:{
    VERSION = 1.0.0
    QMAKE_TARGET_COMPANY = АО ВОЛГОДОНСКИЙ ИТЦ РК
    QMAKE_TARGET_PRODUCT = TASC Monitor
    QMAKE_TARGET_DESCRIPTION = Мониторинг расхода воздуха через установку TASC-HTO-HT-C14
    QMAKE_TARGET_COPYRIGHT = Баляев Дмитрий Алексеевич
}

win32: RC_ICONS = $$PWD/images/appmain.ico

win32-g++:LIBS += libUser32
