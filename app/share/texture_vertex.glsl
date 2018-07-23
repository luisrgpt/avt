#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

layout (std140) uniform vertex {
    mat4 projection;
    mat4 view;
    mat4 model;
};

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    Normal = normalize(vec3(view * model * vec4(normal, 0.0)));
    TexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}