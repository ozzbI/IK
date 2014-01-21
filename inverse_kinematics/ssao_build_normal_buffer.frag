#version 120

varying highp vec4 normal_for_fs;

void main(void)
{
    gl_FragColor = normal_for_fs;
}
