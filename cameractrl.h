#ifndef CAMERACTRL_H
#define CAMERACTRL_H

#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "acquisitionthread.h"
#include <QObject>

class CameraCtrl : public QObject
{
    Q_OBJECT
public:
    explicit CameraCtrl(QObject *parent = 0);
    ~CameraCtrl();

    void setDrawingRect(QRect& rect);
    QRect getDrawingRect();
    void saveParaToFile(DEVICE_INFO& para,QString filename);
    DEVICE_INFO* getParaFromFile(QString filename);   
    xiAPIplusCameraOcv* getCameraHandle();

public:
    CAMSTATUS m_camStatus;
private:
    xiAPIplusCameraOcv m_xiCam;
    ulong m_nDevNumber;
    int m_curDevice;

    AcquisitionThread* m_acqThread;
    QThread* m_objThread;

    DEVICE_INFO m_devInfo;

    QPixmap m_curPic;
    QRect rectROI;

    int m_width;
    int m_hight;
    int m_offsetWidth;
    int m_offsetHeight;
    char m_camName[256];

    XI_IMG_FORMAT m_imageFormat[4];
    XI_TRG_SOURCE m_triggerSource[6];

    void getCameraPara();

public:
    bool openCamera(unsigned long devID);
    void closeCamera();
    QRect getROIRect();

    CAMSTATUS getCameraStatus();
    int setROIOffsetX(int offsetX);
    int setROIOffsetY(int offsetY);
    int setROIWidth(int width);
    int setROIHeight(int height);

    //auto exposure auto gain
    void enableAutoExposure(bool flag);

    bool setImageFormat(int index);
    void setTriggetSource(int index);
    void setTriggetSelector(int index);
    void startObjThread();

    bool readDevParaFromXML(DEVICE_INFO *pDevInfo);
    void writeDevParaToXML(xiAPIplusCameraOcv &cam);

    void startAcquistion();
    void stopAcquistion();
signals:
    void startAcquistionWork();
    void getCameraImage(cv::Mat& image);
    void saveImage();
    void stopSaveImage();

 //   void sendImage(QPixmap* pixmap);
    void sendCameraInfo(char* name,ulong number);

    void rectROIChanged(QRect& rect,bool modifyCam);//ROI changed sign,connected with widget and roidefine
public slots:
    void initialCamera();
private slots:
    void recROIChanged(QRect &rect,bool modifyCam);
};

#endif // CAMERACTRL_H
