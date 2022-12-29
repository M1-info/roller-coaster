#include "headers/Cart.h"

Cart::Cart(const std::string filename)
{

    m_Type = MeshType::CART;

    m_Position = glm::vec3(-2.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    OBJLoader loader(filename);

    m_Vertices = loader.GetVertices();
    std::vector<glm::vec3> normales = loader.GetNormals();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    m_Name = filename.substr(0, filename.find_last_of('.'));
    m_Name[0] = toupper(m_Name[0]);

    CreateMaterial("phong");
    m_Material->SetAmbientColor(material.ambient_color);
    m_Material->SetDiffuseColor(material.diffuse_color);
    m_Material->SetSpecularColor(material.specular_color);
    m_Material->SetSpecularExponent(material.shininess);

    for (auto i : indices)
    {
        m_Indices.push_back(i.vertices[0]);
        m_Indices.push_back(i.vertices[1]);
        m_Indices.push_back(i.vertices[2]);

        // m_Normales.push_back(normales[i.normals[0]]);
        // m_Normales.push_back(normales[i.normals[1]]);
        // m_Normales.push_back(normales[i.normals[2]]);
    }

    m_Normales = normales;

    m_Vertices_size = m_Vertices.size() * sizeof(glm::vec3);
    m_Normales_size = m_Normales.size() * sizeof(glm::vec3);
    m_Indices_size = m_Indices.size() * sizeof(unsigned int);

    // create vertex array
    m_VAO = new VertexArray();

    // create vertex buffer for positions
    m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices_size);
    VertexBufferLayout layout_pos;
    layout_pos.Push<float>(3); // position

    m_VAO->AddBuffer(*m_VBO_pos, layout_pos);

    // create vertex buffer for normales
    m_VBO_norm = new VertexBuffer(m_Normales.data(), m_Normales_size);
    VertexBufferLayout layout_norm;
    layout_norm.Push<float>(3); // normales

    m_VAO->AddBuffer(*m_VBO_norm, layout_norm, 1);

    // create index buffer
    m_IBO = new IndexBuffer(m_Indices.data(), m_Indices_size);

    Clear();
}

void Cart::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    m_IBO->Bind();
    glDrawElements(GL_TRIANGLES, m_Indices_size, GL_UNSIGNED_INT, 0);
    m_Material->GetShader()->Unbind();
    m_VAO->Unbind();
    m_IBO->Unbind();
}