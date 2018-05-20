#ifndef ROIDEFINE_H
#define ROIDEFINE_H

#include <QDialog>
#include "cameractrl.h"
#include <QEvent>
namespace Ui {
class ROIDefine;
}

class ROIDefine : public QDialog
{
    Q_OBJECT

public:
    explicit ROIDefine(QWidget *parent = 0);
    ROIDefine(CameraCtrl *camctrl, QWidget *parent = 0);
    ~ROIDefine();

private slots:
    void on_pbOK_clicked();
    void on_pbCancel_clicked();
    void on_offsetX_valueChanged(int value);

    void on_offsetY_valueChanged(int value);
    void on_Width_valueChanged(int value);
    void on_Height_valueChanged(int value);

public:
    void changedROI(QRect& rect, bool flag=false);//connected with camctrl - rectROIChanged

signals:
    void ROIRectChanged(QRect& rect);
    void notifyParent(QRect rect,bool bSave);
private:
    Ui::ROIDefine *ui;
    CameraCtrl* m_camCtrl;
    xiAPIplusCameraOcv* m_cam;
    QRect m_rectROI;
};

#endif // ROIDEFINE_H
