#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect srcMap;


void main (void)
{
    vec2   b_sign;
    vec2   k;
    vec2   ao_texel_coord = gl_FragCoord.xy / 2.0;
    vec4    res = vec4 ( 0.0 );

    k = fract(ao_texel_coord) - vec2(0.5);
    b_sign = sign( k );
    k = abs(k);

    res = mix( mix(texture2DRect ( srcMap, ao_texel_coord ) , texture2DRect ( srcMap, ao_texel_coord + vec2(b_sign.x, 0.0))  , k.x ),
              mix( texture2DRect ( srcMap, ao_texel_coord + vec2(0.0, b_sign.y)) , texture2DRect ( srcMap, ao_texel_coord + b_sign) , k.x ) , k.y );

    gl_FragColor = vec4(res);
}
