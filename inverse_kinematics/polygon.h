#ifndef POLYGON_H
#define POLYGON_H

#include <QtOpenGL>

class polygon
{
public:

    QGLShaderProgram *program;

    QVector<QVector3D> vertices;


    polygon()
    {

    }

    void draw()
    {

        program->setAttributeArray
        (0, vertices.constData());

        glDrawArrays(GL_POLYGON,0,vertices.size());

    }



};


#endif // POLYGON_H

