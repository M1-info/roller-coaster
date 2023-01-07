#include "meshes/Object3D.h"

Object3D::Object3D()
{
    m_Type = MeshType::MESH;
    m_Name = "Object";
    m_Transform = new Transform();
}

Object3D::Object3D(const std::string name)
{
    m_Type = MeshType::MESH;
    m_Name = name;

    m_Transform = new Transform();
    m_Transform->SetIsDirty(true);
}

void Object3D::Update()
{
    m_Transform->SetIsDirty(true);
}

void Object3D::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    m_IBO->Bind();

    glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

    m_IBO->Unbind();
    m_VAO->Unbind();
    m_Material->GetShader()->Bind();
}