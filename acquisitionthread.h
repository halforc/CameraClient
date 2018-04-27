#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H
#include <QObject>
#include <QMutex>
#include <QThread>
#include <string>
#include "./XiAPI/xiApiPlusOcv.hpp"

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
    xiAPIplusCameraOcv* m_xiCam;
    bool mStart;
    QMutex mutex;
    int m_nCount;
    bool m_bSave;
};


#endif // ACQUISITIONTHREAD_H
