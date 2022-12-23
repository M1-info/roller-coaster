#include "headers/Material.h"

Material::Material()
    : m_SpecularExponent(0.0f), m_AmbiantColor(glm::vec3(0.0f)), 
    m_DiffuseColor(glm::vec3(0.0f)), m_SpecularColor(glm::vec3(0.0f)), 
    m_EmissiveColor(glm::vec3(0.0f)), m_Shader(nullptr)
{
}

Material::Material(float specularExponent, glm::vec3 ambiantColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 emissiveColor)
    : m_SpecularExponent(specularExponent), m_AmbiantColor(ambiantColor), 
        m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), 
        m_EmissiveColor(emissiveColor), m_Shader(nullptr)
{
}

Material::~Material()
{
    delete m_Shader;
}


Shader *Material::GetShader() const
{
    return m_Shader;
}


void Material::AddShader(std::string filename)
{
    m_Shader = new Shader(filename);
}

