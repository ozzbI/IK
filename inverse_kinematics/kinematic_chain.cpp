#include "kinematic_chain.h"

kinematic_chain::kinematic_chain()
{
    cyl.make_cylinder(1,1,20);
    sph.make_sphere(0,0,0,1,20);
    cyl.add_model.rotate(90,0,0,1);
    cyl_ra.make_cylinder(1,1,20);;
    cyl_ra.add_model.rotate(90,1,0,0);
    cyl_t2.make_cylinder(1,1,20);;
    //cyl_t2.add_model.rotate(90,1,0,0);

    link_box.set_material(QVector4D(1,1,1,0));
    cyl.set_material(QVector4D(0.1,0.1,1.2,0));
    cyl_ra.set_material(QVector4D(0.3,0.3,0.3,0));
    cyl_t2.set_material(QVector4D(0.1,0.1,1.2,0));
    sph.set_material(QVector4D(1.0,1.0,0.1,0));
    bone_disabling=false;
    Force_normalizing=false;
    rot_max_Force=false;
    max_Force_id=0;
}

void kinematic_chain::draw_chain()
{
    QMatrix4x4 box_shift_mat,box_shift_mat_2,base_scale;
    box_shift_mat.translate(0,0,-0.5);

    box_shift_mat_2.translate(0,0,0.5);
    base_scale.scale(2,2,0.1);

    int chain_size=links.size();

    for (int i=0;i<chain_size;i++)
    {
        QMatrix4x4 identity;
        //рисуется кость
        Quaterniond rot_qt;
        QMatrix4x4 translate_mat;
        QMatrix4x4 scale_mat;
        QMatrix4x4 rot_mat;

        rot_qt=links[i]->global_transform;
        Matrix3d e_rot=rot_qt.toRotationMatrix();
        Eigen_Matr_to_Qt_Matr(rot_mat,e_rot);

        scale_mat.scale(links[i]->width,links[i]->height,links[i]->length);

        translate_mat.translate(links[i]->global_position(0),links[i]->global_position(1),links[i]->global_position(2));

        link_box.model=translate_mat*rot_mat*scale_mat*box_shift_mat;
        //material
        if(i==chosen_link_id)link_box.set_material(QVector4D(1,0,0,0));
        else link_box.set_material(QVector4D(1,1,1,0));
        link_box.draw();

        //постамент

        if(i==0)
        {
            QMatrix4x4 local_trans;
            QMatrix4x4 base_trans;
            base_trans.translate(0,0,2);
            e_rot=links[0]->local_transform.toRotationMatrix();
            Eigen_Matr_to_Qt_Matr(local_trans,e_rot);

            base_scale=identity;
            base_scale.scale(2,2,0.2);
            link_box.model=local_trans*base_trans*base_scale*box_shift_mat_2;
            link_box.set_material(QVector4D(0.7,0.7,0.7,0.1));
            link_box.draw();

            base_scale=identity;
            base_scale.scale(0.5,0.5,2.0);
            link_box.model=local_trans*base_scale*box_shift_mat_2;
            link_box.set_material(QVector4D(0.7,0.7,0.7,0.7));
            link_box.draw();
        }
        //рисуется основние елси кость телеякопическая
        if(links[i]->telescopic)
        {
            link_box.set_material(QVector4D(0.25,0.9,0.25,0));//цвет осноывния телескопического звена
            scale_mat=identity;
            scale_mat.scale(links[i]->width+0.07,links[i]->height+0.07,links[i]->min_length);

            QMatrix4x4 base_tr;
            base_tr.translate(-links[i]->dir.x()*0.01,-links[i]->dir.y()*0.01,-links[i]->dir.z()*0.01);
            translate_mat=base_tr*translate_mat;
            link_box.model=translate_mat*rot_mat*scale_mat*box_shift_mat;
            link_box.draw();
        }
        //рисуется сочлененеие
        if(links[i]->type==1)//шаровый шаринир
        {
            double w,h;
            int par_id=links[i]->parent_id;
            translate_mat=identity;
            scale_mat=identity;
            translate_mat.translate(links[i]->global_position(0),links[i]->global_position(1),links[i]->global_position(2));

            //выбор болших сторон для сферы шарнира
            if(links[i]->width>links[par_id]->width)w=links[i]->width;
            else w=links[par_id]->width;
            if(links[i]->height>links[par_id]->height)h=links[i]->height;
            else h=links[par_id]->height;

            double rad=sqrt((w/2)*(w/2)+(h/2)*(h/2));
            if(links[i]->telescopic)rad+=0.08;
            else rad+=0.03;
            scale_mat.scale(rad,rad,rad);
            sph.model=translate_mat*scale_mat;
            sph.draw();
        }

        if(links[i]->type==2)//обычный шаринир
        {


            double w,h;
            int par_id=links[i]->parent_id;
            translate_mat=identity;
            scale_mat=identity;
            translate_mat.translate(links[i]->global_position(0),links[i]->global_position(1),links[i]->global_position(2));

            //выбор болших сторон для сферы шарнира
            if(links[i]->width>links[par_id]->width)w=links[i]->width;
            else w=links[par_id]->width;
            if(links[i]->height>links[par_id]->height)h=links[i]->height;
            else h=links[par_id]->height;
            if(links[i]->subtype==1)
            {
                h+=0.02;
                if(links[i]->telescopic)w+=0.1;
                else w+=0.03;
                scale_mat.scale(w,h,h);
                cyl.model=translate_mat*rot_mat*scale_mat;
                cyl.draw();
            }
            if(links[i]->subtype==2)
            {
                w+=0.02;
                if(links[i]->telescopic)h+=0.1;
                else h+=0.03;
                scale_mat.scale(w,h,w);
                cyl_t2.model=translate_mat*rot_mat*scale_mat;
                cyl_t2.draw();
            }

        }

        if(links[i]->type==3)
        {
            double w,h;
            int par_id=links[i]->parent_id;
            translate_mat=identity;
            scale_mat=identity;
            translate_mat.translate(links[i]->global_position(0),links[i]->global_position(1),links[i]->global_position(2));

            //выбор болших сторон для сферы шарнира
            if(links[i]->width>links[par_id]->width)w=links[i]->width;
            else w=links[par_id]->width;
            if(links[i]->height>links[par_id]->height)h=links[i]->height;
            else h=links[par_id]->height;

            double rad=sqrt((w/2)*(w/2)+(h/2)*(h/2));
            if(links[i]->telescopic)rad+=0.08;
            else rad+=0.03;
            scale_mat.scale(rad,rad,rad*2);
            cyl_ra.model=translate_mat*rot_mat*scale_mat;
            cyl_ra.draw();
        }

    }

}


