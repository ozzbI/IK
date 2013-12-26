#include "box.h"

box::box(int clipped)
{
    without_texture=1;

    static const int coords[6][6][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 }, { +1, -1, -1 }, { -1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 }, { +1, +1, -1 }, { -1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 }, { +1, -1, +1 }, { +1, +1, -1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 }, { -1, -1, -1 }, { -1, +1, +1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 }, { +1, -1, +1 }, { -1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 }, { -1, -1, +1 }, { +1, +1, +1 } }
    };


   /*for (int j=0; j < 6; ++j) {
        textures[j] = gl->bindTexture
         (QPixmap(QString(":/images/side%1.png").arg(j + 1)), GL_TEXTURE_2D);
    }*/

    for (int i = 0; i < 6; ++i)
    {
        if (clipped == 1)
        {
            if (i == 0|| i == 5) continue;
        }

        if (clipped == 2)
        {
            if (i == 5) continue;
        }

        for (int j = 0; j < 6; ++j)
        {

            texCoords.append
                (QVector2D(j == 0 || j == 3 , j == 0 || j == 1 || j == 4));//нужно править
            vertices.append
                (QVector4D(0.5*QVector3D(coords[i][j][0],coords[i][j][1],
                           coords[i][j][2]),1));

            if(i==0) normals.append(QVector4D(0,0,-1,0.0));
            if(i==1) normals.append(QVector4D(0,1,0,0.0));
            if(i==2) normals.append(QVector4D(1,0,0,0.0));
            if(i==3) normals.append(QVector4D(-1,0,0,0.0));
            if(i==4) normals.append(QVector4D(0,-1,0,0.0));
            if(i==5) normals.append(QVector4D(0,0,1,0.0));
        }


        /*for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 6; ++j)
            {
                if(i==0) normals.append(QVector4D(0,0,-1,0.0));
                if(i==1) normals.append(QVector4D(0,1,0,0.0));
                if(i==2) normals.append(QVector4D(1,0,0,0.0));
                if(i==3) normals.append(QVector4D(-1,0,0,0.0));
                if(i==4) normals.append(QVector4D(0,-1,0,0.0));
                if(i==5) normals.append(QVector4D(0,0,1,0.0));
            }
        }*/

    }
}

void box::setshaderprog(QGLShaderProgram* new_prog)
{
    program=new_prog;
}
void box::set_texures(GLuint *new_textures)
{
    textures=new_textures;
}

void box::set_gl_func(QGLFunctions* in_glfunc)
{
    glfunc=in_glfunc;
}

void box::set_material(QVector4D new_material)
{
    material=new_material;
}

void box::inverse_normals()
{
    for(int i=0;i<normals.size();i++) normals[i]=-normals[i];
}

void box::draw()
{
    program->setUniformValue("model_matrix", model);
    program->setUniformValue("material", material);
    program->setUniformValue("without_texture",without_texture);

    program->setAttributeArray
    (0, vertices.constData());
    program->setAttributeArray
    (1, texCoords.constData());
    program->setAttributeArray
    (2, normals.constData());


    /*for (int i = 0; i < 6; ++i)
    {
        //glBindTexture(GL_TEXTURE_2D, textures[i]);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }*/

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}
