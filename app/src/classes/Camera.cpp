#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_Fov(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane),
      m_MoveSpeed(BASE_SPEED), m_MoveSensitivity(BASE_SENSITIVITY), m_CurrentMouvementDirection(CameraMovement::NOMOVE)
{

    if (m_Fov == 0.0f)
        m_Fov = 45.0f;

    if (m_AspectRatio == 0.0f)
        m_AspectRatio = 16.0f / 9.0f;

    if (m_NearPlane == 0.0f)
        m_NearPlane = 0.1f;

    if (m_FarPlane == 0.0f)
        m_FarPlane = 100.0f;

    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

Camera::~Camera()
{
}

/*
    Initialize the camera with a position, a target and an up vector
*/
void Camera::Init()
{

    m_Position = glm::vec3(0.0f, 0.0f, 8.0f);
    m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    Update();
}

/**
 * This method update the view matrix
 * It is called when the camera is moved or rotated
 */
void Camera::Update()
{
    m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front.y = sin(glm::radians(m_Pitch));
    m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));

    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::Update(std::shared_ptr<Cart> cart)
{
    // get current tangent of the cart
    glm::vec3 tangent = cart->GetCurrentTangent();
    int offset = 1.0f;
    if (tangent.y > 0.95f)
        offset = -1.0f;

    glm::vec3 target = glm::vec3(tangent.x * 5.0f, tangent.y + offset, tangent.z * 5.0);

    m_View = glm::lookAt(m_Position, m_Position + target, m_Up);
}

void Camera::OnMouseMove(float x, float y)
{
    x *= m_MoveSensitivity;
    y *= m_MoveSensitivity;

    m_Yaw += x;
    m_Pitch += y;

    std::clamp(m_Pitch, -89.0f, 89.0f);

    Update();
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
    float velocity = m_MoveSpeed * deltaTime;

    if (direction == CameraMovement::FORWARD)
    {
        m_Position += m_Front * velocity;
        Update();
        return;
    }

    if (direction == CameraMovement::BACKWARD)
    {
        m_Position -= m_Front * velocity;
        Update();
        return;
    }

    if (direction == CameraMovement::LEFT)
    {
        m_Position -= m_Right * velocity;
        Update();
        return;
    }

    if (direction == CameraMovement::RIGHT)
    {
        m_Position += m_Right * velocity;
        Update();
        return;
    }

    if (direction == CameraMovement::UP)
    {
        m_Position += m_Up * velocity;
        Update();
        return;
    }

    if (direction == CameraMovement::DOWN)
    {
        m_Position += glm::vec3(0.0f, -1.0f, 0.0f) * velocity;
        Update();
    }
}

void Camera::Render(float deltaTime)
{
    if (m_CurrentMouvementDirection == CameraMovement::NOMOVE)
        return;

    if (!m_IsOnCart)
        Move(m_CurrentMouvementDirection, deltaTime);
}
