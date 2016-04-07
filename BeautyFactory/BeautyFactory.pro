#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T12:06:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BeautyFactory
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    filter.h \
    facedetect.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\Libraries_Static\opencv\build\include\
INCLUDEPATH += D:\Libraries_Static\opencv\build\include\opencv\
INCLUDEPATH += D:\Libraries_Static\opencv\build\include\opencv2\

CONFIG(debug,debug|release) {
LIBS += -LD:\Libraries_Static\opencv\build\x64\vc12\lib \
    -lopencv_core249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_features2d249d \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_video249d \
    -lopencv_objdetect249d \
} else {
LIBS += -LD:\Libraries_Static\opencv\build\x64\vc12\lib \
    -lopencv_core249 \
    -lopencv_highgui249 \
    -lopencv_imgproc249 \
    -lopencv_features2d249 \
    -lopencv_calib3d249 \
    -lopencv_contrib249 \
    -lopencv_video249 \
    -lopencv_objdetect249 \
}

RC_FILE = BF-icon.rc
