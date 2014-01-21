#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect posMap;
uniform samplerCube shadowMap;
uniform vec3 light_pos;
uniform mat4 proj_view_matrices[6];

#define shadow_k 0.4

void main (void)
{
   gl_FragColor = vec4(0.0, 0.4, 0.0, 0.0);
}
