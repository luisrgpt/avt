#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) uniform mvp {
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec3 Normal;

void main(void)
{
    //gl_Position = projection * view * model * vec4(position, 1.0);
    gl_Position = vec4(position, 1.0);
    //Normal = normalize(vec3(view * model * vec4(normal, 0.0)));
    Normal = position;
}