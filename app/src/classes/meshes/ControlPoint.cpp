#include "meshes/ControlPoint.h"

ControlPoint::ControlPoint(glm::vec3 point, int index)
{
    m_IBO = nullptr;
    m_Parent = nullptr;

    m_Type = MeshType::CONTROL_POINT;

    m_Transform = new Transform();

    m_Name = "ControlPoint_" + std::to_string(index);

    CreateMaterial("controlPoints");

    m_Vertices.push_back(point);

    m_VAO = new VertexArray();

    m_VBO_positions = new VertexBuffer(m_Vertices.data(), sizeof(glm::vec3));

    VertexBufferLayout layout;
    layout.Push<float>(3);

    m_VAO->AddBuffer(*m_VBO_positions, layout);

    m_VAO->Unbind();
    m_VBO_positions->Unbind();
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