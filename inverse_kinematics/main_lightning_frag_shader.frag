#version 150

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform samplerCube shadowMap;
uniform vec4 ambient;
uniform highp vec4 light_pos;
uniform highp vec4 light_intensity;
uniform highp vec4 material;

uniform int shadows;
uniform int without_texture;
uniform int selected;
uniform float max_bias;

varying highp vec4 texc;
varying highp vec4 pos;
varying highp vec4 normal_for_fs;

const float shadow_k = 1.0 - 0.45;

const float zFar  = 100.0;
const float zNear  = 1.00;
//const float max_bias  = 0.05;
const float min_bias  = 0.00;

const float shadow_map_size = 1024.0;

vec3 vec_projection(in vec3 a, in vec3 b) //b -> a
{
    float a_length = length(a);
    return a * (dot(a,b) / a_length * a_length);
}

void main(void)
{
    vec4 color;
    vec4 vec_to_light = light_pos - pos;
    vec_to_light.w = 0;
    vec_to_light = normalize(vec_to_light);
    float lcos = dot(vec_to_light, normal_for_fs);

    if(lcos < 0) lcos = 0;

    if(without_texture == 1) color = vec4(1,1,1,1);
    else color = texture2D(texture1, texc.st);

    float shadow = 1.0;

    if(bool(shadows))
    {
        vec_to_light = -vec_to_light;

        vec3 max_direction = vec3(sign(vec_to_light.x), 0.0, 0.0);
        float max_axis = vec_to_light.x;

        if(abs(max_axis) < abs(vec_to_light.y))
        {
            max_axis = vec_to_light.y;
            max_direction = vec3(0.0, sign(vec_to_light.y), 0.0);
        }
        if(abs(max_axis) < abs(vec_to_light.z))
        {
            //max_axis = vec_to_light.z;
            max_direction = vec3(0.0, 0.0, sign(vec_to_light.z));
        }

        vec3 vec_to_lihgt_shift_i = vec3(0.0);
        vec3 vec_to_lihgt_shift_j = vec3(0.0);
        if( abs(max_direction.x) < 0.5)
        {
            vec_to_lihgt_shift_i.x = 1.0;
            if( abs(max_direction.y) < 0.5)
                vec_to_lihgt_shift_j.y = 1.0;
        }
        else
        {
            if( abs(max_direction.y) < 0.5)
                vec_to_lihgt_shift_i.y = 1.0;
        }
        if( abs(max_direction.z) < 0.5)
            vec_to_lihgt_shift_j.z = 1.0;

        float bias = max_bias * tan(acos(lcos)); // cosTheta is dot( n,l ), clamped between 0 and 1
        bias = clamp(bias, min_bias, max_bias);

        vec3 view_vec = vec_to_light.xyz * ( 1 / length(vec_projection(max_direction, vec_to_light.xyz)) );

        vec_to_lihgt_shift_i /= shadow_map_size;
        vec_to_lihgt_shift_j /= shadow_map_size;

        float z;
        float visibility = 0.0;

        for( int i = -1; i <= 1; i++  )
        {
            for( int j = -1; j <= 1; j++ )
            {
                z = (texture(shadowMap, vec_to_light.xyz + vec_to_lihgt_shift_i * i + vec_to_lihgt_shift_j * j )).r;
                z = zFar * zNear / (z * (zFar - zNear) - zFar);
                z = length(view_vec * z);
                if(z < length(pos - light_pos ) - bias) visibility += 1.0;
            }
        }

        shadow -= visibility / 9.0 * shadow_k;

/*
        z = texture(shadowMap, vec_to_light.xyz);
        z = zFar * zNear / (z * (zFar - zNear) - zFar); //z not normalized
        z = length(view_vec * z);
        if(z < length(pos - light_pos ) - bias) shadow = shadow_k;
*/
    }

    gl_FragColor = color * lcos * light_intensity * material * shadow + color * material * ambient;

    /*gl_FragDepth = gl_FragCoord.z;

    if ( selected == 0 )
    {
        gl_FragColor = color * lcos * light_intensity * material + color * material * ambient;
    }
    else
    {
        gl_FragColor = vec4(1,1,1,1);
        gl_FragDepth = 0.;
    }*/
}
