#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect   aoMap;
uniform sampler2DRect   srcMap;

uniform vec2 viewport_size;

uniform int ssao;
uniform int shadows;

void main (void)
{
    float   h1 = 0.5 / viewport_size.x;           // half of a pixel (x)
    float   h2 = 0.5 / viewport_size.y;           // half of a pixel (y)
    vec4    ao = vec4 ( 0.0 );

    if(ssao)
    {
        for ( int i = -1; i <= 1; i++ )
            for ( int j = -1; j <= 1; j++ )
                ao += texture2DRect ( aoMap, gl_FragCoord.xy  + vec2 (  (i), (j) ) );
    }

    //ao = texture2DRect ( aoMap, gl_FragCoord.xy);

    if(ssao)
        gl_FragColor = texture2DRect ( srcMap, gl_FragCoord.xy ) * pow ( ao / 9.0, vec4 ( 2.0 ) );
    else
        gl_FragColor = texture2DRect ( srcMap, gl_FragCoord.xy );
    //gl_FragColor = pow ( ao / 25.0, vec4 ( 2.0 ));

}
