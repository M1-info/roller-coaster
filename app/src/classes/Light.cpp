#include "Light.h"

Light::Light()
    : m_Color(glm::vec3(0.0)), m_Intensity(0.0f), m_Transform(new Transform()), m_Name("Light")
{
}

Light::Light(glm::vec3 color, float intensity)
    : m_Color(color), m_Intensity(intensity), m_Transform(new Transform()), m_Name("Light")
{
}

Light::~Light()
{
}
