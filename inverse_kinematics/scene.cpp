#include "scene.h"

Scene::Scene(QGLShaderProgram *program)
{
    scene_program = program;
    chain_links_n = 0;
    b1 = new box(1);
    b1->setshaderprog(program);

}

void Scene::draw_objects()
{
    for(int i = chain_links_n; i < Scene_objects.size(); i++)
    {
        Scene_objects[i].draw();
    }
}

void Scene::move_object(int i, double x,double y, double z)
{

}

void Scene::rotate_object(int i, double x,double y, double z)
{

}

void Scene::add_object(QVector<QVector4D> &vertices, QVector<QVector4D> &normals, QVector<QVector2D> &texCoords,QGLShaderProgram *program, QVector4D material, GLuint *textures, int without_texture)
{
    int i;

    int size = Scene_objects.size();

    Scene_objects.push_back( *(new figure()) );

    for(i = 0; i < vertices.size(); i++)
    {
        Scene_objects[size].vertices.push_back(vertices[i]);
    }

    for(i = 0; i < vertices.size(); i++)
    {
        Scene_objects[size].normals.push_back(normals[i]);
    }

    for(i = 0; i < vertices.size(); i++)
    {
        Scene_objects[size].texCoords.push_back(texCoords[i]);
    }

    Scene_objects[size].program = program;
    Scene_objects[size].material = material;
    Scene_objects[size].textures = textures;
    Scene_objects[size].without_texture = without_texture;

}

void Scene::update_IK_Chain_objects(kinematic_chain* Chain)
{
    /* при удалении/вставке звеньев  */


    box b(1);
    IKChain = Chain;


    chain_links_n = IKChain->links.size();

    int i,j;

    for(j = 0; j < IKChain->links.size() ;j++)
    {

        int size = Scene_objects.size();

        Scene_objects.push_back( *(new figure()) );

        for(i = 0; i < b.vertices.size(); i++)
        {
            Scene_objects[size].vertices.push_back( b.vertices[i]);
        }

        Scene_objects[size].program = scene_program;
    }

    build_chain_vertices_cash();
}

void Scene::recalc_IK_chain_model()
{
    /* каждый кадр*/

    QMatrix4x4 box_shift_mat;
    box_shift_mat.translate(0,0,-0.5);

    Quaterniond rot_qt;
    QMatrix4x4 translate_mat;
    QMatrix4x4 scale_mat;
    QMatrix4x4 rot_mat;

    for(int i = 0; i < chain_links_n; i++)
    {

        rot_qt.setIdentity();
        translate_mat.setToIdentity();
        scale_mat.setToIdentity();
        rot_mat.setToIdentity();

        rot_qt = IKChain->links[i]->global_transform;
        Matrix3d e_rot=rot_qt.toRotationMatrix();
        kinematic_chain::Eigen_Matr_to_Qt_Matr(rot_mat,e_rot);

        scale_mat.scale(IKChain->links[i]->width + 0.07, IKChain->links[i]->height + 0.07, IKChain->links[i]->length);

        translate_mat.translate(IKChain->links[i]->global_position(0), IKChain->links[i]->global_position(1), IKChain->links[i]->global_position(2));

        Scene_objects[i].model = translate_mat*rot_mat*scale_mat*box_shift_mat;
    }

    rebuild_chain_vertices_cash();

}

void Scene::build_object_vertices_cash()
{
    for(int i = chain_links_n; i < Scene_objects.size(); i++)
    {
        Scene_objects[i].update_polys_cash();
    }
}

void Scene::build_chain_vertices_cash()
{
    for(int i = 0; i < chain_links_n; i++)
    {
        Scene_objects[i].update_polys_cash();
    }
}

void Scene::rebuild_chain_vertices_cash()
{
    for(int i = 0; i < chain_links_n; i++)
    {
        Scene_objects[i].rebuild_polys_cash();
    }
}

bool Scene::detect_all_collisions()
{
    for(int i = 0; i < chain_links_n; i++)
    {
        collision_detect(i, Scene_objects);
    }
}


