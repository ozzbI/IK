#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D       rotateMap;
uniform sampler2DRect	posMap;
uniform sampler2DRect	normalMap;

uniform vec2 viewport_size;

void main (void)
{
    const float zFar        = 300.0;
    const float zNear       = 1.0;
    const float radius      = 0.15;
    const float minCrease   = -0.1;
    const float creaseScale = 1.0;
    const float distScale   = 0.65;


    vec2 rndTable [24] = vec2 [24]
    (
       vec2 ( -0.326212, -0.405805 ),
       vec2 ( -0.840144, -0.073580 ),
       vec2 ( -0.695914,  0.457137 ),
       vec2 ( -0.203345,  0.620716 ),
       vec2 (  0.962340, -0.194983 ),
       vec2 (  0.473434, -0.480026 ),
       vec2 (  0.519456,  0.767022 ),
       vec2 (  0.185461, -0.893124 ),
       vec2 (  0.507431,  0.064425 ),
       vec2 (  0.896420,  0.412458 ),
       vec2 ( -0.321940, -0.932615 ),
       vec2 ( -0.791559, -0.597705 ),
       vec2 (  0.538154,  0.185612 ),
       vec2 (  0.3371, 0.5679 ),
       vec2 (  0.0689,-0.1598),
       vec2 (  0.146, 0.4402),
       vec2 ( -0.0146, 0.1402),
       vec2 (  0.0103,-0.5869),
       vec2 (  0.7119,-0.3154),
       vec2 ( -0.1154,-0.8918),
       vec2 ( -0.3169, 0.1063),
       vec2 ( -0.7940, 0.6287),
       vec2 (  0.2486, 0.4430),
       vec2 (  0.0596,-0.5411)

    );


    vec3    pe    = texture2DRect ( posMap,    gl_FragCoord.xy ).xyz;
    vec3    ne    = texture2DRect ( normalMap, gl_FragCoord.xy ).xyz;
    float   att   = 0.0;
    vec3    plane = normalize(2.0 * texture2D ( rotateMap, gl_FragCoord.xy/4.0).xyz - vec3 ( 1.0 ));

    for ( int i = 0; i < 12; i++ )
    {
        vec2    reflected_vec = radius * reflect ( vec3 ( rndTable [i], 0.0 ), plane ).xy / pe.z;
        vec2	sampleTex = gl_FragCoord.xy;

        sampleTex.x += reflected_vec.x * viewport_size.x;
        sampleTex.y += reflected_vec.y * viewport_size.y;


        vec3	sampleEye = texture2DRect ( posMap,  sampleTex ).xyz;
        vec3	toCenter  = sampleEye - pe;
        float	dist      = length ( toCenter );

        toCenter /= dist;								// normalize

        float	normAtten  = clamp ( ( dot ( toCenter, ne ) - minCrease ) * creaseScale, 0.0, 1.0 );
        float	rangeAtten = 1.0 - clamp ( dist / distScale, 0.0, 1.0 );

        att += normAtten * rangeAtten;
    }

    //vec4 res = vec4 ( pow ( (1.0 - att / 24.0)*1.1, 3.0 ) ); //1.06
    gl_FragColor = vec4 ( pow ( (1.0 - att / 12.0)*1.1, 3.0 ) ); //1.06
    //if( isnan(res.x) || isinf(res.x)) res = vec4(1.0);
    //gl_FragColor = res;
}


