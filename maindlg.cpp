#include "maindlg.h"
#include "ui_maindlg.h"
#include <QDebug>
#include <QTabWidget>
#include <QDockWidget>

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg),
    m_camStatus(CAM_CLOSE),
    bRightWidgetShow(false)
{
    //ui->setupUi(this);

    //显示区
    picArea = new QLabel(this);
    picArea->setFixedSize(640,512);
    picArea->setFrameShape(QFrame::Box);
    picArea->setAlignment(Qt::AlignCenter);
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

    camCtrl = new CameraCtlWidget(this);
    connect(controls,SIGNAL(openCamera()),camCtrl,SLOT(openCamera()));
    connect(controls,SIGNAL(closeCamera()),camCtrl,SLOT(closeCamera()));
    connect(camCtrl,SIGNAL(sendImage(QPixmap*)),this,SLOT(showImage(QPixmap*)),Qt::DirectConnection);
    //savefile
    connect(controls,SIGNAL(startRecord()),camCtrl,SIGNAL(saveImage()));
    connect(controls,SIGNAL(stopRecord()),camCtrl,SIGNAL(stopSaveImage()));

    layout->addWidget(camCtrl,0,1);
    camCtrl->hide();

    listFileWidget = new QListWidget(this);
    layout->addWidget(listFileWidget,0,2);
    listFileWidget->hide();
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

void MainDlg::showImage(QPixmap *pic)
{
  //  qDebug()<<"show";
    picArea->setPixmap(*pic);
}
