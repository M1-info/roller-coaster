#include "meshes/Cart.h"

Cart::Cart()
{

    m_Type = MeshType::CART;
    m_Name = "Cart";

    m_Transform = new Transform();
    m_Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

std::shared_ptr<Cart> Cart::Create()
{
    std::shared_ptr<Cart> cart = std::make_shared<Cart>();

    // Load the model
    OBJLoader loader("cart.obj");

    // Get the objects
    std::vector<Object> objects = loader.GetObjects();
    Object main = objects[0];

    // Set the vertices, normals and indices
    cart->m_Vertices = main.GetVertices();
    cart->m_Normals = main.GetNormals();
    cart->m_Indices = main.GetFacesIndices();
    OBJMaterial material = main.GetMaterial();

    // Set the material
    cart->CreateMaterial("phong");
    cart->m_Material->SetMaterialColor(Color(1.0f, 0.0f, 0.0f));
    cart->m_Material->SetAmbientColor(material.ambient_color);
    cart->m_Material->SetDiffuseColor(material.diffuse_color);
    cart->m_Material->SetSpecularColor(material.specular_color);
    cart->m_Material->SetShininess(material.shininess);

    // Add the children
    for (auto object = objects.begin() + 1; object != objects.end(); object++)
    {
        std::shared_ptr<Object3D> child = std::make_shared<Object3D>("cart child");

        // Set the vertices, normals and indices
        child->SetVertices(object->GetVertices());
        child->SetNormals(object->GetNormals());
        child->SetIndices(object->GetFacesIndices());
        OBJMaterial object_material = object->GetMaterial();

        // Set the material
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

void Cart::Animate()
{

    if (m_CurrentCurveIndex < m_CurveCount)
    {
        m_CurrentCurve = &m_Curves.at(m_CurrentCurveIndex);
        glm::vec3 currentPosition = m_Transform->GetPosition();

        // get curve point
        glm::vec3 point = m_CurrentCurve->GetPoint(m_CurrentCurveTime) + glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 direction = point - currentPosition;
        m_Transform->SetPosition(point);

        // get curve tangent
        glm::vec3 tangent = m_CurrentCurve->GetTangent(m_CurrentCurveTime, point);
        m_CurrentTangent = tangent;

        float pitch = 0.0f;
        float yaw = glm::degrees(std::atan2(tangent.x, tangent.z));
        float roll = glm::degrees(std::atan2(direction.y, (float)sqrt(direction.x * direction.x + direction.z * direction.z)));

        if (direction.y < 0.0f)
            roll = -roll;

        // Calculate acceleration based on curvature of track
        // TODO: Fix acceleration calculation
        // glm::vec3 gravity = glm::vec3(0.0f, GRAVITY, 0.0f);
        // glm::vec3 acceleration = glm::dot(gravity, tangent) - (CART_FRICTION / CART_MASS) * m_Velocity;

        // Calculate acceleration based on curvature of track
        glm::vec3 acceleration = glm::cross(tangent, glm::cross(point - currentPosition, tangent));
        acceleration *= 1.0f / glm::length(point - currentPosition);

        // // Include gravity in acceleration
        acceleration += GRAVITY * TIME_STEP * glm::vec3(0.0f, -1.0f, 0.0f);

        // // Update velocity using explicit Euler method
        // m_Velocity += acceleration * TIME_STEP;

        // Set rotation and position of cart
        m_Transform->SetPosition(m_Transform->GetPosition() + m_Velocity * TIME_STEP);
        m_Transform->SetRotation(glm::vec3(pitch, yaw, roll));
        Update();

        if (m_CurrentCurveTime >= 1.0f)
        {
            m_CurrentCurveIndex++;
            m_CurrentCurveTime = 0.0f;
        }
        else
            m_CurrentCurveTime += TIME_STEP;
    }
    else
    {
        m_CurrentCurveIndex = 0;
        m_CurrentCurve = &m_Curves.at(m_CurrentCurveIndex);
        m_CurrentCurveTime = 0.0f;
    }
}