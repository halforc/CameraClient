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

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , playButton(0)
    , stopButton(0)
    , nextButton(0)
    , previousButton(0)
    , rateBox(0)
{
    //控制条
    openButton = new QPushButton(tr("Open"), this);
    comDevice = new QComboBox(this);
    recordButton = new QPushButton(tr("record"));


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

    modeCahngeButton = new QPushButton(tr("Mode"),this);
    connect(modeCahngeButton,SIGNAL(clicked(bool)),this,SLOT(modeClicked()));

    fullScreenButton = new QPushButton(tr("FullScreen"), this);
    fullScreenButton->setCheckable(true);

    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(openButton);
    layout->setMargin(1);
    layout->addWidget(comDevice);
    layout->setMargin(1);
    layout->addWidget(recordButton);
    layout->addStretch(1);

    layout->setMargin(0);
    layout->addWidget(stopButton);
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
    layout->addWidget(nextButton);
    layout->addWidget(rateBox);

    layout->addStretch(1);
    layout->addWidget(modeCahngeButton);
    layout->addWidget(fullScreenButton);

    setLayout(layout);
}


void PlayerControls::playClicked()
{
    qDebug()<<"playClicked";
    emit play();
//    switch (playerState) {
//    case QMediaPlayer::StoppedState:
//    case QMediaPlayer::PausedState:
//        emit play();
//        break;
//    case QMediaPlayer::PlayingState:
//        emit pause();
//        break;
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

void PlayerControls::modeClicked()
{
    qDebug()<<"changed mode";
    emit changeMode();
}


