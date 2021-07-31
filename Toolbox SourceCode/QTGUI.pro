QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    functions.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    functions.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:\Software\OpenCv4\opencv\release\install\include

LIBS += D:\Software\OpenCv4\opencv\release\bin\libopencv_calib3d450.dll
LIBS += D:\Software\OpenCv4\opencv\release\bin\libopencv_core450.dll
LIBS += D:\Software\OpenCv4\opencv\release\bin\libopencv_imgcodecs450.dll
LIBS += D:\Software\OpenCv4\opencv\release\bin\libopencv_imgproc450.dll
LIBS += D:\Software\OpenCv4\opencv\release\bin\libopencv_highgui450.dll

RESOURCES += \
    resources.qrc

DISTFILES +=
