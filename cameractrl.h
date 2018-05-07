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

    void getCameraPara();
    void setDrawingRect(QRect& rect);
    void setInterestRectRect(QRect& rect);
    QRect getDrawingRect();
    void saveParaToFile(DEVICE_INFO& para,QString filename);
    DEVICE_INFO* getParaFromFile(QString filename);
    xiAPIplusCameraOcv* getCameraHandle();

public:
    CAMSTATUS m_camStatus;
private:
    xiAPIplusCameraOcv m_xiCam;
    ulong m_nDevNumber;

    AcquisitionThread* m_acqThread;
    QThread* m_objThread;

    DEVICE_SETTING m_curDevInfo;
    QVector<DEVICE_SETTING> m_vecDevice;

    QPixmap m_curPic;

    int m_width;
    int m_hight;
    int m_offsetWidth;
    int m_offsetHeight;
    char m_camName[256];

public:
    bool openCamera(unsigned long devID);
    void closeCamera();

    void startObjThread();

    bool readDevParaFromXML(DEVICE_SETTING *pDevInfo);
    void writeDevParaToXML(xiAPIplusCameraOcv &cam);
signals:
    void startAcquistionWork();
    void getCameraImage(cv::Mat& image);
    void saveImage();
    void stopSaveImage();

 //   void sendImage(QPixmap* pixmap);
    void sendCameraInfo(char* name,ulong number);
public slots:
    void initialCamera();
};

#endif // CAMERACTRL_H
