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

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:

    double aspect_ratio;
    int active_effector;
    double view_angle;
    bool target_selected, move_target, ctrl_pressed, move_root, root_selected,move_box;
    bool stop_proc,main_stop;
    bool move_rotate_object, redact_scene_mode;
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

    QOpenGLFramebufferObject* shadow_fbo;
    QOpenGLFramebufferObject* shadow_fbo_global_pos;

    CubeMapFBO* cube_fbo;
    QMatrix4x4 light_ViewMatrix[6];
    QMatrix4x4 light_ProjMatrix;
    QMatrix4x4 light_ProjViewMatrix[6];

    bool ssao, shadows, draw_edges;
    int selected_object_id;
    bool geometry_repulsion, target_shift;

    int movement_precision;

    QVector3D inetrs_point;
    int affected_link;

    QVector3D intersect_point;

    QVector3D box_intersect_point;
    QVector3D box_intersect_point_vec;

    figure box_axis; //only axes used

    enum coordAxisStates { xPlane, yPlane, zPlane };
    coordAxisStates current_axis_state;
    float current_axis_sign;

//for tests
    bool test_trigger;
    int test_counter;
    QVector3D a,b;
//-------------------


    Camera cam;
    QVector4D light_pos;
    kinematic_chain KChain;
    Scene* scene;

    GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLWidget();

    void set_lp(QVector4D d_light_pos)
    {
        light_pos += d_light_pos;
        program->setUniformValue("light_pos", light_pos);
        calculate_light_matrices();
    }

    void setClearColor(const QColor &color);
    float poly_intersect( QVector4D origin_in, QVector4D direction_in, QVector4D p0_in, QVector4D p1_in, QVector4D p2_in,QVector4D& res_point );
    float poly_intersect( QVector3D origin, QVector3D direction, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point );
    bool sphere_intersect(QVector3D origin_in, QVector3D direction_in, QVector3D sph_C,double sph_r);
    void draw_fullscreen_texture(GLuint texture, GLenum texture_type = GL_TEXTURE_RECTANGLE);
    void make_ssao_shaded_texture();
    void blur_fbo(QOpenGLFramebufferObject *fbo_1, QOpenGLFramebufferObject *fbo_2, int passes);
    void upsacle_interpolation(QOpenGLFramebufferObject *src, QOpenGLFramebufferObject *dest);
    void combine_textures();
    void draw_scene_to_fbo(QOpenGLFramebufferObject *fbo, QGLShaderProgram *pass_program);
    void calculate_light_matrices();
    void render_to_shadow_cubemap();
    void build_shadows();
    void set_ssao(bool state);
    void set_shadows(bool state);
    void add_box_scene(QVector3D pos,QVector3D scale, QVector3D color);
    void add_box_scene(QMatrix4x4 &rot_scale_matr, QMatrix4x4 &translate_matr, QVector4D &material);

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
    QMatrix4x4 v; //main view matrix
    QMatrix4x4 p; //perspective proje matrix

    QOpenGLFunctions *glFuncs;

    QColor clearColor;
    QPoint lastPos;
    GLuint textures[6];

    QGLShaderProgram *program;

    QGLShaderProgram *ssao_build_norm_program;
    QGLShaderProgram *ssao_build_pos_program;
    QGLShaderProgram *build_global_pos_program;
    QGLShaderProgram *ssao_build_shaded_texture_program;
    QGLShaderProgram *combine_textures_program;
    QGLShaderProgram *ssao_blur_program;
    QGLShaderProgram *upsacle_interpolation_program;

    QGLShaderProgram *fullscreen_texture_program;
    QGLShaderProgram *fullscreen_texture_program_2D;

    QGLShaderProgram *only_depth_program;
    QGLShaderProgram *build_shadow_program;

    QGLShaderProgram *line_program;

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
