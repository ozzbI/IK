#version 120

varying highp vec4 pos;

void main(void)
{
    gl_FragColor = pos;
}
