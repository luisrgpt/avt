#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 m_normal;
uniform vec3 light_position0;
uniform vec3 light_position1;
uniform vec3 light_position2;
uniform vec3 light_position3;
uniform vec3 light_position4;
uniform float light_Ka;
uniform int v_mode;


out vec3 eye;
out vec3 normal;
out vec3 light[5];
out float f_Ka;
out float attenuation;
out vec3 debug;
out vec3 position_viewspace;
out vec3 light_position_viewspace[5];

void main(void)
{
    vec4 v_position = vec4(in_position, 1.0);
    vec4 l_position0 = vec4(light_position0, 1.0);
    vec4 l_position1 = vec4(light_position1, 1.0);
    vec4 l_position2 = vec4(light_position2, 1.0);
    vec4 l_position3 = vec4(light_position3, 1.0);
    vec4 l_position4 = vec4(light_position4, 1.0);
    vec4 l_normal = vec4(in_normal, 0.0);
    mat4 view_model = view * model;

    position_viewspace = vec3(view_model * v_position);
    light_position_viewspace[0] = vec3(view * l_position0);
    light_position_viewspace[1] = vec3(view * l_position1);
    light_position_viewspace[2] = vec3(view * l_position2);
    light_position_viewspace[3] = vec3(view * l_position3);
    light_position_viewspace[4] = vec3(view * l_position4);

    vec4 g_position = projection * view_model * v_position;

    gl_Position = g_position;

    eye = -vec3(view_model * v_position);
    normal = vec3(m_normal * l_normal);
    light[0] = vec3(view * l_position0 - view_model * v_position);
    light[1] = vec3(view * l_position1 - view_model * v_position);
    light[2] = vec3(view * l_position2 - view_model * v_position);
    light[3] = vec3(view * l_position3 - view_model * v_position);
    light[4] = vec3(view * l_position4 - view_model * v_position);

    f_Ka = light_Ka;
    debug = in_position;
}
