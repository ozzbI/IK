#ifndef ABSTARCT_JOINT_H
#define ABSTARCT_JOINT_H

#include<kchain_link.h>
//#include<kinematic_chain.h>
class abstract_joint:public Kchain_link
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    //~abstract_joint();


    bool telescopic;
    double max_length,min_length;
    double slider_Frict_K;
    Vector3d slider_Force;
    int type;
    int subtype;

    abstract_joint* parent;

    //ограничители
    vector<AngleAxisd,Eigen::aligned_allocator<AngleAxisd> > aa_limiter;
    double lim_ang[4];

    double coord_lim[8];

    // параметры упругости сустава
    bool joint_elastic;
    double joint_elastic_K;
    double joint_elastic_K_2;
    double joint_elastic_angle;
    // параметры упроугости слайдера
    bool slider_elastic;
    double slider_elastic_K;
    double slider_elastic_K_2;
    double slider_elastic_L;

    virtual bool rotation_possibility(Quaterniond q,double velocity)=0;
    virtual Vector3d calculate_Force(Vector3d& Force,double K,bool rand_ax,Vector3d& child_dir,double velocity,bool global_elastic)=0;


    abstract_joint(Vector3d in_pos,double in_lenght,int in_parent_id,double in_yaw,double in_pitch,double in_roll,bool in_root)
        :Kchain_link( in_pos, in_lenght, in_parent_id, in_yaw, in_pitch, in_roll, in_root)
    {
        subtype=1;
        telescopic=false;
        max_length=1;
        min_length=1;
        slider_Frict_K=0.5;
        slider_Force=Vector3d(0,0,0);

        //angle test
        /*Vector3d a(1,0,0),b(-1,1,0);
        double r;

        r=angle(a,b);
        //r=angle_2PI(a,b);

        r=0;*/

        // параметры упругости сустава
        joint_elastic = true;
        joint_elastic_K = 0.3;
        joint_elastic_K_2 = 0;
        joint_elastic_angle = 0;
        // параметры упроугости слайдера
        slider_elastic = true;
        slider_elastic_K = 0.3;
        slider_elastic_K_2 = 0;
        slider_elastic_L = 0;


    }

    Vector3d projection(const Vector3d& a,  const Vector3d& b)//b на a
    {
        Vector3d p = a*( a.dot(b)/(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()) ) ;
        return  p;
    }

    static double len(Vector3d& a)
    {
        return(sqrt(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()));
    }

    double angle(Vector3d& a, Vector3d& b)
    {
        return acos(a.dot(b)/(len(a)*len(b)));
    }

   /* double angle_2PI(Vector3d& a, Vector3d& b)
    {
        Vector3d c,d,proj_x,proj_y;
        double x,y;
        c=a.cross(b);
        if(!(c.x()||c.y()||c.z()))
        {
            return acos(a.dot(b)/(len(a)*len(b)));
        }
        d=c.cross(a);
        proj_x=projection(a,b);
        proj_y=projection(d,b);
        x=len(proj_x);
        y=len(proj_y);
        return atan2(x,y);
    }*/

    Vector3d proc_slider(Vector3d &Force, bool global_elastic)
    {
        Vector3d elastic_F;
        elastic_F = Vector3d(0,0,0);

        if(global_elastic && slider_elastic)
        {
           double L;
           L=length-min_length-slider_elastic_L;

           if(L>0) elastic_F=-dir*(slider_elastic_K*L+slider_elastic_K_2*L);
        }

        slider_Force+=elastic_F; //силы упругости

        if(Force.dot(dir)>0&&length==max_length)
        {
            return Force;
        }
        if(Force.dot(dir)<0&&length==min_length)
        {
            return Force;
        }

        slider_Force+=Force*slider_Frict_K;
        return Force*(1-slider_Frict_K);
    }


};

#endif // ABSTARCT_JOINT_H
