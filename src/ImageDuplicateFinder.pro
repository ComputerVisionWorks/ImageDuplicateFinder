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
    imageduplicategroupsjsonexporter.cpp

HEADERS  += mainwindow.h \
            imageduplicatefinder.h \
            dctperceptualhash.h \
            unionfind.h \
            picturelabel.h \
            imageduplicategroups.h \
    imageduplicategroupsjsonexporter.h

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

    QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
}
