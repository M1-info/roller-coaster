#include "headers/ControlPoint.h"

ControlPoint::ControlPoint(glm::vec3 point, int index)
{

    m_Type = MeshType::CONTROL_POINT;

    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    m_Name = "ControlPoint" + std::to_string(index);

    CreateMaterial("basic");

    m_Vertices.push_back(point);

    m_VAO = new VertexArray();

    m_VBO_pos = new VertexBuffer(m_Vertices.data(), sizeof(glm::vec3));

    VertexBufferLayout layout;
    layout.Push<float>(3);

    m_VAO->AddBuffer(*m_VBO_pos, layout);

    m_VAO->Unbind();
    m_VBO_pos->Unbind();
}


void ControlPoint::Draw()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    m_VAO->Bind();
    m_Material->GetShader()->Bind();
    glDrawArrays(GL_POINTS, 0, 1);
    m_VAO->Unbind();
    m_Material->GetShader()->Unbind();
    glDisable(GL_PROGRAM_POINT_SIZE);
}