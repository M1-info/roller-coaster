#include "meshes/Cart.h"

Cart::Cart()
{

    m_Type = MeshType::CART;
    m_Name = "Cart";

    m_Transform = new Transform();

    m_CurrentRail = {};
    m_CurrentTangent = {};
}

std::shared_ptr<Cart> Cart::Create()
{
    std::shared_ptr<Cart> cart = std::make_shared<Cart>();

    OBJLoader loader("cart.obj");

    std::vector<Object> objects = loader.GetObjects();
    Object main = objects[0];

    cart->m_Vertices = main.GetVertices();
    cart->m_Normals = main.GetNormals();
    cart->m_Indices = main.GetFacesIndices();
    OBJMaterial material = main.GetMaterial();

    cart->CreateMaterial("phong");
    cart->m_Material->SetMaterialColor(Color(1.0f, 0.0f, 0.0f));
    cart->m_Material->SetAmbientColor(material.ambient_color);
    cart->m_Material->SetDiffuseColor(material.diffuse_color);
    cart->m_Material->SetSpecularColor(material.specular_color);
    cart->m_Material->SetShininess(material.shininess);

    for (auto object = objects.begin() + 1; object != objects.end(); object++)
    {
        std::shared_ptr<Object3D> child = std::make_shared<Object3D>("cart child");

        child->SetVertices(object->GetVertices());
        child->SetNormals(object->GetNormals());
        child->SetIndices(object->GetFacesIndices());
        OBJMaterial object_material = object->GetMaterial();

        child->CreateMaterial("phong");
        child->GetMaterial()->SetMaterialColor(Color(1.0f, 1.0f, 0.0f));
        child->GetMaterial()->SetAmbientColor(object_material.ambient_color);
        child->GetMaterial()->SetDiffuseColor(object_material.diffuse_color);
        child->GetMaterial()->SetSpecularColor(object_material.specular_color);
        child->GetMaterial()->SetShininess(object_material.shininess);

        cart->AddChildren(child);

        child->SetUpBuffers();
    }

    cart->SetUpBuffers();
    return cart;
}

void Cart::Draw()
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

void Cart::Update()
{
    m_Transform->SetIsDirty(true);

    for (auto child : m_Children)
        child->GetTransform()->SetIsDirty(true);
}

void Cart::Animate(float deltaTime)
{

    if (m_CurrentRail != m_Rails.end())
    {
        glm::vec3 prevPosition;
        if (m_CurrentRail != m_Rails.begin())
            prevPosition = *(m_CurrentRail - 1);
        else
            prevPosition = *(m_Rails.end() - 1);

        glm::vec3 direction = *m_CurrentRail - prevPosition;

        glm::vec3 tangent = *(m_CurrentTangent);

        float pitch = glm::degrees(std::atan2(tangent.y, std::sqrt(tangent.x * tangent.x + tangent.z * tangent.z)));
        float yaw = glm::degrees(std::atan2(tangent.x, tangent.z));
        float roll = 0.0f;

        if (direction.y < 0.0f)
            pitch *= -1.0f;

        m_Transform->SetPosition(*m_CurrentRail);
        m_Transform->SetRotation(glm::vec3(pitch, yaw, roll));
        m_Transform->SetIsDirty(true);

        m_CurrentRail++;
        m_CurrentTangent++;

        return;
    }

    m_CurrentRail = m_Rails.begin();
    m_CurrentTangent = m_Tangents.begin();
}