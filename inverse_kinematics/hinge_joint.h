#ifndef HINGE_JOINT_H
#define HINGE_JOINT_H

#include <abstract_joint.h>

class hinge_joint:public abstract_joint
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    hinge_joint(Vector3d in_pos,double in_lenght,int in_parent_id,double in_yaw,double in_pitch,double in_roll,bool in_root)
        :abstract_joint( in_pos, in_lenght, in_parent_id, in_yaw, in_pitch, in_roll, in_root)
    {
        type=2;
        lim_ang[0]=M_PI/2;
        lim_ang[1]=-M_PI/2;
    }



    virtual bool rotation_possibility(Quaterniond q,double velocity)
    {//нет обработки нескольких таргетов
        Vector3d direct,par_dir,axis;
        Quaterniond temp(0,0,0,0),rot_qt;
        double next_angle;

        AngleAxisd aa(q);
        q=Quaterniond(AngleAxisd(aa.angle()*velocity*rot_vel_K,aa.axis()));


        temp=Quaterniond(0,dir.x(),dir.y(),dir.z());
        rot_qt=q;
        temp=rot_qt*temp*rot_qt.inverse();
        direct=Vector3d(temp.x(),temp.y(),temp.z());


        par_dir=Vector3d(0,0,-1);

        if(id!=0)
        {
            temp=Quaterniond(0,par_dir.x(),par_dir.y(),par_dir.z());
            rot_qt=parent->global_transform*local_transform;
            temp=rot_qt*temp*rot_qt.inverse();
            par_dir=Vector3d(temp.x(),temp.y(),temp.z());
        }
        else
        {
            temp=Quaterniond(0,par_dir.x(),par_dir.y(),par_dir.z());
            rot_qt=local_transform;
            temp=rot_qt*temp*rot_qt.inverse();
            par_dir=Vector3d(temp.x(),temp.y(),temp.z());
        }

        axis = direct.cross(par_dir);
        if(!(axis.x()||axis.y()||axis.z())) next_angle=0;
        else
        {
            if(axis.dot(right)>0) next_angle = angle(direct,par_dir);
            if(axis.dot(right)<0) next_angle = -angle(direct,par_dir);
        }

        //if(id==1)qDebug("angle: %f",next_angle);

        if(lim_ang[1] < next_angle && next_angle < lim_ang[0])return true;
        else return false;
    }

    virtual Vector3d calculate_Force(Vector3d& Force,double K,bool rand_ax,Vector3d& child_dir,double velocity,bool global_elastic)
    {
        Vector3d ret_Force, rot_vel, Force_loss, dir_proj,
                 test_axis, slider_Force_local, norm_Force, elastic_Force;

        bool block=false;

        elastic_Force = Vector3d(0,0,0);

        if(subtype == 2)
        {
            Vector3d temp;
            temp = up;
            up = right;
            right = temp;
        }

        if(global_elastic && joint_elastic) //обработка сил упругости (копипаста с боллджоинта)
        {
            //да это кривой код

            double dir_par_dir_angle=0; //угол между текущим направлением и направлением родителя
            Vector3d par_dir; //parent bone direction
            Quaterniond temp(0,0,0,0),rot_qt;

            par_dir=Vector3d(0,0,-1);

            if(id!=0)
            {
                temp = Quaterniond(0,par_dir.x(),par_dir.y(),par_dir.z());
                rot_qt = parent->global_transform * local_transform; //оставить только это в if
                temp = rot_qt * temp * rot_qt.inverse();
                par_dir = Vector3d(temp.x(),temp.y(),temp.z());
            }
            else
            {
                temp = Quaterniond(0,par_dir.x(),par_dir.y(),par_dir.z());
                rot_qt = local_transform;
                temp = rot_qt * temp * rot_qt.inverse();
                par_dir = Vector3d(temp.x(),temp.y(),temp.z());
            }

            dir_par_dir_angle=angle(dir,par_dir)-joint_elastic_angle;

            if(dir_par_dir_angle>0)
            {
                // !!! не учтено K^2
                elastic_Force = projection(par_dir,dir);
                elastic_Force.normalize();
                elastic_Force *= dir_par_dir_angle*joint_elastic_K;
            }
        }

        dir_proj=projection(dir,Force);
        rot_vel=projection(up,Force);

        Force_loss=rot_vel*joint_Frict_K;
        rot_vel-=Force_loss;

        rot_vel+=elastic_Force;

        //обработка слайдера
        if (telescopic)
        {
            slider_Force_local=proc_slider(dir_proj, global_elastic);
            ret_Force=Force-rot_vel-dir_proj+slider_Force_local;
        }
        else ret_Force=Force-rot_vel;



        test_axis=dir.cross(Force);

        if(!(test_axis.x()||test_axis.y()||test_axis.z()))
        {
            if(ret_Force.dot(dir)<0&&rand_ax)
            {
                Vector3d rand_axis;
                srand(time(0));
                rand_axis.x() = rand()%100;
                rand_axis.y() = rand()%100;
                rand_axis.z() = rand()%100;
                rand_axis.normalize();
                Quaterniond new_Force(AngleAxisd(rand()%10000, right));

                if(rotation_possibility(new_Force,velocity))
                {
                    Result_Force.w() = Result_Force.w()+new_Force.w();
                    Result_Force.x() = Result_Force.x()+new_Force.x();
                    Result_Force.y() = Result_Force.y()+new_Force.y();
                    Result_Force.z() = Result_Force.z()+new_Force.z();
                }
                else block=true;
            }

            norm_Force=projection(up,Force);
            if(subtype==2)
            {
                Vector3d temp;
                temp = up;
                up = right;
                right = temp;
            }

            if(block) return (ret_Force+rot_vel-elastic_Force/*-Force_loss*/) * ret_Force_K;
            //if(block) return (Force-norm_Force)*ret_Force_K;
            else return ret_Force*ret_Force_K;
        }

        double rot_vel_scalar=K*sqrt(rot_vel(0)*rot_vel(0) +
                                     rot_vel(1)*rot_vel(1) +
                                     rot_vel(2)*rot_vel(2));

        if(Force.dot(up)<0&&subtype == 1) rot_vel_scalar = - rot_vel_scalar;
        if(Force.dot(up)>0&&subtype == 2) rot_vel_scalar = - rot_vel_scalar;

        Quaterniond new_Force(AngleAxisd(rot_vel_scalar, right));

        if(rotation_possibility(new_Force, velocity))
        {
            Result_Force.w() = Result_Force.w()+new_Force.w();
            Result_Force.x() = Result_Force.x()+new_Force.x();
            Result_Force.y() = Result_Force.y()+new_Force.y();
            Result_Force.z() = Result_Force.z()+new_Force.z();
        }
        else block=true;

        norm_Force=projection(up,Force);
        if(subtype == 2)
        {
            Vector3d temp;
            temp = up;
            up = right;
            right = temp;
        }
        if(block)return (ret_Force + rot_vel - elastic_Force/*-Force_loss*/) * ret_Force_K; //!
        //if(block)return (Force-norm_Force)*ret_Force_K;
        else return ret_Force * ret_Force_K;
        //else return (Force-norm_Force)*ret_Force_K;
    }

};

#endif // HINGE_JOINT_H
