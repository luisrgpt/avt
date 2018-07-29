#version 450 core

layout(std140, binding = 1) uniform material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
    float shininess;
    int tex_count;
};

layout(location = 0) out vec4 color;

void main(void)
{
	color = vec4(Kd ,1.0);
}