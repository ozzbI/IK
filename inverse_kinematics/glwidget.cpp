#include "glwidget.h"
//#include <QtGui>


GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
 /*   QGLFormat frmt; // создать формат по умолчанию
       frmt.setSampleBuffers(true); // задать простую буферизацию
       frmt.setSamples(4);
       setFormat(frmt); // установить формат в контекст*/


    QGLContext *con;
    QGLFormat fm;
    fm.setProfile(QGLFormat::CoreProfile);
    fm.setAlpha(true);
    fm.setDoubleBuffer(true);
    fm.setVersion(3,3);
    fm.setSampleBuffers(true);
    fm.setSamples(4);
    fm.setAccum(true);
    fm.setDepth(true);
    fm.setDirectRendering(true);
    con = new QGLContext(fm);
    con->setFormat(fm);
    con->create();
    con->makeCurrent();

    view_angle = 60;
    target_selected = false;
    move_target = false;
    stop_proc = false;
    main_stop = false;
    move_vel = 1;
    ctrl_pressed = false;

    clearColor = Qt::black;
    program = 0;

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_screen()));
    timer->start(20);

    cam.setPosition(Vector3d(0,0,10));

    //light_pos=QVector4D(0,20,0,0);

    b1 =new box();
    b2 =new box();
    b1->model.translate(0,2,-4);
    c1=new cylinder();
    c1->model.translate(0,0,-5);


    sph1=new sphere();
    sph1->make_sphere(0,0,0,0.3,50);
    sph1->model.translate(5,0,5);
    target_for_effector=Vector3d(7,7,7);


    pr1=new prism();
    pr1->model.translate(0,2,0);
    QMatrix4x4 bm;
    bm.rotate(70,1,0,0);

    b2->model.translate(2,1,2);


    KChain.set_Force_normalizing(false);
    //test chain
    KChain.enable_bone_disabling(false);

    //test 1
   /* int size=4;
    KChain.add_link(1,Vector3d(0,0,0),2,0,0,0,0);
    KChain.set_link_size(0,0.3,0.3);
    for (int i=0;i<size-1;i++)
    {
        KChain.add_link(1,Vector3d(0,0,1),1,i,0,0,0);
        KChain.set_link_size(i+1,0.05,0.05);
        KChain.enable_telescopic(i+1,true,1,3,0.5);
    }

    KChain.add_link(2,Vector3d(0,0,1),1.12,size-1,0,0,0);
    KChain.set_link_size(size,0.05,0.05);
    KChain.enable_telescopic(size,true,1,2,0.5);



    KChain.add_effector(size-1+1,1,target_for_effector);*/


    //test 2 good

    /*KChain.add_link(2,Vector3d(0,0,0),2,0,0,0,0);
    KChain.set_link_size(0,0.3,0.3);
    KChain.set_joint_Frict(0,0.4);

    KChain.add_link(2,Vector3d(0,0,1),2,0,0,0,0);
    KChain.set_link_size(1,0.6,0.6);
    KChain.enable_telescopic(1,true,1,3,0.5);
    KChain.set_joint_Frict(1,0.4);

    KChain.add_link(1,Vector3d(0,0,1),2,1,0,0,0);
    KChain.set_link_size(2,0.6,0.6);
    KChain.enable_telescopic(2,true,1,3,0.5);
    KChain.set_subtype(2,2);
    KChain.set_joint_Frict(2,0.4);
    KChain.set_aa_limiter(2,M_PI/4,Vector3d(0,0,-1));

    KChain.add_link(3,Vector3d(0,0,1),2,2,0,0,0);
    KChain.set_link_size(3,0.6,0.6);
    KChain.enable_telescopic(3,true,1,3,0.5);
    KChain.set_joint_Frict(3,0.4);

    KChain.add_link(2,Vector3d(0,0,1),2,3,0,0,0);
    KChain.set_link_size(4,0.6,0.6);
    KChain.enable_telescopic(4,true,1,3,0.5);
    KChain.set_subtype(4,2);
    KChain.set_joint_Frict(4,0.4);

    KChain.add_effector(4,1,target_for_effector);*/

    //KChain.total_link_recalculation(0);

 //test 5 good HINGE ONLY
    /*KChain.add_link(2,Vector3d(0,0,0),2,0,0,0,0);
    KChain.set_link_size(0,0.3,0.3);
    KChain.set_joint_Frict(0,0.4);
    KChain.set_lim_angles(0,M_PI/1.5,-M_PI,0.0,0.0);

    KChain.add_link(2,Vector3d(0,0,1),3,0,0,0,0);
    KChain.set_link_size(1,0.6,0.6);
    KChain.enable_telescopic(1,true,1,3,0.5);
    KChain.set_joint_Frict(1,0.4);

    KChain.add_link(2,Vector3d(0,0,1),2,1,0,0,0);
    KChain.set_link_size(2,0.6,0.6);
    KChain.enable_telescopic(2,true,1,3,0.5);
    KChain.set_joint_Frict(2,0.4);
    KChain.set_subtype(2,2);

    KChain.add_link(2,Vector3d(0,0,1),2,2,0,0,0);
    KChain.set_link_size(3,0.6,0.6);
    KChain.enable_telescopic(3,true,1,3,0.5);
    KChain.set_joint_Frict(3,0.4);
    KChain.set_subtype(3,1);

    KChain.add_link(2,Vector3d(0,0,1),2,3,0,0,0);
    KChain.set_link_size(4,0.6,0.6);
    KChain.enable_telescopic(4,true,1,3,0.5);
    KChain.set_joint_Frict(4,0.4);
    KChain.set_subtype(4,2);

    KChain.add_effector(4,1,target_for_effector);
*/
    //test 6 BALL

        /*KChain.add_link(1,Vector3d(0,0,0),2,0,0,0,0);
        KChain.set_link_size(0,0.3,0.3);
        KChain.set_joint_Frict(0,0.4);
        KChain.set_aa_limiter(0,M_PI/3,Vector3d(0,0,-1));


        KChain.add_link(1,Vector3d(0,0,1),3,0,0,0,0);
        KChain.set_link_size(1,0.6,0.6);
        KChain.enable_telescopic(1,true,1,3,0.5);
        KChain.set_joint_Frict(1,0.4);
        KChain.set_aa_limiter(1,M_PI/3,Vector3d(0,0,-1));

        KChain.add_link(1,Vector3d(0,0,1),3,1,0,0,0);
        KChain.set_link_size(2,0.6,0.6);
        KChain.enable_telescopic(2,true,1,3,0.5);
        KChain.set_joint_Frict(2,0.4);
        KChain.set_aa_limiter(2,M_PI/3,Vector3d(0,0,-1));

        KChain.add_effector(2,1,target_for_effector);*/

    //test 7 BALLCHAIN

    int size=10;

    KChain.add_link(1,Vector3d(0,0,0),2,0,0,M_PI/2,0);
    KChain.set_link_size(0,0.3,0.3);
    KChain.set_joint_Frict(0,0.4);
    KChain.set_aa_limiter(0,M_PI/6,Vector3d(0,0,-1));
    KChain.set_lim_angles(0,M_PI/6,-M_PI/6,0,0);

    for (int i=0;i<size;i++)
    {
        KChain.add_link(1,Vector3d(0,0,1),3,i,0,0,0);
        KChain.set_link_size(i+1,0.6,0.6);
        KChain.enable_telescopic(i+1,true,1,3,0.5);
        KChain.set_joint_Frict(i+1,0.4);
        KChain.set_aa_limiter(i+1,M_PI/3,Vector3d(0,0,-1));
        KChain.set_lim_angles(i+1,M_PI/6,-M_PI/6,0,0);
    }


    KChain.add_effector(size,1,target_for_effector);

    //test 3 good )))
  /*  int size=100;
    KChain.add_link(1,Vector3d(0,0,0),2,0,0,0,0);
    KChain.set_link_size(0,0.3,0.3);
   // KChain.set_joint_Frict(0,0.98);

    for (int i=0;i<size-1;i++)
    {
        KChain.add_link(1,Vector3d(0,0,1),0.6,i,0,0,0);
        KChain.set_link_size(i+1,0.05,0.05);
        KChain.enable_telescopic(i+1,true,0.5,1,0.5);
        //KChain.set_joint_Frict(i+1,0.98);

        //KChain.set_rot_vel_K(i+1,0.4);

        if(i%2==0)KChain.set_subtype(i+1,1);
        else KChain.set_subtype(i+1,2);
    }
    KChain.add_effector(size-1,1,target_for_effector);
*/
    //test chain 4


  /*  KChain.add_link(2,Vector3d(0,0,0),3,0,0,0,0);
    KChain.set_link_size(0,0.3,0.3);
    //KChain.set_joint_Frict(0,0.4);
   KChain.enable_telescopic(0,true,2,4,0.5);


   KChain.add_link(1,Vector3d(0,0,1),3,0,0,0,0);
   KChain.set_link_size(1,0.3,0.3);
  // KChain.set_joint_Frict(1,0.4);
   KChain.enable_telescopic(1,true,2,4,0.5);
   KChain.set_ret_Force_K(1,0.6);

    KChain.add_link(2,Vector3d(0,0,1),3,1,0,0,0);
    KChain.set_link_size(2,0.3,0.3);
   // KChain.set_joint_Frict(1,0.4);
    KChain.enable_telescopic(2,true,2,4,0.5);
    KChain.set_ret_Force_K(2,0.6);

    KChain.add_link(3,Vector3d(0,0,1),2,2,0,0,0);
    KChain.set_link_size(3,0.3,0.3);
   // KChain.set_joint_Frict(2,0.4);
    KChain.enable_telescopic(2,true,2,4,0.5);
    KChain.set_ret_Force_K(3,0.6);

    KChain.add_link(1,Vector3d(0,0,1),3,3,0,0,0);
    KChain.set_link_size(4,0.3,0.3);
   // KChain.set_joint_Frict(1,0.4);
    KChain.enable_telescopic(4,true,2,4,0.5);
    KChain.set_ret_Force_K(4,0.6);

    KChain.add_link(2,Vector3d(0,0,1),2,4,0,0,0);
    KChain.set_link_size(5,0.3,0.3);
    KChain.enable_telescopic(5,true,2,4,0.5);
    //KChain.enable_telescopic(1,true,1,3,0.5);
    KChain.set_ret_Force_K(5,0.6);
KChain.add_effector(5,1,target_for_effector);
*/
   /* KChain.add_link(1,Vector3d(0,0,1),2,1,0,0,0);
    KChain.set_link_size(2,0.3,0.3);
    KChain.enable_telescopic(2,true,1,3,0.5);

    KChain.add_link(1,Vector3d(0,0,1),2,2,0,0,0);
    KChain.set_link_size(3,0.3,0.3);
    KChain.enable_telescopic(3,true,1,3,0.5);
*/



    //Hand

  /*  KChain.add_effector(2,1,target_for_effector);
    //0
    KChain.add_link(1,Vector3d(0,0,0),4,0,0,0,0);
    KChain.set_link_size(0,0.7,0.7);
    KChain.enable_telescopic(0,true,1,10,0.5);
    //KChain.set_rot_vel_K(0,0.3);
    //1
    KChain.add_link(1,Vector3d(0,0,1),3,0,0,0,0);
    KChain.set_link_size(1,0.5,0.5);

    KChain.add_link(1,Vector3d(0,0,1),1,1,0,0,0);
    KChain.set_link_size(2,0.5,0.5);
    KChain.enable_telescopic(2,true,2,4,0.3);
*/


    //KChain.set_rot_vel_K(1,0.4);
   /*//2 f1
    KChain.add_link(1,Vector3d(0,0,1),4,1,M_PI/7,0,0);
    KChain.set_link_size(2,0.3,0.3);
   // KChain.set_rot_vel_K(2,0.5);
    //3
    KChain.add_link(1,Vector3d(0,0,1),2.8,2,0,0,0);
    KChain.set_link_size(3,0.2,0.2);
    //KChain.set_rot_vel_K(3,0.5);
    //4
    KChain.add_link(1,Vector3d(0,0,1),0.8,3,0,0,0);
    KChain.set_link_size(4,0.15,0.15);
    //KChain.set_rot_vel_K(4,0.6);
    //5
    KChain.add_link(1,Vector3d(0,0,1),0.7,4,0,0,0);
    KChain.set_link_size(5,0.12,0.12);
    //6 f2
    KChain.add_link(1,Vector3d(0,0,1),2,5,M_PI/20,0,0);
    KChain.set_link_size(6,0.3,0.3);



    */

   /* //7
    KChain.add_link(1,Vector3d(0,0,1),1.1,6,0,0,0);
    KChain.set_link_size(7,0.22,0.22);
    //8
    KChain.add_link(Vector3d(0,0,1),1.1,7,0,0,0);
    KChain.set_link_size(8,0.18,0.18);
    //9
    KChain.add_link(Vector3d(0,0,1),0.7,8,0,0,0);
    KChain.set_link_size(9,0.13,0.13);
    //10 f3
    KChain.add_link(Vector3d(0,0,1),2,1,-M_PI/18,0,0);
    KChain.set_link_size(10,0.3,0.3);
    //11
    KChain.add_link(Vector3d(0,0,1),0.8,10,0,0,0);
    KChain.set_link_size(11,0.2,0.2);
    //12
    KChain.add_link(Vector3d(0,0,1),0.8,11,0,0,0);
    KChain.set_link_size(12,0.15,0.15);
    //13
    KChain.add_link(Vector3d(0,0,1),0.7,12,0,0,0);
    KChain.set_link_size(13,0.13,0.13);
    //14 f4
    KChain.add_link(Vector3d(0,0,1),1.7,1,-M_PI/7,0,0);
    KChain.set_link_size(14,0.2,0.2);
    //15
    KChain.add_link(Vector3d(0,0,1),0.6,14,0,0,0);
    KChain.set_link_size(15,0.12,0.12);
    //16
    KChain.add_link(Vector3d(0,0,1),0.6,15,0,0,0);
    KChain.set_link_size(16,0.1,0.1);
    //17
    KChain.add_link(Vector3d(0,0,1),0.6,16,0,0,0);
    KChain.set_link_size(17,0.09,0.09);
    //18 f5
    KChain.add_link(Vector3d(0,0,1),0.9,1,M_PI/4,0,0);
    KChain.set_link_size(18,0.3,0.3);
    //19
    KChain.add_link(Vector3d(0,0,1),0.6,18,0,0,0);
    KChain.set_link_size(19,0.25,0.25);
    //20
    KChain.add_link(Vector3d(0,0,1),0.6,19,0,0,0);
    KChain.set_link_size(20,0.25,0.25);
*/
    //----Hand
    set_max_links(KChain.get_size());

    active_effector=0;

    //intersect_test
   /* QVector4D res_point;
    float r;
    r=intersect(QVector4D(0,2,0,1),QVector4D(0,1.31,1,1),QVector4D(-10,-10,6,1),QVector4D(0,10,2,1),QVector4D(10,-10,6,1),res_point);


    r=7;
    test_trigger=false;
    test_counter=0;
    bool tr;
    tr=1||0;
    tr=1&&0;
    Vector3d a,b,c;
    a=Vector3d(0,0,-1);
    b=Vector3d(1,0,0);

    c=a.cross(b);

    int co=b.dot(a);

    a=Vector3d(0,0,-1);
    b=Vector3d(0,0,-1);
    c=Vector3d(0,1,0);

    Matrix3d T;
    T=AngleAxisd(1,b);
    // rotate _up and _right around _look vector
    c=T*c;
    T=AngleAxisd(1,b);
*/



    Quaterniond a,b,c;
    Quaterniond qt1(AngleAxisd(M_PI/3,Vector3d(0,0,-1)));
    //qt1=qt1.conjugate();
    Quaterniond qt2;
    qt2=qt1.inverse();
    a=Quaterniond(0,1,2,3);
    Vector3d t(a.x(),a.y(),a.z());
    b=qt1*a*qt2;



    /*Vector3d a(1,0,0),b,c;

    Quaterniond qt1(AngleAxisd(-M_PI/2,Vector3d(0,0,-1)));

    AngleAxisd aa(qt1);
    aa.angle()=-aa.angle();

    Matrix3d M=aa.toRotationMatrix();
    a=M*a;
*/

    //sphere_intersect test

    bool inter_rez;
    inter_rez=sphere_intersect(QVector3D(1.01,0,10),QVector3D(0,0,-1),QVector3D(1,0,0),1.0);
    srand(12);
    int y=4,o=3;
    y=rand();
    y=rand();
    //double t=y/3.0;
    double e=2;

    /* MyGui Test*/

    /*mGUI = new MyGUI::Gui();
    mGUI->initialise();
*/
    /*MyGui---------------*/



    selected_object_id = 0; // 0 - no one object selected
