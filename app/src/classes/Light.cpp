#include "headers/Light.h"

Light::Light()
    : m_Position(glm::vec3(0.0)), m_Rotation(glm::vec3(0.0)), m_Color(glm::vec3(0.0)), m_Intensity(0.0f),
    m_Matrix(glm::mat4(1.0))
{
}

Light::Light(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float intensity)
    : m_Position(position), m_Rotation(rotation), m_Color(color), m_Intensity(intensity),
    m_Matrix(glm::mat4(1.0))
{
}

Light::Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity)
    : m_Position(glm::vec3(0.0)), m_Rotation(glm::vec3(0.0)), m_Color(color), m_Intensity(intensity),
    m_Matrix(glm::mat4(1.0))
{
}

Light::~Light()
{
}

std::string Light::GetName() const
{
    return m_Name;
}

void Light::Translate(glm::vec3 translation)
{
    m_Position += translation;
}

void Light::Rotate(glm::vec3 rotation)
{
    m_Rotation += rotation;
    UpdateMatrix();
}

void Light::UpdateMatrix()
{
    m_Matrix = glm::translate(glm::mat4(1.0f), m_Position) *
               (glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
               glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
               glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)));
}

