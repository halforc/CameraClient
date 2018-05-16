#include "roidefine.h"
#include "ui_roidefine.h"
#include <QDebug>
ROIDefine::ROIDefine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ROIDefine)
{
    ui->setupUi(this);
}

ROIDefine::ROIDefine(CameraCtrl* camctrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ROIDefine),
    m_camCtrl(camctrl)
{
    ui->setupUi(this);
//    m_cam = m_camCtrl->getCameraHandle();
//    ui->offsetX->setMaximum(m_cam->GetOffsetX_Maximum());
//    ui->offsetX->setMinimum(m_cam->GetOffsetX_Minimum());
//    ui->offsetY->setMaximum(m_cam->GetOffsetY_Maximum());
//    ui->offsetY->setMinimum(m_cam->GetOffsetY_Minimum());

//    ui->Width->setMaximum(m_cam->GetWidth_Maximum());
//    ui->Width->setMinimum(m_cam->GetWidth_Minimum());
//    ui->Height->setMaximum(m_cam->GetHeight_Maximum());
//    ui->Height->setMinimum(m_cam->GetHeight_Minimum());

//    m_rectROI = QRect(QPoint(m_cam->GetOffsetX(),m_cam->GetOffsetX()),
//                             QSize(m_cam->GetWidth(),m_cam->GetHeight()));
//    ui->offsetX->setValue(m_rectROI.x());
//    ui->offsetY->setValue(m_rectROI.y());
//    ui->Width->setValue(m_rectROI.width());
//    ui->Height->setValue(m_rectROI.height());
    ui->offsetX->setSingleStep(16);
    ui->offsetY->setSingleStep(2);
    ui->Width->setSingleStep(16);
    ui->Height->setSingleStep(2);
    ui->offsetX->setMaximum(640);
    ui->offsetY->setRange(0,512);
    ui->Width->setRange(160,640);
    ui->Height->setRange(126,512);
    ptTopLeft = QPoint(0,0);
    szROI = QSize(640,521);
}

ROIDefine::~ROIDefine()
{
    delete ui;
}

void ROIDefine::on_pbOK_clicked()
{

    this->close();
}

void ROIDefine::on_pbCancel_clicked()
{
    this->close();
}

void ROIDefine::on_offsetX_valueChanged(int value)
{
    qDebug()<<"on_offsetX_valueChanged"<<value;
//    value = (value + 32+8)/16 * 16;
//    ui->offsetX->setValue(value);
//    ptTopLeft.setX(value);
    emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,szROI),true);
}


void ROIDefine::on_offsetY_valueChanged(int value)
{
    qDebug()<<"on_offsetY_valueChanged"<<value;
    value = (value+32 + 1)/2 * 2;
    ui->offsetY->setValue(value);
    ptTopLeft.setY(value);
    emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,szROI),true);
}

void ROIDefine::on_Width_valueChanged(int value)
{
    qDebug()<<"on_Width_valueChanged"<<value;
    value = (value+32 + 8)/16 * 16;
    ui->Width->setValue(value);
    szROI.setWidth(value);
    emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,szROI),true);
}

void ROIDefine::on_Height_valueChanged(int value)
{
    qDebug()<<"on_Height_valueChanged"<<value;
    value = (value+32 + 1)/2 * 2;
    ui->Height->setValue(value);
    szROI.setHeight(value);
    emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,szROI),true);
}

void ROIDefine::rectROIChanged(QRect &rect,bool flag)
{
//    ui->offsetX->setValue(rect.x());
//    ui->offsetY->setValue(rect.y());
//    ui->Width->setValue(rect.width());
//    ui->Height->setValue(rect.height());
    qDebug()<<"ROIDefine roidefine"<<rect;
}

