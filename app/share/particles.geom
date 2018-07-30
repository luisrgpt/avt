#version 450 core

layout(points) in;
layout(points, max_vertices = 4) out;

uniform vec4 dispersion0;
uniform vec4 dispersion1;
uniform vec4 dispersion2;
uniform vec4 dispersion3;

void main()
{
    gl_Position = gl_in[0].gl_Position + dispersion0;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + dispersion1;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + dispersion2;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + dispersion3;
    EmitVertex();

    EndPrimitive();
}