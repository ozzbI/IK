#ifndef SCENE_H
#define SCENE_H

#include "figure.h"
#include "box.h"
#include "kinematic_chain.h"

class Scene
{
public:

    QVector<figure> Scene_objects; //IK chain object in 1st element
    QVector<figure> IK_Chain_objects;

    QGLShaderProgram *scene_program;

    int chain_links_n;

    kinematic_chain* IKChain;

    box *b1; //for debug purposes

    Scene(QGLShaderProgram *program);

    void draw_objects();
    void move_object(int i, double x,double y, double z);
    void rotate_object(int i, double x,double y, double z);

    /*QVector<QVector3D> vertices;
    QVector<QVector4D> normals;
    QVector<QVector2D> texCoords;

    QGLShaderProgram *program;
    GLuint *textures;
    int without_texture;*/

    void add_object(QVector<QVector4D> &vertices, QVector<QVector4D> &normals, QVector<QVector2D> &texCoords,QGLShaderProgram *program, QVector4D material, GLuint *textures = 0, int without_texture = 1);
    void update_IK_Chain_objects(kinematic_chain* Chain);

    void recalc_IK_chain_model();
    void build_object_vertices_cash();
    void build_chain_vertices_cash();
    void rebuild_chain_vertices_cash();


    bool detect_all_collisions();
    bool collision_detect(int part_id, QVector<figure> &figures);
    bool direct_collision_detect(int part_id,int test_part_id, QVector<figure> &figures);
    int poly_intersect( QVector3D l1, QVector3D l2, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point );

    //debug

    void draw_polys()
    {
        for (int i = 0 ; i < Scene_objects.size(); i++)
        {
            Scene_objects[i].draw_polys();
        }
    }



};




#endif // SCENE_H
