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
    void changeMode();

private:
    Ui::MainDlg *ui;
    QLabel* picArea;
    QSlider *slider;
    QLabel *labelDuration;
    CameraCtlWidget* camCtrl;
    PlayerControls *controls;

    int m_camStatus;

    QLabel* label;
    QGridLayout *layout;
};

#endif // MAINDLG_H
