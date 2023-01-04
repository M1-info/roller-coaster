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
    if (m_IsSelected)
        m_Material->GetShader()->SetUniform1i("u_isSelected", 1);
    else
        m_Material->GetShader()->SetUniform1i("u_isSelected", 0);
    m_Material->GetShader()->Unbind();
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

void Mesh::UpdateMatrix()
{
    m_Transform->ComputeMatrix();

    if (m_Parent != nullptr)
        m_Transform->SetMatrix(m_Transform->GetMatrix() * m_Parent->GetTransform()->ComputeMatrix());
}