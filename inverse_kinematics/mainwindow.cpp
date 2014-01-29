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



   //интрефейс
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

    ui->opne_file_but->hide();
    ui->save_to_file_but->hide();
    ui->line_2->hide();
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
    switch (event->key())
    {
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
            glw->set_lp(QVector4D(0,0,-1.0,0));
            break;
        case Qt::Key_K:
            glw->set_lp(QVector4D(0,0,1.0,0));
            break;
        case  Qt::Key_J:
            glw->set_lp(QVector4D(-1.0,0,0,0));
            break;
        case  Qt::Key_L:
            glw->set_lp(QVector4D(1.0,0,0,0));
            break;
        case  Qt::Key_N:
            glw->set_lp(QVector4D(0,1,0,0));
            break;
        case  Qt::Key_M:
            glw->set_lp(QVector4D(0,-1,0,0));
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

    glw->KChain.collision = false;
}

void MainWindow::on_pitch_sl_valueChanged(int value)
{
    int link_id=ui->link->value();
    double yaw=glw->KChain.links[link_id]->yaw;
    double roll=glw->KChain.links[link_id]->roll;
    double v=value;
    set_link_rotation(ui->link->value(),yaw,v/100*M_PI,roll);

    glw->KChain.collision = false;
}

void MainWindow::on_roll_sl_valueChanged(int value)
{
    int link_id=ui->link->value();
    double yaw=glw->KChain.links[link_id]->yaw;
    double pitch=glw->KChain.links[link_id]->pitch;
    double v=value;
    set_link_rotation(ui->link->value(),yaw,pitch,v/100*M_PI);

    glw->KChain.collision = false;
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
        ui->link->setMaximum(glw->KChain.get_size()- 1);
        glw->scene->update_IK_Chain_objects(&glw->KChain);
    }
}

void MainWindow::on_add_link_but_clicked()
{
    jc = new joint_choise();
    jc->setModal(true);
    connect(jc,SIGNAL(set_max_link()),this,SLOT(set_max_link()));
    jc->set_scene(glw->scene);
    jc->set_kc(&glw->KChain,ui->link->value());
    jc->show();

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

        /* параметры эластичности*/

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

void MainWindow::on_open_file_but_clicked()
{
    bool f=false;
    int type,c=0,tel;
    double len,yaw,pitch,roll,w,h,J_F,min,max,T_K,st,ang1,ang2;
    QString fname,buf;
    fname=QFileDialog::getOpenFileName(this, tr("OpenFile"),"C:/Qt_projects/inverse_kinematics/chain_files",tr("chain1(*.kChain)"));
    QFile file(fname);

    // параметры упругости сустава
    int joint_elastic;
    double joint_elastic_K;
    double joint_elastic_K_2;
    double joint_elastic_angle;
    // параметры упроугости слайдера
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

        // параметры упругости сустава

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

        // параметры упроугости слайдера

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

        // параметры упроугости
        glw->KChain.set_joint_elastic(c,(bool)joint_elastic,joint_elastic_K,joint_elastic_K_2,joint_elastic_angle);
        glw->KChain.set_slider_elastic(c,(bool)slider_elastic,slider_elastic_K,slider_elastic_K_2,slider_elastic_L);

        c++;
    }
    while(true);

    glw->scene->update_IK_Chain_objects(&glw->KChain);

    glw->main_stop = false;
    glw->KChain.effectors[0].id = glw->KChain.links.size() - 1;
    ui->link->setMaximum(glw->KChain.links.size()-1);
    }

}

void MainWindow::on_save_scene_to_file_but_clicked()
{
    QString fname;
    fname = QFileDialog::getSaveFileName(this, tr("Save scene"),"",tr("Scene(*.scn)"));
    if(fname.length() > 0)
    {
        QFile file(fname);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);

        QMatrix4x4 rot_scale;
        QMatrix4x4 translate;
        QVector4D material;

        for(int b = glw->scene->chain_links_n; b < glw->scene->Scene_objects.size(); b++)
        {

            rot_scale = glw->scene->Scene_objects[b].rot_scale_model;
            translate = glw->scene->Scene_objects[b].translate_model;
            material = glw->scene->Scene_objects[b].material;

            for(int i = 0; i < 4; i++)
            {
                QVector4D row = rot_scale.row(i);

                for(int j = 0; j < 4; j++)
                {
                    out<<row[j]; out<<" ";
                }
            }

            for(int i = 0; i < 4; i++)
            {
                QVector4D row = translate.row(i);

                for(int j = 0; j < 4; j++)
                {
                    out<<row[j]; out<<" ";
                }
            }

            for(int j = 0; j < 4; j++)
            {
                out<<material[j]; out<<" ";
            }

            out<<"\n";

        }

        out<<"END";
    }
}

