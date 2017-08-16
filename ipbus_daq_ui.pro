#-------------------------------------------------
#
# Project created by QtCreator 2017-07-22T17:26:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/local/qwt-6.1.3/include
LIBS += -L"/usr/local/qwt-6.1.3/lib/" -lqwt

TARGET = ipbus_daq_ui
TEMPLATE = app


SOURCES += main.cpp\
        mywindow.cpp \
    data_display.cpp \
    log_recv.cxx \
    gva.cxx

HEADERS  += mywindow.h \
    data_display.h \
    log_recv.h \
    gva.h

FORMS    += mywindow.ui
