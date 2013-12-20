#include "figure.h"

void figure::draw()
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

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());



    if (selected)
    {
        program->setUniformValue("selected", selected);

        glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

        program->setUniformValue("selected", 0);
    }
}

void figure::add_poly()
{

}

void figure::update_polys_cash()
{
    int polys_count = (vertices.size() + 1) / 3 ;

    polygon temp_poly;

    for(int i = 0; i < polys_count; i++)
    {
        polys.push_back(temp_poly);

        polys[i].vertices.push_back(QVector3D( model * vertices[i*3] ));
        polys[i].vertices.push_back(QVector3D( model * vertices[i*3 + 1] ));
        polys[i].vertices.push_back(QVector3D( model * vertices[i*3 + 2] ));
    }
}

void figure::rebuild_polys_cash()
{
    for(int i = 0; i < polys.size(); i++)
    {
        polys[i].vertices[0] = (QVector3D( model * vertices[i*3] ));
        polys[i].vertices[1] = (QVector3D( model * vertices[i*3 + 1] ));
        polys[i].vertices[2] = (QVector3D( model * vertices[i*3 + 2] ));
    }
}

void figure::set_model_matrix(QVector3D pos, QVector3D scale)
{
    model.setToIdentity();
    model.translate(pos);

    rot_scale_model.setToIdentity();

    rot_scale_model.scale(scale);

    model = model * rot_scale_model;
}


