#include "headers/Material.h"

Material::Material()
    : m_SpecularExponent(0.0f), m_AmbientColor(Color()), 
    m_DiffuseColor(Color()), m_SpecularColor(Color()), 
    m_EmissiveColor(Color()), m_Shader(nullptr)
{
}

Material::Material(float specularExponent, Color ambientColor, Color diffuseColor, Color specularColor, Color emissiveColor)
    : m_SpecularExponent(specularExponent), m_AmbientColor(ambientColor), 
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

