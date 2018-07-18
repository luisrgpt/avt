#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) uniform mat4 projection_view_model;

out vec4 Color;

void main(void)
{
	gl_Position = projection_view_model * vec4(position, 1.0);
	Color = vec4(position, 1.0);
}