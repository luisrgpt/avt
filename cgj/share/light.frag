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
};

layout(location = 0) out vec4 color;

void main(void) {
    float diffuse;
    float specular;

    diffuse = max(dot(light, normal), 0.0);
    if(diffuse > 0.0) {
        vec3 half_direction = normalize(light + eye);
        specular = max(pow(dot(normal, half_direction), 30), 0.0);
    } else {
        specular = 0.0;
    }

    color = vec4(Ka + diffuse * Kd + specular * Ks, 1.0);
}