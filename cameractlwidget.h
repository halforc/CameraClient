#ifndef CAMERACTLWIDGET_H
#define CAMERACTLWIDGET_H

#include <QWidget>
#include <opencv2/imgproc/imgproc.hpp>

#include<QtNetwork>
#include<QTcpServer>
#include<QTcpSocket>

#include <QPixmap>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "acquisitionthread.h"

#include "cameractrl.h"
#include "roidefine.h"
#include "autoexposuresetup.h"
#include "inputdebouncesetup.h"
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

    void rectROIChanged(QRect &rect,bool flag);

private slots:
    void getCameraImage(cv::Mat& image);
    void onComDPIChanged(int index);
    void onComImageTypeChanged(int index);
    void on_pbDefine_clicked();
    void on_pbROI_clicked();
    void on_pbSWPing_clicked();

    void onReceiveDefine(QRect rect,bool bSave);

    void on_pbAutoExposureSetup_clicked();
    void on_pbDebounceSetup_clicked();
    void onComTriggerSelectorChanged(int index);
    void onComTriggerSourceChanged(int index);
    void onEnableAutoExposure(bool flag);
    void onEnableDebounce(bool flag);

    void on_leExposure_textChanged(QString str);
    void on_leGain_textChanged(QString str);

    void on_sliderExposure_valueChanged(int val);
    void on_sliderGain_valueChanged(int val);
    void on_pbApply_clicked();

    void SendData();

signals:
    void saveImage();
    void stopSaveImage();

    void dlgClose();
    void updatePic(QPixmap& pic);

    void selectROI(QRect& rect);
    void ROIRectChanged(QRect& rect,bool flag);
    void notifyWidgetAreaROI(QRect& rect);
private:
    Ui::CameraCtlWidget *ui;
    QWidget* m_wndParent;
    ROIDefine* m_wROIDefine;

    AutoExposureSetup* m_wAutoExposureDlg;
    AEAG_PARA* m_pAEAGPara;
    InputDebounceSetup* m_wInputDebounceSetup;

    void initial();
    bool m_bIsRecording;
    bool m_bIsAutoExposure;
    DEVICE_INFO getDevInfo(xiAPIplus_Camera& cam);
    void updateFrameRateInfo();

    void readDevParaFromXML(DEVICE_INFO *pDevInfo);
    void writeDevParaToXML(xiAPIplusCameraOcv &cam);

    QImage Mat2QImage(cv::Mat& cvImg);
    QPixmap m_curImage;
    QImage myImage;

    QRect rectROI;

    qint64 blockSize;

    QTcpSocket* tcpSocket;
    QTimer* myTimer;

};

#endif // CAMERACTLWIDGET_H