void kinematic_chain::add_link(int type,Vector3d in_pos,double in_lenght,int in_parent_id,double yaw=0,double pitch=0,double roll=0)
{
    abstract_joint* link;


    if (type==1)
    {
        link=new ball_joint(in_pos, in_lenght, in_parent_id,yaw,pitch,roll,true);
    }
    else if(type==2)
    {
        link=new hinge_joint(in_pos, in_lenght, in_parent_id,yaw,pitch,roll,true);
    }
    else if(type==3)
    {
        link=new hinge_ra(in_pos, in_lenght, in_parent_id,yaw,pitch,roll,true);
    }


    if(links.size()==0) //для корня цепи
    {
        link->root=true;
        link->id=0;
        link->position=Vector3d(0,0,0);
        link->global_position=in_pos;
        link->chain_transform=(AngleAxisd(0,  Vector3d(0,0,0)));

        Quaterniond yaw(AngleAxisd(link->yaw, Vector3d(0,1,0)));
        Quaterniond pitch(AngleAxisd(link->pitch, Vector3d(1,0,0)));
        Quaterniond roll(AngleAxisd(link->roll, Vector3d(0,0,-1)));

        link->local_transform=yaw*pitch*roll;
        link->global_transform=link->local_transform;

        Matrix3d local_transform_matr=link->local_transform.toRotationMatrix();

        link->dir=local_transform_matr*link->dir;
        link->right=local_transform_matr*link->right;
        link->up=local_transform_matr*link->up;

        link->dir.normalize();
        link->right.normalize();
        link->up.normalize();

    }
    else // для отсальных звеньев
    {
        link->root=false;
        links[in_parent_id]->add_child(links.size());// add record about link-child
        link->id=links.size();

        //оси шарнира
        link->joint_dir=links[in_parent_id]->dir;
        link->joint_right=links[in_parent_id]->right;
        link->joint_up=links[in_parent_id]->up;

        //положение шарнира
        link->global_position=links[in_parent_id]->global_position+links[in_parent_id]->dir*links[in_parent_id]->length*link->position(2)
                +links[in_parent_id]->right*link->position(0)+links[in_parent_id]->up*link->position(1);
        //кватернион поворота цепи
        link->chain_transform=links[in_parent_id]->global_transform;

        Quaterniond yaw(AngleAxisd(link->yaw, Vector3d(0,1,0)));
        Quaterniond pitch(AngleAxisd(link->pitch, Vector3d(1,0,0)));
        Quaterniond roll(AngleAxisd(link->roll, Vector3d(0,0,-1)));

        //кватернион поворота звена
        link->local_transform=yaw*pitch*roll;
        //кватернион глобального поворота звена
        link->global_transform=link->chain_transform*link->local_transform;
        Matrix3d global_transform_matr=link->global_transform.toRotationMatrix();

        //оси после поворота звена
        link->dir=global_transform_matr* link->dir;
        link->right=global_transform_matr*link->right;
        link->up=global_transform_matr*link->up;

        link->dir.normalize();
        link->right.normalize();
        link->up.normalize();
    }


    if (type==1)
    {
        ball_joints.push_back(*(ball_joint*)link);
    }
    else if(type==2)
    {
        hinge_joints.push_back(*(hinge_joint*)link);
    }  
    else if(type==3)
    {
        hinge_ra_joints.push_back(*(hinge_ra*)link);
    }

    refresh_links();


}

