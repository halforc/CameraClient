#include "autoexposuresetup.h"
#include "ui_autoexposuresetup.h"

AutoExposureSetup::AutoExposureSetup(CameraCtrl* camCtrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoExposureSetup),
    m_camctrl(camCtrl)
{
    ui->setupUi(this);

    m_xiCam = m_camctrl->getCameraHandle();

    float priority = m_xiCam->GetAutoExposureAutoGainExposurePriority();
    int maxExposure = m_xiCam->GetAutoExposureTopLimit();
    float maxGain = m_xiCam->GetAutoGainTopLimit();
    int targetLevel = m_xiCam->GetAutoExposureAutoGainTargetLevel();

    ui->lbPriority->setText(QString::number(priority*100) + tr("%"));
    ui->lbMaxExposure->setText(QString::number(maxExposure));
    ui->lbMaxGain->setText(QString::number(maxGain));
    ui->lbTargetLevel->setText(QString::number(targetLevel));

    ui->sliderPriority->setRange((int)(m_xiCam->GetAutoExposureAutoGainExposurePriority_Minimum()*100),
                                 (int)(m_xiCam->GetAutoExposureAutoGainExposurePriority_Maximum()*100));
    ui->sliderPriority->setSingleStep((int)(m_xiCam->GetAutoExposureAutoGainExposurePriority_Increment()*100));
    ui->sliderPriority->setValue(priority*100);
    qDebug()<<"range:"<<m_xiCam->GetAutoExposureAutoGainExposurePriority_Minimum()<<"-"
           <<m_xiCam->GetAutoExposureAutoGainExposurePriority_Minimum();
    qDebug()<<"step:"<<m_xiCam->GetAutoExposureAutoGainExposurePriority_Increment();
    qDebug()<<"priosity"<<priority;

    ui->sliderMaxExposure->setRange(m_xiCam->GetAutoExposureTopLimit_Minimum(),
                                    m_xiCam->GetAutoExposureTopLimit_Maximum());
    ui->sliderMaxExposure->setSingleStep(m_xiCam->GetAutoExposureTopLimit_Increment());
    ui->sliderMaxExposure->setValue(maxExposure);

    ui->sliderMaxGain->setRange((int)m_xiCam->GetAutoGainTopLimit_Minimum()*10,
                                (int)m_xiCam->GetAutoGainTopLimit_Maximum()*10);//0.1
    ui->sliderMaxGain->setSingleStep((int)m_xiCam->GetAutoGainTopLimit_Increment()*10);
    ui->sliderMaxGain->setValue(maxGain);

    ui->sliderTargetLevel->setRange(m_xiCam->GetAutoExposureAutoGainTargetLevel_Minimum(),
                                    m_xiCam->GetAutoExposureAutoGainTargetLevel_Maximum());
    ui->sliderTargetLevel->setSingleStep(m_xiCam->GetAutoExposureAutoGainTargetLevel_Increment());
    ui->sliderTargetLevel->setValue(targetLevel);

    connect(ui->sliderPriority,SIGNAL(valueChanged(int)),this,SLOT(onPriorityChanged(int)));
    connect(ui->sliderMaxExposure,SIGNAL(valueChanged(int)),this,SLOT(onMaxExposureChanged(int)));
    connect(ui->sliderMaxGain,SIGNAL(valueChanged(int)),this,SLOT(onMaxGainChanged(int)));
    connect(ui->sliderTargetLevel,SIGNAL(valueChanged(int)),this,SLOT(onTargetLevelChanged(int)));
}

AutoExposureSetup::~AutoExposureSetup()
{
    delete ui;
}

void AutoExposureSetup::onPriorityChanged(int val)
{
    ui->lbPriority->setText(QString::number(val) + tr("%"));
    float l = ((float)val)/100;
    qDebug()<<"priority:"<<l;
    m_xiCam->SetAutoExposureAutoGainExposurePriority(l);
}

void AutoExposureSetup::onMaxExposureChanged(int val)
{
    ui->lbMaxExposure->setText(formatExposureTimeShow(val));
    m_xiCam->SetAutoExposureTopLimit(val);
}

void AutoExposureSetup::onMaxGainChanged(int val)
{
    float value = (float)val/10;
    ui->lbMaxGain->setText(QString::number(value));
    m_xiCam->SetAutoGainTopLimit(value);
}

void AutoExposureSetup::onTargetLevelChanged(int val)
{
    ui->lbTargetLevel->setText(QString::number(val));
    m_xiCam->SetAutoExposureAutoGainTargetLevel(val);
}

QString AutoExposureSetup::formatExposureTimeShow(int val)
{
    QString strValue;
    if(val >= 1000000){
        strValue = QString::number((float)val/1000000) + tr("s");
    }else if(val >= 1000){
        strValue = QString::number((float)val/1000) + tr("ms");
    }else{
        strValue = QString::number(val) + tr("us");
    }
    return strValue;
}
