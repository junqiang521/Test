#-------------------------------------------------
#
# Project created by QtCreator 2014-12-01T21:48:48
#
#-------------------------------------------------
QT += testlib
QT += core gui
QT += script
QT += widgets
QT += uitools
QT += webkitwidgets
QT += network

LIBS += -lhiredis


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Justice
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sel_dev.cpp \
    pcie.cpp \
    justice_cmd.cpp \
    log_setting.cpp \
    buffer_setting.cpp \
    trace.cpp \
    keyin_test.cpp


HEADERS  += mainwindow.h \
    sel_dev.h \
    pcie.h \
    justice_cmd.h \
    log_setting.h \
    buffer_setting.h \
    trace.h \
    keyin_test.h

FORMS    += mainwindow.ui \
    sel_dev.ui \
    pcie.ui \
    log_setting.ui \
    buffer_setting.ui \
    trace.ui \
    keyin_test.ui

OTHER_FILES +=