void kinematic_chain::total_link_recalculation(int start_link_id)
{
    int child_num=links[start_link_id]->childs_id.size();

    #define link links[start_link_id]

    int parent_id=link->parent_id;

    if (link->root==false)
    {
        //положение шарнира
        link->global_position=links[parent_id]->global_position+links[parent_id]->dir*links[parent_id]->length*link->position(2)
                +links[parent_id]->right*link->position(0)+links[parent_id]->up*link->position(1);
        //кватернион поворота цепи
        link->chain_transform=links[parent_id]->global_transform;
    }
    Quaterniond yaw(AngleAxisd(link->yaw, Vector3d(0,1,0)));
    Quaterniond pitch(AngleAxisd(link->pitch, Vector3d(1,0,0)));
    Quaterniond roll(AngleAxisd(link->roll, Vector3d(0,0,-1)));

    //кватернион поворота звена
    link->local_transform=yaw*pitch*roll;
    //кватернион глобального поворота звена
    link->global_transform=link->chain_transform*link->local_transform;

    if(start_link_id!=0)refresh_bones_axis(start_link_id);

    for(int i=0;i<child_num;i++)
    {
        childs_recalculation(links[start_link_id]->childs_id[i]);
    }

}

void kinematic_chain::childs_recalculation(int start_link_id)
{
    int child_num=links[start_link_id]->childs_id.size();

    #define link links[start_link_id]

    int parent_id=link->parent_id;

    //положение шарнира
    link->global_position=links[parent_id]->global_position+links[parent_id]->dir*links[parent_id]->length*link->position(2)
            +links[parent_id]->right*link->position(0)+links[parent_id]->up*link->position(1);
    //кватернион поворота цепи
    link->chain_transform=links[parent_id]->global_transform;

    link->global_transform=link->chain_transform*link->local_transform;

    refresh_bones_axis(start_link_id);

    for(int i=0;i<child_num;i++)
    {
        childs_recalculation(links[start_link_id]->childs_id[i]);
    }

}

