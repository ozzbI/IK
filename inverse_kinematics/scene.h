#ifndef SCENE_H
#define SCENE_H

#include "figure.h"
#include "box.h"
#include "kinematic_chain.h"
#include "octree.h"

#include <QObject>
#include <QThread>

class dc_Worker;

class direct_collision_Thread : public QThread
{
    Q_OBJECT

public:

    direct_collision_Thread()
    {

    }

    void run();

};



class Scene : public QObject
{
    Q_OBJECT

public:

    QVector<figure> Scene_objects; //IK chain object in 1st element
    QVector<figure> IK_Chain_objects;

    QGLShaderProgram *scene_program;

    int chain_links_n;

    kinematic_chain* IKChain;

    box *b1; //for debug purposes



    octree *scene_octree;

    octree *chain_octree;

    direct_collision_Thread *dcThread;

    dc_Worker *dcWorker;

    QMutex dc_mutex;

    QVector<QVector3D> intersect_points; // for direct collision (miltithreded)
    QVector<int> affected_links; // for direct collision (miltithreded)

    bool dc_proc_ended;

    void draw_objects();
    void move_object(int i, double x,double y, double z);
    void rotate_object(int i, double x,double y, double z);

    Scene(QGLShaderProgram *program);

    void add_object(QVector<QVector4D> &vertices, QVector<QVector4D> &normals, QVector<QVector2D> &texCoords,QGLShaderProgram *program, QVector4D material, GLuint *textures = 0, int without_texture = 1);
    void update_IK_Chain_objects(kinematic_chain* Chain);

    void recalc_IK_chain_model();
    void build_object_vertices_cash();
    void rebuild_object_vertices_cash();
    void build_object_vertices_cash(int i);
    void rebuild_object_vertices_cash(int i);
    void build_chain_vertices_cash();
    void rebuild_chain_vertices_cash();
    void update_polys_id();


    bool detect_all_collisions(QVector3D &inters_point, int &affected_link);
    bool collision_detect(int part_id, QVector<figure> &figures);
    bool collision_detect_octree(int part_id, QVector<figure> &figures,  QVector3D &inters_point);
    bool direct_collision_detect(int part_id, int test_part_id, QVector<figure> &figures, QVector3D &inters_point);
    bool direct_collision_detect_closest_to_point(int part_id, int test_part_id, QVector<figure> &figures, QVector3D &inters_point, QVector3D point);
    int poly_intersect( QVector3D l1, QVector3D l2, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point );

    void build_octree(double boundingBoxSize, bool for_chain);

    bool octree_traverse(QVector3D &p1, QVector3D &p2, OctreeNode &node, QVector3D &inters_point);

    bool is_target_visible(QVector3D target);

    //debug

    void draw_polys()
    {
        for (int i = 0 ; i < Scene_objects.size(); i++)
        {
            Scene_objects[i].draw_polys();
        }
    }

    void draw_edges()
    {
        for (int i = 0 ; i < Scene_objects.size(); i++)
        {
            Scene_objects[i].draw_edges();
        }
    }

    void draw_edges(QVector4D material)
    {
        for (int i = 0 ; i < Scene_objects.size(); i++)
        {
            Scene_objects[i].draw_edges(material);
        }
    }

    void set_shader_program_for_objects(QGLShaderProgram *program)
    {
        for(int i = chain_links_n; i < Scene_objects.size(); i++)
        {
            Scene_objects[i].setshaderprog(program);
        }
    }

signals:
    void draw_collision_box(QVector3D collision_point);

    void dc_start_process();
    void lock_dc_mutex();


};


class dc_Worker : public QObject
{
    Q_OBJECT

public:

    Scene *scene;

    dc_Worker()
    {

    }

    void set_scene(Scene *new_scene)
    {
        scene = new_scene;
    }

    bool direct_collision_detect(int part_id,int test_part_id, QVector<figure> &figures);
    bool direct_collision_detect_max_id(int part_id,int test_part_id, QVector<figure> &figures);
    bool direct_collision_detect_octree(int part_id,int test_part_id, QVector<figure> &figures);
    int poly_intersect( QVector3D l1, QVector3D l2, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point );
    bool octree_traverse(QVector3D &p1, QVector3D &p2, OctreeNode& node, QVector3D &inters_point);

protected slots:

    bool process();
    void lock_dc_mutex()
    {
        scene->dc_mutex.lock();
    }

};

#endif // SCENE_H