void MainWindow::on_open_scene_file_but_clicked()
{
    QString fname,buf;
    fname = QFileDialog::getOpenFileName(this, tr("OpenFile"),"",tr("Scene(*.scn)"));
    QFile file(fname);

    QMatrix4x4 rot_scale;
    QMatrix4x4 translate;
    QVector4D material;
    QVector4D row;

    bool fail_flag = false;

    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    glw->scene->Scene_objects.remove(glw->scene->chain_links_n, glw->scene->Scene_objects.size() - glw->scene->chain_links_n);

    do
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                in>>buf;
                if(buf=="END") fail_flag = true;
                row[j] = buf.toDouble();
            }

            rot_scale.setRow(i, row);
        }

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                in>>buf;
                if(buf=="END") fail_flag = true;
                row[j] = buf.toDouble();
            }

            translate.setRow(i, row);
        }

        for(int j = 0; j < 4; j++)
        {
            in>>buf;
            if(buf=="END") fail_flag = true;
            row[j] = buf.toDouble();
        }

        material = row;

        if(fail_flag) break;
        //insert object
        glw->add_box_scene(rot_scale, translate, material);
    }
    while(true);

    glw->scene->scene_octree->cleanup();
    glw->scene->build_octree(100.0, false);

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
    ssaoAct = new QAction("ssao", this);
    ssaoAct->setCheckable(true);
    ssaoAct->toggle();
    connect(ssaoAct,SIGNAL(toggled(bool)),this,SLOT(on_ssao_changed(bool)));

    shadowsAct = new QAction("тени", this);
    shadowsAct->setCheckable(true);
    shadowsAct->toggle();
    connect(shadowsAct,SIGNAL(toggled(bool)),this,SLOT(on_shadows_changed(bool)));

    edgesAct = new QAction("рисовать грани объектов", this);
    edgesAct->setCheckable(true);
    connect(edgesAct,SIGNAL(toggled(bool)),this,SLOT(on_draw_edges_changed(bool)));

    openChainAct = new QAction("Открыть файл цепи", this);
    connect(openChainAct,SIGNAL(triggered()),this,SLOT(on_open_file_but_clicked()));

    saveChainAct = new QAction("Сохранить цепь в файл", this);
    connect(saveChainAct,SIGNAL(triggered()),this,SLOT(on_save_to_file_but_clicked()));

    openSceneAct = new QAction("Открыть файл сцены", this);
    connect(openSceneAct,SIGNAL(triggered()),this,SLOT(on_open_scene_file_but_clicked()));

    saveSceneAct = new QAction("Сохранить сцену в файл", this);
    connect(saveSceneAct,SIGNAL(triggered()),this,SLOT(on_save_scene_to_file_but_clicked()));

    addBoxAct = new QAction("Добавить объект", this);
    connect(addBoxAct,SIGNAL(triggered()),this,SLOT(create_add_box_dialog()));
    addBoxAct->setEnabled(false);

    deleteBoxAct = new QAction("Удалить объект", this);
    connect(deleteBoxAct,SIGNAL(triggered()),this,SLOT(delete_box()));
    deleteBoxAct->setEnabled(false);

    redactSceneAct = new QAction("Режим редактирования сцены", this);
    connect(redactSceneAct,SIGNAL(triggered()),this,SLOT(on_redact_scene()));
    redactSceneAct->setCheckable(true);

    selectMoveAct = new QAction("Перенос/Вращение", this);
    connect(selectMoveAct,SIGNAL(toggled(bool)),this,SLOT(on_select_move(bool)));
    selectMoveAct->setCheckable(true);

    exitAct = new QAction("Выход", this);
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    geometryRepulsion = new QAction("Отталкивание от препятсвий", this);
    geometryRepulsion->setCheckable(true);
    connect(geometryRepulsion,SIGNAL(toggled(bool)),this,SLOT(set_geometry_repulsion(bool)));

    targetShift = new QAction("Сдвиг цели", this);
    targetShift->setCheckable(true);
    connect(targetShift,SIGNAL(toggled(bool)),this,SLOT(set_target_shift(bool)));

    forceNormAct = new QAction("Нормализация действущей силы", this);
    connect(forceNormAct,SIGNAL(toggled(bool)),this,SLOT(set_force_normalizing(bool)));
    forceNormAct->setCheckable(true);

    moveMaxForce = new QAction("Двигать звено с максимальным воздействием", this);
    connect(moveMaxForce,SIGNAL(toggled(bool)),this,SLOT(set_max_force_rotation(bool)));
    moveMaxForce->setCheckable(true);

}

