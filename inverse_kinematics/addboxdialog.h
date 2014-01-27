#ifndef ADDBOXDIALOG_H
#define ADDBOXDIALOG_H

#include <QDialog>
#include "glwidget.h"

namespace Ui {
class AddBoxDialog;
}

class AddBoxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBoxDialog(GLWidget *scene, QWidget *parent = 0);
    ~AddBoxDialog();

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddBoxDialog *ui;
    GLWidget *glw;
};

#endif // ADDBOXDIALOG_H
