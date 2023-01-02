#include "headers/Ground.h"

Ground::Ground(const std::string filename)
{
    m_IBO = nullptr;

    m_Type = MeshType::GROUND;

    m_Position = glm::vec3(0.0f, -10.0f, 0.0f);
    m_Scale = glm::vec3(50.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    OBJLoader loader(filename);

    std::vector<glm::vec3> vertices = loader.GetVertices();
    std::vector<glm::vec3> normales = loader.GetNormals();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    m_Name = "Ground";

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

Ground::~Ground()
{
    // delete m_VAO;
    // delete m_VBO_pos;
    // delete m_VBO_norm;
    // delete m_IBO;
    // delete m_Material;
}

void Ground::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    m_VAO->Unbind();
    m_Material->GetShader()->Unbind();
}
