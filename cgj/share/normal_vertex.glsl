#version 450 core

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec3 vertex_normal;

layout (std140) uniform vertex {
    mat4 projection;
    mat4 view;
    mat4 model;
};
layout(location = 10) uniform mat3 m_normal;
layout(location = 13) uniform vec4 l_pos;

out vec3 light_normal;
out vec3 light_eye;
out vec3 light_direction;

void main()
{
	gl_Position = projection * view * model * vertex_position;

    vec4 pos = view * model * vertex_position;
	light_normal = normalize(m_normal * vertex_normal);
	light_eye = normalize(vec3(-pos));
	light_direction = normalize(vec3(l_pos - pos));
}