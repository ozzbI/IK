#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect aoMap;


void main (void)
{
    vec4    ao = vec4 ( 0.0 );

    for ( int i = -1; i <= 1; i++ )
        for ( int j = -1; j <= 1; j++ )
            ao += texture2DRect ( aoMap, gl_FragCoord.xy + vec2 (  (i), (j) ) );

    gl_FragColor = ao/9;
}
