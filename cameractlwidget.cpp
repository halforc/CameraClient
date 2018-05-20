#include "cameractlwidget.h"
#include "ui_cameractlwidget.h"
#include <QFile>
#include <QDebug>
#include "inputdebouncesetup.h"

CameraCtlWidget::CameraCtlWidget(QWidget *parent) :
    QWidget(parent),
    m_wndParent(parent),
    ui(new Ui::CameraCtlWidget),
    m_camCtrl(nullptr),
    m_bIsRecording(false),
    m_bIsAutoExposure(false),
    m_wROIDefine(nullptr),
    m_wAutoExposureDlg(nullptr)
{
//    //设置界面主题
//    QFile file("..//XiMeaCameraCtrl//style//newstyle.qss");
//    if(!file.open(QFile::ReadOnly)){
//        qDebug()<<"open failed";
//    }
//    QString styleSheet = QLatin1String(file.readAll());
//    qApp->setStyleSheet(styleSheet);
//    file.close();

    //固定窗口大小
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->pbROI->setCheckable(true);
    ui->pbROI->setChecked(false);

    myTimer = new QTimer(this);
    tcpSocket=new QTcpSocket(this);  //TCP
    connect(myTimer,SIGNAL(timeout()),this,SLOT(SendData()));

    QStringList list;
    list << tr("1280*1024")<<tr("640*512");
    ui->comImageDPI->addItems(list);
    ui->comImageDPI->setCurrentIndex(0);

    list.clear();
    list <<tr("Mono8")<<tr("Mono16")<<tr("Raw8")
         <<tr("Raw16");
    ui->comImageType->addItems(list);
    ui->comImageType->setCurrentIndex(1);

    list.clear();
    list<<tr("off")<<tr("Rising")
       <<tr("Falling edge")<<tr("Software");
    ui->comTriggerSource->addItems(list);

    list.clear();
    list<<tr("Frame start")<<tr("Exposure active")
       <<tr("Frame burst start")<<tr("Frame burst active")
      <<tr("Multitle exposure");
    ui->comTriggerSelector->addItems(list);

    list.clear();
    list<<tr("off")<<tr("Trigger");
    ui->comGPI->addItems(list);

    list.clear();
    list<<tr("Off")<<tr("On")<<tr("Frame active")<<tr("Frame active(neg.)")
       <<tr("Exposure active")<<tr("Exposure active(neg.)")
      <<tr("Frame trigger wait")<<tr("Frame trigger wait(neg.)")
     <<tr("Exposure pulse")<<tr("Exposure pulse(neg.)");
    ui->comGPO->addItems(list);

    list.clear();


    ui->pbDebounceSetup->setEnabled(false);
    ui->pbSWPing->setEnabled(false);


    m_camCtrl = new CameraCtrl();
    connect(m_camCtrl,SIGNAL(getCameraImage(cv::Mat&)),this,SLOT(getCameraImage(cv::Mat&)),Qt::DirectConnection);
    connect(ui->comImageDPI,SIGNAL(currentIndexChanged(int)),this,SLOT(onComDPIChanged(int)));
    connect(ui->comImageType,SIGNAL(currentIndexChanged(int)),this,SLOT(onComImageTypeChanged(int)));
    connect(ui->comTriggerSelector,SIGNAL(currentIndexChanged(int)),this,SLOT(onComTriggerSelectorChanged(int)));
    connect(ui->comTriggerSource,SIGNAL(currentIndexChanged(int)),this,SLOT(onComTriggerSourceChanged(int)));
    connect(ui->checkAutoExposure,SIGNAL(clicked(bool)),this,SLOT(onEnableAutoExposure(bool)));
    connect(ui->checkDebounce,SIGNAL(clicked(bool)),this,SLOT(onEnableDebounce(bool)));
}

CameraCtlWidget::~CameraCtlWidget()
{
    qDebug() << "start destroy widget";

    if(m_camCtrl != nullptr){
        delete m_camCtrl;
        m_camCtrl = nullptr;
    }
    qDebug() << "end destroy widget";
    delete ui;
}



void CameraCtlWidget::on_test_clicked()
{
    m_wndParent->close();
}

void CameraCtlWidget::openCamera(unsigned long devID)
{
    m_camCtrl->openCamera(devID);
    initial();
    myTimer->start(40);
}

void CameraCtlWidget::closeCamera()
{
    m_camCtrl->closeCamera();
    myTimer->stop();
}

void CameraCtlWidget::rectROIChanged(QRect &rect, bool flag)
{
//    m_camCtrl->setROIOffsetX(rect.x());
//    m_camCtrl->setROIOffsetY(rect.y());
//    m_camCtrl->setROIWidth(rect.width());
//    m_camCtrl->setROIHeight(rect.height());
    if(m_wROIDefine){
        m_wROIDefine->changedROI(rect);
    }
    qDebug()<<"CameraCtlWidget"<<rect;
}

