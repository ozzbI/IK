#ifndef COORD_AXIS_H
#define COORD_AXIS_H

#include<C:\Qt_projects\Eigen\Dense>
#include <camera.h>
//#include <QGLShaderProgram>
#include <QtOpenGL>
//#include <QGLWidget>
//#include <QGLFunctions>

class coord_axis
{
private:

    QGLShaderProgram *program;
    QMatrix4x4 view;
    Vector3d pos;
    GLfloat vgl[16];
public:
    coord_axis()
    {
    }

    void setshaderprog(QGLShaderProgram* new_prog)
    {
        program=new_prog;
    }

    void draw(Camera *cam,double aspect_ratio)
    {
          cam->getViewMatrix(&view);//получение видовой матрицы
          view=view.transposed();
          cam->getPosition(&pos);

          program->release();

          for(int i=0;i<16;i++)
          {
           vgl[i]=(float)(*(view.data()+i));
          }

          glClear(GL_DEPTH_BUFFER_BIT);
          glLineWidth(2);
          glMatrixMode(GL_PROJECTION);

          glLoadIdentity();
          glOrtho(-aspect_ratio,aspect_ratio,-1,1,1,10);

          glMatrixMode(GL_MODELVIEW);

          glLoadIdentity();
          glTranslated(-aspect_ratio*0.8,-0.8,-2);
          glScaled(0.17,0.17,0.17);
          glMultMatrixf(vgl);

           //Z
          glBegin(GL_LINES);
          glColor3f(0,0,1);
          glVertex3f(pos(0),pos(1),pos(2));
          glColor3f(0,0,1);
          glVertex3f(pos(0),pos(1),pos(2)+1);
          glEnd();
          //X
          glBegin(GL_LINES);
          glColor3f(1,0,0);
          glVertex3f(pos(0),pos(1),pos(2));
          glColor3f(1,0,0);
          glVertex3f(pos(0)+1,pos(1),pos(2));
          glEnd();
          //Y
          glBegin(GL_LINES);
          glColor3f(0,1,0);
          glVertex3f(pos(0),pos(1),pos(2));
          glColor3f(0,1,0);
          glVertex3f(pos(0),pos(1)+1,pos(2));
          glEnd();

           program->bind();
    }
};

#endif // COORD_AXIS_H