// FBO init

    makeCurrent();
    ssao_fbo_normal = 0;
    ssao_fbo_pos = 0;
    shadow_fbo_global_pos = 0;
    final_fbo = 0;
    ssao_fbo_shaded_texture = 0;
    ssao_fbo_blur = 0;

    upscaled_ssao_fbo = 0;
    blur_upscaled_ssao_fbo = 0;

    main_scene_fbo = 0;
    main_scene_fbo_sampled = 0;

    shadow_fbo = 0;

    //plane geometry
    scren_plane_vertex.push_back(QVector4D(-1.0, -1.0, -1.0, 1.0));
    scren_plane_vertex.push_back(QVector4D(1.0, -1.0, -1.0, 1.0));
    scren_plane_vertex.push_back(QVector4D(1.0, 1.0, -1.0, 1.0));
    scren_plane_vertex.push_back(QVector4D(-1.0, 1.0, -1.0, 1.0));

    scren_plane_tex_coord.push_back(QVector2D(1.0, 0.0));
    scren_plane_tex_coord.push_back(QVector2D(0.0, 0.0));
    scren_plane_tex_coord.push_back(QVector2D(0.0, 1.0));
    scren_plane_tex_coord.push_back(QVector2D(1.0, 1.0));

    //axis

    edge axis_edge;
    axis_edge.A = QVector3D(0.0, 0.0, 0.0);
    axis_edge.B = QVector3D(1.0, 0.0, 0.0);
    box_axis.edges.push_back(axis_edge);
    axis_edge.A = QVector3D(0.0, 0.0, 0.0);
    axis_edge.B = QVector3D(0.0, 1.0, 0.0);
    box_axis.edges.push_back(axis_edge);
    axis_edge.A = QVector3D(0.0, 0.0, 0.0);
    axis_edge.B = QVector3D(0.0, 0.0, 1.0);
    box_axis.edges.push_back(axis_edge);
}