void kinematic_chain::childs_glob_trans_recalc(int start_link_id,Quaterniond &q,double velocity)
{
    int child_num=links[start_link_id]->childs_id.size();

    if(links[start_link_id]->type==1&&links[start_link_id]->rotation_possibility(q,velocity)) return;

        links[start_link_id]->global_transform=q*links[start_link_id]->global_transform;

        refresh_bones_axis(start_link_id);

    for(int i=0;i<child_num;i++)
    {
        childs_glob_trans_recalc(links[start_link_id]->childs_id[i],q,velocity);
    }

}

void kinematic_chain::set_link_angles(int link_id,double yaw,double pitch, double roll)
{
    links[link_id]->yaw=yaw;
    links[link_id]->pitch=pitch;
    links[link_id]->roll=roll;
    total_link_recalculation(link_id);
}

void kinematic_chain::rotate_link(int link_id,double yaw,double pitch, double roll)
{
    links[link_id]->yaw+=yaw;
    links[link_id]->pitch+=pitch;
    links[link_id]->roll+=roll;
    total_link_recalculation(link_id);
}

void kinematic_chain::Eigen_Matr_to_Qt_Matr(QMatrix4x4 &qm,Matrix3d &em)
{
    qm(0,0) = em(0,0);  qm(0, 1) = em(0,1);  qm(0, 2) = em(0,2);
    qm(1,0) = em(1,0);  qm(1, 1) = em(1,1);  qm(1, 2) = em(1,2);
    qm(2,0) = em(2,0);  qm(2, 1) = em(2,1);  qm(2, 2) = em(2,2);

}

void kinematic_chain::setshaderprog(QGLShaderProgram* new_prog)
{
    program=new_prog;
    link_box.setshaderprog(program);
    cyl.setshaderprog(program);
    cyl_ra.setshaderprog(program);
    cyl_t2.setshaderprog(program);
    sph.setshaderprog(program);
}

void kinematic_chain::set_link_size(int link_id,double width,double height)
{
    links[link_id]->width=width;
    links[link_id]->height=height;
}

int kinematic_chain::get_size()
{
   return links.size();
}

void kinematic_chain::refresh_links()
{
    int joints_count,br;
    joints_count=ball_joints.size()+hinge_joints.size()+hinge_ra_joints.size();
    links.clear();
    for(int i=0;i<joints_count;i++)
    {
        br=0;
        for(unsigned int b=0;b<ball_joints.size();b++)
        {
            if(i==ball_joints[b].id)
            {
                links.push_back((abstract_joint*)&ball_joints[b]);
                br=1;
                break;
            }
        }

        if(br) continue;

        for(unsigned int h=0;h<hinge_joints.size();h++)
        {
            if(i==hinge_joints[h].id)
            {
                links.push_back((abstract_joint*)&hinge_joints[h]);
                br=1;
                break;
            }
        }

        if(br) continue;

        for(unsigned int hra=0;hra<hinge_ra_joints.size();hra++)
        {
            if(i==hinge_ra_joints[hra].id)
            {
                links.push_back((abstract_joint*)&hinge_ra_joints[hra]);
                break;
            }
        }
    }

    //добавление ссылки на родителя----------------------
     for(unsigned int i=0;i<links.size();i++)
     {
         links[i]->parent=links[links[i]->parent_id];
     }
}

void kinematic_chain::add_effector(int joint_id,double K,Vector3d target)
{
    effector e;
    e.id=joint_id;
    e.K=K;
    e.target=target;
    effectors.push_back(e);
}

