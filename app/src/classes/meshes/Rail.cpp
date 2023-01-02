#include "meshes/Rail.h"

Rail::Rail(const std::string filename, int index)
{

    m_IBO = nullptr;

    m_Type = MeshType::RAIL;

    m_Transform = new Transform();

    OBJLoader loader(filename);

    std::vector<glm::vec3> vertices = loader.GetVertices();
    std::vector<glm::vec3> normales = loader.GetNormals();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    m_Name = "Rail_" + std::to_string(index);

    CreateMaterial("phong");
    m_Material->SetMaterialColor(Color(1.0f, 0.0f, 0.0f));
    m_Material->SetAmbientColor(material.ambient_color);
    m_Material->SetDiffuseColor(material.diffuse_color);
    m_Material->SetSpecularColor(material.specular_color);
    m_Material->SetShininess(material.shininess);

    for (auto i : indices)
    {
        m_Vertices.push_back(vertices[i.vertices[0]]);
        m_Vertices.push_back(vertices[i.vertices[1]]);
        m_Vertices.push_back(vertices[i.vertices[2]]);

        m_Normals.push_back(normales[i.normals[0]]);
        m_Normals.push_back(normales[i.normals[1]]);
        m_Normals.push_back(normales[i.normals[2]]);
    }

    // create vertex array
    m_VAO = new VertexArray();
    m_VBO_positions = new VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));

    // create vertex buffer for positions
    VertexBufferLayout layout_pos;
    layout_pos.Push<float>(3); // position
    m_VAO->AddBuffer(*m_VBO_positions, layout_pos);

    m_VBO_normals = new VertexBuffer(m_Normals.data(), m_Normals.size() * sizeof(glm::vec3));
    VertexBufferLayout layout_normals;
    layout_normals.Push<float>(3); // normals
    m_VAO->AddBuffer(*m_VBO_normals, layout_normals, 1);

    Clear();
}

void Rail::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    m_Material->GetShader()->Unbind();
    m_VAO->Unbind();
}