QT       += core gui printsupport
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += C:\OpenCV\opencv\release\install\include

LIBS += C:\OpenCV\opencv\release\bin\libopencv_core412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_highgui412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgcodecs412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgproc412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_calib3d412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_video412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_videoio412.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_features2d412.dll

SOURCES += \
    convertmatqimage.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    speedsensorsploting.cpp

HEADERS += \
    convertmatqimage.h \
    mainwindow.h \
    qcustomplot.h \
    speedsensorsploting.h

FORMS += \
    mainwindow.ui \
    speedsensorsploting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
