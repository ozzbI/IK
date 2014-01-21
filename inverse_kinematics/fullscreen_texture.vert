#version 120

attribute vec4 vertex;
attribute vec4 MultiTexCoord0;
varying vec4 TexCoord0;

void main(void)
{
    gl_Position = vertex;
    TexCoord0 = MultiTexCoord0;
}
