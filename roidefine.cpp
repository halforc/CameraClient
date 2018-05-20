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
    m_cam = m_camCtrl->getCameraHandle();
    ui->offsetX->setMaximum(m_cam->GetOffsetX_Maximum());
    ui->offsetX->setMinimum(m_cam->GetOffsetX_Minimum());
    ui->offsetY->setMaximum(m_cam->GetOffsetY_Maximum());
    ui->offsetY->setMinimum(m_cam->GetOffsetY_Minimum());

    ui->Width->setMaximum(m_cam->GetWidth_Maximum());
    ui->Width->setMinimum(m_cam->GetWidth_Minimum());
    ui->Height->setMaximum(m_cam->GetHeight_Maximum());
    ui->Height->setMinimum(m_cam->GetHeight_Minimum());

    m_rectROI = QRect(QPoint(m_cam->GetOffsetX(),m_cam->GetOffsetX()),
                             QSize(m_cam->GetWidth(),m_cam->GetHeight()));
    ui->offsetX->setValue(m_rectROI.x());
    ui->offsetY->setValue(m_rectROI.y());
    ui->Width->setValue(m_rectROI.width());
    ui->Height->setValue(m_rectROI.height());
//    ui->offsetX->setSingleStep(16);
//    ui->offsetY->setSingleStep(2);
//    ui->Width->setSingleStep(16);
//    ui->Height->setSingleStep(2);
//    ui->offsetX->setMaximum(640);
//    ui->offsetY->setRange(0,512);
//    ui->Width->setRange(160,640);
//    ui->Height->setRange(126,512);
}

ROIDefine::~ROIDefine()
{
    delete ui;
}

void ROIDefine::on_pbOK_clicked()
{
    emit notifyParent(m_rectROI,true);
}

void ROIDefine::on_pbCancel_clicked()
{
    emit notifyParent(m_rectROI,false);
}

void ROIDefine::on_offsetX_valueChanged(int value)
{
    qDebug()<<"on_offsetX_valueChanged"<<value;
    value = (value+8)/16 * 16;
    if(value > m_cam->GetOffsetX_Maximum())
        value = m_cam->GetOffsetX_Maximum();
    ui->offsetX->setValue(value);
    m_rectROI.setX(value);
    if(ui->offsetX->hasFocus())
        emit ROIRectChanged(m_rectROI);
}

void ROIDefine::on_offsetY_valueChanged(int value)
{
//    qDebug()<<"on_offsetY_valueChanged"<<value;
    value = (value + 1)/2 * 2;
    if(value > m_cam->GetOffsetY_Maximum())
        value = m_cam->GetOffsetX_Maximum();
    ui->offsetY->setValue(value);
    m_rectROI.setY(value);
    if(ui->offsetY->hasFocus())
        emit ROIRectChanged(m_rectROI);
}

void ROIDefine::on_Width_valueChanged(int value)
{
//    qDebug()<<"on_Width_valueChanged"<<value;
    value = (value+ 8)/16 * 16;
    if(value > m_cam->GetWidth_Maximum())
        value = m_cam->GetWidth_Maximum();
    ui->Width->setValue(value);
    m_rectROI.setWidth(value);
    if(ui->Width->hasFocus())
        emit ROIRectChanged(m_rectROI);
}

void ROIDefine::on_Height_valueChanged(int value)
{
//    qDebug()<<"on_Height_valueChanged"<<value;
    value = (value + 1)/2 * 2;
    if(value > m_cam->GetHeight_Maximum())
        value = m_cam->GetHeight_Maximum();
    ui->Height->setValue(value);
    m_rectROI.setHeight(value);
    if(ui->Height->hasFocus())
        emit ROIRectChanged(m_rectROI);
}

void ROIDefine::changedROI(QRect &rect,bool flag)
{
    ui->offsetX->setValue(rect.x());

    ui->offsetY->setValue(rect.y());

    ui->Width->setValue(rect.width());

    ui->Height->setValue(rect.height());

    qDebug()<<"ROIDefine roidefine"<<rect;
    if(flag){
        m_camCtrl->getCameraHandle()->SetOffsetX(rect.x());
        m_camCtrl->getCameraHandle()->SetOffsetY(rect.y());
        m_camCtrl->getCameraHandle()->SetWidth(rect.width());
        m_camCtrl->getCameraHandle()->SetHeight(rect.height());
    }
}