GLWidget::~GLWidget()
{
}


void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void GLWidget::initializeGL()
{

    qDebug() << "OpenGL Versions Supported: " << QGLFormat::openGLVersionFlags();
    QString versionString(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    qDebug() << "Driver Version String:" << versionString;
    qDebug() << "Current Context:" << format();

    glFuncs = new QOpenGLFunctions(QOpenGLContext::currentContext());

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_RECTANGLE);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1
    #define PROGRAM_NORMAL_ATTRIBUTE 2

    QGLShader *vshader;
    QGLShader *fshader;

    // Fullscreen texture shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/fullscreen_texture.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/fullscreen_texture.frag"));

    fullscreen_texture_program = new QGLShaderProgram(this);
    fullscreen_texture_program->addShader(vshader);
    fullscreen_texture_program->addShader(fshader);
    fullscreen_texture_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    fullscreen_texture_program->bindAttributeLocation("MultiTexCoord0", PROGRAM_TEXCOORD_ATTRIBUTE);
    fullscreen_texture_program->link();

    fullscreen_texture_program->bind();
    fullscreen_texture_program->setUniformValue("Texture0", 0);
    fullscreen_texture_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    fullscreen_texture_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

    fullscreen_texture_program->release();

    // Fullscreen texture shader program end -----------------

    // Fullscreen texture 2D shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/fullscreen_texture.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/fullscreen_texture_2D.frag"));

    fullscreen_texture_program_2D = new QGLShaderProgram(this);
    fullscreen_texture_program_2D->addShader(vshader);
    fullscreen_texture_program_2D->addShader(fshader);
    fullscreen_texture_program_2D->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    fullscreen_texture_program_2D->bindAttributeLocation("MultiTexCoord0", PROGRAM_TEXCOORD_ATTRIBUTE);
    fullscreen_texture_program_2D->link();

    fullscreen_texture_program_2D->bind();
    fullscreen_texture_program_2D->setUniformValue("Texture0", 0);
    fullscreen_texture_program_2D->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    fullscreen_texture_program_2D->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

    fullscreen_texture_program_2D->release();

    // Fullscreen texture 2D shader program end -----------------

    // ssao_normal shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_normal_buffer.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_normal_buffer.frag"));

    ssao_build_norm_program = new QGLShaderProgram(this);
    ssao_build_norm_program->addShader(vshader);
    ssao_build_norm_program->addShader(fshader);
    ssao_build_norm_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    ssao_build_norm_program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    ssao_build_norm_program->link();


    ssao_build_norm_program->bind();
    ssao_build_norm_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    ssao_build_norm_program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);

    // ssao_normal shader program end ------------------------

    // ssao_pos shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_pos_buffer.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_pos_buffer.frag"));

    ssao_build_pos_program = new QGLShaderProgram(this);
    ssao_build_pos_program->addShader(vshader);
    ssao_build_pos_program->addShader(fshader);
    ssao_build_pos_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    ssao_build_pos_program->link();

    ssao_build_pos_program->bind();
    ssao_build_pos_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // ssao_pos shader program end ---------------------------

    // build_global_pos_program shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/build_global_pos_buffer.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_pos_buffer.frag"));

    build_global_pos_program = new QGLShaderProgram(this);
    build_global_pos_program->addShader(vshader);
    build_global_pos_program->addShader(fshader);
    build_global_pos_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    build_global_pos_program->link();

    build_global_pos_program->bind();
    build_global_pos_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // build_global_pos_program shader program end ---------------------------

    // combine_textures_program shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/combine_textures.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/combine_textures.frag"));

    combine_textures_program = new QGLShaderProgram(this);
    combine_textures_program->addShader(vshader);
    combine_textures_program->addShader(fshader);
    combine_textures_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    combine_textures_program->link();

    combine_textures_program->bind();
    combine_textures_program->setUniformValue("aoMap", 0);
    combine_textures_program->setUniformValue("srcMap", 1);
    combine_textures_program->setUniformValue("ssao", 1);
    combine_textures_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // combine_textures_program shader program end  -------------------------

    // ssao_blur shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/ssao_blur.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/ssao_blur.frag"));

    ssao_blur_program = new QGLShaderProgram(this);
    ssao_blur_program->addShader(vshader);
    ssao_blur_program->addShader(fshader);
    ssao_blur_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    ssao_blur_program->link();

    ssao_blur_program->bind();
    ssao_blur_program->setUniformValue("aoMap", 0);
    ssao_blur_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // ssao_blur shader program end  -------------------------

    // ssao_shaded_texture shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_shaded_texture.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/ssao_build_shaded_texture.frag"));

    ssao_build_shaded_texture_program = new QGLShaderProgram(this);
    ssao_build_shaded_texture_program->addShader(vshader);
    ssao_build_shaded_texture_program->addShader(fshader);
    ssao_build_shaded_texture_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    ssao_build_shaded_texture_program->link();

    ssao_build_shaded_texture_program->bind();
    ssao_build_shaded_texture_program->setUniformValue("rotateMap", 0);
    ssao_build_shaded_texture_program->setUniformValue("posMap", 1);
    ssao_build_shaded_texture_program->setUniformValue("normalMap", 2);
    ssao_build_shaded_texture_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // ssao_shaded_texture shader program end ----------------

    // upsacle_interpolation shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/upscale_interpolation.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/upscale_interpolation.frag"));

    upsacle_interpolation_program = new QGLShaderProgram(this);
    upsacle_interpolation_program->addShader(vshader);
    upsacle_interpolation_program->addShader(fshader);
    upsacle_interpolation_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    upsacle_interpolation_program->link();

    upsacle_interpolation_program->bind();
    upsacle_interpolation_program->setUniformValue("srcMap", 0);
    upsacle_interpolation_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // upsacle_interpolation shader program end  -------------------------

    // only_depth program
    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/main_lightning_frag_shader.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/only_depth.frag"));

    only_depth_program = new QGLShaderProgram(this);
    only_depth_program->addShader(vshader);
    only_depth_program->addShader(fshader);
    only_depth_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    only_depth_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    only_depth_program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    only_depth_program->link();


    only_depth_program->bind();
    only_depth_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    only_depth_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    only_depth_program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
    // only_depth link end --------------------------

    // build_shadow  program

    //build_shadow_program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/build_shadows.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/build_shadows.frag"));

    build_shadow_program = new QGLShaderProgram(this);
    build_shadow_program->addShader(vshader);
    build_shadow_program->addShader(fshader);
    build_shadow_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    build_shadow_program->link();

    build_shadow_program->bind();
    build_shadow_program->setUniformValue("posMap", 0);
    build_shadow_program->setUniformValue("shadowMap", 1);
    build_shadow_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    // build_shadow_program link end --------------------------

    // line shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/line.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/line.frag"));

    line_program = new QGLShaderProgram(this);
    line_program->addShader(vshader);
    line_program->addShader(fshader);
    line_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    line_program->link();

    line_program->bind();

    line_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    line_program->setUniformValue("material", QVector4D(1.0,1.0,1.0,1));

    // line shader program link end --------------------------

    // Main shader program

    vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/main_lightning_frag_shader.vert"));

    fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/main_lightning_frag_shader.frag"));

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    program->link();


    program->bind();
    program->setUniformValue("texture1", 0);
    program->setUniformValue("texture2", 1);
    program->setUniformValue("shadowMap", 2);

    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);

    program->setUniformValue("ambient", QVector4D(0.33,0.33,0.33,1));
    program->setUniformValue("material", QVector4D(1.0,1.0,1.0,1));
    program->setUniformValue("light_pos",light_pos);
    program->setUniformValue("light_intensity", QVector4D(1.0,1.0,1.0,1));
    program->setUniformValue("without_texture",1);
    program->setUniformValue("shadows",1);
    program->setUniformValue("max_bias", (GLfloat)0.1);


    // Main shader program link end --------------------------




    coordinate_axis.setshaderprog(program);
    //scene_box
    b1->setshaderprog(program);
    b1->set_material(QVector4D(0.5,0.4,1.0,1));
    b1->set_texures(textures);
    b1->inverse_normals();
    QMatrix4x4 m;
    m.scale(75);
    b1->model=m;
    //--------------

    b2->setshaderprog(program);
    b2->set_texures(textures);

    c1->make_cylinder(0.5,2,100);
    c1->setshaderprog(program);
    c1->set_material(QVector4D(0.4,0.2,1,1));
    sph1->setshaderprog(program);
    sph1->set_material(QVector4D(0.4,1,0.5,1));
    pr1->setshaderprog(program);
    pr1->set_material(QVector4D(0.4,0.7,1,1));


    KChain.setshaderprog(program);

    /*Scene init*/

    scene = new Scene(program);

    scene->update_IK_Chain_objects(&KChain); // need to be first


    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 0].set_model_matrix(QVector3D(1.1,3.1,1.1), QVector3D(3.0,10.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 1].set_model_matrix(QVector3D(4.0,3.0,7.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 2].set_model_matrix(QVector3D(4.0,-2.1,-9.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 3].set_model_matrix(QVector3D(8.0,3.0,1.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 4].set_model_matrix(QVector3D(-4.0,3.0,7.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 5].set_model_matrix(QVector3D(10.0,0.0,-9.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 6].set_model_matrix(QVector3D(12.0,-3.0,1.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 7].set_model_matrix(QVector3D(4.0,3.0,7.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 8].set_model_matrix(QVector3D(1.0,0.0,-9.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 9].set_model_matrix(QVector3D(-8.0,3.0,1.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 10].set_model_matrix(QVector3D(-4.0,3.0,17.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 11].set_model_matrix(QVector3D(-10.0,7.0,-9.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 12].set_model_matrix(QVector3D(1.0,3.0,1.0), QVector3D(3.0,1.0,7.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 13].set_model_matrix(QVector3D(4.0,3.0,7.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 14].set_model_matrix(QVector3D(1.0,8.0,-9.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 15].set_model_matrix(QVector3D(8.0,5.0,-1.0), QVector3D(3.0,1.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 16].set_model_matrix(QVector3D(-4.0,3.0,7.0), QVector3D(3.0,1.0,7.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 17].set_model_matrix(QVector3D(11.0,0.0,-3.0), QVector3D(3.0,10.0,1.0));


    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 18].set_model_matrix(QVector3D(20.0,3.0,1.0), QVector3D(3.0,1.0,4.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 19].set_model_matrix(QVector3D(4.0,30.0,7.0), QVector3D(3.0,2.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 20].set_model_matrix(QVector3D(1.0,8.0,-22.0), QVector3D(3.0,1.0,7.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 21].set_model_matrix(QVector3D(8.0,5.0,-1.0), QVector3D(3.0,6.0,1.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 22].set_model_matrix(QVector3D(-4.0,25.0,7.0), QVector3D(7.0,1.0,7.0));

    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(0.4,0.7,1,1));
    scene->Scene_objects[KChain.links.size() + 23].set_model_matrix(QVector3D(11.0,-6.0,-3.0), QVector3D(3.0,10.0,1.0));


    scene->build_object_vertices_cash();



    scene->build_octree(100.0, false);
    /*Scene init end*/

    collision_box = new box();
    collision_box->setshaderprog(program);

    //signals connection

    connect(scene,SIGNAL(draw_collision_box(QVector3D)),this,SLOT(draw_collision_box(QVector3D)));
    //connect(scene->dcThread,SIGNAL(draw_collision_box(QVector3D)),this,SLOT(draw_collision_box(QVector3D)),Qt::QueuedConnection);

    //textures init
    test_pic = bindTexture
        (QPixmap(QLatin1String(":/textures/tron.jpg")), GL_TEXTURE_RECTANGLE);

    ssao_rot_texture =  bindTexture
            (QPixmap(QLatin1String(":/textures/rot_texture.bmp")), GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT);

    ssao = true;
    shadows = true;
    draw_edges = false;
    move_rotate_object = false;
    redact_scene_mode = false;
    move_box = false;

    current_axis_state = xPlane;

    movement_precision = 10.0;

    //shadow fbo init
    set_lp(QVector4D(0,20,0,0));
    cube_fbo = new CubeMapFBO();
    cube_fbo->init(1024 , 1024 , glFuncs);
}

void GLWidget::paintGL()
{
    glClearColor(0.4f,0.4f,0.9f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.getViewMatrix(&v);
    v = v.transposed();

    scene->recalc_IK_chain_model();

    if(!(stop_proc||main_stop)) //move chain and detect collision
    {
        KChain.backup_links();
        KChain.rotation_cycle(movement_precision, 0.1 / movement_precision * move_vel);

        //scene objects
        scene->recalc_IK_chain_model();

        if (scene->detect_all_collisions()) // collisions handler
        {
            KChain.restore_links();
            scene->recalc_IK_chain_model();
            KChain.collision = true;

        }
        else
        {
            if(KChain.collision)
            {
                KChain.collision = false;

            }
            //chain movement test
            //KChain.links[0]->global_position += Vector3d(0.01,0.01,0.01);
            //KChain.glob_pos_recalc(1,0);
        }
    }

    //shadows

    if(shadows)render_to_shadow_cubemap();

    program->bind();
    program->setUniformValue("proj_matrix", p);
    program->setUniformValue("view_matrix", v);

    if(ssao)
    {
        glViewport(0 ,0, w/2, h/2);
        ssao_build_norm_program->bind();
        ssao_build_norm_program->setUniformValue("view_matrix", v);

        //normal buffer build

        draw_scene_to_fbo(ssao_fbo_normal, ssao_build_norm_program);

        //normal buffer build ------------------------

        ssao_build_pos_program->bind();
        ssao_build_pos_program->setUniformValue("view_matrix", v);

        //pos buffer build

        draw_scene_to_fbo(ssao_fbo_pos, ssao_build_pos_program);

        //pos buffer build ------------------------
    }

    /*
    if(shadows)
    {

        glViewport(0 ,0, w, h);
        build_global_pos_program->bind();
        build_global_pos_program->setUniformValue("view_matrix", v);
        draw_scene_to_fbo(shadow_fbo_global_pos, build_global_pos_program);

    }
    */

    //----------main scene

    line_program->bind();
    line_program->setUniformValue("view_matrix", v);

    program->bind();

    main_scene_fbo_sampled->bind();

    glViewport(0 ,0, w, h);
    glClearColor(0.4f,0.4f,0.9f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFuncs->glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_fbo->cube_texture);

    program->setUniformValue("max_bias",(GLfloat)0.125);

    KChain.draw_chain();

    //рисуются цели
    for(unsigned int i = 0; i < KChain.effectors.size(); i++)
    {
        sph1->model.setToIdentity();
        sph1->model.translate(KChain.effectors[i].target.x(),KChain.effectors[i].target.y(),KChain.effectors[i].target.z());
        sph1->draw();
    }

    //ламочка
    QVector4D cur_material = sph1->material;
    program->setUniformValue("ambient", QVector4D(1.33,1.33,1.33,1));
    //program->setUniformValue("shadows",0);
    sph1->set_material(QVector4D(1.4,1.4,1.4,1.0));
    sph1->model.setToIdentity();
    sph1->model.translate(light_pos.toVector3D());
    sph1->draw();
    program->setUniformValue("ambient", QVector4D(0.33,0.33,0.33,1));
    //program->setUniformValue("shadows",1);
    sph1->set_material(cur_material);
    // лампочка -----------------


   // scene->build_octree(100.0, true);

    program->setUniformValue("max_bias",(GLfloat)0.075);
    scene->draw_objects();

    //scene->draw_polys(); //debug

    if(draw_edges)
    {
        program->setUniformValue("ambient", QVector4D(0.0,0.0,0.0,1));
        scene->draw_edges(); //debug
        program->setUniformValue("ambient", QVector4D(0.33,0.33,0.33,1));
    }

    if(selected_object_id)//box axes drawning
    {
        line_program->bind();

        scene->Scene_objects[selected_object_id].draw_edges(line_program, QVector4D(1.0,1.0,1.0,1.0));

        box_axis.set_model_matrix(scene->Scene_objects[selected_object_id].get_edge_center(), QVector3D(1.0,1.0,1.0));

        if(current_axis_state == xPlane)
        {
            box_axis.draw_edge(line_program,QVector4D(0.0,1.0,0.0,1.0),1);
            box_axis.draw_edge(line_program,QVector4D(0.0,0.0,1.0,1.0),2);
        }
        else if(current_axis_state == yPlane)
        {
            box_axis.draw_edge(line_program,QVector4D(1.0,0.0,0.0,1.0),0);
            box_axis.draw_edge(line_program,QVector4D(0.0,0.0,1.0,1.0),2);
        }
        else if(current_axis_state == zPlane)
        {
            box_axis.draw_edge(line_program,QVector4D(1.0,0.0,0.0,1.0),0);
            box_axis.draw_edge(line_program,QVector4D(0.0,1.0,0.0,1.0),1);
        }

        program->bind();
    }

    //scene_box
    glCullFace(GL_FRONT);
    b1->draw();
    glCullFace(GL_BACK);

    main_scene_fbo_sampled->release();

    QOpenGLFramebufferObject::blitFramebuffer(main_scene_fbo, main_scene_fbo_sampled);


    if(ssao)
    {
        //QOpenGLFramebufferObject::blitFramebuffer(ssao_fbo_pos, ssao_fbo_pos_fullres);

        make_ssao_shaded_texture();
        blur_fbo(ssao_fbo_shaded_texture, ssao_fbo_blur, 2);

        upsacle_interpolation(ssao_fbo_shaded_texture, upscaled_ssao_fbo);
        blur_fbo(upscaled_ssao_fbo, blur_upscaled_ssao_fbo, 2);
    }


    /*if(shadows)
    {
        build_shadows();
    }
    */

    combine_textures();


    program->release();
    draw_fullscreen_texture(final_fbo->texture());
    program->bind();


    //оси рисуются первыми или последними так как чистят z-буффер
    coordinate_axis.draw(&cam,aspect_ratio);

}


void GLWidget::rotate_KChain_link(int id,double yaw,double pitch,double roll)
{
    KChain.rotate_link(id,yaw,pitch,roll);
}

void GLWidget::set_KChain_link_angles(int id,double yaw,double pitch,double roll)
{
    KChain.set_link_angles(id,yaw,pitch,roll);
}


void GLWidget::set_time_speed(int new_time_speed)  //SLOT
{

}

void GLWidget::resizeGL(int width, int height)
{
    w = width;
    h = height;
    aspect_ratio = width;
    aspect_ratio /= height;
    glViewport(0, 0, width, height);
    p.setToIdentity();
    p.perspective(view_angle,aspect_ratio, 0.2, 300);

    ssao_build_shaded_texture_program->bind();
    ssao_build_shaded_texture_program->setUniformValue("viewport_size", QVector2D(width / 2.0, height / 2.0));

    program->bind();
    program->setUniformValue("proj_matrix", p);

    line_program->bind();
    line_program->setUniformValue("proj_matrix", p);

    ssao_build_norm_program->bind();
    ssao_build_norm_program->setUniformValue("proj_matrix", p);

    ssao_build_pos_program->bind();
    ssao_build_pos_program->setUniformValue("proj_matrix", p);

    build_global_pos_program->bind();
    build_global_pos_program->setUniformValue("proj_matrix", p);

    QOpenGLFramebufferObjectFormat fbo_format;

    if(ssao_fbo_normal) delete ssao_fbo_normal;
    if(ssao_fbo_pos) delete ssao_fbo_pos;
    if(shadow_fbo_global_pos) delete shadow_fbo_global_pos;
    if(ssao_fbo_shaded_texture) delete ssao_fbo_shaded_texture;
    if(ssao_fbo_blur) delete ssao_fbo_blur;
    if(final_fbo) delete final_fbo;
    if(main_scene_fbo_sampled) delete main_scene_fbo_sampled;
    if(main_scene_fbo) delete main_scene_fbo;

    if(upscaled_ssao_fbo) delete upscaled_ssao_fbo;
    if(blur_upscaled_ssao_fbo) delete blur_upscaled_ssao_fbo;

    if(shadow_fbo) delete shadow_fbo;

    ssao_fbo_normal = new QOpenGLFramebufferObject(width / 2.0, height / 2.0, QOpenGLFramebufferObject::Depth, GL_TEXTURE_RECTANGLE, GL_RGB32F );
    ssao_fbo_pos = new QOpenGLFramebufferObject(width / 2.0, height / 2.0, QOpenGLFramebufferObject::Depth, GL_TEXTURE_RECTANGLE,GL_RGB32F );
    //shadow_fbo_global_pos = new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::Depth, GL_TEXTURE_RECTANGLE,GL_RGB32F );
    ssao_fbo_shaded_texture = new QOpenGLFramebufferObject(width  / 2.0, height  / 2.0, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_R8);
    final_fbo = new QOpenGLFramebufferObject(width , height, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_RGB8);
    ssao_fbo_blur = new QOpenGLFramebufferObject(width  / 2.0, height  / 2.0, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_R8);

    fbo_format.setInternalTextureFormat(GL_RGBA8);
    fbo_format.setMipmap(false);
    fbo_format.setSamples(4);
    fbo_format.setAttachment(QOpenGLFramebufferObject::Depth);
    fbo_format.setTextureTarget(GL_TEXTURE_RECTANGLE);

    main_scene_fbo_sampled = new QOpenGLFramebufferObject(width, height, fbo_format);
    main_scene_fbo = new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_RGBA8);

    upscaled_ssao_fbo = new QOpenGLFramebufferObject(width , height, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_R8);
    blur_upscaled_ssao_fbo = new QOpenGLFramebufferObject(width , height, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_R8);

    shadow_fbo = new QOpenGLFramebufferObject(width , height, QOpenGLFramebufferObject::NoAttachment, GL_TEXTURE_RECTANGLE, GL_R8);

}

void GLWidget::keyPressEvent(QKeyEvent *event)
{

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    QPoint mouse_ccord = event->pos();

    if (event->buttons() & Qt::LeftButton)
    {
        double ang_x, ang_y;

        ang_x=((view_angle/2.0)/180.0)*M_PI;
        ang_y=(view_angle/2.0)/180.0*M_PI;
        ang_x=tan(ang_x);
        ang_y=tan(ang_y);

        if(redact_scene_mode) //трассировка боксов
        {
            QVector3D Qp, Qpos;
            Vector3d p;
            figure ray_figure;
            edge ray_edge;

            p = cam.getPosition() - cam.getLook()+
                    cam.getRight()*(((double)mouse_ccord.x()-(double)w/2.0)/((double)w/2.0))*ang_x*aspect_ratio
                    +cam.getUp()*((-(double)mouse_ccord.y()+(double)h/2.0)/((double)h/2.0))*ang_y;

            Qp = QVector3D(p.x(),p.y(),p.z());

            ray_edge.A = QVector3D(cam.getPosition().x(),cam.getPosition().y(),cam.getPosition().z());

            Qp = Qp - ray_edge.A;

            ray_edge.B = ray_edge.A + Qp * 300.0;


            ray_figure.edges.push_back(ray_edge);


            scene->Scene_objects.push_back(ray_figure);

            QVector3D intersect_point;
            float min_length = 1000000;
            bool is_object_selected = false;
            int old_selected_object_id = selected_object_id;

            for(int i = scene->chain_links_n; i < scene->Scene_objects.size() - 1; i++)
            {
                if ( scene->direct_collision_detect_closest_to_point(scene->Scene_objects.size() - 1, i, scene->Scene_objects, intersect_point, QVector3D(p.x(),p.y(),p.z())) )
                {
                    if(min_length > (intersect_point - ray_edge.A).length())
                    {
                        selected_object_id = i;
                        min_length = (intersect_point - ray_edge.A).length();
                        is_object_selected = true;
                    }
                }
            }

            box_intersect_point = intersect_point;
            box_intersect_point_vec = scene->Scene_objects[selected_object_id].get_edge_center() - intersect_point;

            if(!is_object_selected)
                selected_object_id = 0;

            if(selected_object_id == old_selected_object_id && old_selected_object_id !=0 )
                move_box = true;
            else
                move_box = false;

            scene->Scene_objects.pop_back();

            if(selected_object_id)//box axes (need function)
            {

                QVector3D box_center = scene->Scene_objects[selected_object_id].get_edge_center();

                QVector3D cam_pos = QVector3D(cam.getPosition().x(), cam.getPosition().y(), cam.getPosition().z());

                QVector3D view_vec = (box_center - cam_pos).normalized();

                float cos45 = 1.0/sqrt(2.0);

                if(QVector3D::dotProduct(view_vec,QVector3D(1.0,0.0,0.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(-1.0,0.0,0.0)) > cos45)
                {
                    current_axis_state = xPlane;
                    current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(1.0,0.0,0.0));
                }
                else if(QVector3D::dotProduct(view_vec,QVector3D(0.0,1.0,0.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(0.0,-1.0,0.0)) > cos45 )
                {
                    current_axis_state = yPlane;
                    current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(0.0,1.0,0.0));
                }
                else if(QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,1.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,-1.0)) > cos45 )
                {
                    current_axis_state = zPlane;
                    current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,1.0));
                }

                if(current_axis_sign > 0)
                    current_axis_sign = 1.0;
                else
                    current_axis_sign = -1.0;

            }

        }
        else //трассировка сферы
        {
            QVector3D Qp,Qpos,C,C_root;

            Vector3d p;

            p=cam.getPosition()-cam.getLook()+
                    cam.getRight()*(((double)mouse_ccord.x()-(double)w/2.0)/((double)w/2.0))*ang_x*aspect_ratio
                    +cam.getUp()*((-(double)mouse_ccord.y()+(double)h/2.0)/((double)h/2.0))*ang_y;

            Qp = QVector3D(p.x(),p.y(),p.z());

            Qpos = QVector3D(cam.getPosition().x(),cam.getPosition().y(),cam.getPosition().z());

            C = QVector3D(KChain.effectors[0].target.x(),KChain.effectors[0].target.y(),KChain.effectors[0].target.z());


            C_root = QVector3D(KChain.links[0]->global_position.x(),KChain.links[0]->global_position.y(),KChain.links[0]->global_position.z());

            //a=Qpos;
            Qp = Qp - Qpos;
            //b=Qpos+Qp*40;
            Qp.normalize();

            if(sphere_intersect(Qpos,Qp,C,0.4)) //увеличил радиус расчётной сферы
            {
                sph1->set_material(QVector4D(1.0,0.0,0.0,1.0));
                if(target_selected) move_target = true;
                else target_selected = true;
            }
            else if((sphere_intersect(Qpos,Qp,C_root,0.8)))
            {
                KChain.root_selected = true;
                if(root_selected) move_root = true;
                else root_selected = true;
            }
            else
            {
                sph1->set_material(QVector4D(0.4,1,0.5,1));
                KChain.root_selected = false;
                target_selected = false;
                root_selected = false;
            }
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    //движение цели
    double ang_x,ang_y;
    QVector3D Qp;
    Vector3d p;
    QPoint mouse_coord = event->pos();

    ang_x=((view_angle/2.0)/180.0)*M_PI;
    ang_y=(view_angle/2.0)/180.0*M_PI;
    ang_x=tan(ang_x);
    ang_y=tan(ang_y);

    p = cam.getPosition()-cam.getLook()+
            cam.getRight()*(((double)mouse_coord.x()-(double)w/2.0)/((double)w/2.0))*ang_x*aspect_ratio
            +cam.getUp()*((-(double)mouse_coord.y()+(double)h/2.0)/((double)h/2.0))*ang_y;

    Qp = QVector3D(p.x(),p.y(),p.z());

    if(move_target || move_root)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QVector3D Qpos,new_target,up,right,dir,C;

            Qpos = QVector3D(cam.getPosition().x(), cam.getPosition().y(), cam.getPosition().z());
            up = QVector3D(cam.getUp().x(), cam.getUp().y(), cam.getUp().z());
            right = QVector3D(cam.getRight().x(), cam.getRight().y(), cam.getRight().z());
            dir = QVector3D(cam.getLook().x(), cam.getLook().y(), cam.getLook().z());

            Qp = Qp - Qpos;

            Qp.normalize();

            if(move_target)
            {
                C = QVector3D(KChain.effectors[0].target.x(),KChain.effectors[0].target.y(),KChain.effectors[0].target.z());
            }
            else if(move_root)
            {
                C = QVector3D(KChain.links[0]->global_position.x(),KChain.links[0]->global_position.y(),KChain.links[0]->global_position.z());
            }

            if(ctrl_pressed)//плоскость перп камере
            {
                /*poly_intersect(Qpos,Qp,C+dir*10000
                           ,C-right*10000,C-dir*10000+right*10000
                           ,new_target);*/
                new_target = C + dir*0.1*(double)dy;
            }
            else//плоскость параллельная камере
            {
                poly_intersect(Qpos,Qp,C-right*10000-up*10000
                           ,C+right*10000,C+up*10000
                           ,new_target);
            }

            if(move_target)
            {
                KChain.effectors[0].target.x() = new_target.x();
                KChain.effectors[0].target.y() = new_target.y();
                KChain.effectors[0].target.z() = new_target.z();
            }
            else if(move_root)
            {
                KChain.links[0]->global_position.x() = new_target.x();
                KChain.links[0]->global_position.y() = new_target.y();
                KChain.links[0]->global_position.z() = new_target.z();
                KChain.glob_pos_recalc(1,0);
                KChain.collision = false;
            }
        }
    }
    else if(move_box)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            if(!move_rotate_object) //translate box
            {
                QVector3D Qpos,new_pos,up,right,dir,C;

                Qpos = QVector3D(cam.getPosition().x(), cam.getPosition().y(), cam.getPosition().z());

                if(current_axis_state == xPlane)
                {
                    up = QVector3D(0.0,1.0,0.0);
                    right = QVector3D(0.0,0.0,1.0);
                }
                else if(current_axis_state == yPlane)
                {
                    up = QVector3D(0.0,0.0,1.0);
                    right = QVector3D(1.0,0.0,0.0);
                }
                else if(current_axis_state == zPlane)
                {
                    up = QVector3D(1.0,0.0,0.0);
                    right = QVector3D(0.0,1.0,0.0);
                }

                Qp = Qp - Qpos;

                Qp.normalize();

                C = box_intersect_point;

                poly_intersect(Qpos, Qp, C - right*10000 - up*10000
                           , C + right*10000,C + up*10000
                           , new_pos);

                scene->Scene_objects[selected_object_id].translate_model.setToIdentity();
                scene->Scene_objects[selected_object_id].translate_model.translate(new_pos + box_intersect_point_vec);
                scene->Scene_objects[selected_object_id].update_model_matrix();
                scene->Scene_objects[selected_object_id].rebuild_polys_cash();
            }
            else //rotate box
            {
                QVector3D rot_axis;

                if(current_axis_state == xPlane)
                {
                    rot_axis = QVector3D(1.0,0.0,0.0);
                }
                else if(current_axis_state == yPlane)
                {
                    rot_axis = QVector3D(0.0,1.0,0.0);
                }
                else if(current_axis_state == zPlane)
                {
                    rot_axis = QVector3D(0.0,0.0,1.0);
                }

                QMatrix4x4 rot_matr;
                rot_matr.rotate((float)(current_axis_sign*dx + current_axis_sign*dy)/3.0, rot_axis);


                scene->Scene_objects[selected_object_id].rot_scale_model = rot_matr * scene->Scene_objects[selected_object_id].rot_scale_model;
                scene->Scene_objects[selected_object_id].update_model_matrix();
                scene->Scene_objects[selected_object_id].rebuild_polys_cash();
            }
        }
    }
    else //camera move
    {
        if(!cam.scene_view)
        {
            if (event->buttons() & Qt::LeftButton) {
                cam.pitch(dy/360.0f);
                cam.yaw(dx/360.0f);
            }
            if (event->buttons() & Qt::RightButton) {
                cam.fly(dy/60.0f);
                cam.strafe(-dx/60.0f);
            }
        }
        else
        {
            if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::RightButton)) {
                cam.rot_cam(dy/100.0f,-dx/100.0f,0);
            }
        }

        if(selected_object_id)//box axes
        {

            QVector3D box_center = scene->Scene_objects[selected_object_id].get_edge_center();

            QVector3D cam_pos = QVector3D(cam.getPosition().x(), cam.getPosition().y(), cam.getPosition().z());

            QVector3D view_vec = (box_center - cam_pos).normalized();

            float cos45 = 1.0/sqrt(2.0);

            if(QVector3D::dotProduct(view_vec,QVector3D(1.0,0.0,0.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(-1.0,0.0,0.0)) > cos45)
            {
                current_axis_state = xPlane;
                current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(1.0,0.0,0.0));
            }
            else if(QVector3D::dotProduct(view_vec,QVector3D(0.0,1.0,0.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(0.0,-1.0,0.0)) > cos45 )
            {
                current_axis_state = yPlane;
                current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(0.0,1.0,0.0));
            }
            else if(QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,1.0)) > cos45 || QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,-1.0)) > cos45 )
            {
                current_axis_state = zPlane;
                current_axis_sign = QVector3D::dotProduct(view_vec,QVector3D(0.0,0.0,1.0));
            }

            if(current_axis_sign > 0)
                current_axis_sign = 1.0;
            else
                current_axis_sign = -1.0;

        }

    }

    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    if(move_target)
        move_target = false;

    if(move_root)
        move_root = false;

    move_box = false;

    emit clicked();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    if(!cam.scene_view)cam.walk(event->delta()/400.0f);
    else cam.rot_cam(0,0,-event->delta()/100.0f);
}

void GLWidget::update_screen()   //SLOT
{
    updateGL();
}

void GLWidget::draw_collision_box(QVector3D collision_point)
{
    QMatrix4x4 matr;

    matr.translate(collision_point);

    matr.scale(0.1);

    collision_box->model=matr;
    collision_box->draw();
}

float GLWidget::poly_intersect( QVector4D origin_in, QVector4D direction_in, QVector4D p0_in, QVector4D p1_in, QVector4D p2_in,QVector4D& res_point )
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

float GLWidget::poly_intersect( QVector3D origin, QVector3D direction, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point )
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

bool GLWidget::sphere_intersect(QVector3D origin_in, QVector3D direction_in, QVector3D sph_C,double sph_r)
{
    double D,C,B;

    B = 2 * QVector3D::dotProduct(direction_in,origin_in - sph_C);
    C = QVector3D::dotProduct(origin_in - sph_C,origin_in - sph_C)-sph_r * sph_r;
    D = B*B - 4.0*C;
    if (D < 0) return false;
    else return true;
}

void GLWidget::draw_fullscreen_texture(GLuint texture, GLenum texture_type)
{
    if(texture_type == GL_TEXTURE_RECTANGLE)
    {
        fullscreen_texture_program->bind();

        fullscreen_texture_program->setAttributeArray
        (0, scren_plane_vertex.constData());
        fullscreen_texture_program->setAttributeArray
        (1, scren_plane_tex_coord.constData());

        glFuncs->glActiveTexture(GL_TEXTURE0);
        glBindTexture(texture_type, texture);
        glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

        fullscreen_texture_program->release();
    }
    else if(texture_type == GL_TEXTURE_2D)
    {
        fullscreen_texture_program_2D->bind();

        fullscreen_texture_program->setAttributeArray
        (0, scren_plane_vertex.constData());
        fullscreen_texture_program->setAttributeArray
        (1, scren_plane_tex_coord.constData());

        glFuncs->glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

        fullscreen_texture_program_2D->release();
    }
}

void GLWidget::make_ssao_shaded_texture()
{
    ssao_build_shaded_texture_program->bind();

    ssao_fbo_shaded_texture->bind();

    ssao_build_shaded_texture_program->setAttributeArray
    (0, scren_plane_vertex.constData());

    glFuncs->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssao_rot_texture);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFuncs->glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_RECTANGLE, ssao_fbo_pos->texture());

    glFuncs->glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_RECTANGLE, ssao_fbo_normal->texture());



    //glBindTexture(GL_TEXTURE_RECTANGLE, texture);
    glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

    ssao_fbo_shaded_texture->release();

    ssao_build_shaded_texture_program->release();
}

void GLWidget::blur_fbo(QOpenGLFramebufferObject *fbo_1, QOpenGLFramebufferObject *fbo_2, int passes)
{
    ssao_blur_program->bind();

    for(int i = 0; i < passes; i++)
    {
        fbo_2->bind();
        ssao_blur_program->setAttributeArray
        (0, scren_plane_vertex.constData());

        glFuncs->glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE, fbo_1->texture());

        //glBindTexture(GL_TEXTURE_RECTANGLE, texture);
        glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

        fbo_2->release();

        //2x

        fbo_1->bind();

        ssao_blur_program->setAttributeArray
        (0, scren_plane_vertex.constData());

        glFuncs->glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE, fbo_2->texture());

        //glBindTexture(GL_TEXTURE_RECTANGLE, texture);
        glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

        fbo_1->release();
    }

    ssao_blur_program->release();
}

void GLWidget::combine_textures()
{
    combine_textures_program->bind();

    final_fbo->bind();

    ssao_build_shaded_texture_program->setAttributeArray
    (0, scren_plane_vertex.constData());

    glFuncs->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, upscaled_ssao_fbo->texture());

    glFuncs->glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_RECTANGLE, main_scene_fbo->texture());

    //glBindTexture(GL_TEXTURE_RECTANGLE, texture);
    glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

    final_fbo->release();

    combine_textures_program->release();
}

