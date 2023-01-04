#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "glm/glm.hpp"
#include "Shader.h"
#include "OBJLoader.h"
#include "Light.h"
#include "Camera.h"

class Material
{
public:
    Material();
    Material(float shininess, Color ambiantColor, Color diffuseColor, Color specularColor, Color emissiveColor);
    ~Material();

    inline Color GetMaterialColor() const { return m_MaterialColor; };
    inline Color GetAmbientColor() const { return m_AmbientColor; };
    inline Color GetDiffuseColor() const { return m_DiffuseColor; };
    inline Color GetSpecularColor() const { return m_SpecularColor; };
    inline float GetShininess() const { return m_Shininess; };
    inline Color GetEmissiveColor() const { return m_EmissiveColor; };

    inline Color *GetMaterialColorPtr() { return &m_MaterialColor; };
    inline Color *GetAmbientColorPtr() { return &m_AmbientColor; };
    inline Color *GetDiffuseColorPtr() { return &m_DiffuseColor; };
    inline Color *GetSpecularColorPtr() { return &m_SpecularColor; };
    inline Color *GetEmissiveColorPtr() { return &m_EmissiveColor; };

    inline void SetShininess(float specularExponent) { m_Shininess = specularExponent; };
    inline void SetAmbientColor(Color ambientColor) { m_AmbientColor = ambientColor; };
    inline void SetMaterialColor(Color materialColor) { m_MaterialColor = materialColor; };
    inline void SetDiffuseColor(Color diffuseColor) { m_DiffuseColor = diffuseColor; };
    inline void SetSpecularColor(Color specularColor) { m_SpecularColor = specularColor; };
    inline void SetEmissiveColor(Color emissiveColor) { m_EmissiveColor = emissiveColor; };

    Shader *GetShader() const;
    void SetShader(Shader *shader);
    void AddShader(std::string filename);
    void SetUpShader(std::shared_ptr<Light> light);
    void UpdateShader(glm::mat4 projectionView, glm::mat4 mesh = glm::mat4(1.0), glm::mat4 view = glm::mat4(1.0f), std::shared_ptr<Light> light = nullptr, glm::vec3 cameraPosition = glm::vec3(0.0f));

private:
    Color m_MaterialColor;
    Color m_AmbientColor;
    Color m_DiffuseColor;
    Color m_SpecularColor;
    float m_Shininess;
    Color m_EmissiveColor;

    Shader *m_Shader;
};

#endif // MATERIAL_H