void kinematic_chain::delete_effector(int joint_id)
{
    for(unsigned int i=0;i<effectors.size();i++)
    {
        if(joint_id==effectors[i].id)
        {
            vector<effector,Eigen::aligned_allocator<effector> >::iterator p=effectors.begin();
            effectors.erase(p+5);
            break;
        }
    }
}

void kinematic_chain::set_effector_target(int joint_id, Vector3d target)
{
    for(unsigned int i=0;i<effectors.size();i++)
    {
        if(joint_id==effectors[i].id)
        {
            effectors[i].target=target;
            break;
        }
    }
}

Vector3d kinematic_chain::projection(const Vector3d& a,  const Vector3d& b)//b на a
{
    Vector3d p = a*( a.dot(b)/(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()) ) ;
    return  p;
}



void kinematic_chain::Forces_calculation(double velocity)
{
    int cur_joint_id,child_id;
    bool last_step=false;
    Vector3d Force,child_dir,Max_Force=Vector3d(0,0,0),Force_before;
    //QVector<int> childs;
    for(unsigned int e=0;e<effectors.size();e++)
    {
        //childs.clear();
        cur_joint_id=effectors[e].id;
        child_id=effectors[e].id;
        // не обрабатывается случай со смещенной костью
        Force=effectors[e].target-(links[cur_joint_id]->global_position+links[cur_joint_id]->dir*links[cur_joint_id]->length);    
        Force_before=Force;
        //нормализация вектора силы ----------------------------------------------------
        if(!Force_normalizing)
        {
            double Force_len=sqrt(Force.x()*Force.x()+Force.y()*Force.y()+Force.z()*Force.z());
            if(Force_len>1)Force.normalize();
        }
        else Force.normalize();

       // qDebug("Force: %f, %f, %f ------------------------------------------------------",Force.x(),Force.y(),Force.z());

        while(!last_step)
        {
            if(cur_joint_id==0) last_step=true;
           /* if(links[child_id]->type==3&&links[cur_joint_id]->type==3) //поиск плеча для джоинта крутящегося вокруг свое оси
            {
                bool f=false;
                for(int c=0;c<childs.size();c++)
                {
                    if(links[c]->type!=3)
                    {
                        child_dir=links[c]->dir;
                        f=true;
                        break;
                    }
                }

                if(!f)child_dir=links[child_id]->dir;
            }
            else child_dir=links[child_id]->dir;*/

            child_dir=links[child_id]->dir;

             Force_before=Force;
            Force=links[cur_joint_id]->calculate_Force(Force,effectors[e].K,cur_joint_id==effectors[e].id,child_dir,velocity);
       // qDebug("Force(%d): %f, %f, %f",cur_joint_id,Force.x(),Force.y(),Force.z());
            if(rot_max_Force)
            {
                Vector3d curForce;
                curForce=Force-Force_before;
                if(len(Max_Force)<len(curForce)) max_Force_id=cur_joint_id,Max_Force=curForce;
            }

            //childs.push_front(child_id);
            child_id=cur_joint_id;
            cur_joint_id=links[cur_joint_id]->parent_id;
        }
       // Force=links[cur_joint_id]->calculate_Force(Force,effectors[e].K);
    }
}

