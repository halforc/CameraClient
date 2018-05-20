#include "inputdebouncesetup.h"
#include "ui_inputdebouncesetup.h"



InputDebounceSetup::InputDebounceSetup(CameraCtrl *pCameraCtrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDebounceSetup),
    m_camctrl(pCameraCtrl)
{
    ui->setupUi(this);
    setFixedSize(size());
    m_pixmap.load("../XiMeaCameraCtrl/images/debouncer_big.png");

    ui->lbImage->setPixmap(m_pixmap);
    ui->pbT0Settint->setText(QString::number(m_camctrl->getCameraHandle()->GetGPIDebounceFirstEdge_Maximum())+"us");
    ui->pbT1Setting->setText(QString::number(m_camctrl->getCameraHandle()->GetGPIDebounceSecondEdge_Maximum())+"us");
}

InputDebounceSetup::~InputDebounceSetup()
{
    delete ui;
}

void InputDebounceSetup::on_pbClose_clicked()
{
    this->close();
}

void InputDebounceSetup::on_pbT0Setting_clicked()
{

}

void InputDebounceSetup::on_pbT1Setting_clicked()
{

}
