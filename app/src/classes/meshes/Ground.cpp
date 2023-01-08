#include "meshes/Ground.h"

Ground::Ground()
{
    m_Type = MeshType::GROUND;

    m_Transform = new Transform();
    m_Transform->SetScale(glm::vec3(50.0f));
    m_Transform->SetPosition(glm::vec3(0.0f, -10.0f, 0.0f));
    m_Transform->SetIsDirty(true);

    m_Name = "Ground";
}

std::shared_ptr<Ground> Ground::Create()
{
    std::shared_ptr<Ground> ground = std::make_shared<Ground>();

    // Load the model
    OBJLoader loader("island.obj");

    // Get the objects
    std::vector<Object> objects = loader.GetObjects();
    Object main = objects[0];

    // Set the vertices, normals and indices
    ground->m_Vertices = main.GetVertices();
    ground->m_Normals = main.GetNormals();
    ground->m_Indices = main.GetFacesIndices();
    OBJMaterial material = main.GetMaterial();

    // Set the material
    ground->CreateMaterial("phong");
    ground->m_Material->SetMaterialColor(Color(0.0f, 1.0f, 0.0f));
    ground->m_Material->SetAmbientColor(material.ambient_color);
    ground->m_Material->SetDiffuseColor(material.diffuse_color);
    ground->m_Material->SetSpecularColor(material.specular_color);
    ground->m_Material->SetShininess(material.shininess);

    // Add the children
    for (auto object = objects.begin() + 1; object != objects.end(); object++)
    {
        std::shared_ptr<Object3D> child = std::make_shared<Object3D>("ground child");

        // Set the vertices, normals and indices
        child->SetVertices(object->GetVertices());
        child->SetNormals(object->GetNormals());
        child->SetIndices(object->GetFacesIndices());
        OBJMaterial object_material = object->GetMaterial();

        // Set the material
        child->CreateMaterial("phong");
        child->GetMaterial()->SetMaterialColor(Color(0.2f, 0.04f, 0.017f));
        child->GetMaterial()->SetAmbientColor(object_material.ambient_color);
        child->GetMaterial()->SetDiffuseColor(object_material.diffuse_color);
        child->GetMaterial()->SetSpecularColor(object_material.specular_color);
        child->GetMaterial()->SetShininess(object_material.shininess);

        ground->AddChildren(child);

        child->SetUpBuffers();
        child->GetTransform()->SetIsDirty(true);
    }

    ground->SetUpBuffers();
    return ground;
}

void Ground::Update()
{
    m_Transform->SetIsDirty(true);

    for (auto child : m_Children)
        child->GetTransform()->SetIsDirty(true);
}

void Ground::Draw()
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
