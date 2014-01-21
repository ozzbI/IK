#version 120

attribute highp vec4 vertex;
attribute highp vec4 normal;
varying highp vec4 normal_for_fs;
uniform highp mat4 model_matrix;
uniform highp mat4 view_matrix;
uniform highp mat4 proj_matrix;
void main(void)
{
    gl_Position = proj_matrix * view_matrix * model_matrix * vertex;
    normal_for_fs = normalize(view_matrix * model_matrix * normal);
}
