/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "playercontrols.h"


#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QDebug>

#include <QLabel>
PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , status(camAsquistionMode)
    , camStatus(camClose)
    , playButton(0)
    , stopButton(0)
    , nextButton(0)
    , previousButton(0)
    , rateBox(0)
{
    //设备控制条
    comDevice = new QComboBox(this);
    recordButton = new QPushButton(tr("record"),this);
  //  connect(recordButton,SIGNAL(clicked(bool)),this,SLOT(recordToFile()));
    //设置区
    rWightCtlButton = new QPushButton(tr("setting"),this);
    rWightCtlButton->setCheckable(true);
   // connect(rWightCtlButton,SIGNAL(clicked(bool)),this,SLOT(showCameraSetting()));
    //文件控制
    fileLabel = new QLabel("filename",this);
    fileLabel->hide();
    //播放控制
    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    stopButton = new QToolButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setEnabled(false);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    nextButton = new QToolButton(this);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    previousButton = new QToolButton(this);
    previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(previousButton, SIGNAL(clicked()), this, SIGNAL(previous())); 

    rateBox = new QComboBox(this);
    rateBox->addItem("0.5x", QVariant(0.5));
    rateBox->addItem("1.0x", QVariant(1.0));
    rateBox->addItem("2.0x", QVariant(2.0));
    rateBox->setCurrentIndex(1);
    connect(rateBox, SIGNAL(activated(int)), SLOT(updateRate()));

    //文件列表
    listButton = new QPushButton(tr("list"),this);
    listButton->setCheckable(true);

    listButton->hide();
    //功能区
    modeCahngeButton = new QPushButton(tr("Mode"),this);
    connect(modeCahngeButton,SIGNAL(clicked(bool)),this,SLOT(changeMode()));

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(comDevice);
    layout->addStretch(1);

    layout->setMargin(0);
    layout->addWidget(recordButton);
    layout->addWidget(stopButton);
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
    layout->addWidget(nextButton);
    layout->addWidget(rateBox);

    layout->addStretch(1);
    layout->addWidget(modeCahngeButton);
    layout->addWidget(rWightCtlButton);

    setLayout(layout);
    recordButton->setEnabled(false);
    updateView();
}


void PlayerControls::playClicked()
{
    qDebug()<<"playClicked";
    if(status == camAsquistionMode){
        switch(camStatus){
        case camClose:
            qDebug()<<"open camera";
            emit openCamera();
            camStatus = camOnAsquistion;
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            recordButton->setEnabled(true);
            break;
        case camOnAsquistion:
            qDebug()<<"close camera";
            emit closeCamera();
            camStatus = camClose;
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            recordButton->setEnabled(false);
        }
    }else if(status == fileReplayMode){

    }

//    switch (status) {
//    case fileReplay:
//        emit fileReplay();
//    case camClose:
//        emit camOpen();
//        break;
//    case 0:
//        emit camPauseAsquistion();
//        break;
//    case 1:

//    }
}

void PlayerControls::muteClicked()
{

}


void PlayerControls::setPlaybackRate(float rate)
{
    qDebug()<<"setPlaybackRate";
    for (int i = 0; i < rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(rateBox->itemData(i).toDouble()))) {
            rateBox->setCurrentIndex(i);
            return;
        }
    }

    rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    rateBox->setCurrentIndex(rateBox->count() - 1);
}

void PlayerControls::updateRate()
{
        qDebug()<<"updateRate";
        //    emit changeRate(playbackRate());
}

void PlayerControls::changeMode()
{
    qDebug()<<" changeMode ....."<<status;
    if(status == camAsquistionMode){
        status = fileReplayMode;
    }else if(status == fileReplayMode){
        status = camAsquistionMode;
    }
    updateView();
}

void PlayerControls::updateView()
{
    qDebug()<<"changed mode";
    if(status == fileReplayMode){               //回放
        rateBox->show();
        previousButton->setEnabled(true);
        nextButton->setEnabled(true);

        rWightCtlButton->setText(tr("list"));
        comDevice->hide();
        disconnect(rWightCtlButton,SIGNAL(clicked(bool)),this,SLOT(showCameraSetting()));
        connect(rWightCtlButton,SIGNAL(clicked(bool)),this,SLOT(showFileList()));

        recordButton->setText(tr("open"));
        disconnect(recordButton,SIGNAL(clicked(bool)),this,SLOT(recordToFile()));
        recordButton->setEnabled(true);
        connect(recordButton,SIGNAL(clicked(bool)),this,SLOT(openReplayFile()));
        layout->replaceWidget(comDevice,fileLabel);
        fileLabel->show();
    }else if(status == camAsquistionMode){          //视频采集
        rateBox->hide();
        previousButton->setEnabled(false);
        nextButton->setEnabled(false);

        rWightCtlButton->setText(tr("setting"));
        fileLabel->hide();
        disconnect(rWightCtlButton,SIGNAL(clicked(bool)),this,SLOT(showFileList()));
        connect(rWightCtlButton,SIGNAL(clicked(bool)),this,SLOT(showCameraSetting()));

        recordButton->setText(tr("record"));
        disconnect(recordButton,SIGNAL(clicked(bool)),this,SLOT(openReplayFile()));
        connect(recordButton,SIGNAL(clicked(bool)),this,SLOT(recordToFile()));
        layout->replaceWidget(fileLabel,comDevice);
        comDevice->show();
    }
    emit changeMode(status);
}

void PlayerControls::showFileList()
{
    qDebug()<<"setting check status:"<<rWightCtlButton->isChecked();
    bool flag = rWightCtlButton->isChecked();
    rWightCtlButton->setChecked(!flag);
    qDebug()<<"listbutton check status:"<<rWightCtlButton->isChecked();
    emit listFile(!flag);
}

void PlayerControls::showCameraSetting()
{
    qDebug()<<"setting check status:"<<rWightCtlButton->isChecked();
    bool flag = rWightCtlButton->isChecked();
    rWightCtlButton->setChecked(!flag);
    qDebug()<<"setting check status:"<<rWightCtlButton->isChecked();
    emit settingCamera(!flag);
}

void PlayerControls::recordToFile()
{
    qDebug()<<"record";
    if(camStatus == camRecording){
        emit stopRecord();
        camStatus = camOnAsquistion;
        recordButton->setText(tr("record"));
    }else if(camStatus == camOnAsquistion){
        emit startRecord();
        camStatus = camRecording;
        recordButton->setText(tr("stop"));
    }
}

void PlayerControls::openReplayFile()
{
    qDebug()<<"open file";
}