void kinematic_chain::rotation_step_calculate(double velocity)
{
    vector<abstract_joint*> joints_to_move;
    int cur_joint_id;
    bool last_step=false;
    Forces_calculation(velocity);
    for(unsigned int e=0;e<effectors.size();e++)
    {
        cur_joint_id=effectors[e].id;

        while(!last_step)
        {
            if(cur_joint_id==0) last_step=true;
            joints_to_move.push_back(links[cur_joint_id]);
            cur_joint_id=links[cur_joint_id]->parent_id;
        }

        for(int i=joints_to_move.size()-1;i>=0;i--)//проход по костям от корня к эффектору
        {
            //проверка на отключение поворота----------------------------------------
            if(bone_disabling)
            {
                Vector3d to_target;
                double childs_len=0,to_target_scal;
                to_target=effectors[e].target-
                        (joints_to_move[i]->global_position+joints_to_move[i]->dir*joints_to_move[i]->length);
                for(int c=i-1;c>=0;c--)
                {
                    if(joints_to_move[c]->telescopic)childs_len+=joints_to_move[c]->max_length;
                    else childs_len+=joints_to_move[c]->length;
                }

                to_target_scal=to_target.x()*to_target.x()+to_target.y()*to_target.y()+to_target.z()*to_target.z();

                if(childs_len*childs_len>to_target_scal) continue;
            }


            //расчёт поворота------------------------------------------------------
            AngleAxisd angle_axis(joints_to_move[i]->Result_Force);//кватернион поворота к цели -> угол-ось
            Vector3d axis=angle_axis.axis().normalized();
            double angle=angle_axis.angle();
            angle*=velocity*joints_to_move[i]->rot_vel_K;//уменьшаем угол и умножаем на коэф-т

            Quaterniond rot_quat(AngleAxisd(angle,axis));//обратно в кватернион

            if(rot_max_Force&&joints_to_move[i]->id!=max_Force_id) goto skip_rot;



            //проверка на 0,0,0,0 кватернион (чтобы не допустить умножения если силы не действуют)
            if( (joints_to_move[i]->Result_Force.x()
                ||joints_to_move[i]->Result_Force.y()
                ||joints_to_move[i]->Result_Force.z()
                ||joints_to_move[i]->Result_Force.w()))
            {
                joints_to_move[i]->global_transform=rot_quat*joints_to_move[i]->global_transform;
            }
            joints_to_move[i]->refresh_bone_axis();//обновление осей кости

            //обработка слайдера----------------------------------
            if(joints_to_move[i]->telescopic)
            {
                double slider_Force_len;
                slider_Force_len=sqrt(joints_to_move[i]->slider_Force.x()*joints_to_move[i]->slider_Force.x()
                                     +joints_to_move[i]->slider_Force.y()*joints_to_move[i]->slider_Force.y()
                                     +joints_to_move[i]->slider_Force.z()*joints_to_move[i]->slider_Force.z());

                if(joints_to_move[i]->slider_Force.dot(joints_to_move[i]->dir)>0)
                {
                    joints_to_move[i]->length+=slider_Force_len*velocity;
                    if(joints_to_move[i]->length>joints_to_move[i]->max_length)   joints_to_move[i]->length=joints_to_move[i]->max_length;
                }
                if(joints_to_move[i]->slider_Force.dot(joints_to_move[i]->dir)<0)
                {
                    joints_to_move[i]->length-=slider_Force_len*velocity;
                    if(joints_to_move[i]->length<joints_to_move[i]->min_length)   joints_to_move[i]->length=joints_to_move[i]->min_length;
                }
            }

            skip_rot:
            if (rot_max_Force&&joints_to_move[i]->id!=max_Force_id)joints_to_move[i]->refresh_bone_axis();//обновление осей кости;

            //обновление global_position детей  (возможное место лага при включенном bone_disabling при дальнейшем развитии!!!!!!!!!!!!!)
            for(unsigned int j=0;j<joints_to_move[i]->childs_id.size();j++)
            {
                //нужна проврка на вхождение joints_to_move, для пересчёта потомков не входящих в цепь поворота
                abstract_joint* bone=links[joints_to_move[i]->childs_id[j]];
                bone->global_position=joints_to_move[i]->global_position+joints_to_move[i]->dir*joints_to_move[i]->length*bone->position(2)
                        +joints_to_move[i]->right*bone->position(0)+joints_to_move[i]->up*bone->position(1);
                // Обновление Угла ---------------------------------------
                if (!(rot_max_Force&&joints_to_move[i]->id!=max_Force_id)) childs_glob_trans_recalc(bone->id,rot_quat,velocity);
                   //bone->global_transform=rot_quat*bone->global_transform; //
                   //bone->refresh_bone_axis();//обновление осей кости
                   //bone->global_transform=bone->global_transform*rot_quat;
                  /* if(bone->id==2)
                   {
                       qDebug("Quat1: %f, %f, %f, %f",joints_to_move[i]->global_transform.x(),
                              joints_to_move[i]->global_transform.y(),joints_to_move[i]->global_transform.z(),joints_to_move[i]->global_transform.w());
                       qDebug("Quat2: %f, %f, %f, %f",bone->global_transform.x(),bone->global_transform.y(),bone->global_transform.z(),bone->global_transform.w());
                   }*/
            }


        }

        for(unsigned int i=0;i<joints_to_move.size();i++) //сброс сил после итерации-------------------
        {
            joints_to_move[i]->Result_Force.x()=0;
            joints_to_move[i]->Result_Force.y()=0;
            joints_to_move[i]->Result_Force.z()=0;
            joints_to_move[i]->Result_Force.w()=0;

            joints_to_move[i]->slider_Force=Vector3d(0,0,0);
        }

        joints_to_move.clear();
    }

}

