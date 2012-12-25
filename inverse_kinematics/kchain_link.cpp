#include "kchain_link.h"

Kchain_link::Kchain_link(Vector3d in_pos,double in_length,int in_parent_id,double in_yaw=0,double in_pitch=0,double in_roll=0,bool in_root=false)
{
    yaw=in_yaw;
    pitch=in_pitch;
    roll=in_roll;

    dir=Vector3d(0,0,-1);
    right=Vector3d(1,0,0);
    up=Vector3d(0,1,0);

    joint_dir=Vector3d(0,0,-1);
    joint_right=Vector3d(1,0,0);
    joint_up=Vector3d(0,1,0);

    block=false;
    del_mark=false;
    root=in_root;

    position=in_pos;
    length=in_length;
    parent_id=in_parent_id;

    width=0.2;
    height=0.2;

    joint_Frict_K=0;
    rot_vel_K=1;
    ret_Force_K=1;

    //Result_Force=Quaterniond(0,0,0,0);
    Result_Force=Result_Force.Identity();
    Result_Force.Identity();
}

void Kchain_link::refresh_bone_axis()
{
    Matrix3d global_transform_matr=global_transform.toRotationMatrix();

    //оси после поворота звена
    dir=global_transform_matr*Vector3d(0,0,-1);
    right=global_transform_matr*Vector3d(1,0,0);
    up=global_transform_matr* Vector3d(0,1,0);

    dir.normalize();
    right.normalize();
    up.normalize();
}

Kchain_link::Kchain_link()
{

}

void Kchain_link::add_child(int child_id)
{
    childs_id.push_back(child_id);
}

void Kchain_link::calc_local_transform()
{

}

void Kchain_link::draw()
{

}
