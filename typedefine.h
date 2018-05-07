#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H
#include "./XiAPI/xiApiPlusOcv.hpp"
#include <QStringList>

enum WORKSMODE{fileReplayMode,camAsquistionMode};

enum CAMSTATUS{camClose,camOnAsquistion,camRecording,camStopRecord};

//    //曝光时间
//#define MIN_EXPOSURE_TIME       100
//#define MAX_EXPOSURE_TIME       10000
//#define INC_EXPOSURE_TIME       10
//    //光圈
//#define MIN_LENS_APERTURE       100
//#define MAX_LENS_APERTURE       1000
//#define INC_LENS_APERTURE       10
//  //帧率
//#define MIN_FRAME_RATE          20
//#define MAX_FRAME_RATE          100
//#define INC_FRAME_RATE          10
//    //焦距
//#define MIN_FOCUS_DISTANCE      10
//#define MAX_FOCUS_DISTANCE      100
//#define INC_FOCUS_DISTANCE      1

typedef struct{
    int devID;
    CAMSTATUS devStatus;
    int exposureTime;
    float frameRate;
    bool bIsAutoWhiteBlance;//自动白平衡

    float width;
    float height;
    float offsetX;
    float offsetY;


    //触发源 包括无、上升沿、下降沿、软件、高电平、低电平六种
    XI_TRG_SOURCE trggerSource;
    //触发方式
    XI_TRG_SELECTOR trggerSelector;
}DEVICE_SETTING,*PDEVICE_SETTING;

typedef struct{
    int devNum;
    //曝光时间
    int miniExposureTime;
    int maxExposureTime;
    int exposureTimeIncrement;
    //光圈
    float miniLensAperture;
    float maxLensAperture;
    float lensApertureIncrement;
    //帧率
    float miniFrameRate;
    float maxFrameRate;
    float frameRateIncreament;
    //焦距
    float miniFocusDistance;
    float maxFocusDistance;
    float focusDistanceIncrement;
    //宽度
    float miniWidth;
    float maxWidth;
    float widthIncrement;
    //高度
    float miniHeight;
    float maxHeight;
    float heightIncrement;
    DEVICE_SETTING setting;

}DEVICE_INFO,*PDEV_INFO;


#endif // TYPEDEFINE_H
