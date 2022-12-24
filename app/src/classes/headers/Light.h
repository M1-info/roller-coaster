#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneObject.h"

class Light
{
public:
    Light();
    Light(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float intensity);
    Light (glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity);
    ~Light();

    std::string GetName() const;
    inline glm::mat4 GetMatrix() const { return m_Matrix; }
    inline float GetIntensity() const { return m_Intensity; }

    inline void SetIntensity(float intensity) { m_Intensity = intensity; }

    void UpdateMatrix();
    void Translate(glm::vec3 translation);
    void Rotate(glm::vec3 rotation);

private: 
    glm::mat4 m_Matrix;

    std::string m_Name;

    float m_Intensity;

public: 
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Color;

};


#endif // LIGHT_H