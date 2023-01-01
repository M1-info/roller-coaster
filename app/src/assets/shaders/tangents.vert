#version 460 core

layout(location=0)in vec3 position;

uniform mat4 u_model;
uniform mat4 u_projectionView;

void main()
{
    gl_Position=u_projectionView*u_model*vec4(position,1.);
};
