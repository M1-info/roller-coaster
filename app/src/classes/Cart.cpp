#include "headers/Cart.h"

Cart::Cart(const std::string filename)
{

    m_IBO = nullptr;

    m_Type = MeshType::CART;

    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f, 90.0f, 0.0f);
    m_Matrix = glm::mat4(1.0f);
    m_Velocity = glm::vec3(0.0f);
    m_CurrentRailVertex = {};

    OBJLoader loader(filename);

    std::vector<glm::vec3> vertices = loader.GetVertices();
    std::vector<glm::vec3> normales = loader.GetNormals();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    m_Name = filename.substr(0, filename.find_last_of('.'));
    m_Name[0] = toupper(m_Name[0]);

    CreateMaterial("phong");
    m_Material->SetMaterialColor(Color(1.0f, 0.0f, 0.0f));
    m_Material->SetAmbientColor(material.ambient_color);
    m_Material->SetDiffuseColor(material.diffuse_color);
    m_Material->SetSpecularColor(material.specular_color);
    m_Material->SetSpecularExponent(material.shininess);

    for (auto i : indices)
    {
        m_Vertices.push_back(vertices[i.vertices[0]]);
        m_Vertices.push_back(vertices[i.vertices[1]]);
        m_Vertices.push_back(vertices[i.vertices[2]]);

        m_Normales.push_back(normales[i.normals[0]]);
        m_Normales.push_back(normales[i.normals[1]]);
        m_Normales.push_back(normales[i.normals[2]]);
    }

    m_Vertices_size = m_Vertices.size() * sizeof(glm::vec3);
    m_Normales_size = m_Normales.size() * sizeof(glm::vec3);

    // create vertex array
    m_VAO = new VertexArray();

    // create vertex buffer for positions
    m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices_size);
    VertexBufferLayout layout_pos;
    layout_pos.Push<float>(3); // position
    m_VAO->AddBuffer(*m_VBO_pos, layout_pos);

    // create vertex buffer for normals
    m_VBO_norm = new VertexBuffer(m_Normales.data(), m_Normales_size);
    VertexBufferLayout layout_norm;
    layout_norm.Push<float>(3); // normals
    m_VAO->AddBuffer(*m_VBO_norm, layout_norm, 1);

    Clear();
}

void Cart::SetRailsVertices(std::vector<glm::vec3> railsVertices)
{
    m_RailsVertices = railsVertices;
    m_CurrentRailVertex = m_RailsVertices.begin();
}

void Cart::SetRailsTangents(std::vector<glm::vec3> railsTangents)
{
    m_RailsTangents = railsTangents;
    m_CurrentRailTangent = m_RailsTangents.begin();
}

void Cart::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    m_VAO->Unbind();
    m_Material->GetShader()->Unbind();
}

void Cart::Animate(float deltaTime)
{
    if (m_CurrentRailVertex != m_RailsVertices.end())
    {
        m_Position = *m_CurrentRailVertex;
        m_Position += glm::vec3(0.0f, 1.0f, 0.0f);
        m_CurrentRailVertex++;
    }
    else
        m_CurrentRailVertex = m_RailsVertices.begin();

    if (m_CurrentRailTangent != m_RailsTangents.end())
    {
        // use tangent to rotate the cart
        float angleX = glm::acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), *m_CurrentRailTangent));
        float angleY = glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), *m_CurrentRailTangent));
        float angleZ = glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), *m_CurrentRailTangent));
        m_Rotation = glm::vec3(angleX, angleY, 0.0f);
        m_CurrentRailTangent++;
    }
    else
        m_CurrentRailTangent = m_RailsTangents.begin();
}