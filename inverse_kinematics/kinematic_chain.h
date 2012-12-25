 #ifndef KINEMATIC_CHAIN_H
#define KINEMATIC_CHAIN_H

#include <kchain_link.h>
#include<C:\Qt_projects\Eigen\Dense>
#include <box.h>
#include <QtOpenGL>
#include <link_vec.h>
#include <sphere.h>
#include <cylinder.h>
#include <ball_joint.h>
#include <hinge_joint.h>
#include <hinge_ra.h>

using namespace Eigen;

class kinematic_chain
{
private:

    Kchain_link* link;
    QGLShaderProgram *program;
    box link_box;
    cylinder cyl;
    cylinder cyl_ra;
    cylinder cyl_t2;
    sphere sph;
    bool bone_disabling;//отключение поворота для кости если расстояние от эффекторра до цели меньше длины потомков
    bool Force_normalizing;
    bool rot_max_Force;
    int max_Force_id;

public:

    vector<ball_joint,Eigen::aligned_allocator<ball_joint> > ball_joints;
    vector<hinge_joint,Eigen::aligned_allocator<hinge_joint> > hinge_joints;
    vector<hinge_ra,Eigen::aligned_allocator<hinge_ra> > hinge_ra_joints;
    vector<abstract_joint*> links;
    //выбранное звено
    int chosen_link_id;

    class effector
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        int id;
        double K;
        Vector3d target;
    };
    vector<effector,Eigen::aligned_allocator<effector> > effectors;

    kinematic_chain();
    void Eigen_Matr_to_Qt_Matr(QMatrix4x4 &qm,Matrix3d &em);
    void add_link(int type,Vector3d in_pos,double in_lenght,int in_parent_id,double yaw,double pitch,double roll);
    void total_link_recalculation(int start_link_id);
    void childs_recalculation(int start_link_id);
    void setshaderprog(QGLShaderProgram* new_prog);
    void rotate_link(int link_id,double yaw,double pitch, double roll);
    void set_link_angles(int link_id,double yaw,double pitch, double roll);
    int get_size();
    void set_link_size(int link_id,double width,double height);
    void refresh_links();//обновление масиива указателей на шариниры после добавления
    void draw_chain();//основная ф-я отрисовки цепи
    void add_effector(int joint_id,double K,Vector3d target);//добавление эффектора
    void del_link(int id);
    void change_link_type(int id,int type);
    void add_link_to_middle(int type,Vector3d in_pos,double in_lenght,int in_parent_id,double yaw,double pitch,double roll);
    void delete_effector(int joint_id);
    Vector3d projection(const Vector3d& a,  const Vector3d& b);//проекйция b на ф
    void Forces_calculation(double velocity);// расчёт сил действующих на шарниры
    void rotation_step_calculate(double velocity);//поворот всех шарниров с учётом расчитанных сил (1 раз)
    void rotation_cycle(int steps,double velocity);//цикл нескольких поворотов шарниров с пересчётом сил
    void set_effector_target(int joint_id,Vector3d target);//установка цели для эффектора
    void refresh_bones_axis(int id);
    void set_joint_Frict(int id,double new_K);
    void set_rot_vel_K(int id,double new_K);
    void enable_bone_disabling(bool state);
    void enable_telescopic(int id,bool state,double min_l,double max_l,double slider_Frict_K);
    void set_Force_normalizing(bool state);
    void childs_glob_trans_recalc(int start_link_id,Quaterniond &q,double velocity);
    void set_subtype(int id,int state);
    void set_ret_Force_K(int id,double new_K);
    void set_aa_limiter(int id,double ang_ax,Vector3d a);
    void del_aa_limiter(int id);
    void set_lim_angles(int id,double a1,double a2,double a3,double a4);

    static double len(Vector3d& a)
    {
        return(sqrt(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()));
    }
};

#endif // KINEMATIC_CHAIN_H
