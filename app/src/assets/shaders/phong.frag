#version 460 core

out vec4 out_color;

in vec3 v_position;
in vec3 v_normals;

uniform mat4 u_view;
uniform vec3 u_cameraPos;
uniform int u_isSelected;

uniform struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
}u_light;

uniform struct Material
{
    vec3 color;
    vec3 coeffAmbient;
    vec3 coeffDiffuse;
    vec3 coeffSpecular;
    float specularExponent;
}u_material;

void main()
{
    vec3 ambient=u_light.color*u_material.coeffAmbient;
    
    // // diffuse light
    vec3 normal=normalize(v_normals);
    
    vec3 u_LightPosition=(mat4(1.)*vec4(u_light.position,1.)).xyz;
    vec3 lightDir=normalize(u_LightPosition-v_position);
    
    float diffuse=max(dot(normal,lightDir),0.);
    vec3 diffuseLight=diffuse*u_material.coeffDiffuse;
    
    // specular light
    vec3 camPos=(u_view*vec4(u_cameraPos,1.)).xyz;
    vec3 viewDir=normalize(camPos-v_position);
    vec3 reflectDir=reflect(-lightDir,normal);
    
    float specular=pow(max(dot(viewDir,reflectDir),0.),u_material.specularExponent);
    vec3 specularLight=specular*u_material.coeffSpecular;
    
    vec3 result=(ambient+diffuseLight+specularLight)*u_light.intensity;
    
    if(u_isSelected==1)
    out_color=vec4(1.,1.,0.,1.);
    else
    out_color=vec4(u_material.color,1.)*vec4(result,1.);
};