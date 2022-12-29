#version 460 core

layout(location=0)in vec3 position;
layout(location=1)in vec3 normals;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_position;
out vec3 v_normals;

void main()
{
    v_position=(u_model*vec4(position,1.)).xyz;
    // v_normals=transpose(inverse(mat3(u_model)))*normals;
    v_normals=(u_model*vec4(normals,0.)).xyz;
    gl_Position=u_projection*u_view*vec4(v_position,1.);
};
