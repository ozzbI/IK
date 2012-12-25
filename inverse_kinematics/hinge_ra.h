#ifndef HINGE_RA_H
#define HINGE_RA_H

#include <abstract_joint.h>

class hinge_ra:public abstract_joint
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    hinge_ra(Vector3d in_pos,double in_lenght,int in_parent_id,double in_yaw,double in_pitch,double in_roll,bool in_root)
        :abstract_joint( in_pos, in_lenght, in_parent_id, in_yaw, in_pitch, in_roll, in_root)
    {
        type=3;
    }



    virtual bool rotation_possibility(Quaterniond q,double velocity)
    {
        return 3;
    }

    virtual Vector3d calculate_Force(Vector3d& Force,double K,bool rand_ax,Vector3d& child_dir,double velocity,bool global_elastic)
    {
        Vector3d ret_Force,rot_vel, Force_loss,proj_axis,dir_proj,test_axis,slider_Force_local;


        proj_axis=dir.cross(child_dir);
        rot_vel=projection(proj_axis,Force);
        Force_loss=rot_vel*joint_Frict_K;
        rot_vel-=Force_loss;
        dir_proj=projection(dir,Force);

        //обработка слайдера
        if (telescopic)
        {
            slider_Force_local=proc_slider(dir_proj, global_elastic);
            if((child_dir.x()==dir.x())&&(child_dir.y()==dir.y())&&(child_dir.z()==dir.z()))
                return (Force-dir_proj+slider_Force_local)*ret_Force_K;
            ret_Force=Force-rot_vel-dir_proj+slider_Force_local;
        }
        else
        {
            if((child_dir.x()==dir.x())&&(child_dir.y()==dir.y())&&(child_dir.z()==dir.z()))
                return Force*ret_Force_K;
            ret_Force=Force-rot_vel;
        }

        test_axis=dir.cross(Force);

        if(!(test_axis.x()||test_axis.y()||test_axis.z()))
            return ret_Force*ret_Force_K;

        double rot_vel_scalar=K*sqrt(rot_vel(0)*rot_vel(0)+rot_vel(1)*rot_vel(1)+rot_vel(2)*rot_vel(2));

        if(Force.dot(proj_axis)<0) rot_vel_scalar=-rot_vel_scalar;

        Quaterniond new_Force(AngleAxisd(rot_vel_scalar, dir));

        Result_Force.w()=Result_Force.w()+new_Force.w();
        Result_Force.x()=Result_Force.x()+new_Force.x();
        Result_Force.y()=Result_Force.y()+new_Force.y();
        Result_Force.z()=Result_Force.z()+new_Force.z();


        return ret_Force*ret_Force_K;
    }

};

#endif // HINGE_RA_H
