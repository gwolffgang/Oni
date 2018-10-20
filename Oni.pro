#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T16:20:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Oni
TEMPLATE = app
ICON = pics/Oni.icns

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    oni.cpp \
    card.cpp \
    piece.cpp \
    button.cpp \
    field.cpp \
    cardslot.cpp \
    dialogabout.cpp \
    dialogsave.cpp \
    windowdatabase.cpp \
    match.cpp \
    sensei.cpp

HEADERS  += mainwindow.h \
    oni.h \
    card.h \
    piece.h \
    button.h \
    field.h \
    cardslot.h \
    dialogabout.h \
    dialogsave.h \
    windowdatabase.h \
    match.h \
    sensei.h

FORMS    += mainwindow.ui \
    dialogabout.ui \
    dialogsave.ui \
    windowdatabase.ui

RESOURCES += \
    res.qrc

DISTFILES +=