void GLWidget::draw_scene_to_fbo(QOpenGLFramebufferObject *fbo, QGLShaderProgram *pass_program)
{
    pass_program->bind();

    KChain.setshaderprog(pass_program);

    scene->set_shader_program_for_objects(pass_program);

    fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    KChain.draw_chain();
    scene->draw_objects();

    //рисуются цели
    sph1->setshaderprog(pass_program);
    for(unsigned int i=0;i<KChain.effectors.size();i++)
    {
        QMatrix4x4 targ_mat;
        targ_mat.translate(KChain.effectors[i].target.x(),KChain.effectors[i].target.y(),KChain.effectors[i].target.z());
        sph1->model=targ_mat;
        sph1->draw();
    }
    sph1->setshaderprog(program);

    //scene_box
    glCullFace(GL_FRONT);
    b1->setshaderprog(pass_program);
    b1->draw();
    b1->setshaderprog(program);
    glCullFace(GL_BACK);

    fbo->release();

    scene->set_shader_program_for_objects(program);
    KChain.setshaderprog(program);


    pass_program->release();
}

void GLWidget::upsacle_interpolation(QOpenGLFramebufferObject *src, QOpenGLFramebufferObject *dest)
{
    upsacle_interpolation_program->bind();

    dest->bind();
    ssao_blur_program->setAttributeArray
    (0, scren_plane_vertex.constData());

    glFuncs->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, src->texture());

    glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

    dest->release();

    upsacle_interpolation_program->release();
}

