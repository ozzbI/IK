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
    con=new QGLContext(fm);
    con->setFormat(fm);
    con->create();
    con->makeCurrent();

    view_angle=60;
    target_selected=false,move_target=false;
    stop_proc=false;
    main_stop=false;
    move_vel=1;
    ctrl_pressed=false;

    clearColor = Qt::black;
    program = 0;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_screen()));
    timer->start(20);

    cam.setPosition(&Vector3d(0,0,10));

    light_pos=QVector4D(0,20,0,0);
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
    glFuncs=new QGLFunctions(QGLContext::currentContext());
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1
    #define PROGRAM_NORMAL_ATTRIBUTE 2

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);

    vshader->compileSourceFile(QLatin1String(":/shaders/vert_shader.vert"));

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    fshader->compileSourceFile(QLatin1String(":/shaders/frag_shader.frag"));

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("texture", 0);
    program->setUniformValue("texture2", 1);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);

    program->setUniformValue("ambient", QVector4D(0.33,0.33,0.33,1));
    program->setUniformValue("material", QVector4D(1.0,1.0,1.0,1));
    program->setUniformValue("light_pos",light_pos);
    program->setUniformValue("light_intensity", QVector4D(1.0,1.0,1.0,1));
    program->setUniformValue("without_texture",1);

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

}

void GLWidget::paintGL()
{
    glClearColor(0.4f,0.4f,0.9f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.getViewMatrix(&v);
    v=v.transposed();
    program->setUniformValue("view_matrix", v);

    if(!(stop_proc||main_stop))KChain.rotation_cycle(10,0.01*move_vel);
    KChain.draw_chain();

    //рисуются цели
    for(unsigned int i=0;i<KChain.effectors.size();i++)
    {
        QMatrix4x4 targ_mat;
        targ_mat.translate(KChain.effectors[i].target.x(),KChain.effectors[i].target.y(),KChain.effectors[i].target.z());
        sph1->model=targ_mat;
        sph1->draw();
    }



    /*///sphere trace test
    glBegin(GL_LINES);
    glColor3f(0,0,1);
    glVertex3f(a.x(),a.y(),a.z());
    glColor3f(0,0,1);
    glVertex3f(b.x(),b.y(),b.z());
    glEnd();*/

    //scene objects

    scene->recalc_IK_chain_model();

   // scene->build_octree(100.0, true);

    scene->draw_objects();

    //scene->draw_polys(); //debug
   // scene->draw_edges();

    scene->detect_all_collisions();

    //scene_box
    glCullFace(GL_FRONT);
    b1->draw();
    glCullFace(GL_BACK);
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
    w=width;
    h=height;
    aspect_ratio=width;
    aspect_ratio/=height;
    glViewport(0 ,0, width, height);
    QMatrix4x4 new_perspective;
    p=new_perspective;
    p.perspective(view_angle,aspect_ratio,1,10000);
    program->setUniformValue("proj_matrix", p);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    //трассировка сферы
    if (event->buttons() & Qt::LeftButton)
    {
        QVector3D Qp,Qpos,C;
        QPoint mouse_ccord=event->pos();
        Vector3d p;       
        double ang_x,ang_y;

        ang_x=((view_angle/2.0)/180.0)*M_PI;
        ang_y=(view_angle/2.0)/180.0*M_PI;
        ang_x=tan(ang_x);
        ang_y=tan(ang_y);

        p=cam.getPosition()-cam.getLook()+
                cam.getRight()*(((double)mouse_ccord.x()-(double)w/2.0)/((double)w/2.0))*ang_x*aspect_ratio
                +cam.getUp()*((-(double)mouse_ccord.y()+(double)h/2.0)/((double)h/2.0))*ang_y;

        Qp=QVector3D(p.x(),p.y(),p.z());

        Qpos=QVector3D(cam.getPosition().x(),cam.getPosition().y(),cam.getPosition().z());

        C=QVector3D(KChain.effectors[0].target.x(),KChain.effectors[0].target.y(),KChain.effectors[0].target.z());

        //a=Qpos;
        Qp=Qp-Qpos;
        //b=Qpos+Qp*40;
        Qp.normalize();

        if(sphere_intersect(Qpos,Qp,C,0.4)) //увеличил радиус расчётной сферы
        {
            sph1->set_material(QVector4D(1.0,0.0,0.0,1.0));
            if(target_selected) move_target=true;
            else target_selected=true;
        }
        else
        {
            sph1->set_material(QVector4D(0.4,1,0.5,1));
            target_selected=false;
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    //движение цели
    if(move_target)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QVector3D Qp,Qpos,new_target,up,right,dir,C;
            QPoint mouse_ccord=event->pos();
            Vector3d p;
            double ang_x,ang_y;

            ang_x=((view_angle/2.0)/180.0)*M_PI;
            ang_y=(view_angle/2.0)/180.0*M_PI;
            ang_x=tan(ang_x);
            ang_y=tan(ang_y);

            p=cam.getPosition()-cam.getLook()+
                    cam.getRight()*(((double)mouse_ccord.x()-(double)w/2.0)/((double)w/2.0))*ang_x*aspect_ratio
                    +cam.getUp()*((-(double)mouse_ccord.y()+(double)h/2.0)/((double)h/2.0))*ang_y;

            Qp=QVector3D(p.x(),p.y(),p.z());

            Qpos=QVector3D(cam.getPosition().x(),cam.getPosition().y(),cam.getPosition().z());
            up=QVector3D(cam.getUp().x(),cam.getUp().y(),cam.getUp().z());
            right=QVector3D(cam.getRight().x(),cam.getRight().y(),cam.getRight().z());
            dir=QVector3D(cam.getLook().x(),cam.getLook().y(),cam.getLook().z());

            Qp=Qp-Qpos;

            Qp.normalize();

            C=QVector3D(KChain.effectors[0].target.x(),KChain.effectors[0].target.y(),KChain.effectors[0].target.z());

            if(ctrl_pressed)//плоскость перп камере
            {
                /*poly_intersect(Qpos,Qp,C+dir*10000
                           ,C-right*10000,C-dir*10000+right*10000
                           ,new_target);*/
                new_target=C+dir*0.1*(double)dy;
            }
            else//плоскость параллельная камере
            {
                poly_intersect(Qpos,Qp,C-right*10000-up*10000
                           ,C+right*10000,C+up*10000
                           ,new_target);
            }

            KChain.effectors[0].target.x()=new_target.x();
            KChain.effectors[0].target.y()=new_target.y();
            KChain.effectors[0].target.z()=new_target.z();
        }
        lastPos = event->pos();
    }
    else
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
            if (event->buttons() & Qt::LeftButton) {
                cam.rot_cam(dy/100.0f,-dx/100.0f,0);
            }
        }
        lastPos = event->pos();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    if(move_target)move_target=false;
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

