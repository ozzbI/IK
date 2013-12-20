#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glw=new GLWidget();
    ui->screens_layout->addWidget(glw);

    setWindowTitle("IK");
    move_sens=3;
    start_param_validator=new QDoubleValidator();



   //���������
    ctrl_pressed=false;
    ui->object_cam_but->setDisabled(true);
    //ui->free_cam_but->hide();
    //ui->object_cam_but->hide();
    //ui->vel_slider->hide();
    //ui->label->hide();
    glw->cam.enable_scene_cam(true);
    glw->ctrl_pressed=false;

   //connect(glw,SIGNAL(set_max_links(int ml)),this,SLOT(max_links(int ml)));
    ui->link->setMaximum(glw->KChain.get_size()-1);

    ui->Target_Activated_checkbox->setChecked(true);
    ui->elastic_joints_checkbox->setChecked(true);

    connect(this,SIGNAL(rotate_link(int,double,double,double)),glw,SLOT(rotate_KChain_link(int,double,double,double)));
    connect(this,SIGNAL(set_link_rotation(int,double,double,double)),glw,SLOT(set_KChain_link_angles(int,double,double,double)));

    //menu
    this->createActions();
    this->createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::max_links(int ml)
{

    ui->link->setMaximum(ml);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //set_text("press");
    switch (event->key()) {
    //cam walking
      case  Qt::Key_W:
        glw->cam.walk(0.2*move_sens);
          break;
      case  Qt::Key_S:
        glw->cam.walk(-0.2*move_sens);
          break;
      case Qt::Key_A:
        glw->cam.strafe(-0.2*move_sens);
          break;
      case  Qt::Key_D:
      glw->cam.strafe(0.2*move_sens);
          break;
      case Qt::Key_Z:
      glw->cam.fly(0.2*move_sens);
          break;
      case  Qt::Key_X:
      glw->cam.fly(-0.2*move_sens);
          break;
      //cam turning
   /* case Qt::Key_8:
      glw->cam.pitch(3.14/90);
        break;
    case  Qt::Key_5:
    glw->cam.pitch(-3.14/90);
        break;
    case Qt::Key_6:
    glw->cam.yaw(-3.14/90);
        break;
    case  Qt::Key_4:
    glw->cam.yaw(3.14/90);
        break;*/
    //target turning
    case Qt::Key_1:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(0,0.1,0);
    break;
    case Qt::Key_2:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(0,-0.1,0);
    break;
    case Qt::Key_4:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(-0.1,0,0);
    break;
    case Qt::Key_6:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(0.1,0,0);
    break;
    case Qt::Key_8:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(0,0,-0.1);
    break;
    case Qt::Key_5:
        glw->KChain.effectors[glw->active_effector].target+=Vector3d(0,0,0.1);
    break;


    //light moving
    case  Qt::Key_I:
        glw->light_pos+=QVector4D(0,0,(float)-0.1,0);
    glw->set_lp();
        break;
    case Qt::Key_K:
        glw->light_pos+=QVector4D(0,0,(float)0.1,0);
    glw->set_lp();
        break;
    case  Qt::Key_J:
        glw->light_pos+=QVector4D(-0.1,0,0,0);
    glw->set_lp();
        break;
    case  Qt::Key_L:
        glw->light_pos+=QVector4D(0.1,0,0,0);
    glw->set_lp();;
        break;
   case  Qt::Key_N:
        glw->light_pos+=QVector4D(0,1,0,0);
    glw->set_lp();
        break;
   case  Qt::Key_M:
        glw->light_pos+=QVector4D(0,-1,0,0);
   glw->set_lp();
        break;
   case  Qt::Key_Control:
        //qDebug("cntrl_pressed");
        //ctrl_pressed=true;
        glw->ctrl_pressed=true;
        break;
    }

}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    //cam walking
    case  Qt::Key_Control:
    //qDebug("cntrl_released");
        //ctrl_pressed=false;
        glw->ctrl_pressed=false;
          break;
    }
}



