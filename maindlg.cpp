#include "maindlg.h"
#include "ui_maindlg.h"
#include <QDebug>
#include <QTabWidget>
#include <QDockWidget>

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg),
    m_camStatus(CAM_CLOSE)
{
    //ui->setupUi(this);

    //显示区
    picArea = new QLabel(this);
    picArea->setFixedSize(640,512);
    picArea->setFrameShape(QFrame::Box);

    controls = new PlayerControls(this);

    //进度条
    slider = new QSlider(Qt::Horizontal, this);
    labelDuration = new QLabel("framerate",this);
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(labelDuration);
    hLayout->addWidget(slider);

    layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->addWidget(picArea,0,0);

    layout->addLayout(hLayout,1,0);
    layout->addWidget(controls,2,0);
    connect(controls,SIGNAL(changeMode()),this,SLOT(changeMode()),Qt::DirectConnection);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    camCtrl = new CameraCtlWidget(this);
    layout->addWidget(camCtrl,0,1);
    camCtrl->hide();
}


MainDlg::~MainDlg()
{
    delete ui;
}

void MainDlg::test()
{

}

void MainDlg::changeMode()
{
    qDebug()<<"test";
    static bool flag = true;
    if(flag){
        camCtrl->show();
        flag = false;
    }else{
        camCtrl->hide();
        flag = true;
    }
}
