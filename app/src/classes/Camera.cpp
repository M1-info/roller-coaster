#include "headers/Camera.h"
#include <iostream>

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_Fov(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane), m_MoveSpeed(2.5f)
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

    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

Camera::~Camera()
{
}

void Camera::Init(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
    m_Position = position;
    m_Target = target;
    m_Up = up;

    Update();
}

void Camera::LookAt(glm::vec3 target)
{
    m_Target = target;
    Update();
}

glm::mat4 Camera::GetOrientation()
{
    glm::mat4 orientation = glm::mat4(1.0f);
    orientation = glm::rotate(orientation, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    orientation = glm::rotate(orientation, glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    return orientation;
}

void Camera::Update()
{
    // m_Front = glm::normalize(m_Target - m_Position);
    // m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    // m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front.y = sin(glm::radians(m_Pitch));
    m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));

    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
    float velocity = m_MoveSpeed * deltaTime;

    if(direction == CameraMovement::FORWARD){
        m_Position += m_Front * velocity;
        Update();
        return;
    }

    if(direction == CameraMovement::BACKWARD){
        m_Position -= m_Front * velocity;
        Update();
        return;
    }

    if(direction == CameraMovement::LEFT){
        m_Position -= m_Right * velocity;
        Update();
        return;
    }

    if(direction == CameraMovement::RIGHT){
        m_Position += m_Right * velocity;
        Update();
    }
}


void Camera::Render(float deltaTime)
{
    if(m_CurrentMouvementDirection == CameraMovement::NOMOVE)
        return;

    Move(m_CurrentMouvementDirection, deltaTime);
}



