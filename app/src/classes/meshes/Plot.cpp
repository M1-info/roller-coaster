#include "meshes/Plot.h"

Plot::Plot()
{
    m_Type = MeshType::PLOT;
    m_Transform = new Transform();
}

std::shared_ptr<Plot> Plot::Create(int index, Transform *parent_transform)
{

    std::shared_ptr<Plot> plot = std::make_shared<Plot>();
    plot->m_Name = "Plot " + std::to_string(index);

    // get the head of the plot
    OBJLoader loader_head("plot-head.obj");

    Object head_object = loader_head.GetObjects()[0];

    // set the vertices, normals and indices of the plot
    plot->m_Vertices = head_object.GetVertices();
    plot->m_Normals = head_object.GetNormals();
    plot->m_Indices = head_object.GetFacesIndices();

    // set the material of the plot
    OBJMaterial head_material = head_object.GetMaterial();
    plot->CreateMaterial("phong");
    plot->m_Material->SetMaterialColor(Color(.3f, 0.12f, 0.06f));
    plot->m_Material->SetAmbientColor(head_material.ambient_color);
    plot->m_Material->SetDiffuseColor(head_material.diffuse_color);
    plot->m_Material->SetSpecularColor(head_material.specular_color);
    plot->m_Material->SetShininess(head_material.shininess);

    plot->m_Transform->SetParentTransform(parent_transform);

    if (parent_transform->GetRotationZ() < 0.0f || parent_transform->GetRotationX() < 0.0f)
        plot->m_Transform->SetRotation(plot->m_Transform->GetRotation() - parent_transform->GetRotation());
    else
        plot->m_Transform->SetRotation(plot->m_Transform->GetRotation() + parent_transform->GetRotation());

    // y position of the rail so we can scale the plot to touch the ground
    float parent_y = parent_transform->GetPositionY();

    // get the body of the plot
    OBJLoader loader_plot("plot.obj");
    Object plot_object = loader_plot.GetObjects()[0];
    OBJMaterial plot_material = plot_object.GetMaterial();

    // we add 10 because the ground is 10 units below 0
    int plot_body_scale_y = (parent_y + 10.f) / PLOT_SIZE;

    // create the plot body
    std::shared_ptr<Object3D> child = std::make_shared<Object3D>("plot " + std::to_string(index) + " child ");
    child->SetVertices(plot_object.GetVertices());
    child->SetNormals(plot_object.GetNormals());
    child->SetIndices(plot_object.GetFacesIndices());

    // set the material of the plot body
    child->CreateMaterial("phong");
    child->GetMaterial()->SetMaterialColor(Color(0.3f, 0.3f, 0.3f));
    child->GetMaterial()->SetAmbientColor(plot_material.ambient_color);
    child->GetMaterial()->SetDiffuseColor(plot_material.diffuse_color);
    child->GetMaterial()->SetSpecularColor(plot_material.specular_color);
    child->GetMaterial()->SetShininess(plot_material.shininess);

    child->GetTransform()->SetParentTransform(plot->m_Transform);
    child->GetTransform()->SetScale(glm::vec3(1.0f, plot_body_scale_y, 1.0f));

    // add plot body as a child of the plot
    plot->AddChildren(child);

    child->SetUpBuffers();
    plot->Update();
    plot->SetUpBuffers();

    return plot;
}

void Plot::Draw()
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

void Plot::Update()
{
    m_Transform->SetIsDirty(true);

    for (auto child : m_Children)
        child->GetTransform()->SetIsDirty(true);
}