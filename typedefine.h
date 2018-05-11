#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H
#include "./XiAPI/xiApiPlusOcv.hpp"
#include <QStringList>

enum WORKSMODE{fileReplayMode,camAsquistionMode};

enum CAMSTATUS{camClose,camOnAsquistion,camRecording,camStopRecord};

typedef struct{
    int curDevID;

    int curExposureTime;
    int miniExposureTime; //ExposureTime
    int maxExposureTime;
    int exposureTimeIncrement;

    float curFrameRate;
    float miniFrameRate;    //framerate
    float maxFrameRate;
    float frameRateIncreament;

    int curOffsetX;
    int miniOffsetX;//OffsetX
    int maxOffsetX;
    int offsetXIncreament;

    int curOffsetY;
    int miniOffsetY;//OffsetY
    int maxOffsetY;
    int offsetYIncreament;

    bool bIsAutoWhiteBlance;//Auto White Blance

    int curWidth;
    int miniWidth;//width
    int maxWidth;
    int widthIncrement;

    int curHeight;
    int miniHeight;//height
    int maxHeight;
    int heightIncrement;

    XI_IMG_FORMAT imageFormate;

    XI_TRG_SOURCE trggerSource;

    XI_TRG_SELECTOR trggerSelector;    //触发方式

}DEVICE_INFO,*PDEV_INFO;


#endif // TYPEDEFINE_H
