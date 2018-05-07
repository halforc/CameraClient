#ifndef MAINDLG_H
#define MAINDLG_H
#include "cameractlwidget.h"
#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>

#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QComboBox>
#include <QListWidget>
#include "playercontrols.h"
#include <QPainter>
#include <QTime>
#include "widget.h"
namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MainDlg(QWidget *parent = 0);
    ~MainDlg();

signals:
    void initialCamera();

public slots:
    void showImage(QPixmap &image);

private slots:
    void test();
    void changeMode(int status);//回放和视频采集切换

    void fileList(bool checkedFlag);//文件列表
    void cameraSetting(bool checkedFlag);//相机设置界面
private:
    Ui::MainDlg *ui;
    //QLabel* picArea;
    Widget* picArea;
    QSlider *slider;
    QLabel *labelDuration;
    CameraCtlWidget* camCtrl;
    PlayerControls *controls;

    QListWidget* listFileWidget;

    int m_camStatus;

    QLabel* label;
    QGridLayout *layout;

    bool bRightWidgetShow;

};

#endif // MAINDLG_H
