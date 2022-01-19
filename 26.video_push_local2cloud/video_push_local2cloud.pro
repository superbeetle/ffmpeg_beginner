TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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

# linux平台
unix{
    INCLUDEPATH+= .
    LIBS += -L/usr/lib/x86_64-linux-gnu/ \
            -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc \
            -lswresample -lswscale
}

SOURCES += \
        main.cpp