void CameraCtlWidget::getCameraImage(cv::Mat &image)
{
    myImage = Mat2QImage(image);
    m_curImage = QPixmap::fromImage(myImage);
}

void CameraCtlWidget::SendData()
{

    blockSize=0;
    tcpSocket->abort();
    tcpSocket->connectToHost("192.168.1.101",8888);


    QByteArray byte;
    QBuffer buf(&byte);

    myImage.save(&buf,"PNG");
    QByteArray ss=qCompress(byte,1);
    QByteArray vv=ss.toBase64();

    QByteArray ba;
    QDataStream out(&ba,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);

    out<<(quint64)0;
    out<<vv;
    out.device()->seek(0);
    out<<(quint64)(ba.size()-sizeof(quint64));
    tcpSocket->write(ba);
    emit updatePic(m_curImage);
}

void CameraCtlWidget::onComDPIChanged(int index)
{
    if(camClose == m_camCtrl->getCameraStatus())
        return;
    switch (index) {
    case 0:
        m_camCtrl->getCameraHandle()->SetDownsampling(XI_DWN_1x1);

        break;
    case 1:
        m_camCtrl->getCameraHandle()->SetDownsampling(XI_DWN_2x2);
        break;
    default:
        break;
    }
        updateFrameRateInfo();
}

void CameraCtlWidget::onComImageTypeChanged(int index)
{
    if(camClose == m_camCtrl->getCameraStatus())
        return;
    qDebug()<<"onComImageTypeChanged"<<index;
    m_camCtrl->setImageFormat(index);
}

void CameraCtlWidget::on_pbDefine_clicked()
{
      qDebug()<<"on_pbDefine_clicked";
      if(nullptr != m_wROIDefine){
          m_wROIDefine->close();
          delete m_wROIDefine;
          m_wROIDefine = nullptr;
      }else{
          m_wROIDefine = new ROIDefine(m_camCtrl,this);
          connect(this,SIGNAL(ROIRectChanged(QRect&,bool)),this,SLOT(rectROIChanged(QRect&,bool)),Qt::DirectConnection);
          connect(m_wROIDefine,SIGNAL(ROIRectChanged(QRect&)),this,SIGNAL(notifyWidgetAreaROI(QRect&)),Qt::DirectConnection);
          connect(m_wROIDefine,SIGNAL(notifyParent(QRect,bool)),this,SLOT(onReceiveDefine(QRect,bool)));
          m_wROIDefine->setModal(false);
          m_wROIDefine->show();
      }
      emit selectROI(/*m_camCtrl->getROIRect()*/QRect(0,0,320,256));
}

void CameraCtlWidget::on_pbROI_clicked()
{
    static bool flag = false;
    flag = !flag;
    ui->pbROI->setChecked(flag);
    if(flag){
        if(ui->comImageDPI->currentIndex() == 0){
            m_camCtrl->getCameraHandle()->SetOffsetX(0);
            m_camCtrl->getCameraHandle()->SetOffsetY(0);
            m_camCtrl->getCameraHandle()->SetWidth(1280);
            m_camCtrl->getCameraHandle()->SetHeight(1024);
        }
        if(ui->comImageDPI->currentIndex() == 1){
            m_camCtrl->getCameraHandle()->SetOffsetX(0);
            m_camCtrl->getCameraHandle()->SetOffsetY(0);
            m_camCtrl->getCameraHandle()->SetWidth(640);
            m_camCtrl->getCameraHandle()->SetHeight(512);
        }
        m_camCtrl->getCameraHandle()->SetRegion_selector(0);
    }else{
        m_camCtrl->getCameraHandle()->SetOffsetX(rectROI.x());
        m_camCtrl->getCameraHandle()->SetOffsetY(rectROI.y());
        m_camCtrl->getCameraHandle()->SetWidth(rectROI.width());
        m_camCtrl->getCameraHandle()->SetHeight(rectROI.height());
    }
}

void CameraCtlWidget::on_pbSWPing_clicked()
{
    m_camCtrl->getCameraHandle()->SetTriggerSoftware(1);
    myImage = Mat2QImage(m_camCtrl->getCameraHandle()->GetNextImageOcvMat());
    m_curImage = QPixmap::fromImage(myImage);
}

void CameraCtlWidget::onReceiveDefine(QRect rect, bool bSave)
{
    m_wROIDefine->close();
    delete m_wROIDefine;
    m_wROIDefine = nullptr;
    if(bSave){
        rectROI = rect;
    }
    emit selectROI(rectROI);
}

void CameraCtlWidget::on_pbAutoExposureSetup_clicked()
{
    m_wAutoExposureDlg = new AutoExposureSetup(m_camCtrl,this);
    m_wAutoExposureDlg->setModal(false);
    m_wAutoExposureDlg->show();
}

void CameraCtlWidget::on_pbDebounceSetup_clicked()
{
    m_wInputDebounceSetup = new InputDebounceSetup(m_camCtrl, this);
    m_wInputDebounceSetup->setModal(true);
    m_wInputDebounceSetup->show();
}


