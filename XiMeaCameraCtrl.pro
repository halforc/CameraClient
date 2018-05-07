#-------------------------------------------------
#
# Project created by QtCreator 2018-04-25T15:20:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XiMeaCameraCtrl
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


SOURCES += \
        main.cpp \
        maindlg.cpp \
    cameractlwidget.cpp \
    acquisitionthread.cpp \
    playercontrols.cpp \
    widget.cpp \
    cameractrl.cpp

HEADERS += \
        maindlg.h \
    cameractlwidget.h \
    acquisitionthread.h \
    typedefine.h \
    playercontrols.h \
    widget.h \
    cameractrl.h

FORMS += \
        maindlg.ui \
    cameractlwidget.ui

INCLUDEPATH += D:\Qt\Qt_Opencv3.3_contrib\include
INCLUDEPATH += ./XiAPI/

HEADERS += ./XiAPI/xiApiPlusOcv.hpp
SOURCES += ./XiAPI/xiApiPlusOcv.cpp


CONFIG(debug, debug|release): {
LIBS += -LD:/Qt/Qt_Opencv3.3_contrib/lib \
-lopencv_calib3d330d \
-lopencv_core330d \
-lopencv_dnn330d \
-lopencv_features2d330d \
-lopencv_flann330d \
-lopencv_highgui330d \
-lopencv_imgcodecs330d \
-lopencv_imgproc330d \
-lopencv_ml330d \
-lopencv_objdetect330d \
-lopencv_photo330d \
-lopencv_shape330d \
-lopencv_stitching330d \
-lopencv_superres330d \
-lopencv_video330d \
-lopencv_videoio330d \
-lopencv_videostab330d
} else:CONFIG(release, debug|release): {
LIBS += -LD:/Qt/Qt_Opencv3.3_contrib/lib \
-lopencv_calib3d330d \
-lopencv_core330d \
-lopencv_dnn330d \
-lopencv_features2d330d \
-lopencv_flann330d \
-lopencv_highgui330d \
-lopencv_imgcodecs330d \
-lopencv_imgproc330d \
-lopencv_ml330d \
-lopencv_objdetect330d \
-lopencv_photo330d \
-lopencv_shape330d \
-lopencv_stitching330d \
-lopencv_superres330d \
-lopencv_video330d \
-lopencv_videoio330d \
-lopencv_videostab330d
}


win32: LIBS += -LD:/Qt/XIMEA/API/x64/ -lxiapi64

INCLUDEPATH += D:/Qt/XIMEA/API/x64
DEPENDPATH += D:/Qt/XIMEA/API/x64

win32:!win32-g++: PRE_TARGETDEPS += D:/Qt/XIMEA/API/x64/xiapi64.lib
else:win32-g++: PRE_TARGETDEPS += D:/Qt/XIMEA/API/x64/libxiapi64.a
