#ifndef CAMERACTLWIDGET_H
#define CAMERACTLWIDGET_H

#include <QWidget>
#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "acquisitionthread.h"

#include "cameractrl.h"

namespace Ui {
class CameraCtlWidget;
}

class CameraCtlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraCtlWidget(QWidget *parent = 0);
    ~CameraCtlWidget();
    CameraCtrl* m_camCtrl;
public slots:
    void on_test_clicked();

    void openCamera(unsigned long devID);
    void closeCamera();

private slots:
    void getCameraImage(cv::Mat& image);
    void onComDPIChanged(QString str);
    void onComImageTypeChanged(int index);
    void on_pbDefine_clicked();
    void onComTriggerSelectorChanged(int index);
    void onComTriggerSourceChanged(int index);
signals:
    void saveImage();
    void stopSaveImage();

    void dlgClose();
    void updatePic(QPixmap& pic);

    void selectROI(QRect& rect);

private:
    Ui::CameraCtlWidget *ui;
    QWidget* m_wndParent;


    void initial();
    bool m_bIsRecording;
    DEVICE_INFO getDevInfo(xiAPIplus_Camera& cam);

    void readDevParaFromXML(DEVICE_INFO *pDevInfo);
    void writeDevParaToXML(xiAPIplusCameraOcv &cam);

    QImage Mat2QImage(cv::Mat& cvImg);
    QPixmap m_curImage;
};

#endif // CAMERACTLWIDGET_H
