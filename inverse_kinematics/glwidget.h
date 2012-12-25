
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <camera.h>
#include <box.h>
#include <coord_axis.h>
#include<C:\Qt_projects\Eigen\Dense>
#include <kinematic_chain.h>
#include <cylinder.h>
#include <prism.h>
#include <sphere.h>
#include <redact_joint.h>


/* MyGui Test*/

//#include <C:\MyGUI_3.2.0\MyGUI_3.2.0\MyGUIEngine\include\MyGui.h>
//#include <MyGUI.h>

/*MyGui---------------*/

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLWidget();


    /* MyGui Test*/

   // MyGUI::Gui* mGUI;

    /*MyGui---------------*/

    double aspect_ratio;
    int active_effector;
    double view_angle;
    bool target_selected,move_target,ctrl_pressed;
    bool stop_proc,main_stop;
    double move_vel;


//for tests
    bool test_trigger;
    int test_counter;
    QVector3D a,b;
//-------------------

    void setClearColor(const QColor &color);
    Camera cam;

    QVector4D light_pos;


    kinematic_chain KChain;


    void set_lp()
    {
        program->setUniformValue("light_pos",light_pos);
    }
//впеменно тут
    float poly_intersect( QVector4D origin_in, QVector4D direction_in, QVector4D p0_in, QVector4D p1_in, QVector4D p2_in,QVector4D& res_point )
    {
       QVector3D origin,direction,p0,p1,p2;
       origin=(QVector3D)origin_in;
       direction=(QVector3D)direction_in;
       p0=(QVector3D)p0_in;
       p1=(QVector3D)p1_in;
       p2=(QVector3D)p2_in;

       float result;

      QVector3D e1 = p1 - p0;
      QVector3D e2 = p2 - p0;

      p0 = origin - p0;

      QVector3D P = QVector3D::crossProduct( direction , e2 );

      float  det =  QVector3D::dotProduct( P , e1 );

      if( det == 0.0 )
        return -1000000.0;

      float u = QVector3D::dotProduct( P , p0 ) / det;

      P = QVector3D::crossProduct( p0 , e1 );

      float w = QVector3D::dotProduct( P , direction ) / det ;

      if( u + w > 1 || w < 0 || u < 0 )
        return 1000000.0;


      result=QVector3D::dotProduct( P , e2 ) / det;
      res_point=(QVector4D(origin+direction*result,1));
      return result;

    }

    float poly_intersect( QVector3D origin, QVector3D direction, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point )
    {
      float result;

      QVector3D e1 = p1 - p0;
      QVector3D e2 = p2 - p0;

      p0 = origin - p0;

      QVector3D P = QVector3D::crossProduct( direction , e2 );

      float  det =  QVector3D::dotProduct( P , e1 );

      if( det == 0.0 )
        return -1000000.0;

      float u = QVector3D::dotProduct( P , p0 ) / det;

      P = QVector3D::crossProduct( p0 , e1 );

      float w = QVector3D::dotProduct( P , direction ) / det ;

      if( u + w > 1 || w < 0 || u < 0 )
        return 1000000.0;


      result=QVector3D::dotProduct( P , e2 ) / det;
      res_point=origin+direction*result;
      return result;

    }

    bool  sphere_intersect(QVector3D origin_in, QVector3D direction_in, QVector3D sph_C,double sph_r)
    {
        double D,C,B;

        B=2*QVector3D::dotProduct(direction_in,origin_in-sph_C);
        C=QVector3D::dotProduct(origin_in-sph_C,origin_in-sph_C)-sph_r*sph_r;
        D=B*B-4.0*C;
        if (D<0) return false;
        else return true;
    }

//-----------------------------------------
signals:
    void clicked();
    void get_str(QString);
    void set_max_links(int ml);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent * event);

private:
    int w,h;
    QTimer *timer;
    QTime animation_timer;
    QMatrix4x4 v;
    QMatrix4x4 p;

    QGLFunctions *glFuncs;

    QColor clearColor;
    QPoint lastPos;
    GLuint textures[6];
    GLuint planet_texture[3];
    QGLShaderProgram *program;

    qreal test_angle;

    box* b1;
    box* b2;
    cylinder* c1;
    prism* pr1;
    sphere* sph1;
    Vector3d target_for_effector;

    coord_axis coordinate_axis;

private slots:
    void update_screen();
    void set_time_speed(int new_time_speed);
    void rotate_KChain_link(int id,double yaw,double pitch,double roll);
    void set_KChain_link_angles(int id,double yaw,double pitch,double roll);
};

#endif
