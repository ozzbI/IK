#version 150

attribute highp vec4 vertex;

varying highp vec4 pos;

uniform highp mat4 model_matrix;
uniform highp mat4 view_matrix;
uniform highp mat4 proj_matrix;

void main(void)
{
    pos = model_matrix*vertex;
    gl_Position = proj_matrix*view_matrix*pos;
}
