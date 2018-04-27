#ifndef MAINDLG_H
#define MAINDLG_H
#include "cameractlwidget.h"
#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>

#define CAM_CLOSE       -1
#define ASQUISITION     0
#define RECORDING       1
#define REPLAYING       2

#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QComboBox>
#include <QListWidget>
#include "playercontrols.h"

namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MainDlg(QWidget *parent = 0);
    ~MainDlg();

public slots:
    void test();
    void changeMode(int status);

    void fileList(bool checkedFlag);
    void cameraSetting(bool checkedFlag);
    void showImage(QPixmap* pic);
private:
    Ui::MainDlg *ui;
    QLabel* picArea;
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
