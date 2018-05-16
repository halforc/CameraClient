#ifndef AUTOEXPOSURESETUP_H
#define AUTOEXPOSURESETUP_H

#include <QDialog>
#include "cameractrl.h"
namespace Ui {
class AutoExposureSetup;
}

class AutoExposureSetup : public QDialog
{
    Q_OBJECT

public:
    explicit AutoExposureSetup(CameraCtrl* camCtrl, QWidget *parent = 0);
    ~AutoExposureSetup();

private slots:
    void onPriorityChanged(int val);
    void onMaxExposureChanged(int val);
    void onMaxGainChanged(int val);
    void onTargetLevelChanged(int val);

private:
    Ui::AutoExposureSetup *ui;

    CameraCtrl* m_camctrl;
    xiAPIplusCameraOcv* m_xiCam;

    QString formatExposureTimeShow(int val);
};

#endif // AUTOEXPOSURESETUP_H
