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

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H
#include "typedefine.h"
#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
class QLabel;
QT_END_NAMESPACE



class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    PlayerControls(QWidget *parent = 0);

public slots:
    void setPlaybackRate(float rate);

signals:
    //camera oper
    void openCamera();
    void camStopAsquistion();
    void startRecord();
    void stopRecord();
    void closeCamera();

    //file oper
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeRate(qreal rate);

    void changeMode(int status);
    void listFile(bool checkedFlag);
    void settingCamera(bool checkedFlag);
private slots:
    void playClicked();
    void muteClicked();
    void updateRate();

    void changeMode();

    void showFileList();
    void showCameraSetting();

    void recordToFile();
    void openReplayFile();
private:
    QBoxLayout *layout;
    //设备控制
    QComboBox* comDevice;
    QPushButton* recordButton;
    QPushButton* rWightCtlButton;
    //文件播放控制
    QLabel* fileLabel;

    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    QAbstractButton *nextButton;
    QAbstractButton *previousButton;
    QComboBox *rateBox;
    QPushButton* listButton;

    QPushButton *modeCahngeButton;
    WORKSMODE status;
    CAMSTATUS camStatus;

    void updateView();
};

#endif // PLAYERCONTROLS_H
