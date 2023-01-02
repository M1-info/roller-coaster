#version 460 core

out vec4 out_color;

in vec3 v_position;
in vec3 v_normals;

uniform mat4 u_view;
uniform vec3 u_cameraPosition;

uniform struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
}u_light;

uniform struct Material
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}u_material;

void main()
{
    vec3 ambient=u_light.color*u_material.ambient;
    
    // // diffuse light
    vec3 normal=normalize(v_normals);
    
    vec3 u_LightPosition=(mat4(1.)*vec4(u_light.position,1.)).xyz;
    vec3 lightDir=normalize(u_LightPosition-v_position);
    
    float diffuse=max(dot(normal,lightDir),0.);
    vec3 diffuseLight=diffuse*u_material.diffuse;
    
    // specular light
    vec3 camPos=(u_view*vec4(u_cameraPosition,1.)).xyz;
    vec3 viewDir=normalize(camPos-v_position);
    vec3 reflectDir=reflect(-lightDir,normal);
    
    float specular=pow(max(dot(viewDir,reflectDir),0.),u_material.shininess);
    vec3 specularLight=specular*u_material.specular;
    
    vec3 result=(ambient+diffuseLight+specularLight)*u_light.intensity*u_material.color;
    
    // compute light attenuation
    float distance=distance(u_LightPosition,v_position);
    float attenuation=1./(1.+.09*distance+.032*distance*distance);
    
    result*=attenuation;
    
    out_color=vec4(result,1.);
    
};