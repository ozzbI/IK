#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect   aoMap;
uniform sampler2DRect   srcMap;

uniform vec2 viewport_size;

uniform int ssao;

void main (void)
{
    /*
    //b_filter
    vec2   b_sign;
    vec2   k;
    vec2   ao_texel_coord = gl_FragCoord.xy / 2.0;
    //b_filter -----
    */

    vec4    ao = vec4 ( 0.0 );


    if(bool(ssao)) // bilinear interpolation needed
    {
        /*
        k = fract(ao_texel_coord) - vec2(0.5);
        b_sign = sign( k );
        k = abs(k);

        ao = mix( mix(texture2DRect ( aoMap, ao_texel_coord ).r , texture2DRect ( aoMap, ao_texel_coord + vec2(b_sign.x, 0.0)).r  , k.x ),
                  mix( texture2DRect ( aoMap, ao_texel_coord + vec2(0.0, b_sign.y)).r , texture2DRect ( aoMap, ao_texel_coord + b_sign).r , k.x ) , k.y );
        */

         ao = vec4(texture2DRect ( aoMap, gl_FragCoord.xy ).r);
    }

    if(bool(ssao))
        gl_FragColor = vec4( (texture2DRect ( srcMap, gl_FragCoord.xy ).xyz * pow( vec3(ao), vec3 ( 2.0 ) ) ).xyz, 1.0);
    else
        gl_FragColor = texture2DRect ( srcMap, gl_FragCoord.xy );
}
