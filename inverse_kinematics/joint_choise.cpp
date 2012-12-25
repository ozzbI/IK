#include "joint_choise.h"
#include "ui_joint_choise.h"

joint_choise::joint_choise(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::joint_choise)
{
    ui->setupUi(this);
    setWindowTitle(tr("Выберите тип шарнира"));
    //setModal(true);
}
void joint_choise::set_kc(kinematic_chain* nkc,int ni)
{
    i=ni;
    kc=nkc;
}

joint_choise::~joint_choise()
{
    delete ui;
}

void joint_choise::on_ball_clicked()
{
    kc->add_link_to_middle(1,Vector3d(0,0,1),2,i,0,0,0);
    set_max_link();
    delete this;
}

void joint_choise::on_hinge1_clicked()
{
    kc->add_link_to_middle(2,Vector3d(0,0,1),2,i,0,0,0);
    set_max_link();
    delete this;
}

void joint_choise::on_hinge2_clicked()
{
    kc->add_link_to_middle(3,Vector3d(0,0,1),2,i,0,0,0);
    set_max_link();
    delete this;
}
