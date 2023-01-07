#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform();
    ~Transform();

    inline void SetIsDirty(bool isDirty) { m_IsDirty = isDirty; }

    inline void SetPosition(glm::vec3 position) { m_Position = position; }
    inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
    inline void SetScale(glm::vec3 scale) { m_Scale = scale; }
    inline void SetMatrix(glm::mat4 matrix) { m_Matrix = matrix; }

    inline void Translate(glm::vec3 translation) { m_Position += translation; }
    inline void Rotate(glm::vec3 rotation) { m_Rotation += rotation; }
    inline void Scale(glm::vec3 scale) { m_Scale += scale; }

    inline float GetPositionX() const { return m_Position.x; }
    inline float GetPositionY() const { return m_Position.y; }
    inline float GetPositionZ() const { return m_Position.z; }

    inline float GetRotationX() const { return m_Rotation.x; }
    inline float GetRotationY() const { return m_Rotation.y; }
    inline float GetRotationZ() const { return m_Rotation.z; }

    inline float GetScaleX() const { return m_Scale.x; }
    inline float GetScaleY() const { return m_Scale.y; }
    inline float GetScaleZ() const { return m_Scale.z; }

    inline glm::vec3 GetPosition() const { return m_Position; }
    inline glm::vec3 GetRotation() const { return m_Rotation; }
    inline glm::vec3 GetScale() const { return m_Scale; }

    inline glm::vec3 *GetPositionPtr() { return &m_Position; }
    inline glm::vec3 *GetRotationPtr() { return &m_Rotation; }
    inline glm::vec3 *GetScalePtr() { return &m_Scale; }

    inline glm::mat4 GetMatrix() const { return m_Matrix; }

    inline void SetParentTransform(Transform *parentTransform) { m_ParentTransform = parentTransform; }

    glm::mat4 ComputeMatrix();

private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    glm::mat4 m_Matrix;

    bool m_IsDirty;

    glm::vec3 m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);

    Transform *m_ParentTransform;
};

#endif // !TRANSFORM_H