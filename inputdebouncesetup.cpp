#include "inputdebouncesetup.h"
#include "ui_inputdebouncesetup.h"



InputDebounceSetup::InputDebounceSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDebounceSetup)
{
    ui->setupUi(this);
    setFixedSize(size());
    m_pixmap.load("../XiMeaCameraCtrl/images/debouncer_big.png");

    ui->lbImage->setPixmap(m_pixmap);
}

InputDebounceSetup::~InputDebounceSetup()
{
    delete ui;
}

void InputDebounceSetup::on_pbClose_clicked()
{
    this->close();
}
