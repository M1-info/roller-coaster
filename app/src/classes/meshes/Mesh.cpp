#include "meshes/Mesh.h"

Mesh::~Mesh()
{
    m_Vertices.clear();
    m_Indices.clear();
    m_Normals.clear();
    m_Children.clear();
    m_Parent.reset();

    if (m_Transform != nullptr)
        delete m_Transform;

    if (m_Material != nullptr)
        delete m_Material;

    if (m_VAO != nullptr)
        delete m_VAO;
}

void Mesh::SetUpBuffers()
{

    m_VAO = new VertexArray();

    // VBO for positions

    VertexBufferLayout layout_position;
    layout_position.Push<float>(3);

    m_VBO_positions = new VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    m_VBO_positions->Bind();
    m_VAO->AddBuffer(*m_VBO_positions, layout_position);
    m_VBO_positions->Unbind();
    m_VAO->Unbind();

    // VBO for normals

    VertexBufferLayout layout_normals;
    layout_normals.Push<float>(3);

    m_VBO_normals = new VertexBuffer(m_Normals.data(), m_Normals.size() * sizeof(glm::vec3));
    m_VBO_normals->Bind();
    m_VAO->AddBuffer(*m_VBO_normals, layout_normals, 1);
    m_VBO_normals->Unbind();
    m_VAO->Unbind();

    // IBO for indices

    m_VAO->Bind();
    m_IBO = new IndexBuffer(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));
    m_VAO->Unbind();

    // Unbind all buffers
    Clear();
}

void Mesh::SetUp()
{
    m_VAO->Bind();
    m_VBO_positions->Bind();
    m_IBO->Bind();
}

void Mesh::Clear()
{
    m_VAO->Unbind();
    m_VBO_positions->Unbind();
    m_IBO->Unbind();
}

void Mesh::CreateMaterial(std::string shaderFile)
{
    m_Material = new Material();
    m_Material->AddShader(shaderFile);
}

void Mesh::ToggleIsSelected()
{
    m_IsSelected = !m_IsSelected;

    if (m_Material->GetShader()->HasUniform("u_isSelected") == false)
        return;

    m_Material->GetShader()->Bind();
    m_Material->GetShader()->SetUniform1i("u_isSelected", m_IsSelected);
    m_Material->GetShader()->Unbind();
}

void Mesh::AddChildren(std::shared_ptr<Mesh> child)
{
    m_Children.push_back(child);
    child->SetParent(shared_from_this());
    child->GetTransform()->SetParentTransform(m_Transform);
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