void kinematic_chain::rotation_cycle(int steps,double velocity)
{
    for(int i=0;i<steps;i++)
    {
        rotation_step_calculate(velocity);
    }
}

void kinematic_chain::refresh_bones_axis(int id)
{
    Matrix3d global_transform_matr=links[id]->global_transform.toRotationMatrix();

    //оси после поворота звена
    links[id]->dir=global_transform_matr*Vector3d(0,0,-1);
    links[id]->right=global_transform_matr*Vector3d(1,0,0);
    links[id]->up=global_transform_matr* Vector3d(0,1,0);

    links[id]->dir.normalize();
    links[id]->right.normalize();
    links[id]->up.normalize();
}

void kinematic_chain::set_joint_Frict(int id,double new_K)
{
    links[id]->joint_Frict_K=new_K;
}

void kinematic_chain::set_rot_vel_K(int id,double new_K)
{
    links[id]->rot_vel_K=new_K;
}

void kinematic_chain::enable_bone_disabling(bool state)
{
    bone_disabling=state;
}

void kinematic_chain::enable_telescopic(int id,bool state,double min_l,double max_l,double slider_Frict_K)
{
    if(state)
    {
        links[id]->telescopic=state;
        links[id]->min_length=min_l;
        links[id]->max_length=max_l;
        if(links[id]->length>max_l)links[id]->length=max_l;
        if(links[id]->length<min_l)links[id]->length=min_l;
        links[id]->max_length=max_l;
        links[id]->slider_Frict_K=slider_Frict_K;
    }
    else
    {
        links[id]->telescopic=state;
        //links[id]->length=links[id]->min_length;
    }
}

void kinematic_chain::set_Force_normalizing(bool state)
{
    Force_normalizing=state;
}

void kinematic_chain::set_subtype(int id,int state)
{
    links[id]->subtype=state;
}

void kinematic_chain::set_ret_Force_K(int id,double new_K)
{
    links[id]->ret_Force_K=new_K;
}

void kinematic_chain::set_aa_limiter(int id,double a,Vector3d ang_ax)
{
    AngleAxisd new_aa(a,ang_ax);
    links[id]->aa_limiter.push_back(new_aa);
}

void kinematic_chain::del_aa_limiter(int id)
{
    links[id]->aa_limiter.pop_back();
}

void kinematic_chain::set_lim_angles(int id,double a1,double a2,double a3,double a4)
{
    links[id]->lim_ang[0]=a1;links[id]->lim_ang[1]=a2;links[id]->lim_ang[2]=a3;links[id]->lim_ang[3]=a4;
}

