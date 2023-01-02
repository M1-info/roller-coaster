#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <string>

#include "utils/Transform.h"

class Light
{
public:
    Light();
    Light(glm::vec3 color, float intensity);
    ~Light();

    inline std::string GetName() const { return m_Name; }
    inline float GetIntensity() const { return m_Intensity; }
    inline float *GetIntensityPtr() { return &m_Intensity; }
    inline glm::vec3 GetColor() const { return m_Color; }
    inline glm::vec3 *GetColorPtr() { return &m_Color; }
    inline Transform *GetTransform() const { return m_Transform; }

    inline void SetName(std::string name) { m_Name = name; }
    inline void SetIntensity(float intensity) { m_Intensity = intensity; }
    inline void SetColor(glm::vec3 color) { m_Color = color; }
    inline void SetTransform(Transform *transform) { m_Transform = transform; }

private:
    std::string m_Name;

    float m_Intensity;
    glm::vec3 m_Color;

    Transform *m_Transform;
};

#endif // LIGHT_H