#include "cameractlwidget.h"
#include "ui_cameractlwidget.h"
#include <QFile>

CameraCtlWidget::CameraCtlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraCtlWidget),
    m_acqThread(nullptr),
    m_objThread(nullptr),
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
}

CameraCtlWidget::~CameraCtlWidget()
{
    qDebug() << "start destroy widget";
    if(m_objThread)
    {
        qDebug() << "quit";
        m_objThread->quit();
        m_objThread->wait();
    }
    qDebug() << "end destroy widget";
    delete ui;
}

void CameraCtlWidget::startObjThread()
{
    if(m_objThread)
    {
        return;
    }
    m_objThread= new QThread();
    m_acqThread = new AcquisitionThread(&m_xiCam);
    m_acqThread->moveToThread(m_objThread);
    connect(m_objThread,&QThread::finished,m_objThread,&QObject::deleteLater);
    connect(m_objThread,&QThread::finished,m_acqThread,&QObject::deleteLater);
    connect(this,SIGNAL(startObjThreadWork1()),m_acqThread,SLOT(getImage()));
    connect(m_acqThread,SIGNAL(sendImage(cv::Mat&)),this,SLOT(showImage(cv::Mat&)),Qt::DirectConnection);

    //录制
    connect(this,SIGNAL(saveImage()),m_acqThread,SLOT(saveImageToFile()),Qt::DirectConnection);
    connect(this,SIGNAL(stopSaveImage()),m_acqThread,SLOT(stopSaveImageToFile()),Qt::DirectConnection);
    m_objThread->start();
    qDebug()<<"ok";
}

void CameraCtlWidget::on_test_clicked()
{
    m_xiCam.OpenFirst();
    m_xiCam.SetExposureTime(10000);
    m_xiCam.SetAcquisitionTimingMode(XI_ACQ_TIMING_MODE_FRAME_RATE);//设置帧率模式
    m_xiCam.SetWidth(800);
    m_xiCam.SetHeight(600);
    m_xiCam.SetFrameRate(60.0f);

    //开启采集线程
    if(!m_objThread)
    {
        startObjThread();
    }
    emit startObjThreadWork1();
}

void CameraCtlWidget::showImage(cv::Mat &image)
{
    m_curImage = image;
}

QImage CameraCtlWidget::Mat2QImage(cv::Mat cvImg)
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
