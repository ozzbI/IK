#include "addboxdialog.h"
#include "ui_addboxdialog.h"

AddBoxDialog::AddBoxDialog(GLWidget *glw, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBoxDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Добавить объект"));
    this->glw = glw;
}

AddBoxDialog::~AddBoxDialog()
{
    delete ui;
}

void AddBoxDialog::on_buttonBox_accepted()
{
    glw->add_box_scene(QVector3D(ui->x_SpinBox->value(), ui->y_SpinBox->value(), ui->z_SpinBox->value()),
                       QVector3D(ui->l_SpinBox->value(), ui->h_SpinBox->value(), ui->w_SpinBox->value()),
                       QVector3D(ui->R_SpinBox->value(), ui->G_SpinBox->value(), ui->B_SpinBox->value()));
    delete this;
}

void AddBoxDialog::on_buttonBox_rejected()
{
    delete this;
}
