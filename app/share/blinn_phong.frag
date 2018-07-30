#version 450 core

in vec3 eye;
in vec3 normal;
in vec3 light[5];
in float f_Ka;
in float attenuation;
in vec3 debug;
in vec3 position_viewspace;
in vec3 light_position_viewspace[5];

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ke;
uniform float shininess;
uniform int tex_count;
uniform vec3 light_color;
uniform int f_mode;
uniform float light_attenuation;
uniform float light_angle;
uniform int on_or_off0;
uniform int on_or_off1;
uniform int on_or_off2;
uniform int on_or_off3;
uniform int on_or_off4;
uniform int is_main0;
uniform int is_main1;
uniform int is_main2;
uniform int is_main3;
uniform int is_main4;
uniform vec3 directional_light;
uniform int directional_light_state;

layout(location = 0) out vec4 color;

void main(void)
{
    vec3 ambient;
    float diffuse;
    float specular;

    vec3 light_direction_viewspace;
    float attenuation;
    vec3 n_eye;
    vec3 n_normal;
    vec3 n_light;      

    int on_or_off[5];
    on_or_off[0] = on_or_off0;
    on_or_off[1] = on_or_off1;
    on_or_off[2] = on_or_off2;
    on_or_off[3] = on_or_off3;
    on_or_off[4] = on_or_off4;

    int is_main[5];
    is_main[0] = is_main0;
    is_main[1] = is_main1;
    is_main[2] = is_main2;
    is_main[3] = is_main3;
    is_main[4] = is_main4;

    float main_angle_ratio = 1;

    vec4 acc = vec4(0);

    n_eye = normalize(eye);
    n_normal = normalize(normal);

    if (directional_light_state == 1) {
        n_light = normalize(directional_light);
        attenuation = 1.0;

        ambient = f_Ka * Ka * light_color;

        diffuse = max(dot(n_normal, n_light), 0.0);
        if(diffuse > 0.0) {
            vec3 half_direction = normalize(n_light + n_eye);
            specular = max(pow(dot(n_normal, half_direction), shininess), 0.0);
        } else {
            specular = 0.0;
        }

        acc += vec4(ambient + attenuation * diffuse * Kd * light_color + attenuation * specular * Ks * light_color, 1.0);
    }

    for (int i = 0; i < 5; i++) {
        if (on_or_off[i] == 1) {
            light_direction_viewspace = light_position_viewspace[i] - position_viewspace;

            if (is_main[i] == 1) {
                main_angle_ratio = 10;
            }
            else {
                main_angle_ratio = 1;
            }

            attenuation = 1.0 / (1.0 + light_attenuation * pow(length(light_direction_viewspace), 2));
            if(degrees(acos(dot(-light_direction_viewspace, normalize(light_direction_viewspace)))) > light_angle / main_angle_ratio){
                attenuation = 0.0;
            }

            n_light = normalize(light[i]);

            ambient = f_Ka * Ka * light_color;

            diffuse = max(dot(n_normal, n_light), 0.0);
            if(diffuse > 0.0) {
                vec3 half_direction = normalize(n_light + n_eye);
                specular = max(pow(dot(n_normal, half_direction), shininess), 0.0);
            } else {
                specular = 0.0;
            }

            acc += vec4(ambient + attenuation * diffuse * Kd * light_color + attenuation * specular * Ks * light_color, 1.0);
        }
    }

    color = acc;

    //color = vec4(debug, 1.0);
}