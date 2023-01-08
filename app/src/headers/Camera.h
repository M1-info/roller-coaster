#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "meshes/Cart.h"

#define BASE_SPEED 20.0f
#define BASE_SENSITIVITY 0.2f

enum class CameraMovement
{
    NOMOVE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    Camera() = default;
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
    ~Camera();

    void Init();

    /* GETTERS */

    inline float GetFov() const { return m_Fov; };
    inline float GetAspectRatio() const { return m_AspectRatio; };
    inline float GetNearPlane() const { return m_NearPlane; };
    inline float GetFarPlane() const { return m_FarPlane; };

    inline glm::vec3 GetPosition() const { return m_Position; };
    inline glm::vec3 *GetPositionPtr() { return &m_Position; };
    inline glm::vec3 GetTarget() const { return m_Target; };

    inline glm::mat4 GetProjection() const { return m_Projection; };
    inline glm::mat4 GetView() const { return m_View; };
    inline glm::mat4 GetProjectionView() const { return m_Projection * m_View; };

    inline float GetYaw() const { return m_Yaw; };
    inline float GetPitch() const { return m_Pitch; };
    inline float *GetYawPtr() { return &m_Yaw; };
    inline float *GetPitchPtr() { return &m_Pitch; };

    inline float GetLastX() const { return m_lastX; };
    inline float GetLastY() const { return m_lastY; };

    inline float GetMoveSensitivity() const { return m_MoveSensitivity; };
    inline float GetMoveSpeed() const { return m_MoveSpeed; };
    inline float *GetMoveSensitivityPtr() { return &m_MoveSensitivity; };
    inline float *GetMoveSpeedPtr() { return &m_MoveSpeed; };
    inline CameraMovement GetCurrentMouvementDirection() const { return m_CurrentMouvementDirection; };

    inline glm::vec3 GetFront() const { return m_Front; };
    inline glm::vec3 GetRight() const { return m_Right; };
    inline glm::vec3 GetUp() const { return m_Up; };
    inline glm::vec3 GetWorldUp() const { return m_WorldUp; };

    /* SETTERS */

    inline void SetFov(float fov) { m_Fov = fov; };
    inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; };
    inline void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; };
    inline void SetFarPlane(float farPlane) { m_FarPlane = farPlane; };

    inline void SetPosition(glm::vec3 position) { m_Position = position; };
    inline void SetTarget(glm::vec3 target) { m_Target = target; };

    inline void SetProjection(glm::mat4 projection) { m_Projection = projection; };
    inline void SetView(glm::mat4 view) { m_View = view; };

    inline void SetYaw(float yaw) { m_Yaw = yaw; };
    inline void SetPitch(float pitch) { m_Pitch = pitch; };

    inline void SetLastX(float lastX) { m_lastX = lastX; };
    inline void SetLastY(float lastY) { m_lastY = lastY; };

    inline void SetMoveSensitivity(float moveSensitivity) { m_MoveSensitivity = moveSensitivity; };
    inline void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };
    inline void SetCurrentMovementDirection(CameraMovement direction) { m_CurrentMouvementDirection = direction; };

    inline void SetFront(glm::vec3 front) { m_Front = front; };
    inline void SetRight(glm::vec3 right) { m_Right = right; };
    inline void SetUp(glm::vec3 up) { m_Up = up; };

    void OnMouseMove(float x, float y);

    void Move(CameraMovement direction, float deltaTime);
    void Update();
    void Update(std::shared_ptr<Cart> cart);

    /* Called once per frame */
    void Render(float deltaTime);

private:
    /* Camera projection values */
    float m_Fov;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;

    /* Camera position values */
    glm::vec3 m_Position;
    glm::vec3 m_Target;
    float m_lastX;
    float m_lastY;

    /* Camera orientation values */
    float m_Yaw;
    float m_Pitch;

    /* View matrix values */
    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
    glm::vec3 m_WorldUp;

    /* Parameters to change camera mouvement behavior  */
    float m_MoveSensitivity;
    float m_MoveSpeed;
    CameraMovement m_CurrentMouvementDirection;

    glm::mat4 m_Projection;
    glm::mat4 m_View;

public:
    bool m_IsOnCart;
};

#endif // CAMERA_H