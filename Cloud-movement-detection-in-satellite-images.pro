#-------------------------------------------------
#
# Project created by QtCreator 2011-05-07T15:36:23
#
#-------------------------------------------------

QT       += core gui

TARGET = Cloud-movement-detection-in-satellite-images
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageshandler.cpp \
    movementscene.cpp \
    blockmatching.cpp \
    fullsearch.cpp \
    hexagonalsearch.cpp

HEADERS  += mainwindow.h \
    imageshandler.h \
    movementscene.h \
    blockmatching.h \
    fullsearch.h \
    hexagonalsearch.h

FORMS    += mainwindow.ui