void  MainWindow::createMenus()
{
    mnFile = new QMenu("Файл");
    mnSceneObjects = new QMenu("Сцена");
    mnGraphic = new QMenu("Графика");
    mnObstaclesAvoiding = new QMenu("Обход препятсвий");
    mnChainOptions = new QMenu("Насройки Цепи");

    ui->menuBar->addMenu(mnFile);
    ui->menuBar->addMenu(mnSceneObjects);
    ui->menuBar->addMenu(mnGraphic);
    ui->menuBar->addMenu(mnObstaclesAvoiding);
    ui->menuBar->addMenu(mnChainOptions);

    mnFile->addAction(openChainAct);
    mnFile->addAction(saveChainAct);
    mnFile->addAction(openSceneAct);
    mnFile->addAction(saveSceneAct);
    mnFile->addAction(exitAct);


    mnSceneObjects->addAction(addBoxAct);
    mnSceneObjects->addAction(deleteBoxAct);
    mnSceneObjects->addAction(redactSceneAct);
    mnSceneObjects->addAction(selectMoveAct);

    mnGraphic->addAction(shadowsAct);
    mnGraphic->addAction(ssaoAct);
    mnGraphic->addAction(edgesAct);

    mnObstaclesAvoiding->addAction(geometryRepulsion);
    mnObstaclesAvoiding->addAction(targetShift);

    mnPathFinding = mnObstaclesAvoiding->addMenu(tr("Поиск Пути"));

    mnChainOptions->addAction(forceNormAct);
    mnChainOptions->addAction(moveMaxForce);

}

void MainWindow::on_ssao_changed(bool state)
{
    glw->set_ssao(state);
}

void MainWindow::on_shadows_changed(bool state)
{
    glw->set_shadows(state);
}

void MainWindow::create_add_box_dialog()
{
    addBoxDialog = new AddBoxDialog(glw);
    addBoxDialog->setModal(true);
    addBoxDialog->show();
}

void MainWindow::on_redact_scene()
{
    if(!deleteBoxAct->isEnabled())
    {
        deleteBoxAct->setEnabled(true);
        addBoxAct->setEnabled(true);
        if(!glw->stop_proc)
            ui->stop_but->click();
        ui->stop_but->setEnabled(false);
        glw->redact_scene_mode = true;
        glw->scene->update_polys_id();
    }
    else
    {
        glw->scene->scene_octree->cleanup();
        glw->scene->build_octree(100.0, false);

        deleteBoxAct->setEnabled(false);
        addBoxAct->setEnabled(false);
        ui->stop_but->setEnabled(true);
        ui->stop_but->click();
        glw->redact_scene_mode = false;
        glw->selected_object_id = 0;
    }
}

void MainWindow::on_select_move(bool state)
{
    glw->move_rotate_object = state;
}

void MainWindow::on_draw_edges_changed(bool state)
{
    glw->draw_edges = state;
}

void MainWindow::delete_box()
{
    if(glw->selected_object_id != 0)
        glw->scene->Scene_objects.remove(glw->selected_object_id);

    glw->selected_object_id = 0;
}

void MainWindow::set_force_normalizing(bool state)
{
    glw->KChain.set_Force_normalizing(state);
}

void MainWindow::set_max_force_rotation(bool state)
{
    glw->KChain.set_max_Force_rotation(state);
}

void MainWindow::set_geometry_repulsion(bool state)
{
    glw->geometry_repulsion = state;
}

void MainWindow::set_target_shift(bool state)
{
    glw->target_shift = state;
}
