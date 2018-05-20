#include "maindlg.h"
#include "ui_maindlg.h"
#include <QDebug>
#include <QTabWidget>
#include <QDockWidget>

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg),
    m_camStatus(camClose),
    bRightWidgetShow(false)
{
    //ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    camCtrl = new CameraCtlWidget(this);
    //显示区
    picArea = new Widget(camCtrl->m_camCtrl,this);
    connect(picArea,SIGNAL(ROIRectChanged(QRect&,bool)),camCtrl,SIGNAL(ROIRectChanged(QRect&,bool)),Qt::DirectConnection);
    connect(camCtrl,SIGNAL(notifyWidgetAreaROI(QRect&)),picArea,SLOT(rectROIChanged(QRect&)),Qt::DirectConnection);
    //进度条
    slider = new QSlider(Qt::Horizontal, this);
    labelDuration = new QLabel("framerate",this);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(labelDuration);
    hLayout->addWidget(slider);

    controls = new PlayerControls(this);

    //布局
    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(picArea,0,0);

    layout->addLayout(hLayout,1,0);
    layout->addWidget(controls,2,0);
    connect(controls,SIGNAL(changeMode(int)),this,SLOT(changeMode(int)),Qt::DirectConnection);
    connect(controls,SIGNAL(listFile(bool)),this,SLOT(fileList(bool)));
    connect(controls,SIGNAL(settingCamera(bool)),this,SLOT(cameraSetting(bool)));

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    connect(controls,SIGNAL(openCamera(ulong)),camCtrl,SLOT(openCamera(ulong)));
    connect(controls,SIGNAL(closeCamera()),camCtrl,SLOT(closeCamera()));

    //connect(camCtrl->m_camCtrl,SIGNAL(getCameraImage(cv::Mat&)),this,SLOT(showImage(cv::Mat&)),Qt::DirectConnection);
    connect(camCtrl,SIGNAL(updatePic(QPixmap&)),this,SLOT(showImage(QPixmap&)),Qt::DirectConnection);
    //savefile
    connect(controls,SIGNAL(startRecord()),camCtrl->m_camCtrl,SIGNAL(saveImage()));
    connect(controls,SIGNAL(stopRecord()),camCtrl->m_camCtrl,SIGNAL(stopSaveImage()));

    //程序运行初始化
    connect(camCtrl->m_camCtrl,SIGNAL(sendCameraInfo(char*,ulong)),controls,SLOT(initialCameraInfo(char*,ulong)));

    //ROI
    connect(camCtrl,SIGNAL(selectROI(QRect&)),picArea,SLOT(selectROI(QRect&)));
    //connect(camCtrl->m_camCtrl,SIGNAL(rectROIChanged(QRect&,bool)),picArea,SLOT(rectROIChanged(QRect&,bool)),Qt::DirectConnection);
    //程序布局
    layout->addWidget(camCtrl,0,1);
    camCtrl->hide();

    listFileWidget = new QListWidget(this);
    layout->addWidget(listFileWidget,0,2);
    listFileWidget->hide();

    connect(this,SIGNAL(initialCamera()),camCtrl->m_camCtrl,SLOT(initialCamera()));
    emit initialCamera();
}


MainDlg::~MainDlg()
{
    delete ui;
}

void MainDlg::test()
{

}

void MainDlg::changeMode(int status)
{
    qDebug()<<"status:" << status;
    if(status == camAsquistionMode){
        listFileWidget->hide();
        camCtrl->show();
    }else if(status == fileReplayMode){
        camCtrl->hide();
        listFileWidget->show();
    }
    bRightWidgetShow = true;
}

void MainDlg::fileList(bool checkedFlag)
{
    if(!bRightWidgetShow){
        listFileWidget->show();
        bRightWidgetShow = true;
    }else{
        listFileWidget->hide();
        bRightWidgetShow = false;
    }
}

void MainDlg::cameraSetting(bool checkedFlag)
{
    if(!bRightWidgetShow){
        camCtrl->show();
        bRightWidgetShow = true;
    }else{
        camCtrl->hide();
        bRightWidgetShow = false;
    }
}

void MainDlg::showImage(QPixmap &image)
{
    picArea->setPicture(image);
    picArea->update(20,20,650,522);//部分区域刷新30，30，642*512
}


