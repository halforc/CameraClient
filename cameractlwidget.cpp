#include "cameractlwidget.h"
#include "ui_cameractlwidget.h"
#include <QFile>
#include <QDebug>

CameraCtlWidget::CameraCtlWidget(QWidget *parent) :
    QWidget(parent),
    m_wndParent(parent),
    ui(new Ui::CameraCtlWidget),
    m_camCtrl(nullptr),
    m_bIsRecording(false)
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
    initial();

    m_camCtrl = new CameraCtrl();
    connect(m_camCtrl,SIGNAL(getCameraImage(cv::Mat&)),this,SLOT(getCameraImage(cv::Mat&)),Qt::DirectConnection);
    connect(ui->comImageDPI,SIGNAL(currentIndexChanged(int)),this,SLOT(onComDPIChanged(int)));
    connect(ui->comImageType,SIGNAL(currentIndexChanged(int)),this,SLOT(onComImageTypeChanged(int)));
    connect(ui->comTriggerSelector,SIGNAL(currentIndexChanged(int)),this,SLOT(onComTriggerSelectorChanged(int)));
    connect(ui->comTriggerSource,SIGNAL(currentIndexChanged(int)),this,SLOT(onComTriggerSourceChanged(int)));
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

}

void CameraCtlWidget::closeCamera()
{
    m_camCtrl->closeCamera();

}

void CameraCtlWidget::getCameraImage(cv::Mat &image)
{
    qDebug()<<"CameraCtlWidget::getCameraImage";
    static int count = 10;
    m_curImage = QPixmap::fromImage(Mat2QImage(image));
    if(count++ >= 10){
        emit updatePic(m_curImage);
        count = 0;
    }
}

void CameraCtlWidget::onComDPIChanged(int index)
{
        qDebug()<<"onComDPIChanged"<<index;
}

void CameraCtlWidget::onComImageTypeChanged(int index)
{
    qDebug()<<"onComImageTypeChanged"<<index;
}

void CameraCtlWidget::on_pbDefine_clicked()
{
      qDebug()<<"on_pbDefine_clicked";
      emit selectROI();
}


void CameraCtlWidget::onComTriggerSelectorChanged(int index)
{
        qDebug()<<"onComTriggerSelectorChanged"<<index;
}

void CameraCtlWidget::onComTriggerSourceChanged(int index)
{
        qDebug()<<"onComTriggerSourceChanged"<<index;
}


void CameraCtlWidget::initial()
{
    QStringList list;
    list << tr("1280*1024") << tr("800*600")
         <<tr("640*512") << tr("320*256");
    ui->comImageDPI->addItems(list);
    ui->comImageDPI->setCurrentIndex(2);

    list.clear();
    list <<tr("Mono8")<<tr("Mono16")<<tr("Raw8")
         <<tr("Raw16")<<tr("TRANSPORT");
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



