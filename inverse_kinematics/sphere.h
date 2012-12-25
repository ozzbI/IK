#ifndef SPHERE_H
#define SPHERE_H

 #include <QtOpenGL>

class sphere
{
public:

    QMatrix4x4 model;
    GLuint *textures;
    QVector<QVector4D> vertices;
    QVector<QVector4D> normals;
    QVector<QVector2D> texCoords;
    QGLShaderProgram *program;
    QVector4D material;
    int vertices_num;
    int without_texture;

    sphere()
    {
        vertices_num=0;
        without_texture=1;
        //model.scale(1,1,1);
    }
    ~sphere()
    {}

    void make_sphere( float cx, float cy, float cz, float r, int p )
    {
        vertices.clear();
        normals.clear();
        texCoords.clear();

        //const float PI     = 3.14159265358979f;
        const float TWOPI  = 6.28318530717958f;
        const float PIDIV2 = 1.57079632679489f;

        float theta1 = 0.0;
        float theta2 = 0.0;
        float theta3 = 0.0;

        float ex = 0.0f;
        float ey = 0.0f;
        float ez = 0.0f;

        float px = 0.0f;
        float py = 0.0f;
        float pz = 0.0f;

        // Disallow a negative number for radius.
        if( r < 0 )
            r = -r;

        // Disallow a negative number for precision.
        if( p < 0 )
            p = -p;

        // If the sphere is too small, just render a OpenGL point instead.
        if( p < 4 || r <= 0 )
        {
            glBegin( GL_POINTS );
            glVertex3f( cx, cy, cz );
            glEnd();
            return;
          }
        vertices_num=0;

        for( int i = 0; i < p/2; ++i )
        {
            theta1 = i * TWOPI / p - PIDIV2;
            theta2 = (i + 1) * TWOPI / p - PIDIV2;


                for( int j = 0; j <= p; ++j )
                {
                    theta3 = j * TWOPI / p;

                    ex = cosf(theta2) * cosf(theta3);
                    ey = sinf(theta2);
                    ez = cosf(theta2) * sinf(theta3);
                    px = cx - r * ex;
                    py = cy + r * ey;
                    pz = cz + r * ez;

                    normals.append(QVector4D( -ex, ey, ez,0));
                    texCoords.append(QVector2D( (j/(float)p) , 2*(i+1)/(float)p));
                    vertices.append(QVector4D( px, py, pz,1));

                    ex = cosf(theta1) * cosf(theta3);
                    ey = sinf(theta1);
                    ez = cosf(theta1) * sinf(theta3);
                    px = cx - r * ex;
                    py = cy + r * ey;
                    pz = cz + r * ez;

                    normals.append(QVector4D( -ex, ey, ez,0));
                    texCoords.append(QVector2D( (j/(float)p), 2*i/(float)p ));
                    vertices.append(QVector4D( px, py, pz,1));
                    vertices_num++;
                }
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

    program->setUniformValue("model_matrix", model);
    program->setUniformValue("without_texture",without_texture);

    program->setAttributeArray
        (0, vertices.constData());
    program->setAttributeArray
        (1, texCoords.constData());
    program->setAttributeArray
        (2, normals.constData());

    program->setUniformValue("material",material);
    if(!without_texture)glBindTexture(GL_TEXTURE_2D, textures[0]);
    glDrawArrays(GL_TRIANGLE_STRIP,0, vertices_num*2);

    }

};

#endif // SPHERE_H
