attribute highp vec4 vertex;
attribute highp vec4 texCoord;
attribute highp vec4 normal;
varying highp vec4 texc;
varying highp vec4 pos;
varying highp vec4 normal_for_fs;
uniform highp mat4 model_matrix;
uniform highp mat4 view_matrix;
uniform highp mat4 proj_matrix;
void main(void)
{
    pos = model_matrix*vertex;
    gl_Position = proj_matrix*view_matrix*pos;
    texc = texCoord;
    normal_for_fs=normalize(model_matrix*normal);
}
