#include "headers/Mesh.h"

Mesh::~Mesh()
{
    m_Vertices.clear();
    m_Indices.clear();

    delete m_VAO;
    delete m_VBO_pos;
    delete m_IBO;
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

void Mesh::SetName(std::string name)
{
    m_Name = name;
}

void Mesh::SetVertices(std::vector<Vertex> vertices)
{
    m_Vertices = vertices;
}

void Mesh::SetNormales(std::vector<Normal> normales)
{
    m_Normales = normales;
}

void Mesh::SetIndices(std::vector<IndexesFace> indices)
{
    m_Indices = indices;
}

void Mesh::SetParent(std::shared_ptr<Mesh> parent)
{
    m_Parent = parent;
}

void Mesh::SetChildren(std::vector<std::shared_ptr<Mesh>> children)
{
    m_Children = children;
}

void Mesh::CreateMaterial(std::string shaderFile)
{
    m_Material = new Material();
    m_Material->AddShader(shaderFile);
}

std::string Mesh::GetName() const
{
    return m_Name;
}

VertexArray *Mesh::GetVAO() const
{
    return m_VAO;
}

VertexBuffer *Mesh::GetVBO() const
{
    return m_VBO_pos;
}

IndexBuffer *Mesh::GetIBO() const
{
    return m_IBO;
}

std::vector<Vertex> Mesh::GetVertices() const
{
    return m_Vertices;
}

std::vector<Normal> Mesh::GetNormales() const
{
    return m_Normales;
}

std::vector<IndexesFace> Mesh::GetIndices() const
{
    return m_Indices;
}

std::shared_ptr<Mesh> Mesh::GetParent() const
{
    return m_Parent;
}

std::vector<std::shared_ptr<Mesh>> Mesh::GetChildren() const
{
    return m_Children;
}

glm::mat4 Mesh::GetMatrix() const
{  
    return m_Matrix;
}

Material* Mesh::GetMaterial() const
{  
    return m_Material;
}

glm::vec3 Mesh::GetPosition() const
{
    return m_Position;
}

glm::vec3 Mesh::GetScale() const
{
    return m_Scale;
}

glm::vec3 Mesh::GetRotation() const
{
    return m_Rotation;
}

void Mesh::Translate(glm::vec3 translation)
{
    m_Position = translation;
    m_Matrix = glm::translate(m_Matrix, m_Position);
}

void Mesh::Rotate(GLfloat angle, glm::vec3 axis)
{
    m_Rotation = axis * angle;
    m_Matrix = glm::rotate(m_Matrix, angle, axis);
}

void Mesh::Scale(glm::vec3 scale)
{
    m_Scale = scale;
    m_Matrix = glm::scale(m_Matrix, m_Scale);
}

glm::mat4 Mesh::ComputeMatrix()
{  
    glm::mat4 model(1.0f);

    glm::mat4 translation = glm::translate(model, m_Position);
    glm::mat4 rotation = glm::rotate(model, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                         glm::rotate(model, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::rotate(model, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scale = glm::scale(model, m_Scale);

    m_Matrix = translation * rotation * scale;

    if(m_Parent != nullptr)
        m_Matrix *= m_Parent->ComputeMatrix();

    return m_Matrix;
}

