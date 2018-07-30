#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

layout(std140, binding = 0) uniform vertex {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 m_normal;
    vec3 light_position;
    int v_mode;
};

out vec3 eye;
out vec3 normal;
out vec3 light;

void main(void)
{
    vec4 v_position = vec4(in_position, 1.0);
    vec4 l_position = vec4(light_position, 1.0);
    vec4 l_normal = vec4(in_normal, 0.0);
    mat4 view_model = view * model;

    vec4 g_position = projection * view_model * v_position;

    gl_Position = g_position;

    eye = -vec3(view_model * v_position);
    normal = vec3(m_normal * l_normal);
    light = vec3(view * l_position - view_model * v_position);
}
