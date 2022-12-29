#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include "Shader.h"
#include "OBJLoader.h"

class Material
{
public:
    Material();
    Material(float specularExponent, Color ambiantColor, Color diffuseColor, Color specularColor, Color emissiveColor);
    ~Material();

    inline float GetSpecularExponent() const { return m_SpecularExponent; };
    inline Color GetAmbientColor() const { return m_AmbientColor; };
    inline Color GetMaterialColor() const { return m_MaterialColor; };
    inline Color GetDiffuseColor() const { return m_DiffuseColor; };
    inline Color GetSpecularColor() const { return m_SpecularColor; };
    inline Color GetEmissiveColor() const { return m_EmissiveColor; };

    inline void SetSpecularExponent(float specularExponent) { m_SpecularExponent = specularExponent; };
    inline void SetAmbientColor(Color ambientColor) { m_AmbientColor = ambientColor; };
    inline void SetMaterialColor(Color materialColor) { m_MaterialColor = materialColor; };
    inline void SetDiffuseColor(Color diffuseColor) { m_DiffuseColor = diffuseColor; };
    inline void SetSpecularColor(Color specularColor) { m_SpecularColor = specularColor; };
    inline void SetEmissiveColor(Color emissiveColor) { m_EmissiveColor = emissiveColor; };

    Shader *GetShader() const;
    void AddShader(std::string filename);

private:
    Color m_MaterialColor;
    float m_SpecularExponent; // Ns in MTL file
    Color m_AmbientColor;     // Ka in MTL file
    Color m_DiffuseColor;     // Kd in MTL file
    Color m_SpecularColor;    // Ks in MTL file
    Color m_EmissiveColor;    // Ke in MTL file

    Shader *m_Shader;
};

#endif // MATERIAL_H