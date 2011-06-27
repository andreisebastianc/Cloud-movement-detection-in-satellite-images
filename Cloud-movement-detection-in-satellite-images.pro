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
    hexagonalsearch.cpp \
    rhombussearch.cpp \
    improvesearch.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    imageshandler.h \
    movementscene.h \
    blockmatching.h \
    fullsearch.h \
    hexagonalsearch.h \
    rhombussearch.h \
    improvesearch.h \
    worker.h

FORMS    += mainwindow.ui
