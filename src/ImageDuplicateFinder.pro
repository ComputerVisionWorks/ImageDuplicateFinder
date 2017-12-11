QT       += core gui widgets concurrent

TARGET = ImageDuplicateFinder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
           mainwindow.cpp \
           imageduplicatefinder.cpp \
           dctperceptualhash.cpp \
           unionfind.cpp \
           picturelabel.cpp \
           imageduplicategroups.cpp \
           imageduplicategroupsjsonexporter.cpp \
    consoleapplication.cpp \
    consoleprogressbar.cpp

HEADERS  += mainwindow.h \
            imageduplicatefinder.h \
            dctperceptualhash.h \
            unionfind.h \
            picturelabel.h \
            imageduplicategroups.h \
            imageduplicategroupsjsonexporter.h \
    consoleapplication.h \
    consoleprogressbar.h

FORMS    += mainwindow.ui

## OpenCV settings for Unix/Linux
unix:!mac {
    message("Using settings for Unix/Linux.")
    INCLUDEPATH += /usr/local/include/opencv

    LIBS += -L/usr/local/lib/ \
            -lopencv_core     \
            -lopencv_highgui  \
            -lopencv_imgproc  \
            -lopencv_imgcodecs

    QMAKE_CXXFLAGS_RELEASE += -march=native
}

## OpenCV settings for Windows and OpenCV 3.2.0
win32 {
    message("Using settings for Windows.")

    OPENCV_PATH = "C:\\Program Files\\opencv"

    INCLUDEPATH += $${OPENCV_PATH}"\\build\\include" \
                   $${OPENCV_PATH}"\\build\\include\\opencv" \
                   $${OPENCV_PATH}"\\build\\include\\opencv2"

    CONFIG(debug, debug | release) {
        LIBS += -L$${OPENCV_PATH}"\\build\\x64\\vc14\\lib" \
                -lopencv_world320d
    }

    CONFIG(release, debug | release) {
        LIBS += -L$${OPENCV_PATH}"\\build\\x64\\vc14\\lib" \
                -lopencv_world320
    }
}
