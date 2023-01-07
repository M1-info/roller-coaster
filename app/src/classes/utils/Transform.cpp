#include "utils/Transform.h"

Transform::Transform()
{
    m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_IsDirty = true;

    m_ParentTransform = nullptr;
    m_Matrix = ComputeMatrix();
}

Transform::~Transform()
{
}

glm::mat4 Transform::ComputeMatrix()
{

    if (!m_IsDirty)
        return m_Matrix;

    glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 translationMatrix = glm::translate(identity, m_Position);

    glm::mat4 rotationMatrix = glm::rotate(identity, glm::radians(m_Rotation.x), m_Right) *
                               glm::rotate(identity, glm::radians(m_Rotation.y), m_Up) *
                               glm::rotate(identity, glm::radians(m_Rotation.z), m_Forward);

    glm::mat4 scaleMatrix = glm::scale(identity, m_Scale);

    m_Matrix = translationMatrix * rotationMatrix * scaleMatrix;

    if (m_ParentTransform != nullptr)
        m_Matrix = m_ParentTransform->GetMatrix() * m_Matrix;

    m_IsDirty = false;
    return m_Matrix;
}