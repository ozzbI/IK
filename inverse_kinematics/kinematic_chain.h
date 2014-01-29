 #ifndef KINEMATIC_CHAIN_H
#define KINEMATIC_CHAIN_H

#include <kchain_link.h>
#include <C:\Qt_projects\Eigen\Dense>
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
    bool bone_disabling; //отключение поворота для кости если расстояние от эффекторра до цели меньше длины потомков
    bool Force_normalizing;
    bool rot_max_Force;
    int max_Force_id;

    bool TARGET_ACTIVATED;
    bool ELASTIC_GLOBAL;

public:

    QGLShaderProgram *line_program;//debug

    vector<ball_joint,Eigen::aligned_allocator<ball_joint> > ball_joints;
    vector<hinge_joint,Eigen::aligned_allocator<hinge_joint> > hinge_joints;
    vector<hinge_ra,Eigen::aligned_allocator<hinge_ra> > hinge_ra_joints;
    vector<abstract_joint*> links;

    //links backup for detect collision

    vector<ball_joint,Eigen::aligned_allocator<ball_joint> > ball_joints_backup;
    vector<hinge_joint,Eigen::aligned_allocator<hinge_joint> > hinge_joints_backup;
    vector<hinge_ra,Eigen::aligned_allocator<hinge_ra> > hinge_ra_joints_backup;
    vector<abstract_joint*> links_backup;

    bool collision;
    bool root_selected;

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
    static void Eigen_Matr_to_Qt_Matr(QMatrix4x4 &qm,Matrix3d &em);
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
    void set_max_Force_rotation(bool state);
    void childs_glob_trans_recalc(int start_link_id,Quaterniond &q,double velocity);
    void glob_pos_recalc(int start_link_id, int parent_id);
    void set_subtype(int id,int state);
    void set_ret_Force_K(int id,double new_K);
    void set_aa_limiter(int id,double ang_ax,Vector3d a);
    void del_aa_limiter(int id);
    void set_lim_angles(int id,double a1,double a2,double a3,double a4);

    void set_TARGET_ACTIVATED(bool state);
    void set_ELASTIC_GLOBAL(bool state);

    void set_joint_elastic(int id,bool state,double K,double K2,double angle);
    void set_slider_elastic(int id,bool state,double K,double K2,double L);

    void calculate_repulsion_Force(QVector3D &inetrs_point, int affected_link, float precision);

    static double len(Vector3d& a)
    {
        return(sqrt(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()));
    }

    void set_shader_prog(QGLShaderProgram *program)
    {
        link_box.setshaderprog(program);
        cyl.setshaderprog(program);
        cyl_ra.setshaderprog(program);
        cyl_t2.setshaderprog(program);
        sph.setshaderprog(program);
    }

    void backup_links()
    {
        if(!collision)
        {
            ball_joints_backup = ball_joints;
            hinge_joints_backup = hinge_joints;
            hinge_ra_joints_backup = hinge_ra_joints;
        }
    }

    void restore_links()
    {
        ball_joints = ball_joints_backup;
        hinge_joints = hinge_joints_backup;
        hinge_ra_joints = hinge_ra_joints_backup;
    }
};

#endif // KINEMATIC_CHAIN_H
