#include "headers/Mesh.h"

Mesh::~Mesh()
{
    m_Vertices.clear();
    m_Indices.clear();
    m_Normales.clear();
    m_Children.clear();
    m_Parent.reset();

    if (m_VAO != nullptr)
        delete m_VAO;

    if (m_VBO_pos != nullptr)
        delete m_VBO_pos;

    if (m_VBO_norm != nullptr)
        delete m_VBO_norm;

    if (m_IBO != nullptr)
        delete m_IBO;

    if (m_Material != nullptr)
        delete m_Material;
}

void Mesh::SetUp()
{
    m_VAO->Bind();
    m_VBO_pos->Bind();
    m_VBO_norm->Bind();
    m_IBO->Bind();
}

void Mesh::Clear()
{
    m_VAO->Unbind();
    m_VBO_pos->Unbind();
    m_VBO_norm->Unbind();
    m_IBO->Unbind();
}

void Mesh::CreateMaterial(std::string shaderFile)
{
    m_Material = new Material();
    m_Material->AddShader(shaderFile);
}

void Mesh::AddChildren(std::shared_ptr<Mesh> child)
{
    m_Children.push_back(child);
    child->SetParent(shared_from_this());
}

void Mesh::RemoveChildren(std::shared_ptr<Mesh> child)
{
    std::vector<std::shared_ptr<Mesh>>::iterator it = std::find(m_Children.begin(), m_Children.end(), child);

    if (it != m_Children.end())
    {
        it->get()->SetParent(nullptr);
        m_Children.erase(it);
        m_Children.shrink_to_fit();
    }
}

void Mesh::Translate(glm::vec3 translation)
{
    m_Position += translation;
    m_IsDirty = true;
}

void Mesh::Rotate(GLfloat angle, glm::vec3 axis)
{
    m_Rotation += axis * angle;
    m_IsDirty = true;
}

void Mesh::Scale(glm::vec3 scale)
{
    m_Scale += scale;
    m_IsDirty = true;
}

glm::mat4 Mesh::ComputeMatrix()
{
    if (!m_IsDirty)
        return m_Matrix;

    glm::mat4 model(1.0f);

    glm::mat4 translation = glm::translate(model, m_Position);

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::degrees(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
                         glm::rotate(glm::mat4(1.0f), glm::degrees(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::rotate(glm::mat4(1.0f), glm::degrees(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scale = glm::scale(model, m_Scale);

    m_Matrix = translation * rotation * scale;

    if (m_Parent != nullptr)
        m_Matrix *= m_Parent->ComputeMatrix();

    m_IsDirty = false;

    return m_Matrix;
}