void GLWidget::calculate_light_matrices()
{
    QVector3D l_vectors[6];
    QVector3D u_vectors[6];

    //x+
    l_vectors[0] = QVector3D(1.0, 0.0, 0.0);
    u_vectors[0] = QVector3D(0.0, -1.0, 0.0);
    //x-
    l_vectors[1] = QVector3D(-1.0, 0.0, 0.0);
    u_vectors[1] = QVector3D(0.0, -1.0, 0.0);
    //y+
    l_vectors[2] = QVector3D(0.0, 1.0, 0.0);
    u_vectors[2] = QVector3D(0.0, 0.0, 1.0);
    //y-
    l_vectors[3] = QVector3D(0.0, -1.0, 0.0);
    u_vectors[3] = QVector3D(0.0, 0.0, -1.0);
    //z+
    l_vectors[4] = QVector3D(0.0, 0.0, 1.0);
    u_vectors[4] = QVector3D(0.0, -1.0, 0.0);
    //z-
    l_vectors[5] = QVector3D(0.0, 0.0, -1.0);
    u_vectors[5] = QVector3D(0.0, -1.0, 0.0);

    light_ProjMatrix.setToIdentity();
    light_ProjMatrix.perspective(90.0, 1.0, 1.0, 100.0);

    //light_cam.setPosition(Vector3d(light_pos.x(), light_pos.y(), light_pos.z()));

    for(int i = 0; i < 6; i++)
    {
        light_ViewMatrix[i].setToIdentity();
        light_ViewMatrix[i].lookAt(light_pos.toVector3D(), l_vectors[i] + light_pos.toVector3D(), u_vectors[i]);
        light_ProjViewMatrix[i] = light_ProjMatrix * light_ViewMatrix[i];
    }
}

