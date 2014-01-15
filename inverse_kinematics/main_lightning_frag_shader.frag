uniform sampler2D texture;
uniform sampler2D texture2;
uniform vec4 ambient;
uniform highp vec4 light_pos;
uniform highp vec4 light_intensity;
uniform highp vec4 material;

uniform int without_texture;
uniform int selected;

varying highp vec4 texc;
varying highp vec4 pos;
varying highp vec4 normal_for_fs;

void main(void)
{
        vec4 color;
        vec4 vec_to_light = light_pos-pos;
        vec_to_light.w = 0;
        float lcos = dot(normalize(vec_to_light),normal_for_fs);
        if(lcos<0) lcos = 0;
        if(without_texture == 1) color = vec4(1,1,1,1);
        else color = texture2D(texture, texc.st);
        gl_FragDepth = gl_FragCoord.z;

        if ( selected == 0 ) gl_FragColor = color * lcos * light_intensity * material + color * material * ambient;
        else
        {
            gl_FragColor = vec4(1,1,1,1);
            gl_FragDepth = 0.;
        }
}
