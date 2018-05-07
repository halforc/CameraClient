#include "cameractrl.h"
#include <QFile>

CameraCtrl::CameraCtrl(QObject *parent) : QObject(parent)
  ,m_acqThread(nullptr)
  ,m_objThread(nullptr)
{

}

CameraCtrl::~CameraCtrl()
{    if(m_objThread) {
        if(m_acqThread) {
            m_acqThread->stop();
        }
        qDebug() << "quit";
        m_objThread->quit();
        m_objThread->wait();
    }
    if(m_xiCam.IsAcquisitionActive()){
        m_xiCam.StopAcquisition();
        m_xiCam.Close();
    }

}

bool CameraCtrl::openCamera(unsigned long devID)
{
    qDebug()<<"***open Camera";
    if(devID > m_xiCam.GetNumberOfConnectedCameras()-1){
        return false;
    }
    if(m_camStatus != camClose){
        closeCamera();
    }
    m_xiCam.OpenByID(devID);
   // m_xiCam.SetExposureTime(10000);
    m_xiCam.SetAcquisitionTimingMode(XI_ACQ_TIMING_MODE_FRAME_RATE);//设置帧率模式
    m_xiCam.SetRegion_selector(0); // default is 0
    m_xiCam.SetWidth(640);
    m_xiCam.SetHeight(512);
    m_xiCam.SetFrameRate(400.0f);
    m_xiCam.SetExposureTime(1000);
    //开启采集线程
    if(!m_objThread)
    {
        startObjThread();
    }
    m_camStatus = camOnAsquistion;
    emit startAcquistionWork();
    return true;
}

void CameraCtrl::closeCamera()
{
    qDebug()<<"***close Camera";
    if(m_objThread)
    {
        if(m_acqThread)
        {
            m_acqThread->stop();
        }
    }
    if(m_xiCam.IsAcquisitionActive()){
        m_xiCam.StopAcquisition();
        m_xiCam.Close();
    }
    m_camStatus = camClose;
}

xiAPIplusCameraOcv *CameraCtrl::getCameraHandle()
{
    return &m_xiCam;
}

void CameraCtrl::startObjThread()
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
    connect(this,SIGNAL(startAcquistionWork()),m_acqThread,SLOT(getImage()));
    connect(m_acqThread,SIGNAL(sendImage(cv::Mat&)),this,SIGNAL(getCameraImage(cv::Mat&)),Qt::DirectConnection);

    //录制
    connect(this,SIGNAL(saveImage()),m_acqThread,SLOT(saveImageToFile()),Qt::DirectConnection);
    connect(this,SIGNAL(stopSaveImage()),m_acqThread,SLOT(stopSaveImageToFile()),Qt::DirectConnection);
    connect(this,SIGNAL(setStatus(int)),m_acqThread,SLOT(getStatus(int)));
    m_objThread->start();
    qDebug()<<"ok";
}

bool CameraCtrl::readDevParaFromXML(DEVICE_SETTING *pDevInfo)
{
    return true;
}

void CameraCtrl::writeDevParaToXML(xiAPIplusCameraOcv &cam)
{

}

void CameraCtrl::initialCamera()
{
    m_nDevNumber = m_xiCam.GetNumberOfConnectedCameras();
    if(m_nDevNumber > 0){
        xiGetDeviceInfoString(0,XI_PRM_DEVICE_NAME,m_camName,256);
       // m_xiCam.GetCameraName(m_camName,256);
        emit sendCameraInfo(m_camName,m_nDevNumber);
    }
}
