#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect Texture0;
varying vec4 TexCoord0;

void main(void)
{
    gl_FragColor = texture2DRect(Texture0, gl_FragCoord.xy);
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    gl_FragDepth = 0.;
}
