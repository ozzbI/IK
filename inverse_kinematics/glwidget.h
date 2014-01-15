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
#include <scene.h>
#include "cubemapfbo.h"


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



    QOpenGLFramebufferObject* ssao_fbo_normal;
    QOpenGLFramebufferObject* ssao_fbo_pos;
    QOpenGLFramebufferObject* ssao_fbo_shaded_texture;
    QOpenGLFramebufferObject* ssao_fbo_blur;

    QOpenGLFramebufferObject* upscaled_ssao_fbo;
    QOpenGLFramebufferObject* blur_upscaled_ssao_fbo;

    QOpenGLFramebufferObject* final_fbo;
    QOpenGLFramebufferObject* main_scene_fbo;
    QOpenGLFramebufferObject* main_scene_fbo_sampled;

    CubeMapFBO* cube_fbo;
    Camera light_cam;

    bool ssao;


    QVector3D intersect_point;

//for tests
    bool test_trigger;
    int test_counter;
    QVector3D a,b;
//-------------------

    void setClearColor(const QColor &color);
    Camera cam;

    QVector4D light_pos;


    kinematic_chain KChain;

    Scene* scene;


    void set_lp()
    {
        program->setUniformValue("light_pos",light_pos);
    }

    float poly_intersect( QVector4D origin_in, QVector4D direction_in, QVector4D p0_in, QVector4D p1_in, QVector4D p2_in,QVector4D& res_point );

    float poly_intersect( QVector3D origin, QVector3D direction, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point );

    bool  sphere_intersect(QVector3D origin_in, QVector3D direction_in, QVector3D sph_C,double sph_r);

    void draw_fullscreen_texture(GLuint texture, GLenum texture_type = GL_TEXTURE_RECTANGLE);

    void make_ssao_shaded_texture();

    void blur_fbo(QOpenGLFramebufferObject *fbo_1, QOpenGLFramebufferObject *fbo_2, int passes);

    void upsacle_interpolation(QOpenGLFramebufferObject *src, QOpenGLFramebufferObject *dest);

    void combine_textures();

    void draw_scene_to_fbo(QOpenGLFramebufferObject *fbo, QGLShaderProgram *pass_program);

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

    QGLShaderProgram *program;

    QGLShaderProgram *ssao_build_norm_program;
    QGLShaderProgram *ssao_build_pos_program;
    QGLShaderProgram *ssao_build_shaded_texture_program;
    QGLShaderProgram *combine_textures_program;
    QGLShaderProgram *ssao_blur_program;
    QGLShaderProgram *upsacle_interpolation_program;

    QGLShaderProgram *fullscreen_texture_program;
    QGLShaderProgram *fullscreen_texture_program_2D;

    GLuint test_pic;
    GLuint ssao_rot_texture;

    QVector<QVector4D> scren_plane_vertex;
    QVector<QVector2D> scren_plane_tex_coord;

    qreal test_angle;

    box* b1;
    box* b2;
    cylinder* c1;
    prism* pr1;
    sphere* sph1;

    box* collision_box;

    Vector3d target_for_effector;

    coord_axis coordinate_axis;

private slots:
    void update_screen();
    void set_time_speed(int new_time_speed);
    void rotate_KChain_link(int id,double yaw,double pitch,double roll);
    void set_KChain_link_angles(int id,double yaw,double pitch,double roll);

    void draw_collision_box(QVector3D collision_point);
};

#endif