bool Scene::collision_detect(int part_id, QVector<figure> &figures)
{
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;
    for(int p = 0; p < figures[part_id].polys.size();p++)
    {
        for(int v = 0; v < figures[part_id].polys[p].vertices.size();v++)
        {
            for(int c = chain_links_n; c < figures.size();c++)
            {
                if(c == part_id) continue;

                //if(direct_collision_detect(c, part_id, figures))
                    //return true;
                //direct_collision_detect(c, part_id, figures);

                for(int i = 0; i < figures[c].polys.size();i++)
                {
                    if(v == figures[part_id].polys[p].vertices.size() - 1)
                    {
                        p1 = figures[part_id].polys[p].vertices[v];
                        p2 = figures[part_id].polys[p].vertices[0];
                        pa = figures[c].polys[i].vertices[0];
                        pb = figures[c].polys[i].vertices[1];
                        pc = figures[c].polys[i].vertices[2];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;

                            matr.translate(inters_point);
                            b1->model=matr;
                            b1->draw();
                            qDebug("collision");
                            //return true;
                        }
                        if(figures[c].polys[i].vertices.size() == 4)
                        {
                            p1 = figures[part_id].polys[p].vertices[v];
                            p2 = figures[part_id].polys[p].vertices[0];
                            pa = figures[c].polys[i].vertices[0];
                            pb = figures[c].polys[i].vertices[2];
                            pc = figures[c].polys[i].vertices[3];
                            if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                            {

                                QMatrix4x4 matr;
                               // matr.scale(0.55);
                                matr.translate(inters_point);
                                b1->model = matr;
                                b1->draw();
                                qDebug("collision");
                                //return true;
                            }
                        }

                    }
                    else
                    {
                        p1 = figures[part_id].polys[p].vertices[v];
                        p2 = figures[part_id].polys[p].vertices[v + 1];
                        pa = figures[c].polys[i].vertices[0];
                        pb = figures[c].polys[i].vertices[1];
                        pc = figures[c].polys[i].vertices[2];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;
                           // matr.scale(0.55);
                            matr.translate(inters_point);
                            b1->model=matr;
                            b1->draw();
                            qDebug("collision");
                            //return true;
                        }
                        if(figures[c].polys[i].vertices.size() == 4)
                        {
                            p1 = figures[part_id].polys[p].vertices[v];
                            p2 = figures[part_id].polys[p].vertices[v + 1];
                            pa = figures[c].polys[i].vertices[0];
                            pb = figures[c].polys[i].vertices[2];
                            pc = figures[c].polys[i].vertices[3];
                            if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                            {
                                QMatrix4x4 matr;
                                //matr.scale(0.55);
                                matr.translate(inters_point);
                                b1->model = matr;
                                b1->draw();
                                qDebug("collision");
                                //return true;
                            }
                        }

                    }
                }
            }
        }
    }

    for(int c = chain_links_n; c < figures.size();c++)
    {
        direct_collision_detect(c, part_id, figures);
    }

    return false;
}


bool Scene::direct_collision_detect(int part_id,int test_part_id, QVector<figure> &figures)
{
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;
    for(int p = 0; p < figures[part_id].polys.size();p++)
    {
        for(int v = 0; v < figures[part_id].polys[p].vertices.size();v++)
        {

            for(int i = 0; i < figures[test_part_id].polys.size();i++)
            {
                if(v == figures[part_id].polys[p].vertices.size() - 1)
                {
                    p1 = figures[part_id].polys[p].vertices[v];
                    p2 = figures[part_id].polys[p].vertices[0];
                    pa = figures[test_part_id].polys[i].vertices[0];
                    pb = figures[test_part_id].polys[i].vertices[1];
                    pc = figures[test_part_id].polys[i].vertices[2];
                    if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                    {

                        QMatrix4x4 matr;
                        //matr.scale(0.55);
                        matr.translate(inters_point);
                        b1->model=matr;
                        b1->draw();
                        qDebug("collision");
                        //return true;
                    }
                    if(figures[test_part_id].polys[i].vertices.size() == 4)
                    {
                        p1 = figures[part_id].polys[p].vertices[v];
                        p2 = figures[part_id].polys[p].vertices[0];
                        pa = figures[test_part_id].polys[i].vertices[0];
                        pb = figures[test_part_id].polys[i].vertices[2];
                        pc = figures[test_part_id].polys[i].vertices[3];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;
                            //matr.scale(0.55);
                            matr.translate(inters_point);
                            b1->model = matr;
                            b1->draw();
                            qDebug("collision");
                            //return true;
                        }
                    }

                }
                else
                {
                    p1 = figures[part_id].polys[p].vertices[v];
                    p2 = figures[part_id].polys[p].vertices[v + 1];
                    pa = figures[test_part_id].polys[i].vertices[0];
                    pb = figures[test_part_id].polys[i].vertices[1];
                    pc = figures[test_part_id].polys[i].vertices[2];
                    if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                    {

                        QMatrix4x4 matr;
                        //matr.scale(0.55);
                        matr.translate(inters_point);
                        b1->model=matr;
                        b1->draw();
                        qDebug("collision");
                        //return true;
                    }
                    if(figures[test_part_id].polys[i].vertices.size() == 4)
                    {
                        p1 = figures[part_id].polys[p].vertices[v];
                        p2 = figures[part_id].polys[p].vertices[v + 1];
                        pa = figures[test_part_id].polys[i].vertices[0];
                        pb = figures[test_part_id].polys[i].vertices[2];
                        pc = figures[test_part_id].polys[i].vertices[3];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {
                            QMatrix4x4 matr;
                            //matr.scale(0.55);
                            matr.translate(inters_point);
                            b1->model = matr;
                            b1->draw();
                            qDebug("collision");
                            //return true;
                        }
                    }

                }
            }
        }
    }
    return false;
}




int Scene::poly_intersect( QVector3D l1, QVector3D l2, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point )
{
  QVector3D origin = l1 ;
  QVector3D direction = l2 - l1;

  float result;

  QVector3D e1 = p1 - p0;
  QVector3D e2 = p2 - p0;

  p0 = origin - p0;

  QVector3D P = QVector3D::crossProduct( direction , e2 );

  float  det =  QVector3D::dotProduct( P , e1 );

  if( det == 0.0 )
    return false/*-1000000.0*/;

  float u = QVector3D::dotProduct( P , p0 ) / det;

  P = QVector3D::crossProduct( p0 , e1 );

  float w = QVector3D::dotProduct( P , direction ) / det ;

  if( u + w > 1 || w < 0 || u < 0 )
    return false /*1000000.0*/;


  result = QVector3D::dotProduct( P , e2 ) / det;
  res_point = origin + direction * result;
  return ((l2 - l1).length() > (res_point - l1).length()) && QVector3D::dotProduct(l2 - l1, res_point - l1) > 0 ;

}
