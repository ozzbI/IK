#version 150

uniform highp vec4 material;

void main(void)
{
    gl_FragColor = material;
    gl_FragDepth = 0.;
}
