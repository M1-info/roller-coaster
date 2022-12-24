#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normals;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_position;
out vec3 v_normals;

void main()
{
    gl_Position=u_projection*u_view*u_model*vec4(position, 1.0);
    v_position=position;
    v_normals=normals;
};
