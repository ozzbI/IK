#ifndef BALL_JOINT_H
#define BALL_JOINT_H

#include <abstract_joint.h>
#include <ctime>
class ball_joint:public abstract_joint
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    ball_joint(Vector3d in_pos,double in_lenght,int in_parent_id,double in_yaw,double in_pitch,double in_roll,bool in_root)
        :abstract_joint( in_pos, in_lenght, in_parent_id, in_yaw, in_pitch, in_roll, in_root)
    {
        type=1;
    }

    virtual bool rotation_possibility(Quaterniond q,double velocity)
    {
        Vector3d lim_axis,par_dir,axis;
        Quaterniond temp(0,0,0,0),rot_qt;
        double next_angle;

        AngleAxisd aa(q);
        q=Quaterniond(AngleAxisd(aa.angle()*velocity*rot_vel_K,aa.axis()));

        par_dir=Vector3d(0,0,-1);

        if(id!=0)
        {
            temp=Quaterniond(0,par_dir.x(),par_dir.y(),par_dir.z());
            rot_qt=parent->global_transform*local_transform; //оставить только это в if
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
        for(unsigned int i=0;i<aa_limiter.size();i++)
        {
            temp=Quaterniond(0,aa_limiter[i].axis().x(),aa_limiter[i].axis().y(),aa_limiter[i].axis().z());
            rot_qt=q*global_transform;
            temp=rot_qt*temp*rot_qt.inverse();
            lim_axis=Vector3d(temp.x(),temp.y(),temp.z());
            next_angle=angle(par_dir,lim_axis);
            if(next_angle>aa_limiter[i].angle()) return false;
        }

        return true;
    }

    virtual Vector3d calculate_Force(Vector3d& Force,double K,bool rand_ax,Vector3d& child_dir,double velocity, bool global_elastic)
    {
        Vector3d ret_Force,rot_vel,rot_axis, Force_loss;
        bool block=false;

        ret_Force=projection(dir,Force);
        rot_vel=Force-ret_Force;
        Force_loss=rot_vel*joint_Frict_K;
        rot_vel-=Force_loss;
        //qDebug("rot_vel: %f, %f, %f",rot_vel.x(),rot_vel.y(),rot_vel.z());
        rot_axis=dir.cross(rot_vel);

        //обработка слайдера
        if (telescopic)ret_Force=proc_slider(ret_Force, global_elastic);


        if(!(rot_axis.x()||rot_axis.y()||rot_axis.z()))
        {
            if(ret_Force.dot(dir)<0&&rand_ax)
            {
                Vector3d rand_axis;
                srand(time(0));
                rand_axis.x()=rand()%100;
                rand_axis.y()=rand()%100;
                rand_axis.z()=rand()%100;
                rand_axis.normalize();
                Quaterniond new_Force(AngleAxisd(rand()%10000, rand_axis));

                if(rotation_possibility(new_Force,velocity))
                {
                    Result_Force.w()=Result_Force.w()+new_Force.w();
                    Result_Force.x()=Result_Force.x()+new_Force.x();
                    Result_Force.y()=Result_Force.y()+new_Force.y();
                    Result_Force.z()=Result_Force.z()+new_Force.z();
                }
                else block=true;
            }
            if(block)(ret_Force+rot_vel)*ret_Force_K;
            //if(block)(ret_Force/*+rot_vel*/)*ret_Force_K;
            else return ret_Force*ret_Force_K;
        }

        double rot_vel_scalar=K*sqrt(rot_vel(0)*rot_vel(0)+rot_vel(1)*rot_vel(1)+rot_vel(2)*rot_vel(2));

        Quaterniond new_Force(AngleAxisd(rot_vel_scalar, rot_axis));

        if(rotation_possibility(new_Force,velocity))
        {
            Result_Force.w()=Result_Force.w()+new_Force.w();
            Result_Force.x()=Result_Force.x()+new_Force.x();
            Result_Force.y()=Result_Force.y()+new_Force.y();
            Result_Force.z()=Result_Force.z()+new_Force.z();
        }
        else block=true;

        if(block)return (ret_Force+Force_loss+rot_vel)*ret_Force_K;
       // if(block)return (ret_Force/*+Force_loss+rot_vel*/)*ret_Force_K;
        else return (ret_Force+Force_loss)*ret_Force_K;
    }
};

#endif // BALL_JOINT_H
