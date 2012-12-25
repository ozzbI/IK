#include "redact_joint.h"
#include "ui_redact_joint.h"

redact_joint::redact_joint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::redact_joint)
{
    ui->setupUi(this);
}

void redact_joint::set_kc(kinematic_chain* nkc,int ni)
{
    i=ni;
    kc=nkc;

    setWindowTitle(tr("Редактирование звена"));

    ui->len->setValue(kc->links[i]->length);

    if(kc->links[i]->telescopic)ui->slider_box->setCheckState(Qt::Checked);
    ui->bone->setText(QString("%1").arg(i));
    ui->joint_K->setValue(kc->links[i]->joint_Frict_K);
    ui->slider_K->setValue(kc->links[i]->slider_Frict_K);
    if(kc->links[i]->type==2)
    {
        ui->ang1->setValue(kc->links[i]->lim_ang[0]/M_PI*180);
        ui->ang2->setValue(kc->links[i]->lim_ang[1]/M_PI*180);
    }
    if(kc->links[i]->type==3)
    {
        ui->ang1->setEnabled(false);
        ui->ang2->setEnabled(false);
    }
    if(kc->links[i]->type==1)
    {
        if(kc->links[i]->aa_limiter.size()>0)
        {
            ui->ang1->setValue(kc->links[i]->aa_limiter[0].angle()/M_PI*180);
        }
        ui->ang2->setEnabled(false);
    }

    ui->slider_min->setValue(kc->links[i]->min_length);
    ui->slider_max->setValue(kc->links[i]->max_length);

    if(kc->links[i]->type==2)
    {
        if(kc->links[i]->subtype==2) ui->subt_cbox->setCheckState(Qt::Checked);
        else ui->subt_cbox->setCheckState(Qt::Unchecked);
    }
    else ui->subt_cbox->setEnabled(false);


    // параметры упроугости джоинта
    ui->joint_elastic_checkbox->setChecked(kc->links[i]->joint_elastic);
    ui->joint_elastic_K->setValue(kc->links[i]->joint_elastic_K);
    ui->joint_elastic_angle->setValue(kc->links[i]->joint_elastic_angle);

    // параметры упроугости слайдера
    ui->slider_elastic_checkbox->setChecked(kc->links[i]->slider_elastic);
    ui->slider_elastic_K->setValue(kc->links[i]->slider_elastic_K);
    ui->slider_elastic_L->setValue(kc->links[i]->slider_elastic_L);
    ui->slider_elastic_L->setMaximum(kc->links[i]->max_length - kc->links[i]->min_length);

}


redact_joint::~redact_joint()
{
    delete ui;
}

void redact_joint::on_Ok_but_clicked()
{

    kc->links[i]->length=ui->len->value();
    if(ui->slider_box->checkState()==Qt::Checked)
    {
        kc->links[i]->telescopic=true;
        if(kc->links[i]->length>kc->links[i]->max_length)kc->links[i]->length=kc->links[i]->max_length;
        if(kc->links[i]->length<kc->links[i]->min_length)kc->links[i]->length=kc->links[i]->min_length;
    }
    else kc->links[i]->telescopic=false;
    if(kc->links[i]->type==1)
    {
        AngleAxisd aa(ui->ang1->value()*M_PI/180.0,Vector3d(0,0,-1));
        if(kc->links[i]->aa_limiter.size()==0)kc->links[i]->aa_limiter.push_back(aa);
        else if(kc->links[i]->aa_limiter.size()==1)
        {
            kc->links[i]->aa_limiter.clear();
            kc->links[i]->aa_limiter.push_back(aa);
        }
    }
    if(kc->links[i]->type==2)
    {
        kc->links[i]->lim_ang[0]=ui->ang1->value()*M_PI/180.0;
        kc->links[i]->lim_ang[1]=ui->ang2->value()*M_PI/180.0;
    }

    kc->links[i]->joint_Frict_K=ui->joint_K->value();
    kc->links[i]->slider_Frict_K=ui->slider_K->value();

    kc->links[i]->min_length=ui->slider_min->value();
    kc->links[i]->max_length=ui->slider_max->value();

    if((kc->links[i]->type==2)&&(ui->subt_cbox->checkState()==Qt::Checked))
    {
        kc->links[i]->subtype=2;
        kc->links[i]->global_transform=kc->links[i]->global_transform.Identity();
    }
    else
    {
        kc->links[i]->subtype=1;
        kc->links[i]->global_transform=kc->links[i]->global_transform.Identity();
    }


    // параметры упроугости джоинта
    kc->links[i]->joint_elastic = ui->joint_elastic_checkbox->isChecked();
    kc->links[i]->joint_elastic_K = ui->joint_elastic_K->value();
    kc->links[i]->joint_elastic_angle = ui->joint_elastic_angle->value();

    // параметры упроугости слайдера
    kc->links[i]->slider_elastic = ui->slider_elastic_checkbox->isChecked();
    kc->links[i]->slider_elastic_K = ui->slider_elastic_K->value();
    kc->links[i]->slider_elastic_L = ui->slider_elastic_L->value();


    if(i==0)kc->total_link_recalculation(0);
    else kc->total_link_recalculation(i-1);

    delete this;
}

void redact_joint::on_Canc_but_clicked()
{
    delete this;
}
