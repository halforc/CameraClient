#include "cameractrl.h"
#include <QFile>

CameraCtrl::CameraCtrl(QObject *parent) : QObject(parent)
  ,m_acqThread(nullptr)
  ,m_objThread(nullptr)
{
    m_imageFormat[0] = XI_MONO8;
    m_imageFormat[1] = XI_MONO16;
    m_imageFormat[2] = XI_RAW8;
    m_imageFormat[3] = XI_RAW16;

    connect(this,SIGNAL(rectROIChanged(QRect&,bool)),this,SLOT(recROIChanged(QRect&,bool)),Qt::DirectConnection);
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

//open camera
bool CameraCtrl::openCamera(unsigned long devID)
{
    qDebug()<<"***open Camera";
    if(devID > m_xiCam.GetNumberOfConnectedCameras()-1){
        return false;
    }
    if(m_camStatus != camClose){
        closeCamera();
    }
    m_devInfo.curDevID = devID;
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
//close camera
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

QRect CameraCtrl::getROIRect()
{

    QRect rect(0,0,1,1);
    if(m_camStatus != camClose){
        rect = QRect(QPoint(m_xiCam.GetOffsetX(),m_xiCam.GetOffsetY()),
                       QSize(m_xiCam.GetWidth(),m_xiCam.GetHeight()));
    }
    return rect;
}

int CameraCtrl::setROIOffsetX(int offsetX)
{
    if(m_camStatus != camClose){
        if(offsetX > m_xiCam.GetOffsetX_Maximum() ||
                offsetX < m_xiCam.GetOffsetY_Maximum())
            return -1;
        offsetX = (offsetX + m_xiCam.GetOffsetX_Increment()-1)/m_xiCam.GetOffsetX_Increment();
        m_xiCam.SetOffsetX(offsetX);
    }
    return offsetX;
}

int CameraCtrl::setROIOffsetY(int offsetY)
{
    if(m_camStatus != camClose){
        if(offsetY > m_xiCam.GetOffsetY_Maximum() ||
                offsetY < m_xiCam.GetOffsetY_Minimum())
            return -1;
        offsetY = (offsetY + m_xiCam.GetOffsetY_Increment()-1)/m_xiCam.GetOffsetY_Increment();
        m_xiCam.SetOffsetX(offsetY);
    }
    return offsetY;
}

int CameraCtrl::setROIWidth(int width)
{
    if(m_camStatus != camClose){
        if(width > m_xiCam.GetWidth_Maximum() ||
                width < m_xiCam.GetWidth_Minimum())
            return -1;
        width = (width + m_xiCam.GetWidth_Increment()-1)/m_xiCam.GetWidth_Increment();
        m_xiCam.SetWidth(width);
    }
    return width;
}

int CameraCtrl::setROIHeight(int height)
{
    if(m_camStatus != camClose){
        if(height > m_xiCam.GetHeight_Maximum() ||
                height < m_xiCam.GetHeight_Minimum())
            return -1;
        height = (height + m_xiCam.GetHeight_Increment()-1)/m_xiCam.GetHeight_Increment();
        m_xiCam.SetHeight(height);
    }
    return height;
}

void CameraCtrl::enableAutoExposure(bool flag)
{
    if(flag)
        m_xiCam.EnableAutoExposureAutoGain();
    else
        m_xiCam.DisableAutoExposureAutoGain();
}

bool CameraCtrl::setImageFormat(int index)
{
    m_xiCam.SetImageDataFormat(m_imageFormat[index]);//wait to complete...
    return true;
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

bool CameraCtrl::readDevParaFromXML(DEVICE_INFO *pDevInfo)
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

void CameraCtrl::recROIChanged(QRect &rect, bool modifyCam)
{
//    setROIOffsetX(rect.x());
//    setROIOffsetY(rect.y());
//    setROIWidth(rect.width());
//    setROIHeight(rect.height());
    qDebug()<<"********CamCtrl roi rect"<<rect;
}

void CameraCtrl::getCameraPara()
{
//    m_devInfo.curExposureTime = m_xiCam.GetExposureTime();
//    m_devInfo.miniExposureTime = m_xiCam.GetExposureTime_Minimum();
//    m_devInfo.maxExposureTime = m_xiCam.GetExposureTime_Maximum();
//    m_devInfo.exposureTimeIncrement=m_xiCam.GetExposureTime_Increment();

//    m_devInfo.curFrameRate = m_xiCam.GetFrameRate();
//    m_devInfo.miniFrameRate = m_xiCam.GetFrameRate_Minimum();
//    m_devInfo.maxFrameRate = m_xiCam.GetFrameRate_Maximum();
//    m_devInfo.frameRateIncreament = m_xiCam.GetFrameRate_Increment();

//    m_devInfo.ROIInfo.curOffsetX = m_xiCam.GetOffsetX();
//    m_devInfo.ROIInfo.miniOffsetX = m_xiCam.GetOffsetX_Minimum();
//    m_devInfo.ROIInfo.maxOffsetX = m_xiCam.GetOffsetX_Maximum();
//    m_devInfo.ROIInfo.offsetXIncreament = m_xiCam.GetOffsetX_Increment();

//    m_devInfo.ROIInfo.curOffsetY = m_xiCam.GetOffsetY();
//    m_devInfo.ROIInfo.miniOffsetY = m_xiCam.GetOffsetY_Minimum();
//    m_devInfo.ROIInfo.maxOffsetY = m_xiCam.GetOffsetY_Increment();
//    m_devInfo.ROIInfo.offsetYIncreament = m_xiCam.GetOffsetY_Increment();

//    m_devInfo.bIsAutoWhiteBlance = m_xiCam.IsWhiteBalanceAuto();

//    m_devInfo.ROIInfo.curWidth = m_xiCam.GetWidth();
//    m_devInfo.ROIInfo.miniWidth = m_xiCam.GetWidth_Minimum();
//    m_devInfo.ROIInfo.maxWidth = m_xiCam.GetWidth_Maximum();
//    m_devInfo.ROIInfo.widthIncrement = m_xiCam.GetWidth_Increment();

//    m_devInfo.ROIInfo.curHeight = m_xiCam.GetHeight();
//    m_devInfo.ROIInfo.miniHeight = m_xiCam.GetHeight_Minimum();
//    m_devInfo.ROIInfo.maxHeight = m_xiCam.GetHeight_Maximum();
//    m_devInfo.ROIInfo.heightIncrement = m_xiCam.GetHeight_Increment();

//    m_devInfo.imageFormate = m_xiCam.GetImageDataFormat();
//    m_devInfo.trggerSource = m_xiCam.GetTriggerSource();
//    m_devInfo.trggerSelector = m_xiCam.GetTriggerSelector();
}