void kinematic_chain::del_link(int id)
{
    links[id]->del_mark=true;

    vector<ball_joint,Eigen::aligned_allocator<ball_joint> >::iterator p=ball_joints.begin();
    vector<hinge_joint,Eigen::aligned_allocator<hinge_joint> >::iterator p1=hinge_joints.begin();
    vector<hinge_ra,Eigen::aligned_allocator<hinge_ra> >::iterator p2=hinge_ra_joints.begin();
    unsigned int i=0;

    if(id==0)
    {
        links[1]->dir=links[0]->dir;
        links[1]->up=links[0]->up;
        links[1]->right=links[0]->right;
        links[1]->position=links[0]->position;
        links[1]->global_position=links[0]->global_position;
        links[1]->yaw=links[0]->yaw;
        links[1]->pitch=links[0]->pitch;
        links[1]->roll=links[0]->roll;
        Quaterniond q;
        links[1]->global_transform=q.Identity();
    }
    if(id==(get_size()-1)) links[id-1]->childs_id.clear();
    for(i=id+1;i<links.size();i++)
    {
        links[i]->id--;
        links[i]->parent_id--;
        for(unsigned int j=0;j<links[i]->childs_id.size();j++)
        {
            if(links[i]->childs_id[j]>id)links[i]->childs_id[j]--;
        }
    }

    switch(links[id]->type)
    {
        case 1:
        for(i=0;i<ball_joints.size();i++)
        {
            if(ball_joints[i].del_mark==true)
            {
                ball_joints.erase(p);
                break;
            }
            p++;
        }
        break;
        case 2:
        for(i=0;i<hinge_joints.size();i++)
        {
            if(hinge_joints[i].del_mark==true)
            {
                hinge_joints.erase(p1);
                break;
            }
            p1++;
        }
        break;
        case 3:
        for(i=0;i<hinge_ra_joints.size();i++)
        {
            if(hinge_ra_joints[i].del_mark==true)
            {
                hinge_ra_joints.erase(p2);
                break;
            }
            p2++;
        }
        break;
    }

    refresh_links();

    if(links[id]->id==0)
    {
        links[0]->parent_id=0;
        links[0]->root=true;
        Quaterniond q;
        links[0]->global_transform=q.Identity();
    }
    total_link_recalculation(0);

    effectors[0].id--;
}

void kinematic_chain::change_link_type(int id,int type)
{
    if(links[id]->type!=type)
    {

        links[id]->yaw=links[0]->yaw;
        links[id]->pitch=links[0]->pitch;
        links[id]->roll=links[0]->roll;
        Quaterniond q;
        links[id]->global_transform=q.Identity();
        refresh_links();
    }
}

void kinematic_chain::add_link_to_middle(int type,Vector3d in_pos,double in_lenght,int in_parent_id,double yaw,double pitch,double roll)
{
    add_link(type, in_pos,in_lenght,in_parent_id, yaw, pitch, roll);
    set_link_size(links.size()-1,0.6,0.6);
    enable_telescopic(links.size()-1,true,1,3,0.5);
    set_joint_Frict(links.size()-1,0.4);
    set_aa_limiter(links.size()-1,M_PI/4,Vector3d(0,0,-1));

    if(in_parent_id!=(links.size()-2))
    {

        for(int i=in_parent_id+1;i<links.size();i++)
        {
            links[i]->id++;
            links[i]->parent_id++;
            for(unsigned int j=0;j<links[i]->childs_id.size();j++)
            {
                links[i]->childs_id[j]++;
            }
        }
        links[in_parent_id]->childs_id.clear();
        links[in_parent_id]->childs_id.push_back(in_parent_id+1);
        links[links.size()-1]->childs_id.push_back(in_parent_id+2);
        links[links.size()-1]->parent_id=in_parent_id;
    }

    links[links.size()-1]->id=in_parent_id+1;


    refresh_links();
    total_link_recalculation(0);
    effectors[0].id++;
}