void MainWindow::on_STOP_START_clicked()
{

}

void MainWindow::on_RESTART_clicked()
{

}

void MainWindow::set_move_sens(int new_move_sens)
{
    move_sens=new_move_sens;
}

/*
void MainWindow::on_yaw_plus_clicked()
{
    rotate_link(ui->link->value(),0.1,0,0);
}

void MainWindow::on_yaw_min_clicked()
{
    rotate_link(ui->link->value(),-0.1,0,0);
}

void MainWindow::on_pitch_plus_clicked()
{
    rotate_link(ui->link->value(),0,0.1,0);
}

void MainWindow::on_pitch_min_clicked()
{
    rotate_link(ui->link->value(),0,-0.1,0);
}


void MainWindow::on_roll_plus_clicked()
{
    rotate_link(ui->link->value(),0,0,0.1);
}

void MainWindow::on_roll_min_clicked()
{
    rotate_link(ui->link->value(),0,0,-0.1);
}
*/


void MainWindow::on_link_valueChanged(int arg1)
{
    glw->KChain.chosen_link_id=arg1;

    disconnect(this,SIGNAL(set_link_rotation(int,double,double,double)),
            glw,SLOT(set_KChain_link_angles(int,double,double,double)));

    ui->yaw_sl->setValue(glw->KChain.links[arg1]->yaw*100/M_PI);
    ui->pitch_sl->setValue(glw->KChain.links[arg1]->pitch*100/M_PI);
    ui->roll_sl->setValue(glw->KChain.links[arg1]->roll*100/M_PI);

    connect(this,SIGNAL(set_link_rotation(int,double,double,double)),
            glw,SLOT(set_KChain_link_angles(int,double,double,double)));
}

void MainWindow::on_yaw_sl_valueChanged(int value)
{
    int link_id=ui->link->value();
    double pitch=glw->KChain.links[link_id]->pitch;
    double roll=glw->KChain.links[link_id]->roll;
    double v=value;
    set_link_rotation(ui->link->value(),v/100*M_PI,pitch,roll);
}

void MainWindow::on_pitch_sl_valueChanged(int value)
{
    int link_id=ui->link->value();
    double yaw=glw->KChain.links[link_id]->yaw;
    double roll=glw->KChain.links[link_id]->roll;
    double v=value;
    set_link_rotation(ui->link->value(),yaw,v/100*M_PI,roll);
}

void MainWindow::on_roll_sl_valueChanged(int value)
{
    int link_id=ui->link->value();
    double yaw=glw->KChain.links[link_id]->yaw;
    double pitch=glw->KChain.links[link_id]->pitch;
    double v=value;
    set_link_rotation(ui->link->value(),yaw,pitch,v/100*M_PI);
}

void MainWindow::on_free_cam_but_clicked()
{
    ui->object_cam_but->setEnabled(true);
    ui->free_cam_but->setDisabled(true);
    glw->cam.enable_scene_cam(false);

}

void MainWindow::on_object_cam_but_clicked()
{
    ui->free_cam_but->setEnabled(true);
    ui->object_cam_but->setDisabled(true);
     glw->cam.enable_scene_cam(true);

}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_stop_but_clicked()
{
    glw->stop_proc=!glw->stop_proc;
    if(glw->stop_proc)ui->stop_but->setText("START");
    else ui->stop_but->setText("STOP");
}

void MainWindow::on_vel_slider_valueChanged(int value)
{
    glw->move_vel=value;
}

void MainWindow::on_del_link_but_clicked()
{
    if(glw->KChain.get_size()>1)
    {
        glw->KChain.del_link(ui->link->value());
        ui->link->setMaximum(glw->KChain.get_size()-1);
    }
}

