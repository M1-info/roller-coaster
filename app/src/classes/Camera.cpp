#include "headers/Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_Fov(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane)
{

    if(m_Fov == 0.0f)
        m_Fov = 45.0f;

    if(m_AspectRatio == 0.0f)
        m_AspectRatio = 16.0f / 9.0f;
    
    if(m_NearPlane == 0.0f)
        m_NearPlane = 0.1f;

    if(m_FarPlane == 0.0f)
        m_FarPlane = 100.0f;

    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

Camera::~Camera()
{
}

void Camera::Init(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    m_Position = position;
    m_Target = target;
    m_Direction = glm::normalize(m_Position - m_Target);

    m_View = glm::lookAt(m_Position, m_Target, up);
}

void Camera::LookAt(glm::vec3 target)
{
    m_Target = target;
    m_Direction = glm::normalize(m_Position - m_Target);
    m_View = glm::lookAt(m_Position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetOrientation()
{
    glm::mat4 orientation = glm::mat4(1.0f);
    orientation = glm::rotate(orientation, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    orientation = glm::rotate(orientation, glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    return orientation;
}

glm::vec3 Camera::Forward()
{
    glm::vec4 forward = glm::inverse(GetOrientation()) * glm::vec4(0,0,-1,1);
    return glm::vec3(forward);
}

glm::vec3 Camera::Right()
{
    glm::vec4 right = glm::inverse(GetOrientation()) * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

glm::vec3 Camera::Up()
{
    glm::vec4 up = glm::inverse(GetOrientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}


