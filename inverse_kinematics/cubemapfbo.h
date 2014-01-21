#ifndef CUBEMAPFBO_H
#define CUBEMAPFBO_H

#include <QtOpenGL>

class CubeMapFBO
{
public:

    GLuint cube_texture;
    GLuint fb, depth_rb;

    QOpenGLFunctions *glFuncs;

    int width, height;

    CubeMapFBO();
    ~CubeMapFBO();

    void init(int width, int height, QOpenGLFunctions *glf);
    void bind(GLuint face);
    void release();
};

#endif // CUBEMAPFBO_H
