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
    list << tr("1280*1024") << tr("800*600")
         <<tr("640*512") << tr("320*256");
    ui->comImageDPI->addItems(list);
    ui->comImageDPI->setCurrentIndex(2);

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
    list<<tr("ms")<<tr("s")<<tr("min")<<tr("h");
    ui->comInterval->addItems(list);
    list.clear();

    ui->pbDebounceSetup->setEnabled(false);
    ui->pbSWPing->setEnabled(false);
    ui->tbSoftTriggerCtrl->setEnabled(false);

    m_camCtrl = new CameraCtrl();
    connect(m_camCtrl,SIGNAL(getCameraImage(cv::Mat&)),this,SLOT(getCameraImage(cv::Mat&)),Qt::DirectConnection);
    connect(ui->comImageDPI,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComDPIChanged(QString)));
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
    //m_wndParent->close();
        myTimer->start(30);
}

void CameraCtlWidget::openCamera(unsigned long devID)
{
    m_camCtrl->openCamera(devID);
    initial();

}

void CameraCtlWidget::closeCamera()
{
    m_camCtrl->closeCamera();
    myTimer->stop();
}

void CameraCtlWidget::ROIRectChanged(QRect &rect)
{
    m_camCtrl->setROIOffsetX(rect.x());
    m_camCtrl->setROIOffsetY(rect.y());
    m_camCtrl->setROIWidth(rect.width());
    m_camCtrl->setROIHeight(rect.height());
}

void CameraCtlWidget::getCameraImage(cv::Mat &image)
{
   // qDebug()<<"CameraCtlWidget::getCameraImage";
    static int count = 10;
    myImage = Mat2QImage(image);
    m_curImage = QPixmap::fromImage(myImage);
    if(count++ >= 10){
        emit updatePic(m_curImage);
        count = 0;
    }
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
}

void CameraCtlWidget::onComDPIChanged(QString str)
{
    qDebug()<<"onComDPIChanged"<<str;
    QStringList list = str.split('*');
    qDebug()<<((QString)list[0]).toInt()<<((QString)list[1]).toInt();
    int width = ((QString)list[0]).toInt();
    int height = ((QString)list[1]).toInt();
    m_camCtrl->setROIWidth(width);
    m_camCtrl->setROIHeight(height);
    m_camCtrl->setROIOffsetX(640-width/2);
    m_camCtrl->setROIOffsetY(512-height/2);
}

void CameraCtlWidget::onComImageTypeChanged(int index)
{
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
          connect(m_camCtrl,SIGNAL(rectROIChanged(QRect&,bool)),m_wROIDefine,SLOT(rectROIChanged(QRect&,bool)),Qt::DirectConnection);
          m_wROIDefine->setModal(false);
          m_wROIDefine->show();
      }
}

void CameraCtlWidget::on_pbROI_clicked()
{
    static bool flag = false;
    flag = !flag;
    ui->pbROI->setChecked(flag);
    emit selectROI(/*m_camCtrl->getROIRect()*/QRect(0,0,320,256));
}

void CameraCtlWidget::on_pbAutoExposureSetup_clicked()
{
    m_wAutoExposureDlg = new AutoExposureSetup(m_camCtrl,this);
    m_wAutoExposureDlg->setModal(false);
    m_wAutoExposureDlg->show();
}

void CameraCtlWidget::on_pbDebounceSetup_clicked()
{
    m_wInputDebounceSetup = new InputDebounceSetup(this);
    m_wInputDebounceSetup->setModal(false);
    m_wInputDebounceSetup->show();
}


void CameraCtlWidget::onComTriggerSelectorChanged(int index)
{
        qDebug()<<"onComTriggerSelectorChanged"<<index;
}

void CameraCtlWidget::onComTriggerSourceChanged(int index)
{
    qDebug()<<"onComTriggerSourceChanged"<<index;
    if(index == 3){
        ui->tbSoftTriggerCtrl->setEnabled(true);
        ui->pbSWPing->setEnabled(true);
    }else{
        ui->tbSoftTriggerCtrl->setEnabled(false);
        ui->pbSWPing->setEnabled(false);
    }
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

        QString str = QString("max:%1   mini:%2").arg(m_camCtrl->getCameraHandle()->GetFrameRate_Minimum()).arg(
                    m_camCtrl->getCameraHandle()->GetFrameRate_Maximum());
        ui->lbFrame->setText(str);
        float frame = m_camCtrl->getCameraHandle()->GetFrameRate();
        ui->leFrameRate->setText(QString::number(frame));
}

void CameraCtlWidget::on_sliderGain_valueChanged(int val)
{
        qDebug()<<"onComTriggerSourceChanged"<<val;
        ui->leGain->setText(QString::number(val));
        m_camCtrl->getCameraHandle()->SetGain((float)val/10);

        QString str = QString("max:%1   mini:%2").arg(m_camCtrl->getCameraHandle()->GetFrameRate_Minimum()).arg(
                    m_camCtrl->getCameraHandle()->GetFrameRate_Maximum());
        ui->lbFrame->setText(str);
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

    ui->leFrameRate->setText(QString::number(cam->GetFrameRate()));
    QString str = QString("max:%1   mini:%2").arg(cam->GetFrameRate_Minimum()).arg(
                cam->GetFrameRate_Maximum());
    ui->lbFrame->setText(str);

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



