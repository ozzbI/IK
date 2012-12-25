#ifndef KCHAIN_LINK_H
#define KCHAIN_LINK_H

#include<C:\Qt_projects\Eigen\Dense>
#include<vector>
#include<box.h>
using namespace Eigen;
using namespace std;
class Kchain_link
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    double yaw,pitch,roll;
    double length,width,height;

    double ret_Force_K;
    double joint_Frict_K; // ���������� ������ � �������
    double rot_vel_K;//���������� �������� ��������

    Vector3d position;//for non-root links is used only x
    Vector3d global_position;//parent's end position (joint position)

    Quaterniond chain_transform; // ����� ������� ���� (���������� ����������� �����)
    Quaterniond local_transform; // ������� ����� (���������� ������ yaw*pitch*roll - �������� ������������ ����-�� �����)
    Quaterniond global_transform; // ������� ����� (chain_transform+local_transform)

    //������� ����������� �����
    Vector3d dir;
    Vector3d up;
    Vector3d right;


    //������� ����������� ������������� �����
    Vector3d joint_dir;
    Vector3d joint_up;
    Vector3d joint_right;

    int id,parent_id;
    vector<int> childs_id;
    bool block,root,del_mark;

    box b;

    //vector<Quaterniond,Eigen::aligned_allocator<Quaterniond> > rot_Forces;//���� ����������� �� �����

    Quaterniond Result_Force;

    Kchain_link();
    Kchain_link(Vector3d in_pos,double in_lenght,int in_parent_id,double in_yaw,double in_pitch,double in_roll,bool in_root);
    void add_child(int child_id);
    void calc_local_transform();
    void refresh_bone_axis();
    void draw();
};

#endif // KCHAIN_LINK_H
