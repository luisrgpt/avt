#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

layout(std140, binding = 0) uniform vertex {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat3 m_normal;
    vec3 light_position;
};

void main(void)
{
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}