#ifndef CYLINDER_H
#define CYLINDER_H

#include <QtOpenGL>

class cylinder
{
private:
    GLuint *textures;
    QVector4D material;
    QVector<QVector4D> vertices;
    QVector<QVector4D> normals;
    QVector<QVector2D> texCoords;
    QGLShaderProgram *program;
    int precision;
    int without_texture;
public:
    QMatrix4x4 model;
    QMatrix4x4 add_model;
    cylinder()
    {
        //add_model.rotate(90,0,0,1);
        without_texture=1;
        //model.scale(1,1,1);
    }
    ~cylinder()
    {}

    void make_cylinder(double r, double h, int p )
    {
        precision=p;
        vertices.clear();
        normals.clear();
        texCoords.clear();

        double step_angle=360.0/p;
        QVector4D point(r,h/2,0,1);
        QMatrix4x4 rotate_matr;
        QMatrix4x4 transl_matr;

        rotate_matr.rotate(step_angle,QVector3D(0,1,0));

        for(int i=0;i<p;i++)
        {
            vertices.append(point);
            point=rotate_matr*point;
            normals.append(QVector4D(0,1,0,0));
            texCoords.append(QVector2D(0,0));
        }

        //rotate_matr.rotate(-2*step_angle,QVector3D(0,1,0));

        point=QVector4D(r,-h/2,0,1);
        for(int i=0;i<p;i++)
        {
            vertices.append(point);
            point=rotate_matr*point;
            normals.append(QVector4D(0,-1,0,0));
            texCoords.append(QVector2D(0,0));
        }

        for (int i=0;i<p;i++)
        {
            vertices.append(vertices[i]);

            point=vertices[i];
            point.setY(0);
            point.setW(0);
            point.normalize();
            normals.append(point);

            texCoords.append(QVector2D(0,0));
            vertices.append(vertices[i+p]);

            point=vertices[i+p];
            point.setY(0);
            point.setW(0);
            point.normalize();
            normals.append(point);

            texCoords.append(QVector2D(0,0));
        }

        vertices.append(vertices[0]);

        point=vertices[0];
        point.setY(0);
        point.setW(0);
        point.normalize();
        normals.append(point);

        texCoords.append(QVector2D(0,0));
        vertices.append(vertices[p]);

        point=vertices[p];
        point.setY(0);
        point.setW(0);
        point.normalize();
        normals.append(point);

        texCoords.append(QVector2D(0,0));


        //переворот дна цилиндра
        for(int i=0;i<p/2;i++)
        {
            point=vertices[p+i];
            vertices[p+i]=vertices[2*p-1-i];
            vertices[2*p-1-i]=point;
        }

    }


    void setshaderprog(QGLShaderProgram* new_prog)
    {
        program=new_prog;
    }
    void set_texures(GLuint *new_textures)
    {
        textures=new_textures;
    }

    void set_material(QVector4D new_material)
    {
        material=new_material;
    }
    void draw()
    {

        model=model*add_model;
        program->setUniformValue("model_matrix", model);
        program->setUniformValue("without_texture",without_texture);

        program->setAttributeArray
            (0, vertices.constData());
        program->setAttributeArray
            (1, texCoords.constData());
        program->setAttributeArray
            (2, normals.constData());

        program->setUniformValue("material",material);
        //if(!without_texture)glBindTexture(GL_TEXTURE_2D, textures[0]);
        glDrawArrays(GL_POLYGON,0, precision);
        glDrawArrays(GL_POLYGON,precision, precision);
        glDrawArrays(GL_QUAD_STRIP,precision*2, precision*2+2);

    }

};
#endif // CYLINDER_H
