#-------------------------------------------------
#
# Project created by QtCreator 2016-04-17T10:08:16
#
#-------------------------------------------------

QT       += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simpleQT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        myglwidget.cpp \
    mylibplus.cpp \
    videowidgetsurface.cpp

HEADERS  += mainwindow.h \
        myglwidget.h \
    myglwidget.h \
    mylibplus.h \
    videowidgetsurface.h

FORMS    += mainwindow.ui


# libPLUS DLL
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/x64/v140/ -llibPLUS64dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/x64/v140/ -llibPLUS64dlld

INCLUDEPATH += $$PWD/../../platform/WinDLL
DEPENDPATH += $$PWD/../../platform/WinDLL
