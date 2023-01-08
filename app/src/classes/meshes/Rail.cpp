#include "meshes/Rail.h"

Rail::Rail(int index)
{
    m_Type = MeshType::RAIL;
    m_Name = "Rail_" + std::to_string(index);
    m_Transform = new Transform();
}

std::shared_ptr<Rail> Rail::Create(int index)
{
    std::shared_ptr<Rail> rail = std::make_shared<Rail>(index);

    OBJLoader loader("rail.obj");

    std::vector<Object> objects = loader.GetObjects();
    Object mainObject = objects[0];

    rail->m_Vertices = mainObject.GetVertices();
    rail->m_Normals = mainObject.GetNormals();
    rail->m_Indices = mainObject.GetFacesIndices();

    OBJMaterial material = mainObject.GetMaterial();
    rail->CreateMaterial("phong");
    rail->m_Material->SetMaterialColor(Color(.3f, 0.12f, 0.06f));
    rail->m_Material->SetAmbientColor(material.ambient_color);
    rail->m_Material->SetDiffuseColor(material.diffuse_color);
    rail->m_Material->SetSpecularColor(material.specular_color);
    rail->m_Material->SetShininess(material.shininess);

    int children_index = 0;
    for (auto object = objects.begin() + 1; object != objects.end(); object++)
    {
        std::string name = "rail " + std::to_string(index) + " child " + std::to_string(children_index++);
        std::shared_ptr<Object3D> child = std::make_shared<Object3D>(name);

        child->SetVertices(object->GetVertices());
        child->SetNormals(object->GetNormals());
        child->SetIndices(object->GetFacesIndices());
        OBJMaterial object_material = object->GetMaterial();

        child->CreateMaterial("phong");
        child->GetMaterial()->SetMaterialColor(Color(0.3f, 0.3f, 0.3f));
        child->GetMaterial()->SetAmbientColor(object_material.ambient_color);
        child->GetMaterial()->SetDiffuseColor(object_material.diffuse_color);
        child->GetMaterial()->SetSpecularColor(object_material.specular_color);
        child->GetMaterial()->SetShininess(object_material.shininess);

        rail->AddChildren(child);

        child->SetUpBuffers();
    }

    rail->SetUpBuffers();
    return rail;
}

void Rail::GeneratePlots(int index)
{
    std::shared_ptr<Plot> plot = Plot::Create(index, m_Transform);
    AddChildren(plot);
}

void Rail::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    m_IBO->Bind();
    glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
    m_VAO->Unbind();
    m_IBO->Unbind();
    m_Material->GetShader()->Unbind();

    for (auto child : m_Children)
        child->Draw();
}

void Rail::Update()
{
    m_Transform->SetIsDirty(true);

    for (auto child : m_Children)
        child->GetTransform()->SetIsDirty(true);
}