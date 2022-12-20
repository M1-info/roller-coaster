#version 460 core

out vec4 out_color;

in vec3 v_texcoord;

uniform samplerCube u_texture;

void main()
{
    out_color=texture(u_texture,v_texcoord);
};