void MainWindow::on_add_link_but_clicked()
{
    jc=new joint_choise();
    jc->setModal(true);
    connect(jc,SIGNAL(set_max_link()),this,SLOT(set_max_link()));
    jc->show();
    jc->set_kc(&glw->KChain,ui->link->value());
    //glw->KChain.add_link_to_middle(1,Vector3d(0,0,1),2,ui->link->value(),0,0,0);
}

void MainWindow::on_red_link_but_clicked()
{
    redact_joint* rj=new redact_joint();
    rj->set_kc(&glw->KChain,ui->link->value());
    rj->setModal(true);
    rj->show();
}

void MainWindow::on_save_to_file_but_clicked()
{
    QString fname;
    fname=QFileDialog::getSaveFileName(this, tr("Save File"),"C:/Qt_projects/inverse_kinematics/chain_files",tr("chain1(*.kChain)"));
    QFile file(fname);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
   // out<<"test1 ";out<<"test1";out<<"\n";out<<"test1";
    for(int i=0;i<glw->KChain.links.size();i++)
    {
        out<<glw->KChain.links[i]->type; out<<" ";
        out<<glw->KChain.links[i]->length; out<<" ";
        out<<glw->KChain.links[i]->yaw; out<<" ";
        out<<glw->KChain.links[i]->pitch; out<<" ";
        out<<glw->KChain.links[i]->roll; out<<" ";
        out<<glw->KChain.links[i]->width; out<<" ";
        out<<glw->KChain.links[i]->height; out<<" ";
        out<<glw->KChain.links[i]->joint_Frict_K; out<<" ";
        out<<glw->KChain.links[i]->telescopic; out<<" ";
        out<<glw->KChain.links[i]->min_length; out<<" ";
        out<<glw->KChain.links[i]->max_length; out<<" ";
        out<<glw->KChain.links[i]->slider_Frict_K; out<<" ";
        out<<glw->KChain.links[i]->subtype; out<<" ";

        if(glw->KChain.links[i]->type==1)
        {
            out<<glw->KChain.links[i]->aa_limiter[0].angle(); out<<" ";
            out<<0<<" ";
        }
        else
        {
            out<<glw->KChain.links[i]->lim_ang[0]; out<< " ";
            out<<glw->KChain.links[i]->lim_ang[1]; out<< " ";
        }

        /* ��������� ������������*/

        out << glw->KChain.links[i]->joint_elastic << " " ;
        out << glw->KChain.links[i]->joint_elastic_K << " " ;
        out << glw->KChain.links[i]->joint_elastic_K_2 << " " ;
        out << glw->KChain.links[i]->joint_elastic_angle << " " ;

        out << glw->KChain.links[i]->slider_elastic << " " ;
        out << glw->KChain.links[i]->slider_elastic_K << " " ;
        out << glw->KChain.links[i]->slider_elastic_K_2 << " " ;
        out << glw->KChain.links[i]->slider_elastic_L << " " ;

        out<<"\n";
    }
    out<<"END";
}