void CameraCtlWidget::onComTriggerSelectorChanged(int index)
{
        qDebug()<<"onComTriggerSelectorChanged"<<index;

}

void CameraCtlWidget::onComTriggerSourceChanged(int index)
{
    qDebug()<<"onComTriggerSourceChanged"<<index;
    m_camCtrl->stopAcquistion();
    if(index == 3){
        ui->pbSWPing->setEnabled(true);
    }else{
        ui->pbSWPing->setEnabled(false);
    }
    m_camCtrl->setTriggetSource(index);
    m_camCtrl->getCameraHandle()->StartAcquisition();
}

void CameraCtlWidget::onEnableAutoExposure(bool flag)
{
    qDebug()<<flag;
    ui->sliderExposure->setEnabled(!flag);
    ui->sliderGain->setEnabled(!flag);
    m_camCtrl->enableAutoExposure(flag);
    m_bIsAutoExposure = flag;
}

void CameraCtlWidget::onEnableDebounce(bool flag)
{
    ui->pbDebounceSetup->setEnabled(flag);
    if(flag){
        m_camCtrl->getCameraHandle()->EnableGPIDebounce();
    }else{
        m_camCtrl->getCameraHandle()->DisableGPIDebounce();
    }
}

void CameraCtlWidget::on_leExposure_textChanged(QString str)
{
        qDebug()<<"on_leExposure_textChanged"<<str;
}

void CameraCtlWidget::on_leGain_textChanged(QString str)
{
        qDebug()<<"onComTriggerSourceChanged"<<str;
}


void CameraCtlWidget::on_sliderExposure_valueChanged(int val)
{
        qDebug()<<"onComTriggerSourceChanged"<<val;
        ui->leExposure->setText(QString::number(val));
        m_camCtrl->getCameraHandle()->SetExposureTime(val);

        updateFrameRateInfo();
}

void CameraCtlWidget::on_sliderGain_valueChanged(int val)
{
        qDebug()<<"onComTriggerSourceChanged"<<val;
        ui->leGain->setText(QString::number(val));
        m_camCtrl->getCameraHandle()->SetGain((float)val/10);

        updateFrameRateInfo();
}

void CameraCtlWidget::on_pbApply_clicked()
{
    int frameRate = ui->leFrameRate->text().toInt();
    qDebug()<<"frame:"<<frameRate;
    if(frameRate > m_camCtrl->getCameraHandle()->GetFrameRate_Maximum()){
        frameRate = m_camCtrl->getCameraHandle()->GetFrameRate_Maximum();
    }else if(frameRate < m_camCtrl->getCameraHandle()->GetFrameRate_Minimum()){
        frameRate = m_camCtrl->getCameraHandle()->GetFrameRate_Minimum() + 1;
    }
    ui->leFrameRate->setText(QString::number(frameRate));
    m_camCtrl->getCameraHandle()->SetFrameRate(ui->leFrameRate->text().toFloat());
}

void CameraCtlWidget::initial()
{
    xiAPIplusCameraOcv* cam = m_camCtrl->getCameraHandle();

    ui->leExposure->setText(QString::number(cam->GetExposureTime()));
    ui->sliderExposure->setRange(cam->GetExposureTime_Minimum(),10000/*cam->GetExposureTime_Maximum()*/);
    ui->sliderExposure->setSingleStep(100);

    ui->leGain->setText(QString::number(cam->GetGain()));
    ui->sliderGain->setRange(cam->GetGain_Minimum()*10,cam->GetGain_Maximum()*10);
    ui->sliderGain->setSingleStep(cam->GetGain_Increment()*10);

    updateFrameRateInfo();

}

void CameraCtlWidget::updateFrameRateInfo()
{
    QString str = QString("mini:%1   max:%2").arg((int)m_camCtrl->getCameraHandle()->GetFrameRate_Minimum()+1).arg(
                (int)m_camCtrl->getCameraHandle()->GetFrameRate_Maximum());
    ui->lbFrame->setText(str);
    float frame = m_camCtrl->getCameraHandle()->GetFrameRate();
    ui->leFrameRate->setText(QString::number(frame));
}

QImage CameraCtlWidget::Mat2QImage(cv::Mat& cvImg)
{
    QImage qImg;
    if(cvImg.channels() == 3){
        cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg = QImage((const uchar*)(cvImg.data),
                      cvImg.cols,cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_RGB888);
    }
    else if(cvImg.channels() == 1){
        qImg = QImage((const uchar*)(cvImg.data),
                      cvImg.cols,cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_Indexed8);
    }
    else{
        qImg = QImage((const uchar*)(cvImg.data),
                      cvImg.cols,cvImg.rows,
                      cvImg.cols * cvImg.channels(),
                      QImage::Format_RGB888);
    }
    return qImg;
}




