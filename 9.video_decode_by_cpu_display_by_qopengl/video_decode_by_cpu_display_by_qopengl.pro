#-------------------------------------------------
#
# Project created by QtCreator 2021-04-02T17:31:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

# 包含FFmpeg库文件位置
contains(QT_ARCH, i386) {
message("32-bit")
DESTDIR = D:/WorkSpace/Projects/ffmpeg_beginner/bin32
} else {
message("64-bit")
DESTDIR = $${PWD}/bin64
}

# windows平台
win32{
INCLUDEPATH += D:\\WorkSpace\\Projects\\ffmpeg-libs\\win32\\ffmpeg\\include
LIBS += -LD:/WorkSpace/Projects/ffmpeg-libs/win32/ffmpeg/lib \
        -lavcodec \
        -lavdevice \
        -lavformat \
        -lavutil   \
        -lpostproc \
        -lswresample \
        -lswscale
}

unix:{
LIBS += -lavcodec -lavdevice -lavfilter \
        -lavformat -lavutil -lpostproc \
        -lswscale
}

SOURCES += \
    i420render2.cpp \
        main.cpp \
        mainwindow.cpp \
    ffmpegdecoder.cpp \
    i420render.cpp

HEADERS += \
    i420render2.h \
        mainwindow.h \
    ffmpegdecoder.h \
    i420render.h

FORMS += \
        mainwindow.ui
