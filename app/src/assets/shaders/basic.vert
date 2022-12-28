#version 460 core

layout(location=0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform int u_isSelected;

void main()
{
    if(u_isSelected == 1)
        gl_PointSize=20.0;
    else
        gl_PointSize=10.0;
    gl_Position=u_projection*u_view*u_model*vec4(position, 1.0);
};
