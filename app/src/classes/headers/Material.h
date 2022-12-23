#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H


#include "glm/glm.hpp"
#include "Shader.h"

class Material
{
public :
    Material();
    Material(float specularExponent, glm::vec3 ambiantColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 emissiveColor);
    ~Material();

    inline float GetSpecularExponent() const { return m_SpecularExponent; };
    inline glm::vec3 GetAmbiantColor() const { return m_AmbiantColor; };
    inline glm::vec3 GetDiffuseColor() const { return m_DiffuseColor; };
    inline glm::vec3 GetSpecularColor() const { return m_SpecularColor; };
    inline glm::vec3 GetEmissiveColor() const { return m_EmissiveColor; };

    inline void SetSpecularExponent(float specularExponent) { m_SpecularExponent = specularExponent; };
    inline void SetAmbiantColor(glm::vec3 ambiantColor) { m_AmbiantColor = ambiantColor; };
    inline void SetDiffuseColor(glm::vec3 diffuseColor) { m_DiffuseColor = diffuseColor; };
    inline void SetSpecularColor(glm::vec3 specularColor) { m_SpecularColor = specularColor; };
    inline void SetEmissiveColor(glm::vec3 emissiveColor) { m_EmissiveColor = emissiveColor; };

    Shader *GetShader() const;
    void AddShader(std::string filename);


private:
    float m_SpecularExponent;   // Ns in MTL file
    glm::vec3 m_AmbiantColor;   // Ka in MTL file
    glm::vec3 m_DiffuseColor;   // Kd in MTL file
    glm::vec3 m_SpecularColor;  // Ks in MTL file
    glm::vec3 m_EmissiveColor;  // Ke in MTL file

    Shader* m_Shader;
};


#endif // MATERIAL_H