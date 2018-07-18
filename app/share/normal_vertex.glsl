#version 450 core

layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec3 vertex_normal;

layout(location = 2) uniform mat4 projection_view_model;
layout(location = 6) uniform mat4 view_model;
layout(location = 10) uniform mat3 m_normal;
layout(location = 13) uniform vec4 l_pos;

layout(location = 0) out vec3 light_normal;
layout(location = 1) out vec3 light_eye;
layout(location = 2) out vec3 light_direction;

void main(void)
{
	gl_Position = projection_view_model * vertex_position;

    vec4 pos = view_model * vertex_position;
	light_normal = normalize(m_normal * vertex_normal);
	light_eye = normalize(vec3(-pos));
	light_direction = normalize(vec3(l_pos - pos));
}