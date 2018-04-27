#ifndef CAMERACTLWIDGET_H
#define CAMERACTLWIDGET_H

#include <QWidget>
#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "acquisitionthread.h"

namespace Ui {
class CameraCtlWidget;
}

class CameraCtlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraCtlWidget(QWidget *parent = 0);
    ~CameraCtlWidget();


    void startObjThread();
public slots:
    void on_test_clicked();
    void openCamera();
    void closeCamera();

    void showImage(cv::Mat& image);

signals:
    void startObjThreadWork1();
    void saveImage();
    void stopSaveImage();

    void sendImage(QPixmap* pixmap);

private:
    Ui::CameraCtlWidget *ui;

    xiAPIplusCameraOcv m_xiCam;

    AcquisitionThread* m_acqThread;
    QThread* m_objThread;

    QImage Mat2QImage(cv::Mat cvImg);
    void initial();
    bool m_bIsRecording;
    DEVICE_INFO getDevInfo(xiAPIplus_Camera& cam);

    void readDevParaFromXML(DEVICE_SETTING *pDevInfo);
    void writeDevParaToXML(xiAPIplusCameraOcv &cam);
    QPixmap m_curImage;
};

#endif // CAMERACTLWIDGET_H
