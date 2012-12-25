#ifndef BOX_H
#define BOX_H

//#include <QtCore>
//#include <QGLShaderProgram>
#include <QtOpenGL>
//#include <QGLWidget>

class box
{
public:
    box ();
    void setshaderprog(QGLShaderProgram* new_prog);
    void set_material(QVector4D new_material);
    void set_texures(GLuint* new_textures);
    void set_gl_func(QGLFunctions* in_glfunc);
    void inverse_normals();
    void draw();

    int without_texture;
    QGLFunctions* glfunc;
    QMatrix4x4 model;
    GLuint *textures;
    QVector<QVector4D> vertices;
    QVector<QVector4D> normals;
    QVector<QVector2D> texCoords;
    QGLShaderProgram *program;
    QVector4D material;
};

#endif // BOX_H
