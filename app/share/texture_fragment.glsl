#version 450 core

in vec3 Normal;

layout(location = 15) uniform material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
    int tex_count;
};

layout(location = 0) out vec4 Output;

void main(void)
{
    //vec3 color;
    //float intensity;
    //vec3 light_dir;
    //vec3 test;
    //light_dir = normalize(vec3(1.0, 1.0, 1.0));
    //intensity = max(dot(light_dir, Normal), 0.0);

    //Output = vec4((diffuse * intensity) + ambient, 1.0);
    Output = vec4(Normal, 1.0);
}