void MainWindow::on_opne_file_but_clicked()
{
    bool f=false;
    int type,c=0,tel;
    double len,yaw,pitch,roll,w,h,J_F,min,max,T_K,st,ang1,ang2;
    QString fname,buf;
    fname=QFileDialog::getOpenFileName(this, tr("OpenFile"),"C:/Qt_projects/inverse_kinematics/chain_files",tr("chain1(*.kChain)"));
    QFile file(fname);

    // ��������� ��������� �������
    int joint_elastic;
    double joint_elastic_K;
    double joint_elastic_K_2;
    double joint_elastic_angle;
    // ��������� ���������� ��������
    int slider_elastic;
    double slider_elastic_K;
    double slider_elastic_K_2;
    double slider_elastic_L;

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    if(!fname.isEmpty())
    {
        //delete old chain
        glw->KChain.ball_joints.clear();
        glw->KChain.hinge_joints.clear();
        glw->KChain.hinge_ra_joints.clear();
        glw->KChain.refresh_links();
    glw->main_stop=true;
        //load new chain
    do
    {
        in>>buf;
        if(buf=="END") break;
        type=buf.toInt();

        in>>buf;
        if(buf=="END") break;
        len=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        yaw=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        pitch=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        roll=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        w=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        h=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        J_F=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        tel=buf.toInt();

        in>>buf;
        if(buf=="END") break;
        min=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        max=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        T_K=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        st=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        ang1=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        ang2=buf.toDouble();

        // ��������� ��������� �������

        in>>buf;
        if(buf=="END") break;
        joint_elastic=buf.toInt();

        in>>buf;
        if(buf=="END") break;
        joint_elastic_K=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        joint_elastic_K_2=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        joint_elastic_angle=buf.toDouble();

        // ��������� ���������� ��������

        in>>buf;
        if(buf=="END") break;
        slider_elastic=buf.toInt();

        in>>buf;
        if(buf=="END") break;
        slider_elastic_K=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        slider_elastic_K_2=buf.toDouble();

        in>>buf;
        if(buf=="END") break;
        slider_elastic_L=buf.toDouble();




        if(c==0)c++,f=true;

        if(f) glw->KChain.add_link(type,Vector3d(0,0,0),len,c-1,yaw,pitch,roll);
        else glw->KChain.add_link(type,Vector3d(0,0,1),len,c-1,yaw,pitch,roll);
        if(f)c--,f=false;
        glw->KChain.set_link_size(c,w,h);
        if(tel==1) glw->KChain.enable_telescopic(c,true,min,max,T_K);
        else glw->KChain.enable_telescopic(c,false,min,max,T_K);
        glw->KChain.set_subtype(c,st);
        glw->KChain.set_joint_Frict(c,J_F);
        if(type==1)
        {
            if( glw->KChain.links[c]->aa_limiter.size()==0)glw->KChain.links[c]->aa_limiter.clear();
            glw->KChain.set_aa_limiter(c,ang1,Vector3d(0,0,-1));
        }
        else glw->KChain.set_lim_angles(c,ang1,ang2,0,0);

        // ��������� ����������
        glw->KChain.set_joint_elastic(c,(bool)joint_elastic,joint_elastic_K,joint_elastic_K_2,joint_elastic_angle);
        glw->KChain.set_slider_elastic(c,(bool)slider_elastic,slider_elastic_K,slider_elastic_K_2,slider_elastic_L);

        c++;
    }
    while(true);

    glw->main_stop=false;
    glw->KChain.effectors[0].id=glw->KChain.links.size()-1;
    ui->link->setMaximum(glw->KChain.links.size()-1);
    }

}

void MainWindow::set_max_link()
{
    ui->link->setMaximum(glw->KChain.get_size()-1);
}

void MainWindow::on_vel_slider_actionTriggered(int action)
{

}

void MainWindow::on_Target_Activated_checkbox_clicked(bool checked)
{
    /*nothing*/
}

void MainWindow::on_elastic_joints_checkbox_clicked(bool checked)
{
    /*nothing*/
}

void MainWindow::on_Target_Activated_checkbox_stateChanged(int arg1)
{
    if(arg1==2)glw->KChain.set_TARGET_ACTIVATED(true);
    else if(arg1==0) glw->KChain.set_TARGET_ACTIVATED(false);
}

void MainWindow::on_elastic_joints_checkbox_stateChanged(int arg1)
{
    if(arg1==2)glw->KChain.set_ELASTIC_GLOBAL(true);
    else if(arg1==0) glw->KChain.set_ELASTIC_GLOBAL(false);
}

//menu
void  MainWindow::createActions()
{

}

void  MainWindow::createMenus()
{
    QMenu * mnFile = new QMenu("File");
    QMenu * mnSceneObjects = new QMenu("Scene Objects");

    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnSceneObjects);

}
