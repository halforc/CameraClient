#include "acquisitionthread.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <fstream>
#include <QDateTime>
#include <QDir>
AcquisitionThread::AcquisitionThread(QObject *parent)
    : QObject(parent)
{

}

AcquisitionThread::AcquisitionThread(xiAPIplusCameraOcv* cam, QObject *parent)
    : QObject(parent),
      m_xiCam(cam),
      m_nCount(0),
      m_bSave(false)
{
     // m_thread.start();
    //  this->moveToThread(&m_thread);
}

void AcquisitionThread::stop()
{
    QMutexLocker locker(&mutex);
    mStart = false;
    m_xiCam->StopAcquisition();
}

void AcquisitionThread::getImage()
{
    mStart = true;
    cv::Mat cv_mat_image;
    QString strName;
    XI_IMG_FORMAT format;
    QString strPath;
    {
        QMutexLocker locker(&mutex);
        m_xiCam->StartAcquisition();
    }
    while(true){
        {
            QMutexLocker locker(&mutex);
            if(!mStart)
                return;

            format = m_xiCam->GetImageDataFormat();
            cv_mat_image = m_xiCam->GetNextImageOcvMat();
        }
        if(m_bSave){
            if(m_nCount == 0){
                QDateTime local(QDateTime::currentDateTime());
                QString localTime = local.toString("yyyyMMddhhmmss");
                qDebug()<<localTime;
                QDir dir;
                if(!dir.exists(localTime))//判断需要创建的文件夹是否存在
                {
                    qDebug()<<"Create File Dir";
                    dir.mkdir(localTime); //创建文件夹
                }
                strPath = ".//"+localTime + "//";
            }
            strName = strPath + "image "+ QString::number(m_nCount)+".dat";
            imageToStreamFile(cv_mat_image,strName);
            qDebug()<<"save file "<<strName;
            m_nCount++;
        }else{
            m_nCount = 0;
        }
            if (format == XI_RAW16 || format == XI_MONO16)
                cv::normalize(cv_mat_image, cv_mat_image, 0, 65536, cv::NORM_MINMAX, -1, cv::Mat()); // 0 - 65536, 16 bit unsigned integer range
            emit sendImage(cv_mat_image);
      }
}

void AcquisitionThread::test()
{
    qDebug()<<"test in asq thread";
    message("send from acq thread");
}

void AcquisitionThread::saveImageToFile()
{
    qDebug()<<"save";
    m_bSave = true;
}

void AcquisitionThread::stopSaveImageToFile()
{
    qDebug()<<"stop";
    m_bSave = false;
}

bool AcquisitionThread::imageToStreamFile(cv::Mat image, QString filename)
{
    if (image.empty()) return false;
    QByteArray cpath = filename.toLocal8Bit();
    const char *filenamechar = cpath.data();
    FILE *fpw = fopen(filenamechar, "wb");//如果没有则创建，如果存在则从头开始写
    if (fpw == NULL)
    {
        fclose(fpw);
        return false;
    }
    int channl = image.channels();//第一个字节  通道
    int rows = image.rows;     //四个字节存 行数
    int cols = image.cols;   //四个字节存 列数

    fwrite(&channl, sizeof(char), 1, fpw);
    fwrite(&rows, sizeof(char), 4, fpw);
    fwrite(&cols, sizeof(char), 4, fpw);
    char* dp = (char*)image.data;
    if (channl == 3)
    {
        for (int i = 0; i < rows*cols; i++)
        {
            fwrite(&dp[i * 3], sizeof(char), 1, fpw);
            fwrite(&dp[i * 3 + 1], sizeof(char), 1, fpw);
            fwrite(&dp[i * 3 + 2], sizeof(char), 1, fpw);
        }
    }
    else if (channl == 1)
    {
        for (int i = 0; i < rows*cols; i++)
        {
            fwrite(&dp[i], sizeof(char), 1, fpw);
        }
    }
    fclose(fpw);
    return true;
}

bool AcquisitionThread::StreamFileToImage(QString filename, cv::Mat &image)
{
    QByteArray cpath = filename.toLocal8Bit();
    const char *filenamechar = cpath.data();
    FILE *fpr = fopen(filenamechar, "rb");
    if (fpr == NULL)
    {
        fclose(fpr);
        return false;
    }
    int channl(0);
    int imagerows(0);
    int imagecols(0);
    fread(&channl, sizeof(char), 1, fpr);//第一个字节 通道
    fread(&imagerows, sizeof(char), 4, fpr); //四个字节存 行数
    fread(&imagecols, sizeof(char), 4, fpr); //四个字节存 列数
    if (channl == 3)
    {
        image = cv::Mat::zeros(imagerows,imagecols, CV_8UC3);
        char* pData = (char*)image.data;
        for (int i = 0; i < imagerows*imagecols; i++)
        {
            fread(&pData[i * 3], sizeof(char), 1, fpr);
            fread(&pData[i * 3 + 1], sizeof(char), 1, fpr);
            fread(&pData[i * 3 + 2], sizeof(char), 1, fpr);
        }
    }
    else if (channl == 1)
    {
        image = cv::Mat::zeros(imagerows, imagecols, CV_8UC1);
        char* pData = (char*)image.data;
        for (int i = 0; i < imagerows*imagecols; i++)
        {
            fread(&pData[i], sizeof(char), 1, fpr);
        }
    }
    fclose(fpr);
    return true;
}
