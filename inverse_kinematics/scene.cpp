#include "scene.h"

Scene::Scene(QGLShaderProgram *program)
{
    scene_program = program;
    chain_links_n = 0;
    b1 = new box();
    b1->setshaderprog(program);

    scene_octree = new octree(4);
    chain_octree = new octree(50);

    dcThread = new direct_collision_Thread();

    dcWorker = new dc_Worker ();
    dcWorker->set_scene(this);

    dc_proc_ended = false;

    //dcThread->set_scene(this);

    dcWorker->moveToThread(dcThread);

    connect(this,SIGNAL(dc_start_process()),dcWorker, SLOT(process()));
    connect(this,SIGNAL(lock_dc_mutex()),dcWorker, SLOT(lock_dc_mutex()),Qt::BlockingQueuedConnection);


    dcThread->start();

    emit lock_dc_mutex();

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

    //добавление ребер (только для кубов , если переданная геметрия не соответсвует кубу то :( )
    edge e;

    e.A = QVector3D(-1.0, -1.0, 1.0) * 0.5; e.B = QVector3D(1.0, -1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, -1.0, 1.0) * 0.5; e.B = QVector3D(1.0, 1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, 1.0, 1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(-1.0, 1.0, 1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);




    e.A = QVector3D(-1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, -1.0, -1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, 1.0, -1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, -1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(-1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, -1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);



    e.A = QVector3D(-1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, -1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(1.0, 1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);

    e.A = QVector3D(-1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, 1.0) * 0.5;
    Scene_objects[size].edges_ref.push_back(e);


    for (i = 0; i < Scene_objects[size].edges_ref.size(); i++)
    {
        Scene_objects[size].edges.push_back(e);
    }

    //edges end

    Scene_objects[size].program = program;
    Scene_objects[size].material = material;
    Scene_objects[size].textures = textures;
    Scene_objects[size].without_texture = without_texture;

}

void Scene::update_IK_Chain_objects(kinematic_chain* Chain)
{
    /* при удалении/вставке звеньев  */

    box b(1);
    box b0(2);

    IKChain = Chain;

    IKChain->collision = false;

    if(chain_links_n != 0) //если цепь уже задавалась
    {
        Scene_objects.remove(0, chain_links_n);
    }

    chain_links_n = IKChain->links.size();

    int i,j;

    for(j = IKChain->links.size() - 1; j >= 0; j--)
    {

        int size = 0; // need to refact

        Scene_objects.push_front( *(new figure()) );


        if(j == chain_links_n - 1 )
        {
            for(i = 0; i < b0.vertices.size(); i++)
            {
                Scene_objects[size].vertices.push_back( b0.vertices[i]);
            }
        }
        else
        {
            for(i = 0; i < b.vertices.size(); i++)
            {
                Scene_objects[size].vertices.push_back( b.vertices[i]);
            }
        }

        Scene_objects[size].program = scene_program;


        //добавление ребер (только для кубов , если переданная геметрия не соответсвует кубу то :( )
        edge e;

        e.A = QVector3D(-1.0, -1.0, 1.0) * 0.5; e.B = QVector3D(1.0, -1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, -1.0, 1.0) * 0.5; e.B = QVector3D(1.0, 1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, 1.0, 1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(-1.0, 1.0, 1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);


        e.A = QVector3D(-1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, -1.0, -1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, 1.0, -1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, -1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(-1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, -1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);


        e.A = QVector3D(-1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, -1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, -1.0, -1.0) * 0.5; e.B = QVector3D(1.0, -1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(1.0, 1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        e.A = QVector3D(-1.0, 1.0, -1.0) * 0.5; e.B = QVector3D(-1.0, 1.0, 1.0) * 0.5;
        Scene_objects[size].edges_ref.push_back(e);

        for (i = 0; i < Scene_objects[size].edges_ref.size(); i++)
        {
            Scene_objects[size].edges.push_back(e);
        }

        //edges end
    }

    build_chain_vertices_cash();

    scene_octree->cleanup();
    build_octree(100.0, false);
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

void Scene::build_object_vertices_cash(int i)
{

    Scene_objects[i].update_polys_cash();

}

void Scene::rebuild_object_vertices_cash(int i)
{

    Scene_objects[i].rebuild_polys_cash();

}

void Scene::build_object_vertices_cash()
{
    for(int i = chain_links_n; i < Scene_objects.size(); i++)
    {
        Scene_objects[i].update_polys_cash();
    }
}

void Scene::rebuild_object_vertices_cash()
{
    for(int i = chain_links_n; i < Scene_objects.size(); i++)
    {
        Scene_objects[i].rebuild_polys_cash();
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
    bool res = false;

    emit dc_start_process();

    QVector3D inters_point;
    int figure_id;

    for(int i = 0; i < chain_links_n; i++)
    {
        res = collision_detect_octree(i, Scene_objects, inters_point, figure_id);
        if(res) break;
    }


    //40% direct collision

    /*int n40 = (Scene_objects.size() - chain_links_n ) * 4 / 10 + chain_links_n;

    for(int c = chain_links_n; c < n40 ; c++)
    {
        for(int i = 0; i < chain_links_n; i++)
        {
            direct_collision_detect(c, i, Scene_objects);
        }
            //direct_collision_detect(c, part_id, figures);
            //if(direct_collision_detect(c, part_id, figures))
                //return true;
    }*/


    dc_mutex.lock();
    /*for (int i = 0 ; i < intersect_points.size(); i++)
    {
        //emit draw_collision_box(intersect_points[i]);

        QMatrix4x4 matr;

        matr.translate(intersect_points[i]);

        matr.scale(0.1);

        b1->model=matr;
        b1->draw();
    }*/

    if(!res)
    {
        if(intersect_points.size() > 0)
            res = true;
    }

    intersect_points.clear();
    dc_mutex.unlock();

    emit lock_dc_mutex();

    return res;

    /*for(int c = chain_links_n; c < Scene_objects.size();c++)
    {
        for(int i = 0; i < chain_links_n; i++)
        {
            direct_collision_detect(c, i, Scene_objects);
        }
            //direct_collision_detect(c, part_id, figures);
            //if(direct_collision_detect(c, part_id, figures))
                //return true;
    }*/

}


bool Scene::collision_detect(int part_id, QVector<figure> &figures)
{/*
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
                        p1 = figures[part_id].polys[p].vertices[v];// надобы вынести
                        p2 = figures[part_id].polys[p].vertices[0];// из цикла
                        pa = figures[c].polys[i].vertices[0];
                        pb = figures[c].polys[i].vertices[1];
                        pc = figures[c].polys[i].vertices[2];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;

                            matr.translate(inters_point);

                            matr.scale(0.1);

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

                                matr.translate(inters_point);

                                matr.scale(0.1);

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

                            matr.translate(inters_point);

                            matr.scale(0.1);

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

                                matr.translate(inters_point);

                                matr.scale(0.1);

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
    */

    //---------------------------
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;
    for(int p = 0; p < figures[part_id].polys.size();p++)
    {
        for(int v = 0; v < figures[part_id].polys[p].vertices.size();v++)
        {

            if(v == figures[part_id].polys[p].vertices.size() - 1)
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[0];
            }
            else
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[v + 1];
            }

            for(int c = chain_links_n; c < figures.size();c++)
            {
                if(c == part_id) continue;

                for(int i = 0; i < figures[c].polys.size();i++)
                {
                    pa = figures[c].polys[i].vertices[0];
                    pb = figures[c].polys[i].vertices[1];
                    pc = figures[c].polys[i].vertices[2];
                    if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                    {

                        QMatrix4x4 matr;

                        matr.translate(inters_point);

                        matr.scale(0.1);

                        b1->model=matr;
                        b1->draw();
                        //qDebug("collision");
                        //return true;
                    }
                    if(figures[c].polys[i].vertices.size() == 4)
                    {

                        pa = figures[c].polys[i].vertices[0];
                        pb = figures[c].polys[i].vertices[2];
                        pc = figures[c].polys[i].vertices[3];
                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;

                            matr.translate(inters_point);

                            matr.scale(0.1);

                            b1->model = matr;
                            b1->draw();
                            //qDebug("collision");
                            //return true;
                        }
                    }

                }
            }
        }
    }

    /*for(int c = chain_links_n; c < figures.size();c++)
    {
        direct_collision_detect(c, part_id, figures);
        //if(direct_collision_detect(c, part_id, figures))
            //return true;
    }*/

    return false;
}


bool Scene::collision_detect_octree(int part_id, QVector<figure> &figures, QVector3D &inters_point, int &figure_id)
{
    //QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;

    /*for(int p = 0; p < figures[part_id].polys.size();p++) //edges
    {
        for(int v = 0; v < figures[part_id].polys[p].vertices.size();v++)
        {

            if(v == figures[part_id].polys[p].vertices.size() - 1)
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[0];
            }
            else
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[v + 1];
            }
*/
    for(int p = 0; p < figures[part_id].edges.size();p++)
    {

        p1 = figures[part_id].edges[p].A;
        p2 = figures[part_id].edges[p].B;

            //octree

            //определить ноду в которой находиться p1 и p2

            OctreeNode *current_node = scene_octree->getRoot();

            int stop_flag = 1;

            for(;;)
            {
                for(int n = 0; n < current_node->children().size(); n++  )
                {
                    int hit_flag = 0;
                    OctreeNode *current_child_node = &*current_node->children()[n];


                    double i, j, k;

                    //p1
                    i = p1.x();
                    j = p1.y();
                    k = p1.z();

                    if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                        j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                        k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                      hit_flag++ ;

                    //p2 можно поставить if если hit_flag < 1
                    i = p2.x();
                    j = p2.y();
                    k = p2.z();

                    if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                        j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                        k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                      hit_flag++ ;

                    if(hit_flag == 2)
                    {

                        for( int i = 0; i < current_node->polys().size(); i++ )//проверка полигонов в текущей ноде
                        {
                            //QVector3D pa,pb,pc;

                            pa = current_node->polys()[i]->vertices[0];
                            pb = current_node->polys()[i]->vertices[1];
                            pc = current_node->polys()[i]->vertices[2];

                            if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                            {

                                /*QMatrix4x4 matr;

                                matr.translate(inters_point);

                                matr.scale(0.1);

                                b1->model=matr;
                                b1->draw();*/

                                //draw_collision_box(inters_point);

                                //qDebug("collision");
                                figure_id = current_node->polys()[i]->figure_id;
                                return true;
                            }
                        }

                        current_node = current_child_node;

                        if(current_child_node->children().size() == 0)
                            stop_flag = 1;
                        else
                            stop_flag = 0;

                        break;
                    }
                    else
                    {
                        stop_flag = 1;
                    }

                }

                if(stop_flag) break;

            }

            if (octree_traverse(p1, p2, *current_node, inters_point, figure_id))
                return true;

            // octree
        //} //edges
    }

    /*for(int c = chain_links_n; c < figures.size();c++)
    {
        direct_collision_detect(c, part_id, figures);
        //if(direct_collision_detect(c, part_id, figures))
            //return true;
    }*/

    return false;

}

bool Scene::octree_traverse(QVector3D &p1, QVector3D &p2, OctreeNode& node, QVector3D &inters_point, int &figure_id)
{
    for( int i = 0; i < node.polys().size();i++ )//проверка полигонов в ноде
    {
        QVector3D pa,pb,pc;

        pa = node.polys()[i]->vertices[0];
        pb = node.polys()[i]->vertices[1];
        pc = node.polys()[i]->vertices[2];

        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
        {

            /*QMatrix4x4 matr;

            matr.translate(inters_point);

            matr.scale(0.1);

            b1->model=matr;
            b1->draw();*/
            //qDebug("collision");
            figure_id = node.polys()[i]->figure_id;
            return true;
        }
    }


    for( int i = 0; i < node.children().size(); i++ ) // проверка потомков ноды
    {
        if(octree_traverse(p1, p2, *node.children()[i], inters_point, figure_id ))
            return true;
    }

    return false;
}

bool Scene::direct_collision_detect(int part_id,int test_part_id, QVector<figure> &figures, QVector3D &inters_point)
{
    /*
      //1 var
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;
    for(int p = 0; p < figures[part_id].polys.size();p++)
    {
        for(int v = 0; v < figures[part_id].polys[p].vertices.size();v++)
        {

            if(v == figures[part_id].polys[p].vertices.size() - 1)
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[0];
            }
            else
            {
                p1 = figures[part_id].polys[p].vertices[v];
                p2 = figures[part_id].polys[p].vertices[v + 1];
            }

            for(int i = 0; i < figures[test_part_id].polys.size();i++)
            {

                pa = figures[test_part_id].polys[i].vertices[0];
                pb = figures[test_part_id].polys[i].vertices[1];
                pc = figures[test_part_id].polys[i].vertices[2];
                if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                {

                    QMatrix4x4 matr;

                    matr.translate(inters_point);

                    matr.scale(0.1);

                    b1->model=matr;
                    b1->draw();
                    qDebug("collision");
                    //return true;
                }
                if(figures[test_part_id].polys[i].vertices.size() == 4)
                {

                    pa = figures[test_part_id].polys[i].vertices[0];
                    pb = figures[test_part_id].polys[i].vertices[2];
                    pc = figures[test_part_id].polys[i].vertices[3];
                    if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                    {

                        QMatrix4x4 matr;
                        matr.translate(inters_point);

                        matr.scale(0.1);

                        b1->model = matr;
                        b1->draw();
                        qDebug("collision");
                        //return true;
                    }
                }

            }
        }
    }
    return false;
    */

    QVector3D p1,p2,pa,pb,pc;

    figure& f = figures[part_id];
    figure& ft = figures[test_part_id];

    int e_size = figures[part_id].edges.size();

    int t_size = figures[test_part_id].polys.size();

    for(int p = 0; p < e_size; p++)
    {
        p1 = f.edges[p].A;
        p2 = f.edges[p].B;

        for(int i = 0; i < t_size; i++)
        {
            polygon& p = ft.polys[i];

            pa = p.vertices[0];
            pb = p.vertices[1];
            pc = p.vertices[2];

            if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
            {

                /*QMatrix4x4 matr;

                matr.translate(inters_point);

                matr.scale(0.1);

                b1->model=matr;
                b1->draw();*/
                //qDebug("collision");
                return true;
            }
            /*if(figures[test_part_id].polys[i].vertices.size() == 4)
            {

                pa = figures[test_part_id].polys[i].vertices[0];
                pb = figures[test_part_id].polys[i].vertices[2];
                pc = figures[test_part_id].polys[i].vertices[3];
                if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                {

                    QMatrix4x4 matr;
                    matr.translate(inters_point);

                    matr.scale(0.1);

                    b1->model = matr;
                    b1->draw();
                    //qDebug("collision");
                    //return true;
                }
            }*/

        }

        //octree optimization
/*
        OctreeNode *current_node = chain_octree->getRoot();

        int stop_flag = 1;

        for(;;)
        {


            for(int n = 0; n < current_node->children().size(); n++  )
            {
                int hit_flag = 0;
                OctreeNode *current_child_node = &*current_node->children()[n];


                int i, j, k;

                //p1
                i = p1.x();
                j = p1.y();
                k = p1.z();

                if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                    j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                    k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                  hit_flag++ ;

                //p2
                i = p2.x();
                j = p2.y();
                k = p2.z();

                if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                    j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                    k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                  hit_flag++ ;

                if(hit_flag == 2)
                {

                    for( int i = 0; i < current_node->polys().size();i++ )//проверка полигонов в текущей ноде
                    {
                        QVector3D pa,pb,pc;

                        pa = current_node->polys()[i]->vertices[0];
                        pb = current_node->polys()[i]->vertices[1];
                        pc = current_node->polys()[i]->vertices[2];

                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {

                            QMatrix4x4 matr;

                            matr.translate(inters_point);

                            matr.scale(0.1);

                            b1->model=matr;
                            b1->draw();
                            qDebug("collision");
                            //return true;
                        }
                    }

                    current_node = current_child_node;

                    if(current_child_node->children().size() == 0)
                        stop_flag = 1;
                    else
                        stop_flag = 0;

                    break;
                }
                else
                {
                    stop_flag = 1;
                }

            }

            if(stop_flag) break;

        }

        octree_traverse(p1, p2, *current_node, inters_point);
        */
        //octree end

    }
    return false;
}

bool Scene::direct_collision_detect_closest_to_point(int part_id,int test_part_id, QVector<figure> &figures, QVector3D &inters_point, QVector3D point)
{
    QVector3D p1, p2, pa, pb, pc, temp_inters_point;

    figure& f = figures[part_id];
    figure& ft = figures[test_part_id];

    int e_size = figures[part_id].edges.size();

    int t_size = figures[test_part_id].polys.size();

    bool intersect = false;
    float min_length = 1000000;

    for(int p = 0; p < e_size; p++)
    {
        p1 = f.edges[p].A;
        p2 = f.edges[p].B;

        for(int i = 0; i < t_size; i++)
        {
            polygon& p = ft.polys[i];

            pa = p.vertices[0];
            pb = p.vertices[1];
            pc = p.vertices[2];

            if(poly_intersect(p1, p2, pa, pb, pc, temp_inters_point))
            {
                if(min_length > (temp_inters_point - point).length())
                {
                    inters_point = temp_inters_point;
                    min_length = (temp_inters_point - point).length();
                }
                intersect = true;
            }
        }
    }
    return intersect;
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

void Scene::build_octree(double boundingBoxSize, bool for_chain)
{
    if(!for_chain)
        scene_octree->build(Scene_objects, chain_links_n,QVector3D(-boundingBoxSize/2.0 , -boundingBoxSize/2.0, -boundingBoxSize/2.0), QVector3D(boundingBoxSize/2.0 , boundingBoxSize/2.0, boundingBoxSize/2.0), for_chain);
    else
        chain_octree->build(Scene_objects, chain_links_n,QVector3D(-boundingBoxSize/2.0 , -boundingBoxSize/2.0, -boundingBoxSize/2.0), QVector3D(boundingBoxSize/2.0 , boundingBoxSize/2.0, boundingBoxSize/2.0), for_chain);

}

void Scene::update_polys_id()
{
    for(int i = chain_links_n; i < Scene_objects.size() ; i++)
    {
        for(int j = 0; j < Scene_objects[i].polys.size(); j++)
        {
            Scene_objects[i].polys[j].figure_id = i;
        }
    }
}

void direct_collision_Thread::run()
{
   /* for(int c = scene->chain_links_n; c < scene->Scene_objects.size();c++)
    {
        for(int i = 0; i < scene->chain_links_n; i++)
        {
            direct_collision_detect(c, i, scene->Scene_objects);
        }
    }
    */
    exec();
}

bool dc_Worker::process()
{
    //scene->dc_mutex.lock();

    /*int n = (scene->Scene_objects.size() - scene->chain_links_n);

    int n40 = (n) * 4 / 10 + scene->chain_links_n;

    int o = ((n) * 4) % 10;

    int n60 = ((n) * 6 + o) / 10 + n40;

    for(int c = n40; c < n60; c++)
        {
            for(int i = 0; i < scene->chain_links_n; i++)
            {
                direct_collision_detect(c, i, scene->Scene_objects);
            }
        }*/

    /*for(int c = scene->chain_links_n; c < scene->Scene_objects.size(); c++)
        {

                    direct_collision_detect_octree(c, 0, scene->Scene_objects);

        }
*/
    int res = false;

    for(int c = scene->chain_links_n; c < scene->Scene_objects.size(); c++)
    {
        for(int i = 0; i < scene->chain_links_n; i++)
        {
            if(direct_collision_detect(c, i, scene->Scene_objects))
            {
                res = true;
                break;
            }
        }

        if (res) break;
    }

    scene->dc_mutex.unlock();
    return res;
}

bool dc_Worker::direct_collision_detect(int part_id,int test_part_id, QVector<figure> &figures)
{
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;

    figure& f = figures[part_id];
    figure& ft = figures[test_part_id];

    int e_size = figures[part_id].edges.size();

    int t_size = figures[test_part_id].polys.size();

    for(int p = 0; p < e_size; p++)
    {
        p1 = f.edges[p].A;
        p2 = f.edges[p].B;

        for(int i = 0; i < t_size; i++)
        {
            polygon& p = ft.polys[i];

            pa = p.vertices[0];
            pb = p.vertices[1];
            pc = p.vertices[2];

            if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
            {
                scene->intersect_points.push_back(inters_point);
                return true;
            }
            /*
            if(figures[test_part_id].polys[i].vertices.size() == 4)
            {

                pa = figures[test_part_id].polys[i].vertices[0];
                pb = figures[test_part_id].polys[i].vertices[2];
                pc = figures[test_part_id].polys[i].vertices[3];
                if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                {
                    scene->intersect_points.push_back(inters_point);
                    //emit draw_collision_box(inters_point);
                    //qDebug("collision");
                    //return true;
                }
            }*/
        }
    }

    return false;
}

int dc_Worker::poly_intersect( QVector3D l1, QVector3D l2, QVector3D p0, QVector3D p1, QVector3D p2,QVector3D& res_point )
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

bool dc_Worker::direct_collision_detect_octree(int part_id,int test_part_id, QVector<figure> &figures)
{
    QVector3D inters_point;
    QVector3D p1,p2,pa,pb,pc;

    figure& f = figures[part_id];
    figure& ft = figures[test_part_id];

    int e_size = figures[part_id].edges.size();

    int t_size = figures[test_part_id].polys.size();

    for(int p = 0; p < e_size; p++)
    {
        p1 = f.edges[p].A;
        p2 = f.edges[p].B;

        //octree optimization

        OctreeNode *current_node = scene->chain_octree->getRoot();

        int stop_flag = 1;

        for(;;)
        {
            for(int n = 0; n < current_node->children().size(); n++  )
            {
                int hit_flag = 0;
                OctreeNode *current_child_node = &*current_node->children()[n];


                int i, j, k;

                //p1
                i = p1.x();
                j = p1.y();
                k = p1.z();

                if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                    j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                    k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                  hit_flag++ ;

                //p2
                i = p2.x();
                j = p2.y();
                k = p2.z();

                if( i <= current_child_node->maxBorder.x() && i >= current_child_node->minBorder.x() &&
                    j <= current_child_node->maxBorder.y() && j >= current_child_node->minBorder.y() &&
                    k <= current_child_node->maxBorder.z() && k >= current_child_node->minBorder.z() )
                  hit_flag++ ;

                if(hit_flag == 2)
                {

                    for( int i = 0; i < current_node->polys().size();i++ )//проверка полигонов в текущей ноде
                    {
                        QVector3D pa,pb,pc;

                        pa = current_node->polys()[i]->vertices[0];
                        pb = current_node->polys()[i]->vertices[1];
                        pc = current_node->polys()[i]->vertices[2];

                        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
                        {
                            scene->intersect_points.push_back(inters_point);
                        }
                    }

                    current_node = current_child_node;

                    if(current_child_node->children().size() == 0)
                        stop_flag = 1;
                    else
                        stop_flag = 0;

                    break;
                }
                else
                {
                    stop_flag = 1;
                }

            }

            if(stop_flag) break;

        }

        octree_traverse(p1, p2, *current_node, inters_point);

        //octree end

    }

    return false;

}

bool dc_Worker::octree_traverse(QVector3D &p1, QVector3D &p2, OctreeNode& node, QVector3D &inters_point)
{
    for( int i = 0; i < node.polys().size();i++ )//проверка полигонов в ноде
    {
        QVector3D pa,pb,pc;

        pa = node.polys()[i]->vertices[0];
        pb = node.polys()[i]->vertices[1];
        pc = node.polys()[i]->vertices[2];

        if(poly_intersect(p1,p2,pa,pb,pc,inters_point))
        {
            scene->intersect_points.push_back(inters_point);
        }
    }


    for( int i = 0; i < node.children().size(); i++ ) // проверка потомков ноды
    {
        if(octree_traverse(p1, p2, *node.children()[i], inters_point ))
            return true;
    }

    return false;
}
