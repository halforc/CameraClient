#ifndef INPUTDEBOUNCESETUP_H
#define INPUTDEBOUNCESETUP_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class InputDebounceSetup;
}

class InputDebounceSetup : public QDialog
{
    Q_OBJECT

public:
    explicit InputDebounceSetup(QWidget *parent = 0);
    ~InputDebounceSetup();

private slots:
    void on_pbClose_clicked();

private:
    Ui::InputDebounceSetup *ui;

    QPixmap m_pixmap;
};

#endif // INPUTDEBOUNCESETUP_H
