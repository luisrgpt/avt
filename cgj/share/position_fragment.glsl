#version 450 core

in vec4 Color;

layout(location = 0) out vec4 Output;

void main(void)
{
	Output = Color;
}