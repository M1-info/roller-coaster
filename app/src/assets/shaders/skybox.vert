#version 460 core

layout(location=0) in vec3 position;

uniform mat4 u_projectionView;

out vec3 v_texcoord;

void main()
{
    vec4 pos = u_projectionView * vec4(position, 1.0);
    gl_Position = pos.xyww;
    v_texcoord=position;
};