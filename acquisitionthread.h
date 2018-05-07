#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H
#include <QObject>
#include <QMutex>
#include <QThread>
#include <string>
#include "./XiAPI/xiApiPlusOcv.hpp"
#include "typedefine.h"

class AcquisitionThread : public QObject
{
       Q_OBJECT
public:
    AcquisitionThread(QObject* parent = NULL);
    AcquisitionThread(xiAPIplusCameraOcv* cam, QObject* parent = NULL);
    ~AcquisitionThread(){}

    void stop();
public slots:
    void getImage();
    void test();
    void saveImageToFile();
    void stopSaveImageToFile();
signals:
    void sendImage(cv::Mat& image);
    void message(QString str);
private:
       //QThread m_thread;
    bool imageToStreamFile(cv::Mat image, QString filename);
    bool StreamFileToImage(QString filename, cv::Mat &image);
    void writeConfigFile(QString filename);
    xiAPIplusCameraOcv* m_xiCam;
    bool mStart;
    QMutex mutex;
    int m_nCount;
    int m_status;

    QString strPath;
};


#endif // ACQUISITIONTHREAD_H
