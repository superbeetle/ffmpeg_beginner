QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

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

unix{

#INCLUDEPATH += $$PWD/ffmpeg-4.4/include
#LIBS += -L$$PWD/ffmpeg-4.4/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

INCLUDEPATH += /data/home/jun/mysoft-installed/ffmpeg-4.4/include
LIBS += -L/data/home/jun/mysoft-installed/ffmpeg-4.4/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
