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

out vec3 eye;
out vec3 normal;
out vec3 light;

void main(void)
{
    vec4 v_position = vec4(in_position, 1.0);
    vec4 l_position = vec4(-light_position, 1.0);
    mat4 view_model = view * model;

    gl_Position = projection * view_model * v_position;

    eye = normalize(-vec3(view_model * v_position));
    normal = normalize(m_normal * in_normal);
    light = normalize(vec3(view * l_position - view_model * v_position));
}
