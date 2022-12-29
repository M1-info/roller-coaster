#version 460 core

layout(location=0)in vec3 position;

uniform mat4 u_model;
uniform mat4 u_projectionView;
uniform int u_isSelected;

void main()
{
    if(u_isSelected==1)
    gl_PointSize=20.;
    else
    gl_PointSize=10.;
    gl_Position=u_projectionView*u_model*vec4(position,1.);
};
