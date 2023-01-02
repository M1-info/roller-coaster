#version 460 core

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_projectionView;

void main()
{
    gl_Position=u_projectionView*u_model*vec4(position+normal*.05f,1.);
};