void GLWidget::render_to_shadow_cubemap()
{
    only_depth_program->bind();
    KChain.setshaderprog(only_depth_program);
    scene->set_shader_program_for_objects(only_depth_program);

    only_depth_program->setUniformValue("proj_matrix", light_ProjMatrix);

    glViewport(0 ,0, cube_fbo->width, cube_fbo->height);
    glCullFace(GL_FRONT);

    for(int i = 0; i < 6; i++)
    {
        cube_fbo->bind(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        only_depth_program->setUniformValue("view_matrix", light_ViewMatrix[i]);

        KChain.draw_chain();
        scene->draw_objects();

        //рисуются цели
        sph1->setshaderprog(only_depth_program);
        for(unsigned int i = 0; i < KChain.effectors.size(); i++)
        {
            QMatrix4x4 targ_mat;
            targ_mat.translate(KChain.effectors[i].target.x(),KChain.effectors[i].target.y(),KChain.effectors[i].target.z());
            sph1->model = targ_mat;
            sph1->draw();
        }
        sph1->setshaderprog(program);

        //scene_box
        /*
        glCullFace(GL_BACK);
        b1->setshaderprog(only_depth_program);
        b1->draw();
        b1->setshaderprog(program);
        glCullFace(GL_FRONT);
        */
    }

    glCullFace(GL_BACK);
    KChain.setshaderprog(program);
    scene->set_shader_program_for_objects(program);

    cube_fbo->release();
    only_depth_program->release();
}

void GLWidget::build_shadows()
{
    build_shadow_program->bind();

    shadow_fbo->bind();

    build_shadow_program->setAttributeArray
    (0, scren_plane_vertex.constData());

    glFuncs->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, shadow_fbo_global_pos->texture());

    glFuncs->glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_fbo->cube_texture);

    //glBindTexture(GL_TEXTURE_RECTANGLE, texture);
    glDrawArrays(GL_QUADS, 0, scren_plane_vertex.size());

    shadow_fbo->release();

    build_shadow_program->release();
}

