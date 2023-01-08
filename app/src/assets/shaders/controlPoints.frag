#version 460 core

out vec4 out_color;

uniform int u_isSelected;

void main()
{
    if(u_isSelected==1)
    out_color=vec4(1.,1.,0.,1.);
    else
    out_color=vec4(1.,0.,0.,1.);
};