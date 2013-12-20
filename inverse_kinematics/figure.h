#ifndef FIGURE_H
#define FIGURE_H
#include <polygon.h>

class figure
{
public:
    QVector<polygon> polys;


    QMatrix4x4 model;
    QMatrix4x4 rot_scale_model;


    QVector<QVector4D> vertices;
    QVector<QVector4D> normals;
    QVector<QVector2D> texCoords;

    QGLShaderProgram *program;
    GLuint *textures;
    int without_texture;

    QVector4D material;

    int wireframe_mode;

    bool selected;


    figure()
    {
        wireframe_mode=false;
        selected = 0;

    }

    void set_wireframe(bool wireframe)
    {
        if(wireframe);
        {

        }

    }

    void setshaderprog(QGLShaderProgram* new_prog)
    {
        program=new_prog;
    }

    void draw();
    void add_poly();
    void update_polys_cash();
    void rebuild_polys_cash();
    void set_model_matrix(QVector3D pos, QVector3D scale);




    void draw_polys()
    {
        QMatrix4x4 identity;

        program->setUniformValue("model_matrix", identity);

        for(int i=0;i<polys.size();i++)
        {
            polys[i].program = program;
            polys[i].draw();
        }
    }


};

#endif // FIGURE_H
