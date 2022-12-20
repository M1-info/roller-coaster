#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    Camera() = default;
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
    ~Camera();

    void Init(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    inline float GetFov() const { return m_Fov; }
    inline float GetAspectRatio() const { return m_AspectRatio; }
    inline float GetNearPlane() const { return m_NearPlane; }
    inline float GetFarPlane() const { return m_FarPlane; }

    inline glm::vec3 GetPosition() const { return m_Position; }
    inline glm::vec3 GetDirection() const { return m_Direction; }
    inline glm::vec3 GetTarget() const { return m_Target; }

    inline glm::mat4 GetProjection() const { return m_Projection; }
    inline glm::mat4 GetView() const { return m_View; }
    inline glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

    inline void SetFov(float fov) { m_Fov = fov; }
    inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
    inline void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; }
    inline void SetFarPlane(float farPlane) { m_FarPlane = farPlane; }

    inline void SetPosition(glm::vec3 position) { m_Position = position; }

    inline void SetProjection(glm::mat4 projection) { m_Projection = projection; }
    inline void SetView(glm::mat4 view) { m_View = view; }

    glm::mat4 GetOrientation();
    glm::vec3 Forward();
    glm::vec3 Right();
    glm::vec3 Up();

    void LookAt(glm::vec3 target);

private:
    float m_Fov;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;

    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_Target;

    float m_Yaw;
    float m_Pitch;

    glm::mat4 m_Projection;
    glm::mat4 m_View;
};

#endif // CAMERA_H