void GLWidget::set_ssao(bool state)
{
    if(state)
    {
        ssao = true;
        combine_textures_program->bind();
        combine_textures_program->setUniformValue("ssao", 1);
    }
    else
    {
        ssao = false;
        combine_textures_program->bind();
        combine_textures_program->setUniformValue("ssao", 0);
    }
}

void GLWidget::set_shadows(bool state)
{
    if(state)
    {
        shadows = true;
        program->bind();
        program->setUniformValue("shadows",1);
    }
    else
    {
        shadows = false;
        program->bind();
        program->setUniformValue("shadows", 0);
    }
}

void GLWidget::add_box_scene(QVector3D pos,QVector3D scale, QVector3D color)
{
    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, QVector4D(color, 1.0));
    scene->Scene_objects[ scene->Scene_objects.size() - 1].set_model_matrix(pos, scale);
    scene->build_object_vertices_cash( scene->Scene_objects.size() - 1);
    scene->update_polys_id();
}

void GLWidget::add_box_scene(QMatrix4x4 &rot_scale_matr, QMatrix4x4 &translate_matr, QVector4D &material)
{
    scene->add_object(b2->vertices, b2->normals, b2->texCoords, program, material);
    scene->Scene_objects[ scene->Scene_objects.size() - 1].rot_scale_model = rot_scale_matr;
    scene->Scene_objects[ scene->Scene_objects.size() - 1].translate_model = translate_matr;
    scene->Scene_objects[ scene->Scene_objects.size() - 1].update_model_matrix();
    scene->build_object_vertices_cash( scene->Scene_objects.size() - 1);
    scene->update_polys_id();
}
