#version 450 core

in vec3 eye;
in vec3 normal;
in vec3 light;

layout(std140, binding = 1) uniform material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
    float shininess;
    int tex_count;
    int f_mode;
};

layout(location = 0) out vec4 color;

void main(void)
{
    float diffuse;
    float specular;

    vec3 n_eye = normalize(eye);
    vec3 n_normal = normalize(normal);
    vec3 n_light = normalize(light);

    diffuse = max(dot(n_light, n_normal), 0.0);
    if(diffuse > 0.0) {
        vec3 half_direction = normalize(n_light + n_eye);
        specular = max(pow(dot(n_normal, half_direction), shininess), 0.0);
    } else {
        specular = 0.0;
    }

    if (f_mode == 1) {
        color = vec4(Ka + diffuse * Kd, 1.0);
    }
    else {
        color = vec4(Ka + diffuse * Kd + specular * Ks, 1.0);
    }
}